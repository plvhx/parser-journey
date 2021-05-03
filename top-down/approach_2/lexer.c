#include "lexer.h"
#include <stdlib.h>

lexer_val_t *lexer_val_alloc(void) {
  lexer_val_t *val = calloc(1, sizeof(lexer_val_t));

  if (val == NULL) {
    return NULL;
  }

  return val;
}

static void __lexer_val_dtor_single(lexer_val_t *val) { free(val); }

void lexer_val_del(lexer_t *lexer, lexer_val_t *val) {
  // handle 'prev' boundary
  if (lexer_get_prev(val) == NULL) {
    lexer_set_prev(lexer_get_next(val), NULL);
    lexer_set_head(lexer, lexer_get_next(val));
    goto cleanup;
  }

  // handle 'next' boundary
  if (lexer_get_next(val) == NULL) {
    lexer_set_next(lexer_get_prev(val), NULL);
    lexer_set_tail(lexer, lexer_tail(lexer) == val ? lexer_get_prev(val)
                                                   : lexer_tail(lexer));
    goto cleanup;
  }

  lexer_set_next(lexer_get_prev(val), lexer_get_next(val));
  lexer_set_prev(lexer_get_next(val), lexer_get_prev(val));

cleanup:
  __lexer_val_dtor_single(val);
}

void lexer_val_dtor(lexer_val_t *val) {
  if (val == NULL) {
    return;
  }

  lexer_val_dtor(lexer_get_next(val));

  if (lexer_val_var(val) != NULL) {
    str_rewind_tail(lexer_val_var(val));
    str_dtor(lexer_val_var(val));
  }

  free(val);
}

lexer_t *lexer_alloc(void) {
  lexer_t *lexer = calloc(1, sizeof(lexer_t));

  if (lexer == NULL) {
    return NULL;
  }

  return lexer;
}

void lexer_add(lexer_t *lexer, lexer_val_t *val) {
  if (lexer_head(lexer) == NULL && lexer_tail(lexer) == NULL) {
    // maintain 'next' and 'prev' of
    // new value node
    lexer_set_next(val, NULL);
    lexer_set_prev(val, NULL);

    // set 'head' and 'tail' just
    // right to new value node.
    lexer_set_head(lexer, val);
    lexer_set_tail(lexer, val);
    return;
  }

  // maintain 'next' and 'prev' of
  // new value node.
  lexer_set_next(val, NULL);
  lexer_set_prev(val, lexer_tail(lexer));

  // point tail->next to new value node.
  lexer_set_next(lexer_tail(lexer), val);
  // point tail to new value node.
  lexer_set_tail(lexer, lexer_get_next(lexer_tail(lexer)));
  return;
}

void lexer_add_type(lexer_t *lexer, token_type_t ttype) {
  lexer_add(lexer, lexer_val_alloc());
  lexer_tail_set_token_type(lexer, ttype);
}

void lexer_dtor(lexer_t *lexer) {
  lexer_rewind_tail(lexer);
  lexer_val_dtor(lexer_tail(lexer));
  str_rewind_tail(lexer_get_str(lexer));
  str_dtor(lexer_get_str(lexer));
  free(lexer);
}

static void lexer_push_byte(str_t *str, unsigned char c) {
  if (str == NULL) {
    return;
  }

  if ((str_head(str) == NULL && str_tail(str) == NULL) ||
      str_curr_get_len(str) == STR_MAX_LEN) {
    str_add(str, str_val_alloc());
  }

  str_curr_get_buf_at(str, str_curr_get_len(str)++) = c;
}

static void lexer_handle_plus_op(lexer_t *lexer) {
  lexer_add_type(lexer, TOKEN_PLUS_OP);
}

static void lexer_handle_open_parenthesis(lexer_t *lexer) {
  lexer_add_type(lexer, TOKEN_OPEN_PARENTHESIS);
}

static void lexer_handle_close_parenthesis(lexer_t *lexer) {
  lexer_add_type(lexer, TOKEN_CLOSE_PARENTHESIS);
}

static int lexer_is_lower(unsigned char c) {
  if (c >= 97 && c <= 122) {
    return 1;
  }

  return 0;
}

static int lexer_is_upper(unsigned char c) {
  if (c >= 65 && c <= 90) {
    return 1;
  }

  return 0;
}

static int lexer_is_digit(unsigned char c) {
  if (c >= 48 && c <= 57) {
    return 1;
  }

  return 0;
}

static int lexer_is_underscore(unsigned char c) { return c == 95 ? 1 : 0; }

static void lexer_handle_variable(lexer_t *lexer) {
  str_t *str = str_alloc();

  if (str == NULL) {
    return;
  }

  unsigned char token = lexer_get_token(lexer);

  if (!lexer_is_lower(token) && !lexer_is_upper(token) &&
      !lexer_is_underscore(token)) {
    str_dtor(str);
    return;
  }

  lexer_push_byte(str, token);

  while ((token = str_next_byte(lexer_get_str(lexer))) != 0xff) {
    if (!lexer_is_lower(token) && !lexer_is_upper(token) &&
        !lexer_is_digit(token) && !lexer_is_underscore(token)) {
      str_dec_index(lexer_get_str(lexer));
      break;
    }

    lexer_push_byte(str, token);
  }

  lexer_add_type(lexer, TOKEN_VARIABLE);
  lexer_tail_set_var(lexer, str);
}

static void lexer_scan_token(lexer_t *lexer) {
  switch (lexer_get_token(lexer)) {
  case '+':
    lexer_handle_plus_op(lexer);
    break;
  case '(':
    lexer_handle_open_parenthesis(lexer);
    break;
  case ')':
    lexer_handle_close_parenthesis(lexer);
    break;
  default:
    lexer_handle_variable(lexer);
    break;
  }
}

lexer_t *lexer_scan_buffer(const char *buffer, unsigned int buflen) {
  lexer_t *lexer = lexer_alloc();

  if (lexer == NULL) {
    return NULL;
  }

  lexer_set_str(lexer, str_alloc());

  if (lexer_get_str(lexer) == NULL) {
    lexer_dtor(lexer);
    return NULL;
  }

  for (int i = 0; i < buflen; i++) {
    lexer_push_byte(lexer_get_str(lexer), buffer[i]);
  }

  // emulate POSIX 'feof' function
  // by adding trailing 8-byte 0xff to end of the buffer.
  for (int i = 0; i < 8; i++) {
    lexer_push_byte(lexer_get_str(lexer), 0xff);
  }

  while (lexer_set_token(lexer, str_next_byte(lexer_get_str(lexer))) != 0xff) {
    lexer_scan_token(lexer);
  }

  return lexer;
}

#ifdef DEBUG
void lexer_debug(lexer_t *lexer) {
  lexer_for_each_self(lexer) {
    printf("%s -> ", __func__);

    switch (lexer_tail_token_type(lexer)) {
    case TOKEN_VARIABLE:
      printf("(variable)");
      break;
    case TOKEN_PLUS_OP:
      printf("(plus-op)");
      break;
    case TOKEN_OPEN_PARENTHESIS:
      printf("(open-parenthesis)");
      break;
    case TOKEN_CLOSE_PARENTHESIS:
      printf("(close-parenthesis)");
      break;
    }

    printf("\n");
  }
}
#endif
#include "lexer.h"
#include <stdlib.h>

lexer_t *lexer_alloc(void) {
  lexer_t *lexer = calloc(1, sizeof(lexer_t));

  if (lexer == NULL) {
    return NULL;
  }

  return lexer;
}

void lexer_add(lexer_t *lexer, lexer_val_t *val) {
  if (lexer_head(lexer) == NULL && lexer_tail(lexer) == NULL) {
    lexer_set_head(lexer, val);
    lexer_set_tail(lexer, val);

    // maintain 'next'
    lexer_set_next(lexer_head(lexer), NULL);
    lexer_set_next(lexer_tail(lexer), NULL);

    // maintain 'prev'
    lexer_set_prev(lexer_head(lexer), NULL);
    lexer_set_prev(lexer_tail(lexer), NULL);
    return;
  }

  // maintain 'next' and 'prev' of the
  // new node.
  lexer_set_next(val, NULL);
  lexer_set_prev(val, lexer_tail(lexer));

  // point tail->next into new node.
  lexer_set_next(lexer_tail(lexer), val);
  // point tail into new node
  lexer_set_tail(lexer, val);
  return;
}

void lexer_del(lexer_t *lexer, lexer_val_t *val) { lexer_val_del(lexer, val); }

void lexer_add_value(lexer_t *lexer, token_kind_t token_type) {
  lexer_add(lexer, lexer_val_alloc());
  lexer_tail_set_token_type(lexer, token_type);
}

void lexer_dtor(lexer_t *lexer) {
  lexer_rewind_tail(lexer);
  lexer_val_dtor(lexer_tail(lexer));

  if (lexer->str != NULL) {
    str_dtor(lexer->str);
  }

  free(lexer);
}

lexer_val_t *lexer_val_alloc(void) {
  lexer_val_t *val = calloc(1, sizeof(lexer_val_t));

  if (val == NULL) {
    return NULL;
  }

  return val;
}

static void __lexer_val_dtor_single(lexer_val_t *val) { free(val); }

void lexer_val_del(lexer_t *lexer, lexer_val_t *val) {
  // handle 'prev' boundary.
  if (val->prev == NULL) {
    val->next->prev = NULL;
    lexer_set_head(lexer, val->next);
    goto cleanup;
  }

  // handle 'next' boundary.
  if (val->next == NULL) {
    val->prev->next = NULL;
    lexer_set_tail(lexer, lexer_tail(lexer) == val ? val->prev
                                                         : lexer_tail(lexer));
    goto cleanup;
  }

  val->prev->next = val->next;
  val->next->prev = val->prev;

cleanup:
  __lexer_val_dtor_single(val);
}

void lexer_val_dtor(lexer_val_t *val) {
  if (val == NULL) {
    return;
  }

  lexer_val_dtor(val->next);

  if (val->x.var != NULL) {
    str_dtor(val->x.var);
  }

  free(val);
}

static void lexer_push_byte(lexer_t *lexer, unsigned char c) {
  if (lexer->str == NULL) {
    lexer->str = str_alloc();
  }

  if ((str_head(lexer->str) == NULL && str_tail(lexer->str) == NULL) ||
      str_curr_len(lexer->str) == STR_MAX_LEN) {
    str_add(lexer->str, str_val_alloc());
  }

  str_curr_str_at(lexer->str, str_curr_len(lexer->str)++) = c;
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

static void lexer_handle_open_parenthesis(lexer_t *lexer) {
  lexer_add_value(lexer, TOKEN_OPEN_PARENTHESIS);
}

static void lexer_handle_close_parenthesis(lexer_t *lexer) {
  lexer_add_value(lexer, TOKEN_CLOSE_PARENTHESIS);
}

static void lexer_scan_variable(lexer_t *lexer) {
  str_t *str = str_alloc();

  if (str == NULL) {
    return;
  }

  unsigned char token = lexer_get_curr_token(lexer);

  if (!lexer_is_lower(token) && !lexer_is_upper(token) &&
      !lexer_is_underscore(token)) {
    return;
  }

  if ((str_head(str) == NULL && str_tail(str) == NULL) ||
      str_curr_len(str) == STR_MAX_LEN) {
    str_add(str, str_val_alloc());
  }

  str_push_byte(str, token);

  while ((token = str_next_byte(lexer->str)) != 0xff) {
    if (!lexer_is_lower(token) && !lexer_is_upper(token) &&
        !lexer_is_digit(token) && !lexer_is_underscore(token)) {
      str_dec_index(lexer->str);
      break;
    }

    if ((str_head(str) == NULL && str_tail(str) == NULL) ||
        str_curr_len(str) == STR_MAX_LEN) {
      str_add(str, str_val_alloc());
    }

    str_push_byte(str, token);
  }

  lexer_add_value(lexer, TOKEN_VARIABLE);
  lexer_tail_set_var(lexer, str);
}

static void lexer_scan_token(lexer_t *lexer) {
  switch (lexer_get_curr_token(lexer)) {
  case '(':
    lexer_handle_open_parenthesis(lexer);
    break;
  case ')':
    lexer_handle_close_parenthesis(lexer);
    break;
  default:
    lexer_scan_variable(lexer);
    break;
  }
}

lexer_t *lexer_scan_buffer(const char *buffer, unsigned int buflen) {
  lexer_t *lexer = lexer_alloc();

  if (lexer == NULL) {
    return NULL;
  }

  for (int i = 0; i < buflen; i++) {
    lexer_push_byte(lexer, buffer[i]);
  }

  // emulate POSIX 'feof' function
  // by adding trailing 8-byte 0xff to end of the buffer
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);
  lexer_push_byte(lexer, 0xff);

  while (lexer_set_curr_token(lexer, str_next_byte(lexer->str)) != 0xff) {
    lexer_scan_token(lexer);
  }

  return lexer;
}

#ifdef DEBUG
void lexer_debug(lexer_t *lexer) {
  lexer_for_each_self(lexer) {
    switch (lexer_curr_get_token_type(lexer)) {
    case TOKEN_VARIABLE:
      printf("(variable)");
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

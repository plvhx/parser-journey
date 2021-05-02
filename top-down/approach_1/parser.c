#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static parser_state_t *parser_state_alloc(void) {
  parser_state_t *pst = calloc(1, sizeof(parser_state_t));

  if (pst == NULL) {
    return NULL;
  }

  return pst;
}

static void parser_state_dtor(parser_state_t *pst) { free(pst); }

static int __lexer_is_open_parenthesis(lexer_val_t *val) {
  if (val == NULL || val->token_type != TOKEN_OPEN_PARENTHESIS) {
    return 0;
  }

  return 1;
}

static int __lexer_is_close_parenthesis(lexer_val_t *val) {
  if (val == NULL || val->token_type != TOKEN_CLOSE_PARENTHESIS) {
    return 0;
  }

  return 1;
}

static ast_t *parse_variable(parser_state_t *pst, lexer_t *lexer) {
  while (1) {
    if (!__lexer_is_close_parenthesis(lexer_get_next(lexer_curr(lexer))) ||
        !__lexer_is_open_parenthesis(lexer_get_prev(lexer_curr(lexer)))) {
      break;
    }

    lexer_del(lexer, lexer_get_next(lexer_curr(lexer)));
    lexer_del(lexer, lexer_get_prev(lexer_curr(lexer)));
  }

  if (__lexer_is_close_parenthesis(lexer_get_next(lexer_curr(lexer)))) {
    fprintf(stderr, "Wild (close-parenthesis) has been occurred.\n");
    parser_state_set_is_error(pst, 1);
    return NULL;
  }

  if (__lexer_is_open_parenthesis(lexer_get_prev(lexer_curr(lexer)))) {
    fprintf(stderr, "Wild (open-parenthesis) has been occurred.\n");
    parser_state_set_is_error(pst, 1);
    return NULL;
  }

  return ast_create(AST_VARIABLE);
}

parser_state_t *parse_buffer(const unsigned char *str) {
  parser_state_t *pst = parser_state_alloc();

  if (pst == NULL) {
    return NULL;
  }

  lexer_t *lexer = lexer_scan_buffer(str, strlen(str));

  if (lexer == NULL) {
    parser_state_dtor(pst);
    return NULL;
  }

  lexer_for_each_self(lexer) {
    switch (lexer_curr_get_token_type(lexer)) {
    case TOKEN_OPEN_PARENTHESIS:
    case TOKEN_CLOSE_PARENTHESIS:
      break;
    case TOKEN_VARIABLE:
      pst->ast = pst->ast == NULL
                     ? parse_variable(pst, lexer)
                     : ast_add_child(pst->ast, parse_variable(pst, lexer));
      break;
    }
  }

  // check grouping state after parsing.
  if (parser_state_is_error(pst) == 1) {
    lexer_dtor(lexer);
    parser_dtor(pst);
    return NULL;
  }

  lexer_dtor(lexer);
  return pst;
}

void parser_dump(parser_state_t *pst) {
  if (pst->ast == NULL) {
    return;
  }

  ast_traverse(pst->ast);
}

void parser_dtor(parser_state_t *pst) {
  ast_dtor(pst->ast);
  parser_state_dtor(pst);
}

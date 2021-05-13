#include "parser.h"
#include "lexer.h"
#include "str.h"
#include "token_stack.h"
#include <stdlib.h>
#include <string.h>

parser_t *parser_alloc(void) {
  parser_t *parser = calloc(1, sizeof(parser_t));

  if (parser == NULL) {
    return NULL;
  }

  return parser;
}

static int __parser_is_opar(lexer_val_t *val) {
  if (val == NULL || lexer_val_token_type(val) != TOKEN_OPEN_PARENTHESIS) {
    return 0;
  }

  return 1;
}

static int __parser_is_cpar(lexer_val_t *val) {
  if (val == NULL || lexer_val_token_type(val) != TOKEN_CLOSE_PARENTHESIS) {
    return 0;
  }

  return 1;
}

static int __parser_is_plus(lexer_val_t *val) {
  if (val == NULL || lexer_val_token_type(val) != TOKEN_PLUS_OP) {
    return 0;
  }

  return 1;
}

static void parse_variable(parser_t *parser, lexer_t *lexer) {
  if (parser_is_error(parser)) {
    return;
  }
}

static void parse_addition_expr(parser_t *parser, lexer_t *lexer) {
  if (parser_is_error(parser)) {
    return;
  }
}

parser_t *parse_buffer(const char *buffer) {
  parser_t *parser = parser_alloc();

  if (parser == NULL) {
    return NULL;
  }

  token_stack_t *stack = token_stack_alloc();

  if (stack == NULL) {
    parser_dtor(parser);
    return NULL;
  }

  lexer_t *lexer = lexer_scan_buffer(buffer, strlen(buffer));

  if (lexer == NULL) {
    token_stack_dtor(stack);
    parser_dtor(parser);
    return NULL;
  }

  while (lexer_tail(lexer) != NULL && !parser_is_error(parser)) {
  }

  if (parser_is_error(parser)) {
    lexer_dtor(lexer);
    token_stack_dtor(stack);
    parser_dtor(parser);
    return NULL;
  }

  token_stack_dtor(stack);
  lexer_dtor(lexer);
  return parser;
}

void parser_dtor(parser_t *parser) {
  if (parser_get_ast(parser) != NULL) {
    ast_dtor(parser_get_ast(parser));
  }

  free(parser);
}

#ifdef DEBUG
void parser_dump(parser_t *parser) { ast_dump(parser_get_ast(parser)); }
#endif

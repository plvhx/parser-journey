#include "parser.h"
#include "lexer.h"
#include "str.h"
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

static void parse_variable(parser_t *parser, lexer_t *lexer) {
  if (parser_is_error(parser)) {
    return;
  }

  for (;;) {
    if (!__parser_is_cpar(lexer_get_next(lexer_tail(lexer))) ||
        !__parser_is_opar(lexer_get_prev(lexer_tail(lexer)))) {
      break;
    }

    lexer_val_del(lexer, lexer_get_prev(lexer_tail(lexer)));
    lexer_val_del(lexer, lexer_get_next(lexer_tail(lexer)));
  }

  if (__parser_is_cpar(lexer_get_next(lexer_tail(lexer))) ||
      __parser_is_opar(lexer_get_prev(lexer_tail(lexer)))) {
    parser_set_is_error(parser, 1);
    return;
  }

  if (parser_get_ast(parser) != NULL &&
      ast_get_num_childs(parser_get_ast(parser)) > 0) {
    ast_t *related = ast_get_childs_at(
        parser_get_ast(parser), ast_get_num_childs(parser_get_ast(parser)) - 1);

    if (ast_get_kind(related) == AST_ARITHMETIC_ADD) {
      parser_set_ast(parser, ast_add_child(parser_get_ast(parser),
                                           ast_create(AST_VARIABLE)));

      return;
    }
  }

  parser_set_ast(parser, ast_create(AST_VARIABLE));
}

static void parse_addition_expr(parser_t *parser, lexer_t *lexer) {
  if (parser_is_error(parser)) {
    return;
  }

  // validate first operand.
  if (parser_get_ast(parser) == NULL &&
      ast_get_kind(parser_get_ast(parser)) != AST_VARIABLE) {
    parser_set_is_error(parser, 1);
    return;
  }

  // if pass, assign current ast into
  // 'first_op'
  ast_t *first_op = parser_get_ast(parser);

  // create ast object for '+' operator.
  ast_t *operator = ast_create(AST_ARITHMETIC_ADD);

  // merge collected ast into one
  // expression
  parser_set_ast(parser,
                 ast_create_multi_ex(AST_EXPR, first_op, operator, NULL));
}

parser_t *parse_buffer(const char *buffer) {
  parser_t *parser = parser_alloc();

  if (parser == NULL) {
    return NULL;
  }

  lexer_t *lexer = lexer_scan_buffer(buffer, strlen(buffer));

  if (lexer == NULL) {
    parser_dtor(parser);
    return NULL;
  }

#ifdef DEBUG
  lexer_debug(lexer);
#endif

  lexer_for_each_self(lexer) {
    switch (lexer_tail_token_type(lexer)) {
    case TOKEN_OPEN_PARENTHESIS:
    case TOKEN_CLOSE_PARENTHESIS:
      break;
    case TOKEN_VARIABLE:
      parse_variable(parser, lexer);
      break;
    case TOKEN_PLUS_OP:
      parse_addition_expr(parser, lexer);
      break;
    }
  }

  if (parser_is_error(parser)) {
    lexer_dtor(lexer);
    parser_dtor(parser);
    return NULL;
  }

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

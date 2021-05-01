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

ast_t *parse_variable(lexer_t *lexer) { return ast_create(AST_VARIABLE); }

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
      ++pst->num_op;
      break;
    case TOKEN_CLOSE_PARENTHESIS:
      ++pst->num_cp;
      break;
    case TOKEN_VARIABLE:
      pst->ast = pst->ast == NULL
                     ? parse_variable(lexer)
                     : ast_add_child(pst->ast, parse_variable(lexer));
      break;
    }
  }

  // check grouping state after parsing.
  if (pst->num_cp - pst->num_op != 0) {
    fprintf(stderr, "Wild (%s) has been occurred.\n",
            pst->num_cp - pst->num_op > 0 ? "closing parenthesis"
                                          : "opening parenthesis");

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

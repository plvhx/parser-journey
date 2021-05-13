#include "ast.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast_t *ast_alloc(unsigned int size) {
  ast_t *ast = malloc(size);

  if (ast == NULL) {
    return NULL;
  }

  memset(ast, 0, size);
  return ast;
}

ast_t *ast_realloc(ast_t *ast, unsigned int old_size, unsigned int new_size) {
  ast_t *new = realloc(ast, new_size);

  if (new == NULL) {
    return NULL;
  }

  memset(new + old_size, 0, new_size - old_size);
  return new;
}

unsigned int ast_calculate_child_size(unsigned int size) {
  return (sizeof(ast_t) - sizeof(ast_t *)) + (sizeof(ast_t *) * size);
}

ast_t *ast_create(ast_kind_t ast_kind) {
  ast_t *ast = ast_alloc(ast_calculate_child_size(4));

  if (ast == NULL) {
    return NULL;
  }

  ast_set_kind(ast, ast_kind);
  return ast;
}

static ast_t *ast_create_from_va_list(ast_kind_t ast_kind, va_list args) {
  ast_t *ast = ast_create(ast_kind);
  ast_t *child;

  while ((child = (ast_t *)va_arg(args, ast_t *)) != NULL) {
    ast = ast_add_child(ast, child);
  }

  return ast;
}

ast_t *ast_create_multi_ex(ast_kind_t ast_kind, ...) {
  va_list ap;
  ast_t *ast;

  va_start(ap, ast_kind);
  ast = ast_create_from_va_list(ast_kind, ap);
  va_end(ap);

  return ast;
}

ast_t *ast_add_child(ast_t *ast, ast_t *child) {
  if (ast_get_num_childs(ast) >= ast_get_alloc_size(ast)) {
    ast = ast_realloc(ast, ast_calculate_child_size(ast_get_alloc_size(ast)),
                      ast_calculate_child_size(ast_set_alloc_size(
                          ast, ast_get_alloc_size(ast) + 4)));
  }

  ast_get_childs_at(ast, ast_get_num_childs(ast)++) = child;
  return ast;
}

void ast_dtor(ast_t *ast) {
  if (ast == NULL) {
    return;
  }

  for (int i = 0; i < ast_get_num_childs(ast); i++) {
    ast_dtor(ast_get_childs_at(ast, i));
  }

  free(ast);
}

#ifdef DEBUG
static void ast_dump_kind(ast_t *ast) {
  switch (ast_get_kind(ast)) {
  case AST_VARIABLE:
    printf("(variable, nspaces: %d)\n", ast_get_num_spaces(ast));
    break;
  case AST_EXPR:
    printf("(expression, nspaces: %d)\n", ast_get_num_spaces(ast));
    break;
  case AST_ARITHMETIC_ADD:
    printf("(arithmetic-add, nspaces: %d)\n", ast_get_num_spaces(ast));
    break;
  }
}

void ast_dump(ast_t *ast) {
  if (ast == NULL) {
    return;
  }

  for (int i = 0; i < ast_get_num_spaces(ast); i++) {
    printf(" ");
  }

  ast_dump_kind(ast);

  for (int i = 0; i < ast_get_num_childs(ast); i++) {
    ast_dump(ast_get_childs_at(ast, i));
  }
}
#endif

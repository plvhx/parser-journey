#include "ast.h"
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

static unsigned int ast_calculate_child_alloc_size(unsigned int num_child) {
  return (sizeof(ast_t) - sizeof(ast_t *)) + (sizeof(ast_t *) * num_child);
}

ast_t *ast_create(ast_kind_t ast_kind) {
  ast_t *ast =
      ast_alloc(ast_calculate_child_alloc_size(AST_DEFAULT_CHILD_ALLOC_SIZE));

  if (ast == NULL) {
    return NULL;
  }

  ast_set_kind(ast, ast_kind);
  return ast;
}

ast_t *ast_add_child(ast_t *ast, ast_t *child) {
  if (ast_get_num_childs(ast) >= ast_get_num_childs_alloc_size(ast)) {
    ast = ast_realloc(
        ast, ast_calculate_child_alloc_size(ast_get_num_childs_alloc_size(ast)),
        ast_calculate_child_alloc_size(ast_get_num_childs_alloc_size(ast) +=
                                       AST_DEFAULT_CHILD_ALLOC_SIZE));
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

void ast_traverse(ast_t *ast) {
  if (ast == NULL) {
    return;
  }

  printf("kind: ");

  switch (ast_get_kind(ast)) {
  case AST_VARIABLE:
    printf("(variable)");
    break;
  }

  printf("\n");

  for (int i = 0; i < ast_get_num_childs(ast); i++) {
    ast_traverse(ast_get_childs_at(ast, i));
  }
}

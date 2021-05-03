#ifndef __AST_H__
#define __AST_H__

typedef enum ast_kind { AST_VARIABLE, AST_EXPR, AST_ARITHMETIC_ADD } ast_kind_t;

typedef struct ast {
  ast_kind_t ast_kind;
  unsigned int num_childs;
  unsigned int alloc_size;
  struct ast *childs[1];
} ast_t;

#define ast_get_kind(ast) ((ast)->ast_kind)
#define ast_set_kind(ast, v) (ast_get_kind(ast) = (v))

#define ast_get_num_childs(ast) ((ast)->num_childs)
#define ast_set_num_childs(ast, v) (ast_get_num_childs(ast) = (v))
#define ast_dec_num_childs(ast) (ast_get_num_childs(ast)--)
#define ast_inc_num_childs(ast) (ast_get_num_childs(ast)++)

#define ast_get_alloc_size(ast) ((ast)->alloc_size)
#define ast_set_alloc_size(ast, v) (ast_get_alloc_size(ast) = (v))
#define ast_dec_alloc_size(ast) (ast_get_alloc_size(ast)--)
#define ast_inc_alloc_size(ast) (ast_get_alloc_size(ast)++)

#define ast_get_childs(ast) ((ast)->childs)
#define ast_get_childs_at(ast, i) (ast_get_childs(ast)[(i)])

ast_t *ast_alloc(unsigned int size);
ast_t *ast_realloc(ast_t *ast, unsigned int old_size, unsigned int new_size);
ast_t *ast_create(ast_kind_t ast_kind);
ast_t *ast_create_multi_ex(ast_kind_t ast_kind, ...);
ast_t *ast_add_child(ast_t *ast, ast_t *child);
void ast_dtor(ast_t *ast);

#ifdef DEBUG
void ast_dump(ast_t *ast);
#endif

#endif /* __AST_H__ */

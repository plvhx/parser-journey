#ifndef __PARSER_H__
#define __PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ast.h"

typedef struct parser {
  ast_t *ast;
  int is_error;
} parser_t;

parser_t *parser_alloc(void);
parser_t *parse_buffer(const char *buffer);

#ifdef DEBUG
void parser_dump(parser_t *parser);
#endif

void parser_dtor(parser_t *parser);

#define parser_get_ast(p) ((p)->ast)
#define parser_set_ast(p, a) (parser_get_ast(p) = (a))

#define parser_is_error(p) ((p)->is_error)
#define parser_set_is_error(p, v) (parser_is_error(p) = (v))

#ifdef __cplusplus
}
#endif

#endif /* __PARSER_H__ */

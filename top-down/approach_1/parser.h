#ifndef __PARSER_H__
#define __PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ast.h"

typedef struct parser_state {
  ast_t *ast;
  int is_error;
} parser_state_t;

parser_state_t *parse_buffer(const unsigned char *str);
void parser_dump(parser_state_t *pst);
void parser_dtor(parser_state_t *pst);

#define parser_state_is_error(pst) ((pst)->is_error)
#define parser_state_set_is_error(pst, v) (parser_state_is_error(pst) = (v))

#ifdef __cplusplus
}
#endif

#endif /* __PARSER_H__ */

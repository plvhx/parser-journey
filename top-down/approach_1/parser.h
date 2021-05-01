#ifndef __PARSER_H__
#define __PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ast.h"

typedef struct parser_state {
  ast_t *ast;
  int num_op;
  int num_cp;
} parser_state_t;

parser_state_t *parse_buffer(const unsigned char *str);
void parser_dump(parser_state_t *pst);
void parser_dtor(parser_state_t *pst);

#ifdef __cplusplus
}
#endif

#endif /* __PARSER_H__ */

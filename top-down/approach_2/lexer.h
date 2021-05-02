#ifndef __LEXER_H__
#define __LEXER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "str.h"

typedef enum token_type {
	TOKEN_VARIABLE,
	TOKEN_PLUS_OP,
	TOKEN_OPEN_PARENTHESIS,
	TOKEN_CLOSE_PARENTHESIS
} token_type_t;

typedef struct lexer_val {
	token_type_t token_type;
	union {
		str_t *var;
	} x;
	struct lexer_val *next;
} lexer_val_t;

typedef struct lexer {
	unsigned char c;
	str_t *str;
	lexer_val_t *head;
	lexer_val_t *tail;
} lexer_t;

#define lexer_val_token_type(v) ((v)->token_type)
#define lexer_val_var(v) ((v)->x.var)
#define lexer_val_adv(v) ((v)->next)

#define lexer_get_token(l) ((l)->c)
#define lexer_get_str(l) ((l)->str)
#define lexer_head(l) ((l)->head)
#define lexer_tail(l) ((l)->tail)

#define lexer_head_token_type(l) (lexer_val_token_type(lexer_head(l)))
#define lexer_head_set_token_type(l, t) (lexer_head_token_type(l) = (t))

#define lexer_head_var(l) (lexer_val_var(lexer_head(l)))
#define lexer_head_var_at(l, i) (lexer_head_var(l)[(i)])

#define lexer_head_adv(l) (lexer_val_adv(lexer_head(l)))

#define lexer_tail_token_type(l) (lexer_val_token_type(lexer_tail(l)))
#define lexer_head_set_token_type(l, t) ()
#ifdef __cplusplus
}
#endif

#endif /* __LEXER_H__ */

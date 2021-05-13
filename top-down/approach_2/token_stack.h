#ifndef __TOKEN_STACK_H__
#define __TOKEN_STACK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lexer.h"

typedef struct token_val {
  lexer_val_t *val;
  struct token_val *next;
  struct token_val *prev;
} token_val_t;

typedef struct token_stack {
  unsigned int len;
  token_val_t *head;
  token_val_t *tail;
} token_stack_t;

token_val_t *token_val_alloc(void);
token_val_t *token_val_create(lexer_val_t *lval);
void token_val_dtor(token_val_t *val);
void token_val_dtor_single(token_val_t *val);

token_stack_t *token_stack_alloc(void);
void token_stack_push(token_stack_t *stack, token_val_t *val);
token_val_t *token_stack_pop(token_stack_t *stack);
int token_stack_empty(token_stack_t *stack);
void token_stack_dtor(token_stack_t *stack);

#define token_val_val(t) ((t)->val)
#define token_val_next(t) ((t)->next)
#define token_val_prev(t) ((t)->prev)

#define token_val_set_val(t, v) (token_val_val(t) = (v))
#define token_val_set_next(t, v) (token_val_next(t) = (v))
#define token_val_set_prev(t, v) (token_val_prev(t) = (v))

#define token_stack_len(s) ((s)->len)
#define token_stack_set_len(s, l) (token_stack_len(s) = (v))
#define token_stack_inc_len(s) (token_stack_len(s)++)
#define token_stack_dec_len(s) (token_stack_len(s)--)

#define token_stack_head(s) ((s)->head)
#define token_stack_tail(s) ((s)->tail)

#define token_stack_rewind_tail(s) (token_stack_tail(s) = token_stack_head(s))

#define token_stack_set_head(s, v) (token_stack_head(s) = (v))
#define token_stack_set_tail(s, v) (token_stack_tail(s) = (v))

#ifdef __cplusplus
}
#endif

#endif /* __TOKEN_STACK_H__ */

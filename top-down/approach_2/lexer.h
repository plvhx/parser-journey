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
  struct lexer_val *prev;
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

#define lexer_set_token(l, v) (lexer_get_token(l) = (v))
#define lexer_set_str(l, s) (lexer_get_str(l) = (s))

#define lexer_get_next(l) ((l)->next)
#define lexer_get_prev(l) ((l)->prev)

#define lexer_set_next(l, v) (lexer_get_next(l) = (v))
#define lexer_set_prev(l, v) (lexer_get_prev(l) = (v))

#define lexer_head(l) ((l)->head)
#define lexer_tail(l) ((l)->tail)

#define lexer_set_head(l, v) (lexer_head(l) = (v))
#define lexer_set_tail(l, v) (lexer_tail(l) = (v))

#define lexer_head_token_type(l) (lexer_val_token_type(lexer_head(l)))
#define lexer_head_set_token_type(l, t) (lexer_head_token_type(l) = (t))

#define lexer_head_var(l) (lexer_val_var(lexer_head(l)))
#define lexer_head_var_at(l, i) (lexer_head_var(l)[(i)])

#define lexer_head_adv(l) (lexer_val_adv(lexer_head(l)))

#define lexer_tail_token_type(l) (lexer_val_token_type(lexer_tail(l)))
#define lexer_tail_set_token_type(l, t) (lexer_tail_token_type(l) = (t))

#define lexer_tail_var(l) (lexer_val_var(lexer_tail(l)))
#define lexer_tail_set_var(l, v) (lexer_tail_var(l) = (v))
#define lexer_tail_var_at(l, i) (lexer_tail_var(l)[(i)])

#define lexer_tail_adv(l) (lexer_get_next(lexer_tail(l)))

#define lexer_rewind_tail(l) (lexer_tail(l) = lexer_head(l))

#define lexer_for_each_self(l)                                                 \
  for (lexer_rewind_tail(l); lexer_tail(l) != NULL;                            \
       lexer_set_tail(l, lexer_tail_adv(l)))

#define lexer_peek(l)                                                          \
  do {                                                                         \
    if (lexer_get_next(l) == NULL)                                             \
      (NULL);                                                                  \
    (lexer_get_next(l));                                                       \
  } while (0)

lexer_val_t *lexer_val_alloc(void);
void lexer_val_del(lexer_t *lexer, lexer_val_t *val);
void lexer_val_dtor(lexer_val_t *val);

lexer_t *lexer_alloc(void);
void lexer_add(lexer_t *lexer, lexer_val_t *val);
void lexer_add_type(lexer_t *lexer, token_type_t ttype);
void lexer_dtor(lexer_t *lexer);

lexer_t *lexer_scan_buffer(const char *buffer, unsigned int buflen);

#ifdef DEBUG
void lexer_debug(lexer_t *lexer);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LEXER_H__ */

#ifndef __LEXER_H__
#define __LEXER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "str.h"

typedef enum token_kind {
  TOKEN_VARIABLE,
  TOKEN_OPEN_PARENTHESIS,
  TOKEN_CLOSE_PARENTHESIS
} token_kind_t;

typedef struct lexer_val {
  token_kind_t token_type;
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

lexer_t *lexer_alloc(void);
void lexer_add(lexer_t *lexer, lexer_val_t *val);
void lexer_del(lexer_t *lexer, lexer_val_t *val);
void lexer_add_value(lexer_t *lexer, token_kind_t token_type);
void lexer_dtor(lexer_t *lexer);

lexer_val_t *lexer_val_alloc(void);
void lexer_val_del(lexer_t *lexer, lexer_val_t *val);
void lexer_val_dtor(lexer_val_t *val);

#define lexer_head(lex) ((lex)->head)
#define lexer_tail(lex) ((lex)->tail)

#define lexer_set_head(lex, v) (lexer_head(lex) = (v))
#define lexer_set_tail(lex, v) (lexer_tail(lex) = (v))

#define lexer_get_next(lex) ((lex)->next)
#define lexer_set_next(lex, v) (lexer_get_next(lex) = (v))

#define lexer_get_prev(lex) ((lex)->prev)
#define lexer_set_prev(lex, v) (lexer_get_prev(lex) = (v))

#define lexer_rewind_tail(lex) (lexer_tail(lex) = lexer_head(lex))

#define lexer_head_get_token_type(lex) (lexer_head(lex)->token_type)
#define lexer_head_set_token_type(lex, v) (lexer_head_get_token_type(lex) = (v))

#define lexer_head_get_var(lex) (lexer_head(lex)->x.var)
#define lexer_head_set_var(lex, v) (lexer_head_get_var(lex) = (v))

#define lexer_tail_get_token_type(lex) (lexer_tail(lex)->token_type)
#define lexer_tail_set_token_type(lex, v) (lexer_tail_get_token_type(lex) = (v))

#define lexer_tail_get_var(lex) (lexer_tail(lex)->x.var)
#define lexer_tail_set_var(lex, v) (lexer_tail_get_var(lex) = (v))

#define lexer_curr(lex) (lexer_tail(lex))
#define lexer_curr_get_token_type(lex) (lexer_tail_get_token_type(lex))
#define lexer_curr_set_token_type(lex, v) (lexer_curr_get_token_type(lex) = (v))

#define lexer_tail_move_next(lex) (lexer_tail(lexer) = lexer_tail(lexer)->next)

#define lexer_for_each_self(lex)                                               \
  for (lexer_rewind_tail(lex); lexer_tail(lex) != NULL;                        \
       lexer_tail_move_next(lex))

#define lexer_get_curr_token(lex) ((lex)->c)
#define lexer_set_curr_token(lex, v) ((lex)->c = (v))

#ifdef DEBUG
void lexer_debug(lexer_t *lexer);
#endif

lexer_t *lexer_scan_buffer(const char *buffer, unsigned int buflen);

#ifdef __cplusplus
}
#endif

#endif /* __LEXER_H__ */

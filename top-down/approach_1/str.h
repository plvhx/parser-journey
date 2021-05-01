#ifndef __STR_H__
#define __STR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define STR_MAX_LEN 256

typedef struct str_val {
  unsigned int len;
  char str[STR_MAX_LEN + 1];
  struct str_val *next;
} str_val_t;

typedef struct str {
  unsigned int index;
  str_val_t *head;
  str_val_t *tail;
} str_t;

str_t *str_alloc(void);
void str_add(str_t *str, str_val_t *val);
void str_push_byte(str_t *str, unsigned char c);
void str_dtor(str_t *str);
unsigned char str_next_byte(str_t *str);

str_val_t *str_val_alloc(void);
void str_val_dtor(str_val_t *val);

#define str_dec_index(s) ((s)->index--)
#define str_inc_index(s) ((s)->index++)
#define str_get_index(s) ((s)->index)
#define str_rewind_index(s) ((s)->index = 0)

#define str_head(s) ((s)->head)
#define str_tail(s) ((s)->tail)

#define str_set_head(s, v) (str_head(s) = (v))
#define str_set_tail(s, v) (str_tail(s) = (v))

#define str_get_next(s) ((s)->next)
#define str_set_next(s, v) ((s)->next = (v))

#define str_head_get_len(s) (str_head(s)->len)
#define str_head_get_str(s) (str_head(s)->str)
#define str_head_get_str_at(s, i) (str_head_get_str(s)[i])

#define str_tail_get_len(s) (str_tail(s)->len)
#define str_tail_get_str(s) (str_tail(s)->str)
#define str_tail_get_str_at(s, i) (str_tail_get_str(s)[i])

#define str_val_get_next(s) (str_get_next(s))
#define str_val_move_next(s) ((s) = str_val_get_next(s))

#define str_curr_len(s) (str_tail_get_len(s))
#define str_curr_str(s) (str_tail_get_str(s))
#define str_curr_str_at(s, i) (str_tail_get_str_at(s, i))

#define str_rewind_tail(s) (str_tail(s) = str_head(s))

#ifdef __cplusplus
}
#endif

#endif /* __STR_H__ */

#ifndef __STR_H__
#define __STR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define STR_MAX_LEN 256

typedef struct str_val {
	unsigned int len;
	unsigned char buf[STR_MAX_LEN + 1];
	struct str_val *next;
} str_val_t;

typedef struct str {
	unsigned int index;
	str_val_t *head;
	str_val_t *tail;
} str_t;

#define str_get_index(s) ((s)->index)
#define str_set_index(s, v) ((s)->index = (v))
#define str_dec_index(s) ((s)->index--)
#define str_inc_index(s) ((s)->index++)

#define str_head(s) ((s)->head)
#define str_tail(s) ((s)->tail)

#define str_set_head(s, v) (str_head(s) = (v))
#define str_set_tail(s, v) (str_tail(s) = (v))

#define str_head_get_len(s) (str_head(s)->len)
#define str_head_set_len(s, v) (str_head_get_len(s) = (v))
#define str_head_dec_len(s, v) (str_head_get_len(s)--)
#define str_head_inc_len(s, v) (str_head_get_len(s)++)

#define str_head_get_buf(s) (str_head(s)->buf)
#define str_head_get_buf_at(s, i) (str_head_get_buf(s)[(i)])

#define str_tail_get_len(s) (str_tail(s)->len)
#define str_tail_set_len(s, v) (str_tail_get_len(s) = (v))
#define str_tail_dec_len(s, v) (str_tail_get_len(s)--)
#define str_tail_inc_len(s, v) (str_tail_get_len(s)++)

#define str_tail_get_buf(s) (str_tail(s)->buf)
#define str_tail_get_buf_at(s, i) (str_tail_get_buf(s)[(i)])

#define str_adv_tail(s) (str_tail(s) = str_tail(s)->next)
#define str_next_tail(s) (str_tail(s)->next)
#define str_set_next_tail(s, v) (str_next_tail(s) = (v))
#define str_rewind_tail(s) (str_tail(s) = str_head(s))

#define str_curr_get_len(s) (str_tail_get_len(s))
#define str_curr_set_len(s, v) (str_tail_set_len(s, v))
#define str_curr_dec_len(s) (str_tail_dec_len(s))
#define str_curr_inc_len(s) (str_tail_inc_len(s))

#define str_curr_get_buf(s) (str_tail_get_buf(s))
#define str_curr_get_buf_at(s, i) (str_tail_get_buf_at(s, i))

#define str_adv_curr(s) (str_adv_tail(s))
#define str_curr_next(s) (str_next_tail(s))

str_val_t *str_val_alloc(void);
void str_val_dtor(str_val_t *val);

str_t *str_alloc(void);
void str_add(str_t *str, str_val_t *val);
void str_add_byte(str_t *str, unsigned char c);
unsigned char str_next_byte(str_t *str);
void str_dtor(str_t *str);

#ifdef __cplusplus
}
#endif

#endif /* __STR_H__ */

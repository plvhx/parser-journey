#include <stdlib.h>
#include "str.h"

str_val_t *str_val_alloc(void) {
	str_val_t *val = calloc(1, sizeof(str_val_t));

	if (val == NULL) {
		return NULL;
	}

	return val;
}

void str_val_dtor(str_val_t *val) {
	if (val == NULL) {
		return;
	}

	str_val_dtor(val->next);
	free(val);
}

str_t *str_alloc(void) {
	str_t *str = calloc(1, sizeof(str_t));

	if (str == NULL) {
		return NULL;
	}

	return str;
}

void str_add(str_t *str, str_val_t *val) {
	if (str_head(str) == NULL && str_tail(str) == NULL) {
		str_set_head(str, val);
		str_set_tail(str, val);
		return;
	}

	str_set_next_tail(str, val);
	str_set_tail(str, val);
	return;
}

void str_add_byte(str_t *str, unsigned char c) {
	if ((str_head(str) == NULL &&
		 str_tail(str) == NULL) ||
		str_curr_get_len(str) == STR_MAX_LEN) {
		str_add(str, str_val_alloc());
	}

	str_curr_get_buf(str, str_curr_inc_len(str)) = c;
}

unsigned char str_next_byte(str_t *str) {
	if (str_get_index(str) == STR_MAX_LEN &&
		str_curr_next(str) != NULL) {
		str_adv_curr(str);
		str_set_index(str, 0);
	}

	return str_curr_get_buf_at(str, str_inc_index(str));
}

void str_dtor(str_t *str) {
	str_rewind_tail(str);
	str_val_dtor(str_tail(str));
	free(str);
}

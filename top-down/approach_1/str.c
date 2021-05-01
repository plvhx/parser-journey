#include "str.h"
#include <stdlib.h>

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

  str_set_next(str_tail(str), val);
  str_set_tail(str, val);
  return;
}

void str_push_byte(str_t *str, unsigned char c) {
  if (str_curr_len(str) == STR_MAX_LEN) {
    return;
  }

  str_curr_str_at(str, str_curr_len(str)++) = c;
  return;
}

void str_dtor(str_t *str) {
  str_rewind_tail(str);
  str_val_dtor(str_tail(str));
  free(str);
}

unsigned char str_next_byte(str_t *str) {
  if (str_curr_len(str) == STR_MAX_LEN &&
      str_val_get_next(str_tail(str)) != NULL) {
    str_val_move_next(str_tail(str));
    str_rewind_index(str);
  }

  return str_curr_str_at(str, str_get_index(str)++);
}

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

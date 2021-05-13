#include "token_stack.h"
#include <stdlib.h>

token_val_t *token_val_alloc(void) {
  token_val_t *val = calloc(1, sizeof(token_val_t));

  if (val == NULL) {
    return NULL;
  }

  return val;
}

token_val_t *token_val_create(lexer_val_t *lval) {
  token_val_t *val = token_val_alloc();

  if (val == NULL) {
    return NULL;
  }

  token_val_set_val(val, lval);
  return val;
}

void token_val_dtor(token_val_t *val) {
  if (val == NULL) {
    return;
  }

  token_val_dtor(token_val_next(val));
  token_val_dtor_single(val);
}

void token_val_dtor_single(token_val_t *val) {
  if (val == NULL) {
    return;
  }

  free(val);
}

token_stack_t *token_stack_alloc(void) {
  token_stack_t *stack = calloc(1, sizeof(token_stack_t));

  if (stack == NULL) {
    return NULL;
  }

  return stack;
}

void token_stack_push(token_stack_t *stack, token_val_t *val) {
  if (token_stack_head(stack) == NULL && token_stack_tail(stack) == NULL) {
    token_val_set_next(val, NULL);
    token_val_set_prev(val, NULL);
    token_stack_set_head(stack, val);
    token_stack_set_tail(stack, val);
    token_stack_inc_len(stack);
    return;
  }

  token_val_set_next(val, NULL);
  token_val_set_prev(val, token_stack_tail(stack));

  token_val_set_next(token_stack_tail(stack), val);
  token_stack_set_tail(stack, val);
  token_stack_inc_len(stack);
}

token_val_t *token_stack_pop(token_stack_t *stack) {
  if (token_stack_empty(stack)) {
    return NULL;
  }

  token_val_t *val = token_stack_tail(stack);

  if (token_stack_tail(stack) != token_stack_head(stack)) {
    token_val_set_next(token_val_prev(token_stack_tail(stack)), NULL);
    token_stack_set_tail(stack, token_val_prev(token_stack_tail(stack)));
    goto finalize;
  }

  token_stack_set_head(stack, NULL);
  token_stack_set_tail(stack, NULL);

finalize:
  token_stack_dec_len(stack);
  return val;
}

int token_stack_empty(token_stack_t *stack) {
  return !token_stack_len(stack) ? 1 : 0;
}

void token_stack_dtor(token_stack_t *stack) {
  token_stack_rewind_tail(stack);
  token_val_dtor(token_stack_tail(stack));
  free(stack);
}

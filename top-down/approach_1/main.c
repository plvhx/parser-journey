#include "ast.h"
#include "parser.h"
#include <stdio.h>

#define unused(x) ((void)(x))

static void test_case_0(void) {
  parser_state_t *pst = parse_buffer("foo_bar_baz");

  if (pst == NULL) {
    return;
  }

  printf("----- %s -----\n", __func__);
  parser_dump(pst);
  printf("\n");

  parser_dtor(pst);
}

static void test_case_1(void) {
  parser_state_t *pst = parse_buffer("(foo_bar_baz)");

  if (pst == NULL) {
    return;
  }

  printf("----- %s -----\n", __func__);
  parser_dump(pst);
  printf("\n");

  parser_dtor(pst);
}

static void test_case_2(void) {
  parser_state_t *pst = parse_buffer("((foo_bar_baz))");

  if (pst == NULL) {
    return;
  }

  printf("----- %s -----\n", __func__);
  parser_dump(pst);
  printf("\n");

  parser_dtor(pst);
}

static void test_case_3(void) {
  parser_state_t *pst = parse_buffer("(((foo_bar_baz)))");

  if (pst == NULL) {
    return;
  }

  printf("----- %s -----\n", __func__);
  parser_dump(pst);
  printf("\n");

  parser_dtor(pst);
}

static void test_case_4(void) {
  parser_state_t *pst = parse_buffer("((((foo_bar_baz))))");

  if (pst == NULL) {
    return;
  }

  printf("----- %s -----\n", __func__);
  parser_dump(pst);
  printf("\n");

  parser_dtor(pst);
}

static void test_case_5__closing_parenthesis_overload(void) {
  parser_state_t *pst = parse_buffer("((((foo_bar_baz)))))");
}

static void test_case_5__opening_parenthesis_overload(void) {
  parser_state_t *pst = parse_buffer("(((((foo_bar_baz))))");
}

int main(int argc, char **argv) {
  unused(argc);
  unused(argv);

  test_case_0();
  test_case_1();
  test_case_2();
  test_case_3();
  test_case_4();
  test_case_5__closing_parenthesis_overload();
  test_case_5__opening_parenthesis_overload();

  return 0;
}

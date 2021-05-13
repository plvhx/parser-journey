#include "parser.h"
#include <stddef.h>
#include <stdio.h>

#define unused(x) ((void)(x))

static void test_case_0(void) {
  printf("---- %s ----\n", __func__);
  parser_t *parser = parse_buffer("a + b");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  printf("\n");
  parser_dtor(parser);
}

static void test_case_1(void) {
  printf("---- %s ----\n", __func__);
  parser_t *parser = parse_buffer("(a) + b");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  printf("\n");
  parser_dtor(parser);
}

static void test_case_2(void) {
  printf("---- %s ----\n", __func__);
  parser_t *parser = parse_buffer("a + (b)");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  printf("\n");
  parser_dtor(parser);
}

static void test_case_3(void) {
  printf("---- %s ----\n", __func__);
  parser_t *parser = parse_buffer("(a) + (b)");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  printf("\n");
  parser_dtor(parser);
}

static void test_case_4(void) {
  printf("---- %s ----\n", __func__);
  parser_t *parser = parse_buffer("(a + b) + c");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  printf("\n");
  parser_dtor(parser);
}

static void test_case_5(void) {
  printf("---- %s ----\n", __func__);
  parser_t *parser = parse_buffer("a + (b + c)");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  printf("\n");
  parser_dtor(parser);
}

int main(int argc, char **argv) {
  unused(argc);
  unused(argv);

  test_case_0();
  test_case_1();
  test_case_2();
  test_case_3();

  test_case_4();
  test_case_5();
  return 0;
}

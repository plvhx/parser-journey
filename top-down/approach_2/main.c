#include "parser.h"
#include <stddef.h>

#define unused(x) ((void)(x))

static void test_case_0(void) {
  parser_t *parser = parse_buffer("a + b");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  parser_dtor(parser);
}

static void test_case_1(void) {
  parser_t *parser = parse_buffer("(a) + b");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  parser_dtor(parser);
}

static void test_case_2(void) {
  parser_t *parser = parse_buffer("a + (b)");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  parser_dtor(parser);
}

static void test_case_3(void) {
  parser_t *parser = parse_buffer("(a) + (b)");

  if (parser == NULL) {
    return;
  }

  parser_dump(parser);
  parser_dtor(parser);
}

int main(int argc, char **argv) {
  unused(argc);
  unused(argv);

  test_case_0();
  test_case_1();
  test_case_2();
  test_case_3();
  return 0;
}

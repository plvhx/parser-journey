#include "ast.h"
#include "parser.h"
#include <stdio.h>

#define unused(x) ((void)(x))

int main(int argc, char **argv) {
  unused(argc);
  unused(argv);

  parser_state_t *pst = parse_buffer("((((foo_bar_baz))))");

  if (pst == NULL) {
    return 1;
  }

  parser_dump(pst);
  parser_dtor(pst);
  return 0;
}

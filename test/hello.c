#include <stdio.h>

static int foo(void) {
  int x = 10;
  int y = 10;
  return x - y;
}

int main() {
  return foo();
}

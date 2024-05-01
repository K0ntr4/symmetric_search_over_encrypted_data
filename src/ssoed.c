#include "../include/ssoed.h"

int main(void) {
  if (sodium_init() < 0) {
    perror("Sodium initialization failed");
    exit(EXIT_FAILURE);
  }

  basic_scheme();
  controlled_scheme();
  controlled_chapter_scheme();
  controlled_hierarchical_scheme();
  return 0;
}

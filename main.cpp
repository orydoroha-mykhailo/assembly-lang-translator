#include "translator.h"

int main() {
  Translator translator("test.asm");
  translator.createListing();
  return 0;
}

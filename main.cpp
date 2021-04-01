#include "translator.h"

int main() {
  Translator translator("test.asm");
  translator.createListing();
  translator.outAllLexems();
  return 0;
}

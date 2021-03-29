#include "analysis.h"
#include <string>
#include <iomanip>

bool isTextConstant(const std::string& str) {
  if ((str[0] == '\'' || str[0] == '"') 
      && (str.back() == '\'' || str.back() == '"')) {
    return true;
  } 
  return false;
}

std::string getLexemType(const Lexem& lexem) {
  if (isTextConstant(lexem)) {
    return "text constant";
  } 
  else if (isdigit(lexem[0]) || lexem[0] == '-') {
    if (lexem.back() == 'H') {
      return "hex constant";
    }
    else if (lexem.back() == 'B') {
      return "binary constant";
    }
    else if (isdigit(lexem.back()) 
        || lexem.back() == 'D') {
      return "decimal constant";
    }
    else {
      throw std::runtime_error(lexem);
    }
  }
  else if (isAsmLexem(lexem)) {
    return asm_dict.at(lexem);
  }
  else {
    return "user identifier";
  }
}
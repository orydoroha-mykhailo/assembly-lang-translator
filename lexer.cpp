#include "lexer.h"

using namespace std;

Expression parseLineToExpression(const std::string& s) {
  Expression result;
  
  Lexem current_lexem;
  for (size_t i = 0; i < s.size(); i++) {
    if (s.at(i) == ';') 
    {
      result.push_back(";");
      result.push_back({s.begin() + i + 1, s.end() - 1});
      break;
    }

    if (s.at(i) == ' ' || s.at(i) == '\t' || s.at(i) == '\r' || isCharacterLexem(s.at(i))) {
      if (!current_lexem.empty()) {
        result.push_back(current_lexem);
        current_lexem.clear();
      }

      if (isCharacterLexem(s.at(i))) 
        result.push_back({s.at(i)});
    } 
    else if (s.at(i) == '\'' || s.at(i) == '"') {
      current_lexem.push_back(s.at(i++));
      while (s.at(i) != '\'' && s.at(i) != '"') {
        current_lexem.push_back(s.at(i));
        ++i;
      }
      current_lexem.push_back(s.at(i));
      result.push_back(current_lexem);
      current_lexem.clear();
    } 
    else {
      current_lexem.push_back(s.at(i));
    }
  }

  if (!current_lexem.empty()) {
    result.push_back(current_lexem);
  }

  return result;
}

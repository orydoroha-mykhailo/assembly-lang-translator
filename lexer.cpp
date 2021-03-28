#include "lexer.h"

using namespace std;

Lexems parseIntoLexems(const std::string& s) {
  Lexems result;
  
  Lexem current_lexem;
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == ';') 
      break;

    if (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || isCharacterLexem(s[i])) {
      if (!current_lexem.empty()) {
        result.push_back(current_lexem);
        current_lexem.clear();
      }

      if (isCharacterLexem(s[i])) 
        result.push_back({s[i]});
    } 
    else if (s[i] == '\'' || s[i] == '"') {
      current_lexem.push_back(s[i++]);
      while (s[i] != '\'' && s[i] != '"') {
        current_lexem.push_back(s[i]);
        ++i;
      }
      current_lexem.push_back(s[i]);
      result.push_back(current_lexem);
      current_lexem.clear();
    } 
    else {
      current_lexem.push_back(s[i]);
    }
  }

  if (!current_lexem.empty()) {
    result.push_back(current_lexem);
  }

  return result;
}

#include "analysis.h"
using namespace std;
#include <string>
#include <sstream>
#include <iomanip>

bool isTextConstant(const std::string& str) {
  if ((str[0] == '\'' || str[0] == '"') 
      && (str.back() == '\'' || str.back() == '"')) {
    return true;
  } 
  return false;
}

std::string lexemTypeToStr(const LEXEM_TYPE& lt) {
  return Lexem_type_to_string.at(lt);
}

LEXEM_TYPE getLexemType(const Lexem& lexem) {
  if (isTextConstant(lexem)) {
    return LEXEM_TYPE::TEXT_CONST;
  } 
  else if (isdigit(lexem[0]) || lexem[0] == '-') {
    if (lexem.back() == 'H') {
      return LEXEM_TYPE::HEX_CONST;
    }
    else if (lexem.back() == 'B') {
      return LEXEM_TYPE::BIN_CONST;
    }
    else if (isdigit(lexem.back()) 
        || lexem.back() == 'D') {
      return LEXEM_TYPE::DEC_CONST;
    }
    else {
      throw std::runtime_error(lexem);
    }
  }
  else if (isAsmLexem(lexem)) {
    try
    {
      return asm_dict_to_lexem_type.at(asm_dict.at(lexem));
    }
    catch(const std::exception& e)
    {
      return LEXEM_TYPE::U_ID;
    }
    
  }
  else {
    return LEXEM_TYPE::U_ID;
  }
}

std::string getExpressionTable(const Expression& expression) {
    std::stringstream result;
  result << 
    "-----------------------------------------------------------------------\n"
    "|  №  |       Lexem       |   Length   |             Type             |\n"
    "-----------------------------------------------------------------------\n";

  size_t num = 1;
  for (const auto& lexem : expression) {
    result << "| " << setw(3) << num++;
    result << " | " << setw(17) << (isTextConstant(lexem) 
                                   ? std::string{lexem.begin() + 1, lexem.end() - 1} 
                                   : lexem);
    result << " | " << setw(6) << lexem.size() << setw(7);
    result << " | " << setw(28) << lexemTypeToStr(getLexemType(lexem)) << " |\n";
  }
  result << "-----------------------------------------------------------------------";
  return result.str();
}

std::string getSentenceStructure(const Expression& expression) {
  stringstream result;
  result << 
    "-----------------------------------------------------------------------\n"
    "|       |  Mnemocode  |   Operand 1   |   Operand 2   |   Operand 3   |\n"
    "| Label |-------------|------------------------------------------------\n"
    "|       |   №  |  Lex |    №  |  Lex  |    №  |  Lex  |    №  |  Lex  |\n"
    "-----------------------------------------------------------------------\n";

  bool lable = !isAsmLexem(expression.front());
  result << "| " << setw(3) << lable << "  ";
  
  if (expression.size() == 2 && expression.back() == ":") {
    result << " | " << setw(4) << 0 << " | " << setw(4) << 0;
    result << " | " << setw(5) << 0 << " | " << setw(4) << 0;
    result << " | " << setw(5) << 0 << " | " << setw(4) << 0;
    result << " | " << setw(5) << 0 << " | " << setw(4) << 0 << " | \n";
  }
  else {
    bool colon = expression.size() > 2 && expression[1] == ":";
    
    result << " | " << setw(4) << 1 + lable + colon << " | " << setw(4) << 1;

    size_t op1 = 0;
    size_t op1_cnt = 0;
    for (size_t i = 1 + lable + colon; i < expression.size() && expression[i] != ","; i++) {
      op1 = 2 + lable + colon;
      op1_cnt++;
    }

    result << " | " << setw(5) << op1 << " | " << setw(5) << op1_cnt;

    if (op1 != 0 && op1 + op1_cnt - 1 < expression.size()) {
      size_t op2 = 0;
      size_t op2_cnt = 0;
      for (size_t i = op1 + op1_cnt; i < expression.size() && expression[i] != ","; i++) {
        op2 = op1 + op1_cnt + 1;
        op2_cnt++;
      }

      result << " | " << setw(5) << op2 << " | " << setw(5) << op2_cnt;

      if (op2 != 0 && op2 + op2_cnt - 1 < expression.size()) {
        result << " | " << setw(5) << op2 + op2_cnt + 1 
          << " | " << setw(5) << expression.size() - 3 - op2_cnt - op1_cnt - lable - colon << " | \n";
      }
      else
      result << " | " << setw(5) << 0 << " | " << setw(5) << 0 << " | \n";
    }

    else {
      result << " | " << setw(5) << 0 << " | " << setw(5) << 0;
      result << " | " << setw(5) << 0 << " | " << setw(5) << 0 << " | \n";
    }
  }

  result << "-----------------------------------------------------------------------";

  return result.str();
}

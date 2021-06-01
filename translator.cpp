#include "translator.h"

using namespace std;

#include <cctype>
#include <iomanip>
#include <string>
#include <set>
int MOV_validator(const Expression& expression, size_t& offset);
bool isAsmReg(const Lexem& lexem);
bool isReg_32(const Lexem& lexem);
bool isNumber(const Lexem& str);
bool isIMM(const Lexem& str);
int mem_validator(const Expression& expression,
 const size_t& bracket_id, size_t& end_id);

string ToUpper(string str) {
  for (size_t i = 0; i < str.size(); i++) 
    str[i] = toupper(str[i]);
  return str;
}

string covertNameToLstExt(string asm_file_name) {
  if (asm_file_name.size() > 4) {
      return std::string{asm_file_name.begin(), asm_file_name.end() - 4} + ".lst";
  }
  return asm_file_name;
}

ostream& operator<<(ostream& ostr, const Expression& expression) {
  for (const auto& lexem : expression) 
    ostr << lexem << " ";
  return ostr;
}

Translator::Translator(const std::string& file_name) {
  file_name_ = file_name;
  ifstream asm_file(file_name);
  ofstream listing_file(covertNameToLstExt(file_name_));
  listing_file << "Assembly Translator.\nWritten by Orydoroha Mykhailo.\n" << endl 
    << "Filename: " << file_name_ << setfill('\n') << setw(4) << "";
  listing_file.close();
  for (string line; getline(asm_file, line); ) {
    if (auto expression = parseLineToExpression(ToUpper(line)); !expression.empty()) {
      all_expressions_.push_back(expression);
    }
  }
}

void Translator::outAllLexems() {
  ofstream listing_file(covertNameToLstExt(file_name_), ios::app);

  set<Lexem> all_expressions;

  for (const auto& expression: all_expressions_) {
    all_expressions.insert(expression.begin(), expression.end());
  }

  listing_file << getExpressionTable({all_expressions.begin(), all_expressions.end()}) << "\n";
}

// Old listing from lexer-step
/*
void Translator::createListing() {
  ofstream listing_file(covertNameToLstExt(file_name_));

  for (const auto& expression: all_expressions_) {
    listing_file << expression << endl; 
    listing_file << getExpressionTable(expression) << endl;
    listing_file << getSentenceStructure(expression) << endl << endl;
  }
}
*/

void Translator::createListing() {
  size_t offset = 0;
  size_t line_num = 0;
  size_t current_offset;
  for (const auto& line: all_expressions_) {
    current_offset = 0;
    if(!validate_expression(line, current_offset)) {
      offset += current_offset;
      line_num++;
      release_expression(line, line_num, offset);
    }
    //else {
      // throw exception
    //}
  }
}


// returns 0 if creation succsessful
// otherwise returns error code
int Translator::validate_expression(const Expression& expression, size_t& offset) {
  if(getAsmLexemType(expression[0]) == LEXEM_TYPE::MACHINE_INSTRUCT) {
      /* Review all machine instructions */
    if(asm_dict.at(expression.front())
     == ASM_DICT::MOV) {
       try
       {
        if(!MOV_validator(expression, offset)){
          return 0;
        }
       }
       catch(const std::exception& e)
       {
         return -1;
       }
       return -1;
       
     }
  }
  else if(getAsmLexemType(expression.back()) == LEXEM_TYPE::SEGMENT_INSTRUCTION) {
    if(expression.back() == "SEGMENT") {
      return 0;
    }
    else if(expression.front() == "ENDS") {
      return 0;
    }
  }
  else if(expression.back() == ":") {
    /* Label */
      return 0;
  }
  else if(expression.front() == ";") {
    /* Commment */
      return 0;
  }

}

void Translator::release_expression(const Expression& expression,
 const size_t& line_num, const size_t& offset) const {
  ofstream listing_file(covertNameToLstExt(file_name_), ios_base::app);

  listing_file << setfill(' ');
  listing_file << setw(6) << line_num << "\t" << setw(4);
  listing_file << setfill('0');
  listing_file << setw(4) << std::hex << offset;
  listing_file << setfill(' ');
  listing_file << setw(20)  << ""/* flags */;
  listing_file << expression << endl;
  listing_file.close();
}

// seems like done!
int MOV_validator(const Expression& expression, size_t& offset) {
  if(expression.size() > 1){
    if(expression[0] == "MOV"){
      // MOV mem, imm
      size_t id;
      if(!mem_validator(expression, 1, id)) {
        if(expression[id++] == ",") {
          // imm check
          if(isIMM(expression[id++])){
            return 0;
          }
        }
      }
      else if(expression[1] == "DWORD" && expression[2] == "PTR") {
        if(!mem_validator(expression, 3, id)) {
          if(expression[id++] == ",") {
            // imm check
            if(isIMM(expression[id++])){
              if(expression[id++] == ";") {
                return 0;
              }
              else if(expression.size() == id){
                return 0;
              }
            }
          }
        }
      }
      else if(expression[1] == "WORD" && expression[2] == "PTR") {
        if(!mem_validator(expression, 3, id)) {
          if(expression[id++] == ",") {
            // imm check
            if(isIMM(expression[id++])){
              if(expression[id++] == ";") {
                return 0;
              }
              else if(expression.size() == id){
                return 0;
              }
            }
          }
        }
      }
      else if(expression[1] == "BYTE" && expression[2] == "PTR") {
        if(!mem_validator(expression, 3, id)) {
          if(expression[id++] == ",") {
            // imm check
            if(isIMM(expression[id++])){
              if(expression[id++] == ";") {
                return 0;
              }
              else if(expression.size() == id){
                return 0;
              }
            }
          }
        }
      }
      // MOV reg, mem 
      else if(isAsmReg(expression[1])){
        if(expression[2] == ","){
          if(!mem_validator(expression, 3, id)){
            return 0;
          }
          else if(expression[3] == "DWORD" && expression[4] == "PTR") {
            if(isReg_32(expression[1])) {
              if(!mem_validator(expression, 5, id)){
                return 0;
              }
            }
          }
          else if(expression[3] == "WORD" && expression[4] == "PTR") {
            if(getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_32
             || getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_16) {
              if(!mem_validator(expression, 5, id)){
                return 0;
              }
            }
          }
          else if(expression[3] == "BYTE" && expression[4] == "PTR") {
            if(getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_32
             || getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_16
             ||  getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_8) {
              if(!mem_validator(expression, 5, id)){
                return 0;
              }
            }
          }
        }
      }
    }
  }
  return -1;
}

int mem_validator(const Expression& expression, const size_t& bracket_start_id, size_t& end_id) {
  try {
    size_t i = bracket_start_id;
    if(expression.at(i) == "[") {
      if(isReg_32(expression.at(++i))) {
        if(expression.at(++i) == "+") {
          if(isReg_32(expression.at(++i))) {
            if(expression.at(++i) == "*") {
              if(isNumber(expression.at(++i))){
                if(expression.at(++i) == "+"){
                  if(isNumber(expression.at(++i))){
                    if(expression.at(++i) == "]"){
                      end_id = i + 1;
                      return 0;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else if(getAsmLexemType(expression.at(i)) == LEXEM_TYPE::REGISTER_SEGMENT) {
      if(expression.at(++i) == ":") {
        if(expression.at(++i) == "[") {
          if(isReg_32(expression.at(++i))) {
            if(expression.at(++i) == "+") {
              if(isReg_32(expression.at(++i))) {
                if(expression.at(++i) == "*") {
                  if(isNumber(expression.at(++i))){
                    if(expression.at(++i) == "+"){
                      if(isNumber(expression.at(++i))){
                        if(expression.at(++i) == "]"){
                          end_id = i + 1;
                          return 0;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    return -1;
  }
  catch(const std::exception& e) {
    return -1;
  }
}

bool isReg_32(const Lexem& lexem) {
  return getAsmLexemType(lexem) == LEXEM_TYPE::REGISTER_32 ? true : false;
}

bool isAsmReg(const Lexem& lexem) {
  try
  {
    LEXEM_TYPE lex = getAsmLexemType(lexem);
    if(lex == LEXEM_TYPE::REGISTER_32 || lex == LEXEM_TYPE::REGISTER_16
    || lex == LEXEM_TYPE::REGISTER_8 || lex == LEXEM_TYPE::REGISTER_SEGMENT){
      return true;
    }
    return false;
  }
  catch(const std::exception& e)
  {
    return false;
  }
  
}

bool isNumber(const Lexem& lexem) {
  try
  {
    for (char const &c : lexem) {
      if (std::isdigit(c) == 0) return false;
    }
    return true;
  }
  catch(const std::exception& e)
  {
    return false;
  }
  
}

bool isIMM(const Lexem& lexem) {
  try
  {
    if(isNumber(lexem))
      return true;
    else false;
  }
  catch(const std::exception& e)
  {
    return false;
  }
  
}
#include "translator.h"

using namespace std;

#include <cctype>
#include <regex>
#include <iomanip>
#include <string>
#include <set>
int POP_validator(const Expression &expression, size_t& offset);
int LABLE_validator(const Expression &expression, size_t& offset);
int PUSH_validator(const Expression &expression, size_t& offset);
int OR_validator(const Expression &expression, size_t& offset);
int MOV_validator(const Expression& expression, size_t& offset);
int VAR_name_validator(const Lexem& lexem);
bool isAsmReg(const Lexem& lexem);
bool isReg_8(const Lexem& lexem);
bool isReg_16(const Lexem& lexem);
bool isReg_32(const Lexem& lexem);
bool isNumber(const Lexem& str);
LEXEM_TYPE IMM_size(const Lexem& lexem);
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
    if(getAsmDictType(expression.front())
     == ASM_DICT::MOV) {
      try {
        if(!MOV_validator(expression, offset)) {
          return 0;
        }
      }
      catch(const std::exception& e) {
      }
     }
    else if(getAsmDictType(expression.front())
     == ASM_DICT::OR) {
      try {
        if(!OR_validator(expression, offset)) {
          return 0;
        }
      }
      catch(const std::exception& e) {
      }
    }
    else if(getAsmDictType(expression.front())
     == ASM_DICT::RET) {
       if(expression.size() == 1){
         return 0;
       }
       else if(getAsmDictType(expression.at(1)) == ASM_DICT::SEMICOL){
         return 0;
       }
    }
    else if(getAsmDictType(expression.front())
     == ASM_DICT::PUSH) {
      try {
        if(!PUSH_validator(expression, offset)) {
          return 0;
        }
      }
      catch(const std::exception& e) {
      }
    }
    else if(getAsmDictType(expression.front())
     == ASM_DICT::POP) {
       try
       {
         if(!POP_validator(expression, offset)){
           return 0;
         }
       }
       catch(const std::exception& e)
       {}
       
     }
  }

  if(expression.size() > 1) {
    if(getAsmLexemType(expression.back()) == LEXEM_TYPE::SEGMENT_INSTRUCTION){
      if(expression.at(1) == "SEGMENT") {
        if(!VAR_name_validator(expression.front())) {
          if(expression.size() > 2){
            if(getAsmDictType(expression.at(2)) == ASM_DICT::SEMICOL) {
              offset = 0;
              Segments.insert(expression.front());
              return 0;
            }
          }
          offset = 0;
          Segments.insert(expression.front());
          return 0;
        }
      }
      else if(expression.at(1) == "ENDS") {
        if(expression.size() == 2) {
          for(auto seg: Segments) {
            if(seg.GetName() == expression.front()){
              seg.Close();
              return 0;
            }
          }
        }
        else if(getAsmDictType(expression.at(2)) == ASM_DICT::SEMICOL) {
          for(auto seg: Segments) {
            if(seg.GetName() == expression.front()){
              seg.Close();
              return 0;
            }
          }
        }
      }
    }
    else if(expression.at(1) == ":") {
      try {
        if(!LABLE_validator(expression, offset)) {
          if(count_if(Segments.begin(), Segments.end(),
          [](const Segment& seg){
            return seg.isActive();
          }) == 1){
            for(auto s : Segments){
              if(s.isActive()){
                s.AddLabel({expression.at(0), offset});
                return 0;
              }
            }
          }
        }
      }
      catch(const std::exception& e) {
      }
    }
  }
  if(expression.front() == "END") {
    return 0;
  }
  if(expression.front() == ";") {
    /* Commment */
      return 0;
  }
  return -1;

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
// returns 0 if mov expression is valid
int MOV_validator(const Expression& expression, size_t& offset) {
  if(expression.size() > 1){
    if(expression[0] == "MOV"){
      // MOV mem, imm
      size_t id;
      if(!mem_validator(expression, 1, id)) {
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
            if(getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_16) {
              if(!mem_validator(expression, 5, id)){
                return 0;
              }
            }
          }
          else if(expression[3] == "BYTE" && expression[4] == "PTR") {
            if(getAsmLexemType(expression[1]) == LEXEM_TYPE::REGISTER_8) {
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

int POP_validator(const Expression &expression, size_t& offset){
  if(getAsmDictType(expression.at(0)) == ASM_DICT::POP){
    if(isReg_32(expression.at(1)) || isReg_16(expression.at(1))){
      return 0;
    }
  }
  return -1;
}

int OR_validator(const Expression &expression, size_t& offset) {
  if(getAsmDictType(expression.at(0)) == ASM_DICT::OR) {
    if(getAsmDictType(expression.at(2)) == ASM_DICT::COM) {
      if(isAsmReg(expression.at(1)) && isAsmReg(expression.at(3))) {
        LEXEM_TYPE reg_1_size = getAsmLexemType(expression.at(1));
        LEXEM_TYPE reg_2_size = getAsmLexemType(expression.at(3));
        if(reg_1_size == reg_2_size) {
          if(expression.size() == 4) {
            return 0;
          }
          else if(getAsmDictType(expression.at(4)) == ASM_DICT::SEMICOL){
            return 0;
          }
        }
      } 
    }
  }
  return -1;
}

int PUSH_validator(const Expression &expression, size_t& offset) {
  size_t id = 0;
  if(getAsmDictType(expression.at(id++)) == ASM_DICT::PUSH) {
    /* Let it be error if no keyword before MEM */
    //if(!mem_validator(expression, id, id)) {
      //return 0;
    //}
    if(getAsmDictType(expression.at(id)) == ASM_DICT::DWORD) {
      if(getAsmDictType(expression.at(++id)) == ASM_DICT::PTR) {
        if(!mem_validator(expression, ++id, id)) {
          return 0;
        }
      }
    }
    else if(getAsmDictType(expression.at(id)) == ASM_DICT::WORD) {
      if(getAsmDictType(expression.at(++id)) == ASM_DICT::PTR) {
        if(!mem_validator(expression, ++id, id)) {
          return 0;
        }
      }
    }
  }
  return -1;
}

int LABLE_validator(const Expression &expression, size_t& offset) {
  if(getAsmDictType(expression.back()) == ASM_DICT::COL) {
    if(expression.size() == 2) {
      if(!VAR_name_validator(expression.front())){
        return 0;
      }
    }
    else if(getAsmDictType(expression.at(2)) == ASM_DICT::SEMICOL) {
      if(!VAR_name_validator(expression.front())) {
        return 0;
      }
    }
  }
  return -1;
}

int mem_validator(const Expression& expression, const size_t& bracket_start_id,
 size_t& end_id) {
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

bool isReg_32(const Lexem& lexem) {
  return getAsmLexemType(lexem) == LEXEM_TYPE::REGISTER_32 ? true : false;
}

bool isReg_16(const Lexem& lexem) {
  return getAsmLexemType(lexem) == LEXEM_TYPE::REGISTER_16 ? true : false;
}

bool isReg_8(const Lexem& lexem) {
  return getAsmLexemType(lexem) == LEXEM_TYPE::REGISTER_8 ? true : false;
}

bool isAsmReg(const Lexem& lexem) {
  LEXEM_TYPE lex = getAsmLexemType(lexem);
  if(lex == LEXEM_TYPE::REGISTER_32 || lex == LEXEM_TYPE::REGISTER_16
  || lex == LEXEM_TYPE::REGISTER_8 || lex == LEXEM_TYPE::REGISTER_SEGMENT){
    return true;
  }
  return false;
}

bool isNumber(const Lexem& lexem) {
  for (char const &c : lexem) {
    if (!std::isdigit(c)) return false;
  }
  return true;
}

int VAR_name_validator(const Lexem& lexem) {
  const regex r(R"(\w+)");
  if(regex_match(lexem, r) && lexem.size() < 7) {
    return 0;
  }
  return -1;
}

LEXEM_TYPE IMM_size(const Lexem& lexem) {
  if(isIMM(lexem)) {
    int tmp = stoi(lexem);
    if(tmp > -128 && tmp < 127) {
      return LEXEM_TYPE::DATA_TYPE_8;
    }
    if(tmp > -32'768  && tmp < -32'767) {
      return LEXEM_TYPE::DATA_TYPE_16;
    }
    if(tmp > -2'147'483'648 && tmp < -2'147'483'647) {
      return LEXEM_TYPE::DATA_TYPE_32;
    }
  } 
  return LEXEM_TYPE::UNDEFINED;
}

bool isIMM(const Lexem& lexem) {
  if(isNumber(lexem))
    return true;
  return false;
}

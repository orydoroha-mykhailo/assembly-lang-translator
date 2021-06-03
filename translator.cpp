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
int IMUL_validator(const Expression& expression, size_t& offset);
int VAR_validator(const Expression& expression, size_t& offset, ASM_DICT& var_type);
LEXEM_TYPE getIMM_type(const Lexem& lexem);
int VAR_name_validator(const Lexem& lexem);
int mem_validator(const Expression& expression,
 const size_t& bracket_id, size_t& end_id);
bool isAsmReg(const Lexem& lexem);
bool isReg_8(const Lexem& lexem);
bool isReg_16(const Lexem& lexem);
bool isReg_32(const Lexem& lexem);
bool isNumber(const Lexem& str);
LEXEM_TYPE IMM_size(const Lexem& lexem);
bool isIMM(const Lexem& str);

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
  global_offset = 0;
  size_t line_num = 0;
  size_t current_offset = 0;
  for (const auto& line: all_expressions_) {
    line_num++;
    if(!validate_expression(line, current_offset)) {
      release_expression(line, line_num, global_offset);
    }
    else {
      release_expression(line, line_num, global_offset);
      error_msg({"Analysis interrupt!"}, line_num);
      //return;
    }
    global_offset += current_offset;
  }
}


// returns 0 if creation succsessful
// otherwise returns error code
int Translator::validate_expression(const Expression& expression, size_t& offset) {
  if(getAsmLexemType(expression.at(0)) == LEXEM_TYPE::MACHINE_INSTRUCT) {
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
       offset = 1;
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
    else if(getAsmDictType(expression.front())
     == ASM_DICT::IMUL) {
       try
       {
         if(!IMUL_validator(expression, offset)){
           return 0;
         }
       }
       catch(const std::exception& e)
       {}
       
     }
  }

  if(expression.size() > 1) {
    if(expression.at(1) == "DB" || expression.at(1) == "DW"
     || expression.at(1) == "DD") {
      ASM_DICT var_type;
      if(!VAR_validator(expression, offset, var_type)) {
        for(auto seg: Segments) {
          if(seg.isActive()) {
            Segments.erase(seg);
            seg.AddVariable({expression.at(0), var_type});
            Segments.insert(seg);
          }
        }
        return 0;
      }
    }
    else if(expression.at(1) == "PROC") {
      if(!PROC_validator(expression, offset)) {
        return 0;
      }
    }
    else if(expression.at(1) == "ENDP") {
      offset = 0;
      return 0;
    }
    else if(expression.front() == "JZ") {
      if(!JZ_validator(expression, offset)) {
        return 0;
      }
    }
    else if(expression.front() == "CALL"){
      try{
        if(!CALL_validator(expression, offset)) {
          return 0;
        }
      }
      catch(const std::exception& e){}
      
    }
    else if(expression.at(1) == "SEGMENT") {
      offset = 0;
      if(!VAR_name_validator(expression.front())) {
        if(expression.size() > 2){
          if(getAsmDictType(expression.at(2)) == ASM_DICT::SEMICOL) {
            global_offset = 0;
            Segments.insert(expression.front());
            for(auto s: Segments) {
              if(s.GetName() == expression.at(0)) {
                Segments.erase(s);
                s.Activate();
                Segments.insert(s);
                return 0;
              }
            }
          }
        }
        global_offset = 0;
        Segments.insert(expression.front());
        for(auto s: Segments) {
          if(s.GetName() == expression.at(0)) {
            Segments.erase(s);
            s.Activate();
            Segments.insert(s);
            return 0;
          }
        }
      }
    }
    else if(expression.at(1) == "ENDS") {
      offset = 0;
      if(expression.size() == 2) {
        for(auto seg: Segments) {
          if(seg.GetName() == expression.at(0)
           && seg.isActive()){
            Segments.erase(seg);
            seg.Close();
            Segments.insert(seg);
            return 0;
          }
        }
      }
      else if(getAsmDictType(expression.at(2)) == ASM_DICT::SEMICOL) {
        offset = 0;
        for(auto seg: Segments) {
          if(seg.GetName() == expression.at(0)
           && seg.isActive()){
            Segments.erase(seg);
            seg.Close();
            Segments.insert(seg);
            return 0;
          }
        }
      }
    }
    else if(expression.at(1) == ":") {
      try {
        if(!LABLE_validator(expression, offset)) {
          return 0;
        }
      }
      catch(const std::exception& e) {
      }
    }
  }
  else if(expression.front() == "END") {
    offset = 0;
    return 0;
  }
  if(expression.front() == ";") {
    offset = 0;
    /* Commment */
      return 0;
  }
  return -1;

}

void Translator::release_expression(const Expression& expression,
 const size_t& line_num, const size_t& offset) const {
  ofstream listing_file(covertNameToLstExt(file_name_), ios_base::app);
  stringstream ss;
  ss << std::hex << offset;
  string offset_ = ToUpper(ss.str());

  listing_file << setfill(' ')
  << setw(6) << line_num << "\t" << setw(4)
  << setfill('0')
  << setw(4) << offset_
  << setfill(' ')
  << setw(20)  << ""/* flags */
  << expression << endl;
  listing_file.close();
}

void Translator::print_Symbol_Table() const {
  ofstream listing_file(covertNameToLstExt(file_name_), ios_base::app);
  listing_file << "Symblo Table" << endl << endl
  << "Symbol Name" 
  << setfill(' ') 
  << setw(20) << "Type"
  << setw(10) << "Value"
  << endl;
  for(auto s : Segments) {
    // for(auto v: s.vars) {}
  }
  listing_file.close();
}

void Translator::error_msg(const std::string msg, const size_t& line) const {
  ofstream listing_file(covertNameToLstExt(file_name_), ios_base::app);
  listing_file << "*Error* " << file_name_ << "(" << line<< ") " << msg << endl;
  listing_file.close();

}

int Translator::JZ_validator(const Expression& expression, size_t& offset) {
  offset = 0;
  if(expression.at(0) == "JZ"){
    offset += 4;
    return 0;
  }
  return -1;
}


int Translator::add_Variable(Translator::Variable var){
  for(auto s: Segments){
    if(s.isActive()) {
      if(s.count_var(var) == 0) {
        Segments.erase(s);
        s.AddVariable(var);
        Segments.insert(s);
        return 0;
      }
    }
  }
  return -1;
}

int Translator::PROC_validator(const Expression& expression, size_t& offset) {
  offset = 0;
  if(!VAR_name_validator(expression.at(0))) {
    if(expression.at(1) == "PROC") {
      if(expression.size() == 2){
        if(!add_Variable({expression.at(0), ASM_DICT::NEAR}))
          return 0;
      }
      else if(expression.at(2) == ";") {
        if(!add_Variable({expression.at(0), ASM_DICT::NEAR}))
          return 0;
      }
      else if(expression.at(2) == "FAR") {
        if(expression.size() == 3){
          if(!add_Variable({expression.at(0), ASM_DICT::FAR}))
            return 0;
        }
        else if(expression.at(3) == ";") {
          if(!add_Variable({expression.at(0), ASM_DICT::FAR}))
            return 0;
        }

      }
    }
  }
  return -1;
}

int Translator::CALL_validator(const Expression& expression, size_t& offset) {
  offset = 0;
  if(expression.front() == "CALL"){
    if(!VAR_name_validator(expression.at(1))) {
      for(auto s: Segments) {
        if(s.isActive()) {
          if(s.count_var({expression.at(1), ASM_DICT::NEAR}) == 1) {
            if(expression.size() == 2) {
              offset += 3;
              return 0;
            }
            else if(expression.at(2) == ";") {
              offset += 3;
              return 0;
            }
          }
          else if(s.count_var({expression.at(1), ASM_DICT::FAR}) == 1) {
            if(expression.size() == 2) {
              offset += 3;
              return 0;
            }
            else if(expression.at(2) == ";") {
              offset += 3;
              return 0;
            }
          }
        }
      }
      for(auto s: Segments) {
        if(!s.isActive()) {
          if(s.count_var({expression.at(1), ASM_DICT::FAR}) == 1) {
            if(expression.size() == 2) {
              offset += 5;
              return 0;
            }
            else if(expression.at(2) == ";") {
              offset += 5;
              return 0;
            }
          }
        }
      }
    }
  }
  return -1;
}
int VAR_validator(const Expression& expression, size_t& offset, ASM_DICT& var_type) {
  offset = 0;
  if(expression.at(1) == "DB") {
    var_type = ASM_DICT::DB;
    if(!VAR_name_validator(expression.at(0))) {
      if(isIMM(expression.at(2))) {
        LEXEM_TYPE IMM_type = IMM_size(expression.at(2));
        if(IMM_type == LEXEM_TYPE::DATA_TYPE_8) {
          offset += 1;
          return 0;
        }
      }
      else if(expression.at(2).front() == '\"'
       || expression.at(2).front() == '\'') {
         offset += expression.at(2).size() - 2; 
         return 0;
       }
    }
  }
  else if(expression.at(1) == "DW") {
    var_type = ASM_DICT::DW;
    if(!VAR_name_validator(expression.at(0))) {
      if(isIMM(expression.at(2))) {
        LEXEM_TYPE IMM_type = IMM_size(expression.at(2));
        if(IMM_type == LEXEM_TYPE::DATA_TYPE_16
         || IMM_type == LEXEM_TYPE::DATA_TYPE_8) {
          offset += 2;
          return 0;
        }
      }
    }
  }
  else if(expression.at(1) == "DD") {
    var_type = ASM_DICT::DD;
    if(!VAR_name_validator(expression.at(0))) {
      if(isIMM(expression.at(2))) {
        LEXEM_TYPE IMM_type = IMM_size(expression.at(2));
        if(IMM_type == LEXEM_TYPE::DATA_TYPE_32
         || IMM_type == LEXEM_TYPE::DATA_TYPE_16
         || IMM_type == LEXEM_TYPE::DATA_TYPE_8) {
          offset += 4;
          return 0;
        }
      }
    }
  }
  return -1;
}

int IMUL_validator(const Expression& expression, size_t& offset) {
  offset = 0;
  size_t id = 0;
  if(getAsmDictType(expression.at(id++)) == ASM_DICT::IMUL) {
    if(isAsmReg(expression.at(id++))){
      if(getAsmDictType(expression.at(id++)) == ASM_DICT::COM) {
        if(isReg_32(expression.at(id-2))) {
          if(expression.at(id) == "DWORD" && expression.at(id+1) == "PTR") {
            id += 2;
            offset += 2;
            if(!mem_validator(expression, id, id)) {
              offset += 4;
              if(getAsmDictType(expression.at(id++)) == ASM_DICT::COM) {
                if(isIMM(expression.at(id))) {
                  LEXEM_TYPE IMM = IMM_size(expression.at(id));
                  if(IMM == LEXEM_TYPE::DATA_TYPE_8) {
                    offset += 1;
                    return 0;
                  }
                  else if(IMM == LEXEM_TYPE::DATA_TYPE_32 || IMM == LEXEM_TYPE::DATA_TYPE_16) {
                    offset += 4;
                    return 0;
                  }
                }
              }
            }
          }
        }
        else if(isReg_16(expression.at(id-2))) {
          offset = 0;
          if(expression.at(id) == "WORD" && expression.at(id+1) == "PTR") {
            id += 2;
            offset += 1;
            if(!mem_validator(expression, id, id)) {
              offset += 4;
              if(getAsmDictType(expression.at(id++)) == ASM_DICT::COM) {
                if(isIMM(expression.at(id))) {
                  LEXEM_TYPE IMM = IMM_size(expression.at(id));
                  if(IMM == LEXEM_TYPE::DATA_TYPE_8) {
                    offset += 1;
                    return 0;
                  }
                  else if(IMM == LEXEM_TYPE::DATA_TYPE_16) {
                    offset += 2;
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
  return -1;
}

// seems like done!
// returns 0 if mov expression is valid
int MOV_validator(const Expression& expression, size_t& offset) {
  offset = 0;
  if(expression.size() > 1){
    if(expression.at(0) == "MOV"){
      // MOV mem, imm
      size_t id;
      if(expression.at(1) == "DWORD" && expression.at(2) == "PTR") {
        offset += 10;
        if(!mem_validator(expression, 3, id)) {
          if(expression.at(id++) == ",") {
            // imm check
            if(isIMM(expression.at(id++))){
              if(expression.size() == id) {
                return 0;
              }
              else if(expression.at(id) == ";"){
                return 0;
              }
            }
          }
        }
      }
      else if(expression.at(1) == "WORD" && expression.at(2) == "PTR") {
        offset = 0;
        offset += 7;
        if(!mem_validator(expression, 3, id)) {
          if(expression.at(id++) == ",") {
            // imm check
            if(isIMM(expression.at(id++))){
              if(expression.size() == id) {
                return 0;
              }
              else if(expression.at(id) == ";"){
                return 0;
              }
            }
          }
        }
      }
      else if(expression.at(1) == "BYTE" && expression.at(2) == "PTR") {
        offset = 0;
        offset += 6;
        if(!mem_validator(expression, 3, id)) {
          if(expression.at(id++) == ",") {
            // imm check
            if(isIMM(expression.at(id++))){
              if(expression.size() == id) {
                return 0;
              }
              else if(expression.at(id) == ";"){
                return 0;
              }
            }
          }
        }
      }
      // MOV reg, mem 
      else if(isAsmReg(expression.at(1))){
        offset = 0;
        if(isReg_32(expression.at(1))){
          offset += 2;
        }
        else if(isReg_16(expression.at(1)) || isReg_16(expression.at(1))){
          offset += 1;
        }
        if(expression.at(2) == ","){
          if(!mem_validator(expression, 3, id)){
            offset += 4;
            return 0;
          }
          else if(expression.at(3) == "DWORD" && expression.at(4) == "PTR") {
            if(isReg_32(expression.at(1))) {
              if(!mem_validator(expression, 5, id)){
                offset += 4;
                return 0;
              }
            }
          }
          else if(expression.at(3) == "WORD" && expression.at(4) == "PTR") {
            if(getAsmLexemType(expression.at(1)) == LEXEM_TYPE::REGISTER_16) {
              if(!mem_validator(expression, 5, id)){
                offset += 4;
                return 0;
              }
            }
          }
          else if(expression.at(3) == "BYTE" && expression.at(4) == "PTR") {
            if(getAsmLexemType(expression.at(1)) == LEXEM_TYPE::REGISTER_8) {
              if(!mem_validator(expression, 5, id)){
                offset += 4;
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
  offset = 0;
  if(getAsmDictType(expression.at(0)) == ASM_DICT::POP){
    if(isReg_32(expression.at(1))) {
      offset += 2;
      return 0;
    }
    else if(isReg_16(expression.at(1))){
      offset += 1;
      return 0;
    }
  }
  return -1;
}

int OR_validator(const Expression &expression, size_t& offset) {
  offset = 0;
  if(getAsmDictType(expression.at(0)) == ASM_DICT::OR) {
    if(getAsmDictType(expression.at(2)) == ASM_DICT::COM) {
      if(isAsmReg(expression.at(1)) && isAsmReg(expression.at(3))) {
        LEXEM_TYPE reg_1_size = getAsmLexemType(expression.at(1));
        LEXEM_TYPE reg_2_size = getAsmLexemType(expression.at(3));
        if(reg_1_size == reg_2_size) {
          if(reg_1_size == LEXEM_TYPE::REGISTER_8
           || reg_2_size == LEXEM_TYPE::REGISTER_16)
            offset += 2;
          else if(reg_2_size == LEXEM_TYPE::REGISTER_32)
            offset += 3;
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
  offset = 0;
  size_t id = 0;
  if(getAsmDictType(expression.at(id++)) == ASM_DICT::PUSH) {
    /* Let it be error if no keyword before MEM */
    //if(!mem_validator(expression, id, id)) {
      //return 0;
    //}
    if(getAsmDictType(expression.at(id)) == ASM_DICT::DWORD) {
      offset += 6;
      if(getAsmDictType(expression.at(++id)) == ASM_DICT::PTR) {
        if(!mem_validator(expression, ++id, id)) {
          return 0;
        }
      }
    }
    else if(getAsmDictType(expression.at(id)) == ASM_DICT::WORD) {
      offset += 5;
      if(getAsmDictType(expression.at(++id)) == ASM_DICT::PTR) {
        if(!mem_validator(expression, ++id, id)) {
          return 0;
        }
      }
    }
  }
  return -1;
}

int Translator::LABLE_validator(const Expression &expression, size_t& offset) {
  offset = 0;
  if(getAsmDictType(expression.back()) == ASM_DICT::COL) {
    if(expression.size() == 2) {
      if(!add_Variable({expression.at(0), ASM_DICT::NEAR})) {
        return 0;
      }
    }
    else if(getAsmDictType(expression.at(2)) == ASM_DICT::SEMICOL) {
      if(!VAR_name_validator(expression.front())) {
        if(!add_Variable({expression.at(0), ASM_DICT::NEAR})) {
        return 0;
        }
      }
    }
  }
  return -1;
}

int mem_validator(const Expression& expression,
 const size_t& bracket_start_id, size_t& end_id) {
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

bool isHEX(const Lexem& lexem) {
  return lexem.size() > 2
    && lexem.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}
bool isBIN(const Lexem& lexem) {
  for(auto c: lexem) {
    if(c != '1' && c != '0') {
      return false;
    }
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
  int tmp;
  if(isIMM(lexem)) {
    LEXEM_TYPE imm = getIMM_type(lexem);
    if(imm == LEXEM_TYPE::HEX_CONST){
      tmp = stoi(lexem, nullptr, 16);
    }
    else if(imm == LEXEM_TYPE::BIN_CONST) {
      tmp = stoi(lexem, nullptr, 2);
    }
    else if(imm == LEXEM_TYPE::DEC_CONST) {
      if(lexem.back() == 'D'){
        tmp = stoi(string{lexem.begin(), lexem.end() - 1});
      }
      else {
        tmp = stoi(lexem, nullptr, 10);
      }
    }
    if(tmp > -128 && tmp < 127) {
      return LEXEM_TYPE::DATA_TYPE_8;
    }
    if(tmp > 0 && tmp < 255) {
      return LEXEM_TYPE::DATA_TYPE_8;
    }
    if(tmp > -32'768  && tmp < 32'767) {
      return LEXEM_TYPE::DATA_TYPE_16;
    }
    if(tmp > 0 && tmp < 65'535) {
      return LEXEM_TYPE::DATA_TYPE_16;
    }
    if(tmp > -2'147'483'648 && tmp < 2'147'483'647) {
      return LEXEM_TYPE::DATA_TYPE_32;
    }
    if(tmp > 0 && tmp < 4'294'967'295) {
      return LEXEM_TYPE::DATA_TYPE_32;
    }
  } 
  return LEXEM_TYPE::UNDEFINED;
}

bool isIMM(const Lexem& lexem) {
  if(isNumber(lexem)){
    return true;
  }
  else if(lexem.back() == 'D') {
    if(isNumber({lexem.begin(), lexem.end() - 1})) {
      return true;
    }
  }
  else if(lexem.back() == 'H')
    if(isHEX({lexem.begin(), lexem.end() - 1})) {
      return true;
  }
  else if(lexem.back() == 'B')
    if(isBIN({lexem.begin(), lexem.end() - 1})) {
      return true;
  }
  return false;
}

LEXEM_TYPE getIMM_type(const Lexem& lexem) {
  if(isNumber(lexem)){
    return LEXEM_TYPE::DEC_CONST;
  }
  else if(lexem.back() == 'D') {
    if(isNumber({lexem.begin(), lexem.end() - 1})) {
      return LEXEM_TYPE::DEC_CONST;
    }
  }
  else if(lexem.back() == 'H')
    if(isHEX({lexem.begin(), lexem.end() - 1})) {
      return LEXEM_TYPE::HEX_CONST;
  }
  else if(lexem.back() == 'B')
    if(isBIN({lexem.begin(), lexem.end() - 1})) {
      return LEXEM_TYPE::BIN_CONST;
  }
  return LEXEM_TYPE::UNDEFINED;
}
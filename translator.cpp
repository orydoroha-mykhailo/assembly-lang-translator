#include "translator.h"

using namespace std;

#include <cctype>
#include <iomanip>
#include <string>
#include <set>

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
    //if(!validate_expression(line, current_offset)) {
      offset += current_offset;
      line_num++;
      release_expression(line, line_num, offset);
    //}
    //else {
      // throw exception
    //}
  }
}


// returns 0 if creation succsessful
// otherwise returns error code
int Translator::validate_expression(const Expression& expression, size_t& offset) {
  if(asm_dict_to_lexem_type.at(asm_dict.at(expression.front())) 
    == LEXEM_TYPE::MACHINE_INSTRUCT) {
      /* Review all machine instructions */
  }
  else if(asm_dict_to_lexem_type.at(asm_dict.at(expression.back())) 
    == LEXEM_TYPE::SEGMENT_INSTRUCTION) {
    if(asm_dict.at(expression.back()) == ASM_DICT::SEGMENT) {
    }
    else if(asm_dict.at(expression.front()) == ASM_DICT::ENDS) {
    }
  }
  else if(expression.back() == ":") {
    /* Label */
  }
  else if(expression.front() == ";") {
    /* Commment */
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
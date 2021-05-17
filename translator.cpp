#include "translator.h"

using namespace std;

#include <cctype>
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

ostream& operator<<(ostream& ostr, const Lexems& lexems) {
  for (const auto& lexem : lexems) 
    ostr << lexem << " ";
  return ostr;
}

Translator::Translator(const std::string& file_name) {
  file_name_ = file_name;
  ifstream asm_file(file_name);

  for (string line; getline(asm_file, line); ) {
    if (auto lexems = parseIntoLexems(ToUpper(line)); !lexems.empty()) {
      all_lexems_.push_back(lexems);
    }
  }
}

void Translator::createListing() {
  ofstream listing_file(covertNameToLstExt(file_name_));

  for (const auto& lexems : all_lexems_) {
    listing_file << lexems << endl; 
    listing_file << getLexemTable(lexems) << endl;
    listing_file << getSentenceStructure(lexems) << endl << endl;
  }
}

void Translator::outAllLexems() {
  ofstream listing_file(covertNameToLstExt(file_name_), ios::app);

  set<Lexem> all_lexems;

  for (const auto& lexems : all_lexems_) {
    all_lexems.insert(lexems.begin(), lexems.end());
  }

  listing_file << getLexemTable({all_lexems.begin(), all_lexems.end()}) << "\n";

}

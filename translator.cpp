#include "translator.h"

using namespace std;

#include <string>

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

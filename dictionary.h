#pragma once

#include <string>
#include <map>

const std::map<std::string, std::string> asm_dict = {
  {",", "character"},
  {"+", "character"},
  {"-", "character"},
  {"*", "character"},
  {":", "character"},
  {"[", "character"},
  {"]", "character"},
  {"RET", "machine instuction"},
  {"PUSH", "machine instuction"},
  {"POP", "machine instuction"},
  {"OR", "machine instuction"},
  {"IMUL", "machine instuction"},
  {"MOV", "machine instuction"},
  {"CALL", "machine instuction"},
  {"JZ", "machine instuction"},
  {"BYTE", "data type 1"},
  {"WORD", "data type 2"},
  {"DWORD", "data type 4"},
  {"DB", "data type 1"},
  {"DW", "data type 2"},
  {"DD", "data type 4"},
  {"CS", "segment register"},
  {"DS", "segment register"},
  {"SS", "segment register"},
  {"ES", "segment register"},
  {"FS", "segment register"},
  {"GS", "segment register"},
  {"EAX", "32-bit register"},
  {"EBX", "32-bit register"},
  {"ECX", "32-bit register"},
  {"EDX", "32-bit register"},
  {"ESI", "32-bit register"},
  {"EDI", "32-bit register"},
  {"ESP", "32-bit register"},
  {"EBP", "32-bit register"},
  {"AX", "16-bit register"},
  {"BX", "16-bit register"},
  {"CX", "16-bit register"},
  {"DX", "16-bit register"},
  {"SI", "16-bit register"},
  {"DI", "16-bit register"},
  {"SP", "16-bit register"},
  {"BP", "16-bit register"},
  {"AL", "8-bit register"},
  {"BL", "8-bit register"},
  {"CL", "8-bit register"},
  {"DL", "8-bit register"},
  {"AH", "8-bit register"},
  {"BH", "8-bit register"},
  {"CH", "8-bit register"},
  {"DH", "8-bit register"},
  {"PTR", "type inference operator"},
  {"SEGMENT", "segment instruction"},
  {"ENDS", "segment instruction"},
  {"PROC", "procedure instruction"},
  {"ENDP", "procedure instruction"},
  {"END", "code statement"},
};

inline bool isCharacterLexem(const char c) {
  return c == '*' || c == '+' || c == ',' || c == ':' || c == '[' || c == '[' || c == ']';
}

inline bool isTasmLexem(const std::string& lexem) {
  return asm_dict.count(lexem);
}

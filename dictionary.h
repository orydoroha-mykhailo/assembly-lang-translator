#pragma once

#include <string>
#include <map>

enum class LEXEM_TYPE{
  BIN_CONST,
  DEC_CONST,
  HEX_CONST,
  TEXT_CONST,
  CHARACTER,
  OP,
  MACHINE_INSTRUCT,
  OPERATOR_TYPE,
  DATA_TYPE_1,
  DATA_TYPE_2,
  DATA_TYPE_4,
  REGISTER_SEGMENT, 
  REGISTER_32,
  REGISTER_16,
  REGISTER_8,
  SEGMENT_INSTRUCTION,
  PROC_INSTRUCTION,
  DIRECTIVE,
  U_ID,
  UNDEFINED,
};

const std::map<LEXEM_TYPE, std::string> lex_t_to_str = {
  {LEXEM_TYPE::BIN_CONST, "Binary constant"},
  {LEXEM_TYPE::DEC_CONST, "Decimal constant"},
  {LEXEM_TYPE::HEX_CONST, "Hex constant"},
  {LEXEM_TYPE::TEXT_CONST, "Text constant"},
  {LEXEM_TYPE::CHARACTER, "Character"},
  {LEXEM_TYPE::OP, "Op code"},
  {LEXEM_TYPE::MACHINE_INSTRUCT, "Machine instruction"},
  {LEXEM_TYPE::OPERATOR_TYPE, "Op type"},
  {LEXEM_TYPE::DATA_TYPE_1, "8-bit data type"},
  {LEXEM_TYPE::DATA_TYPE_2, "16-bit data type"},
  {LEXEM_TYPE::DATA_TYPE_4, "32-bit data type"},
  {LEXEM_TYPE::REGISTER_SEGMENT,  "Segment register"},
  {LEXEM_TYPE::REGISTER_32, "32-bit register"},
  {LEXEM_TYPE::REGISTER_16, "16-bit register"},
  {LEXEM_TYPE::REGISTER_8, "8-bit register"},
  {LEXEM_TYPE::SEGMENT_INSTRUCTION, "Segment instruction"},
  {LEXEM_TYPE::PROC_INSTRUCTION, "Procedure instruction"},
  {LEXEM_TYPE::DIRECTIVE, "Directive"},
  {LEXEM_TYPE::U_ID, "User identifier"},
  {LEXEM_TYPE::UNDEFINED, "Undefined"},
};

const std::map<std::string, LEXEM_TYPE> asm_dict = {
  {",", LEXEM_TYPE::CHARACTER},
  {"+", LEXEM_TYPE::OP},
  {"-", LEXEM_TYPE::OP},
  {"*", LEXEM_TYPE::OP},
  {"(", LEXEM_TYPE::OP},
  {")", LEXEM_TYPE::OP},
  {":", LEXEM_TYPE::CHARACTER},
  {"[", LEXEM_TYPE::CHARACTER},
  {"]", LEXEM_TYPE::CHARACTER},
  {"RET", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"PUSH", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"POP", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"OR", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"IMUL", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"MOV", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"CALL", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"JZ", LEXEM_TYPE::MACHINE_INSTRUCT},
  {"BYTE", LEXEM_TYPE::DATA_TYPE_1},
  {"DB", LEXEM_TYPE::DATA_TYPE_1},
  {"WORD", LEXEM_TYPE::DATA_TYPE_2},
  {"DW", LEXEM_TYPE::DATA_TYPE_2},
  {"DWORD", LEXEM_TYPE::DATA_TYPE_4},
  {"DD", LEXEM_TYPE::DATA_TYPE_4},
  {"CS", LEXEM_TYPE::REGISTER_SEGMENT},
  {"DS", LEXEM_TYPE::REGISTER_SEGMENT},
  {"SS", LEXEM_TYPE::REGISTER_SEGMENT},
  {"ES", LEXEM_TYPE::REGISTER_SEGMENT},
  {"FS", LEXEM_TYPE::REGISTER_SEGMENT},
  {"GS", LEXEM_TYPE::REGISTER_SEGMENT},
  {"EAX", LEXEM_TYPE::REGISTER_32},
  {"EBX", LEXEM_TYPE::REGISTER_32},
  {"ECX", LEXEM_TYPE::REGISTER_32},
  {"EDX", LEXEM_TYPE::REGISTER_32},
  {"ESI", LEXEM_TYPE::REGISTER_32},
  {"EDI", LEXEM_TYPE::REGISTER_32},
  {"ESP", LEXEM_TYPE::REGISTER_32},
  {"EBP", LEXEM_TYPE::REGISTER_32},
  {"AX", LEXEM_TYPE::REGISTER_16},
  {"BX", LEXEM_TYPE::REGISTER_16},
  {"CX", LEXEM_TYPE::REGISTER_16},
  {"DX", LEXEM_TYPE::REGISTER_16},
  {"SI", LEXEM_TYPE::REGISTER_16},
  {"DI", LEXEM_TYPE::REGISTER_16},
  {"SP", LEXEM_TYPE::REGISTER_16},
  {"BP", LEXEM_TYPE::REGISTER_16},
  {"AL", LEXEM_TYPE::REGISTER_8},
  {"BL", LEXEM_TYPE::REGISTER_8},
  {"CL", LEXEM_TYPE::REGISTER_8},
  {"DL", LEXEM_TYPE::REGISTER_8},
  {"AH", LEXEM_TYPE::REGISTER_8},
  {"BH", LEXEM_TYPE::REGISTER_8},
  {"CH", LEXEM_TYPE::REGISTER_8},
  {"DH", LEXEM_TYPE::REGISTER_8},
  {"PTR", LEXEM_TYPE::OPERATOR_TYPE},
  {"SEGMENT", LEXEM_TYPE::SEGMENT_INSTRUCTION},
  {"ENDS", LEXEM_TYPE::SEGMENT_INSTRUCTION},
  {"PROC", LEXEM_TYPE::PROC_INSTRUCTION},
  {"FAR", LEXEM_TYPE::PROC_INSTRUCTION},
  {"ENDP", LEXEM_TYPE::PROC_INSTRUCTION},
  {"END", LEXEM_TYPE::DIRECTIVE},
};

inline bool isCharacterLexem(const char c) {
  try
  {
    LEXEM_TYPE res = asm_dict.at(std::string{c});
    return res == LEXEM_TYPE::CHARACTER || res == LEXEM_TYPE::OP;
  }
  catch(const std::exception& e)
  {
    return false;
  }
  
}

inline bool isAsmLexem(const std::string& lexem) {
  return asm_dict.count(lexem);
}

inline LEXEM_TYPE getAsmLexemType(const std::string& lexem) {
  try
  {
    return asm_dict.at(lexem);
  }
  catch(const std::exception& e)
  {
    return LEXEM_TYPE::UNDEFINED;
  }
  
}
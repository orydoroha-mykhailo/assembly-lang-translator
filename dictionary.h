#pragma once

#include <string>
#include <unordered_map>

enum class ASM_DICT{
  COM,
  PLUS,
  MINUS,
  STAR,
  BR_O,
  BR_C,
  COL,
  SEMICOL,
  BR_SQ_O,
  BR_SQ_C,
  RET,
  PUSH,
  POP,
  OR,
  IMUL,
  MOV,
  CALL,
  JZ,
  BYTE,
  DB,
  WORD,
  DW,
  DWORD,
  DD,
  CS,
  DS,
  SS,
  ES,
  FS,
  GS,
  EAX,
  EBX,
  ECX,
  EDX,
  ESI,
  EDI,
  ESP,
  EBP,
  AX,
  BX,
  CX,
  DX,
  SI,
  DI,
  SP,
  BP,
  AL,
  BL,
  CL,
  DL,
  AH,
  BH,
  CH,
  DH,
  PTR,
  SEGMENT,
  ENDS,
  PROC,
  FAR,
  NEAR,
  ENDP,
  END,
  UNDEFINED
};

enum class LEXEM_TYPE{
  BIN_CONST,
  DEC_CONST,
  HEX_CONST,
  TEXT_CONST,
  CHARACTER,
  OP,
  MACHINE_INSTRUCT,
  OPERATOR_TYPE,
  DATA_TYPE_8,
  DATA_TYPE_16,
  DATA_TYPE_32,
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

const std::unordered_map<LEXEM_TYPE, std::string> Lexem_type_to_string = {
  {LEXEM_TYPE::BIN_CONST, "Binary constant"},
  {LEXEM_TYPE::DEC_CONST, "Decimal constant"},
  {LEXEM_TYPE::HEX_CONST, "Hex constant"},
  {LEXEM_TYPE::TEXT_CONST, "Text constant"},
  {LEXEM_TYPE::CHARACTER, "Character"},
  {LEXEM_TYPE::OP, "Op code"},
  {LEXEM_TYPE::MACHINE_INSTRUCT, "Machine instruction"},
  {LEXEM_TYPE::OPERATOR_TYPE, "Op type"},
  {LEXEM_TYPE::DATA_TYPE_8, "8-bit data type"},
  {LEXEM_TYPE::DATA_TYPE_16, "16-bit data type"},
  {LEXEM_TYPE::DATA_TYPE_32, "32-bit data type"},
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
const std::unordered_map<ASM_DICT, LEXEM_TYPE> asm_dict_to_lexem_type = {
  {ASM_DICT::COM, LEXEM_TYPE::CHARACTER},
  {ASM_DICT::PLUS, LEXEM_TYPE::OP},
  {ASM_DICT::MINUS, LEXEM_TYPE::OP},
  {ASM_DICT::STAR, LEXEM_TYPE::OP},
  {ASM_DICT::BR_O, LEXEM_TYPE::OP},
  {ASM_DICT::BR_C, LEXEM_TYPE::OP},
  {ASM_DICT::COL, LEXEM_TYPE::CHARACTER},
  {ASM_DICT::SEMICOL, LEXEM_TYPE::CHARACTER},
  {ASM_DICT::BR_SQ_O, LEXEM_TYPE::CHARACTER},
  {ASM_DICT::BR_SQ_C, LEXEM_TYPE::CHARACTER},
  {ASM_DICT::RET, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::PUSH, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::POP, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::OR, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::IMUL, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::MOV, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::CALL, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::JZ, LEXEM_TYPE::MACHINE_INSTRUCT},
  {ASM_DICT::BYTE, LEXEM_TYPE::DATA_TYPE_8},
  {ASM_DICT::DB, LEXEM_TYPE::DATA_TYPE_8},
  {ASM_DICT::WORD, LEXEM_TYPE::DATA_TYPE_16},
  {ASM_DICT::DW, LEXEM_TYPE::DATA_TYPE_16},
  {ASM_DICT::DWORD, LEXEM_TYPE::DATA_TYPE_32},
  {ASM_DICT::DD, LEXEM_TYPE::DATA_TYPE_32},
  {ASM_DICT::CS, LEXEM_TYPE::REGISTER_SEGMENT},
  {ASM_DICT::DS, LEXEM_TYPE::REGISTER_SEGMENT},
  {ASM_DICT::SS, LEXEM_TYPE::REGISTER_SEGMENT},
  {ASM_DICT::ES, LEXEM_TYPE::REGISTER_SEGMENT},
  {ASM_DICT::FS, LEXEM_TYPE::REGISTER_SEGMENT},
  {ASM_DICT::GS, LEXEM_TYPE::REGISTER_SEGMENT},
  {ASM_DICT::EAX, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::EBX, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::ECX, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::EDX, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::ESI, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::EDI, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::ESP, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::EBP, LEXEM_TYPE::REGISTER_32},
  {ASM_DICT::AX, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::BX, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::CX, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::DX, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::SI, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::DI, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::SP, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::BP, LEXEM_TYPE::REGISTER_16},
  {ASM_DICT::AL, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::BL, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::CL, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::DL, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::AH, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::BH, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::CH, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::DH, LEXEM_TYPE::REGISTER_8},
  {ASM_DICT::PTR, LEXEM_TYPE::OPERATOR_TYPE},
  {ASM_DICT::SEGMENT, LEXEM_TYPE::SEGMENT_INSTRUCTION},
  {ASM_DICT::ENDS, LEXEM_TYPE::SEGMENT_INSTRUCTION},
  {ASM_DICT::PROC, LEXEM_TYPE::PROC_INSTRUCTION},
  {ASM_DICT::FAR, LEXEM_TYPE::PROC_INSTRUCTION},
  {ASM_DICT::ENDP, LEXEM_TYPE::PROC_INSTRUCTION},
  {ASM_DICT::END, LEXEM_TYPE::DIRECTIVE},
};


const std::unordered_map<std::string, ASM_DICT> asm_dict = {
  {",", ASM_DICT::COM},
  {"+", ASM_DICT::PLUS},
  {"-", ASM_DICT::MINUS},
  {"*", ASM_DICT::STAR},
  {"(", ASM_DICT::BR_O},
  {")", ASM_DICT::BR_C},
  {":", ASM_DICT::COL},
  {";", ASM_DICT::SEMICOL},
  {"[", ASM_DICT::BR_SQ_O},
  {"]", ASM_DICT::BR_SQ_C},
  {"RET", ASM_DICT::RET},
  {"PUSH", ASM_DICT::PUSH},
  {"POP", ASM_DICT::POP},
  {"OR", ASM_DICT::OR},
  {"IMUL", ASM_DICT::IMUL},
  {"MOV", ASM_DICT::MOV},
  {"CALL", ASM_DICT::CALL},
  {"JZ", ASM_DICT::JZ},
  {"BYTE", ASM_DICT::BYTE},
  {"DB", ASM_DICT::DB},
  {"WORD", ASM_DICT::WORD},
  {"DW", ASM_DICT::DW},
  {"DWORD", ASM_DICT::DWORD},
  {"DD", ASM_DICT::DD},
  {"CS", ASM_DICT::CS},
  {"DS", ASM_DICT::DS},
  {"SS", ASM_DICT::SS},
  {"ES", ASM_DICT::ES},
  {"FS", ASM_DICT::FS},
  {"GS", ASM_DICT::GS},
  {"EAX", ASM_DICT::EAX},
  {"EBX", ASM_DICT::EBX},
  {"ECX", ASM_DICT::ECX},
  {"EDX", ASM_DICT::EDX},
  {"ESI", ASM_DICT::ESI},
  {"EDI", ASM_DICT::EDI},
  {"ESP", ASM_DICT::ESP},
  {"EBP", ASM_DICT::EBP},
  {"AX", ASM_DICT::AX},
  {"BX", ASM_DICT::BX},
  {"CX", ASM_DICT::CX},
  {"DX", ASM_DICT::DX},
  {"SI", ASM_DICT::SI},
  {"DI", ASM_DICT::DI},
  {"SP", ASM_DICT::SP},
  {"BP", ASM_DICT::BP},
  {"AL", ASM_DICT::AL},
  {"BL", ASM_DICT::BL},
  {"CL", ASM_DICT::CL},
  {"DL", ASM_DICT::DL},
  {"AH", ASM_DICT::AH},
  {"BH", ASM_DICT::BH},
  {"CH", ASM_DICT::CH},
  {"DH", ASM_DICT::DH},
  {"PTR", ASM_DICT::PTR},
  {"SEGMENT", ASM_DICT::SEGMENT},
  {"ENDS", ASM_DICT::ENDS},
  {"PROC", ASM_DICT::PROC},
  {"FAR", ASM_DICT::FAR},
  {"ENDP", ASM_DICT::ENDP},
  {"END", ASM_DICT::END},
};

inline bool isCharacterLexem(const char c) {
  try
  {
    LEXEM_TYPE res = asm_dict_to_lexem_type.at(asm_dict.at(std::string{c}));
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

inline ASM_DICT getAsmDictType(const std::string& lexem) {
  try
  {
    return asm_dict.at(lexem);
  }
  catch(const std::exception& e)
  {
    return ASM_DICT::UNDEFINED;
  }
  
}

inline LEXEM_TYPE getAsmLexemType(const std::string& lexem) {
  try
  {
    return asm_dict_to_lexem_type.at(getAsmDictType(lexem));
  }
  catch(const std::exception& e)
  {
    return LEXEM_TYPE::UNDEFINED;
  }
}
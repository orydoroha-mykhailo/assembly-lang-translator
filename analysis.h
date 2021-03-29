#pragma once

#include "lexer.h"
#include "dictionary.h"

std::string getLexemTable(const Lexems& lexems);

std::string getSentenceStructure(const Lexems& lexems);
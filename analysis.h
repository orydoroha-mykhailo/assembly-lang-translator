#pragma once

#include "lexer.h"
#include "dictionary.h"

std::string getExpressionTable(const Expression& lexems);

std::string getSentenceStructure(const Expression& lexems);
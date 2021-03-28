#pragma once

#include "dictionary.h"

#include <string>
#include <vector>

typedef std::string Lexem;
typedef std::vector<std::string> Lexems;

Lexems parseIntoLexems(const std::string& line);

#pragma once

#include "dictionary.h"

#include <string>
#include <vector>

typedef std::string Lexem;
typedef std::vector<std::string> Expression;

Expression parseLineToExpression(const std::string& line);

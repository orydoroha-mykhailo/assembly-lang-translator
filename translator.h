#pragma once

#include "analysis.h"

#include <iostream>
#include <fstream>

class Translator {
public:
  Translator(const std::string& file_name);

  void createListing();

  void outAllLexems();
  
private:
  std::string file_name_;

  std::vector<Lexems> all_lexems_;
};

#pragma once

#include "analysis.h"
#include <set>

#include <iostream>
#include <fstream>

class Translator {
public:
  Translator(const std::string& file_name);

  void createListing();

  void outAllLexems();
  
private:
  friend class Segment;
  class Variable {
    const std::string var_name;
    const LEXEM_TYPE size;
  };
  class Segment {
    bool isclosed = false;
    std::string seg_name;
    std::set<Variable> vars;
  };
  // class Register{
    // ASM_DICT reg_name;
    // const LEXEM_TYPE reg_size;
  // };
  class Label {
    const std::string lbl_name;
    const size_t addr;
  };
  int validate_expression(const Expression& expression, size_t& offset);
  void release_expression(const Expression& expression, const size_t& line_num, const size_t& offset) const;
  size_t calculate_offset(const Expression& expression) const;
  std::string file_name_;
  std::set<Segment> Segments;
  std::set<Label> labels;
  std::vector<Expression> all_expressions_;
};

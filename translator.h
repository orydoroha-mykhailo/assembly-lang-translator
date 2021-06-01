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
  class Label {
     public:
    Label(const std::string& name, const std::size_t& id)
     :lbl_name(name), addr(id){}
    bool operator<(const Label& rhs) const {
     return lbl_name < rhs.lbl_name;}
     std::string GetName()const{return lbl_name;}
     private:
    const std::string lbl_name;
    const size_t addr;
  };
  friend class Segment;
  class Variable {
    const std::string var_name;
    const LEXEM_TYPE size;
  };
  class Segment {
  public:
    bool isActive() const
    {return isactive;}
    bool operator<(const Segment& rhs) const {
     return seg_name < rhs.seg_name;}
    void AddLabel(const Label& lbl){
      for(auto this_lbl: labels){
        if(lbl.GetName() == this_lbl.GetName()){
          throw std::invalid_argument("label with the same name already exist!");
        }
      }
      labels.insert(lbl);
    }

  private:
    bool isactive = false;
    std::string seg_name;
    std::set<Variable> vars;
    std::set<Label> labels;
  };
  // class Register{
    // ASM_DICT reg_name;
    // const LEXEM_TYPE reg_size;
  // };
  int validate_expression(const Expression& expression, size_t& offset);
  void release_expression(const Expression& expression, const size_t& line_num, const size_t& offset) const;
  size_t calculate_offset(const Expression& expression) const;
  std::string file_name_;
  std::set<Segment> Segments;
  std::vector<Expression> all_expressions_;
};

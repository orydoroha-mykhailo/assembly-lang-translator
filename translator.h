#pragma once

#include "analysis.h"
#include <set>

#include <iostream>
#include <fstream>

static size_t global_offset = 0;

class Translator {
public:
  Translator(const std::string& file_name);

  //friend int mem_validator(Translator& obj, const Expression& expression, const size_t& bracket_start_id,
   //size_t& end_id);

  friend int mem_validator(const Expression& expression,
   const size_t& bracket_id, size_t& end_id);
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
  public:
    Variable(const std::string& name, const ASM_DICT& var_type)
     :var_name(name), attribute(var_type){}
    bool operator==(const Variable& rhs) const {
     return var_name == rhs.var_name && attribute == rhs.GetType();}
    bool operator<(const Variable& rhs) const {
     return var_name < rhs.var_name;}
    std::string GetName() const{return var_name;}
    ASM_DICT GetType() const{return attribute;}

  private:
    const std::string var_name;
    const ASM_DICT attribute;
  };
  class Segment {
  public:
    Segment(std::string name):seg_name(name){}
    bool isActive() const
    {return isactive;}
    std::string GetName() const{return seg_name;}
    bool operator<(const Segment& rhs) const {
     return seg_name < rhs.seg_name;}
    
    size_t count_var(const Variable& var) {
      size_t counter = 0;
      for(auto v : vars) {
        if(var == v) {
          counter++;
        }
      }
      return counter;
    }

    void AddLabel(const Label& lbl){
      for(auto this_lbl: labels){
        if(lbl.GetName() == this_lbl.GetName()){
          throw std::invalid_argument("label with the same name already exist!");
        }
      }
      labels.insert(lbl);
    }
    void AddVariable(const Variable& var){
      for(auto this_var: vars){
        if(this_var.GetName() == var.GetName()){
          throw std::invalid_argument("variable with the same name already exist!");
        }
      }
      vars.insert(var);
    }
    void Close(){
      isactive = false;
    }
    void Activate(){
      isactive = true;
    }

  private:
    bool isactive;
    std::string seg_name;
    std::set<Variable> vars;
    std::set<Label> labels;
  };
  // class Register{
    // ASM_DICT reg_name;
    // const LEXEM_TYPE reg_size;
  // };


  int JZ_validator(const Expression& expression, size_t& offset);
  int LABLE_validator(const Expression &expression, size_t& offset);
  int PROC_validator(const Expression& expression, size_t& offset);
  int CALL_validator(const Expression& expression, size_t& offset);
  int add_Variable(const Translator::Variable& var);

  int validate_expression(const Expression& expression, size_t& offset);
  void release_expression(const Expression& expression, const size_t& line_num, const size_t& offset) const;
  void error_msg(const std::string msg, const size_t& line) const;
  size_t calculate_offset(const Expression& expression) const;
  std::string file_name_;
  std::set<Segment> Segments;
  std::vector<Expression> all_expressions_;
};

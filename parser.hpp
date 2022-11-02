#pragma once
#include"VirtualMachine.h"
#include "lexer.hpp"
#include<unordered_set>
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>
#include"../milestone2-almzayyen/Instruction.h"
#include<vector>
#include<math.h>
#include<limits.h>
#include<string>
#include<cstring>
#include<iostream>
#include<unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
class Parser {
public:
    Parser();

    ~Parser();

    // tokenize

    bool tokenize(std::istream &ins);

    bool tokenize(std::string &str);

    bool tokenize_file( std::string file_name);
    int get_instructions();
    // error
    const std::string get_error();

    // file
  //  bool parse_file(const std::string &file_name);
    bool parse_text( );
    int get_line_num();
    // data section
    bool parse_int_layout();
    bool parse_string_layout();
    bool parse_integer();
    bool parse_string();
    bool parse_layout();
    bool parse_constant();
    bool parse_declaration();
    bool parse_label();
    bool parse_integer(TokenList::iterator t);

    // text section
    bool parse_memref();
    bool parse_opcode();
    bool parse_operation();
    bool parse_instruction();
    bool parse_register();
    bool parse_immediate();
    bool parse_op_args();
    bool parse_source();
    bool parse_text_constant();

//    bool parse_memref_t(std::string s);
//    bool parse_opcode_t(std::string s);
//    bool parse_operation_t(std::string s);
//    bool parse_instruction_t(std::string s);
//    bool parse_register_t(std::string s);
//    bool parse_immediate_t(std::string s);
//    bool parse_op_args_t(std::string s);
//    bool parse_source_t(std::string s);
//    bool parse_text_constant_t(std::string s);
//    bool parse_integer_t(std::string s);
//    bool parse_string_t(std::string s);
//    bool parse_label_t(std::string s);


    TokenList token_to_inst()
    {
        TokenList::iterator it_start = m_it;
        TokenList tokens;
        while(m_it->type()!= EOL)
        {
            tokens.emplace_back(*m_it);
        }
        m_it = it_start;
        return tokens;
    }

    //   bool is_operator(const std::string & str);
    bool is_op(char c);

    void print_token_list() {
        TokenList::iterator it_start = m_it;
      for (; m_it != m_tl.end(); ++m_it) {
        std::cout << *m_it << std::endl;
      }
        m_it = it_start;

    }
    bool is_char(char c);

    bool is_alpha(char c);

    bool is_digit(char c);

    bool is_section_specifier();

private:




private:
    int instructions;
    int text_line;
    TokenList m_tl;
    TokenList::iterator m_it;
    std::string temp_label;
    //temp for instruction
    std::string arguments[3];
    int curr_arg;
    int num_args;
    int inst_add;
    //temp for data
    bool is_offset = false;
    Instruction *inst;
    std::string operation;
    //bool is_data;
    int label_add ;
    Labels* m_label;
    //temp for both data and text
    bool data_true_text_false;


    std::unordered_set<std::string> valid_op{"lw", "lh", "lb", "li", "la", "sw", "sw", "sh", "sb", "move", "mfhi",
                                             "mflo", "mthi", "mtlo", "add", "sub", "addu", "subu", "mul", "mulo", "mulou", "mult", "multu", "div", "divu",
                                             "rem", "remu", "abs", "neg", "negu", "and", "nor", "not", "or", "xor", "j", "beq", "bne", "blt", "ble", "bgt", "bge"};

    std::unordered_set<std::string> data_mov_inst{"lw", "lh", "lb", "li", "la", "sw", "sw", "sh", "sb", "move", "mfhi",
                                             "mflo", "mthi", "mtlo"};

    std::unordered_set<std::string> int_arithmetic_inst {"add", "sub", "addu", "subu", "mul", "mulo", "mulou", "mult", "multu", "div", "divu",
                                                    "rem", "remu", "abs", "neg", "negu"};
    std::unordered_set<std::string> logic_inst {"and", "nor", "not", "or", "xor"};

    std::unordered_set<std::string> control_inst {"j", "beq", "bne", "blt", "ble", "bgt", "bge"};


    std::unordered_set<std::string> registers{"$0", "$1", "$2","$3", "$4", "$5" ,"$6", "$7", "$8", "$9", "$10", "$11",
                                              "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21",
                                              "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31","$zero","$at", "$v0","$v1", "$a0","$a1",
                                                 "$a2","$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7","$s0","$s1",
                                              "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0","$k1","$gp","$sp","$fp","$ra"   };

    std::unordered_map<std::string, int> constant_list;

    VirtualMachine vm;
};
//
// Created by Abs on 3/5/2018.
//

#pragma once

#ifndef M2_VIRTUALMACHINE_H
#define M2_VIRTUALMACHINE_H

#include<vector>
#include<string>
#include<unordered_map>

#include"../milestone2-almzayyen/Instruction.h"
#include<algorithm>
#include<cstring>
#include<math.h>
#include<limits.h>
#include<string>
#include<cstring>
#include<unordered_set>


enum types {
    data, text
};

/*
 *
 * TODO:
 * make an instruction class, with an enum of instruction types.
 * just like token, and make an std::vector of instructions.
 * API should have instruction type return , and details about instruction.
 *
 *
 *
 *
 * ///////////////////////////////////
 * create a function to turn the value backwards
 * and a couple of functions to add the values one by one or all at once backwards.
 *
 * maybe add to string, take two digits by 2 and then turn to hex. then add those two
 * digits to memory.
 * I will make two maps each for a register value, then another map for the aliases.
 * or I can update both values when adding.
 *
 * should make a data structure to hold the instructions
 *
 * use a pc size_t or int to know where the program is it , while reading or parsing.
 * and again while executing it resets.
 *
 * make another data structure to hold labels type and values.
 *
 *
 *
 *
*/

class Labels {
public:
    Labels();

    Labels(types t, int address);


    int address() const;

    bool data_or_text() const;

    std::string type() const;

private:
    std::string m_label;
    bool m_is_data;
    int m_address;
    types m_type;

};

struct m_Label {
    bool is_data;
    unsigned int data_add_or_text;
};

class VirtualMachine {
public:
    VirtualMachine();

    ~VirtualMachine();

    void step() {
        int orig_pc = pc;
        //  reg_map["pc"]= orig_pc;
        if (!(instruction_list.empty()) && instruction_list.size() != pc) {


            pc += 1;

            Instruction *instr = instruction_list[orig_pc];
            instr->exec();

        }

        printf("0x%08x\n", pc);


    }
    void step2() {
        int orig_pc = pc;
        //  reg_map["pc"]= orig_pc;
        if (!(instruction_list.empty()) && instruction_list.size() != pc) {


            pc += 1;

            Instruction *instr = instruction_list[orig_pc];
            instr->exec();

        }




    }

    bool exec() {
        size_t i = 0;
        if (!(instruction_list.empty())) {
            while (instruction_list.size() < i) {
                step();
                i++;
            }
        }
        return true;
    }

public:
    void reg_change();
    Instruction *temp;
//    bool is_offset(std::string s);

    int get_pc();



    unsigned int load_word(unsigned int index);

    void print(std::string s, bool type);

    bool constant_exists(std::string);

//    std::string to_hex(std::string decimal); // convert the value to hex to store to bytes.
    void reg_init();

    void add_instruction(std::string operation, std::string dest);

    void add_instruction(std::string operation, std::string dest, std::string OpA); // add instruction to inst_list
    void add_instruction(std::string operation, std::string dest, std::string OpA, std::string OpB);

    void add_instruction_off(std::string operation, std::string dest, std::string OpA,
                             int off); // add instruction to inst_list
//    void print_inst();
    unsigned long inst_size();
    void add_inst_label(std::string key, Labels val);

    int get_constant(std::string s);

    bool store_mem(unsigned int h, int index); // store memory in little endian order ( least significant byte first)
    char at_mem(const int &index);

    bool valid_label(std::string s);

    bool valid_constant(std::string s);

    //change this to a function that uses typecasting
    //word is 2 unsigned char
    //unsigned short
    // signed for signed numbers,
    //loading loads decimal as well
    //printing is the only one that does hex.
    //
//    void print_reg();

//    void print_mem(int min, int max);

    void set_pc(int k);

//    void load_mem(int min, int max, int num);

    static VirtualMachine *getVirtualMachine();

    void add_data(std::string layout, std::string value, bool signed_int);

    int return_constant(std::string s);

    void add_constant(std::string key, int val);

    Labels return_label(std::string s);

    void add_label(std::string key, Labels val);

    int get_label_addval(std::string label);

    void reset_mem();

//    void print_label();

    int get_data_add();

    int get_label_add2();

    unsigned int get_reg(std::string s);

    bool write_reg(std::string s, unsigned int t);

    unsigned int at_reg(std::string regid);

    bool valid_reg(std::string s);


private:
    int pc; // program counter
    int label_address;
    unsigned int data_add;

    std::unordered_map<std::string, int> constant_list;

    static VirtualMachine *s_instance;

    std::unordered_map<std::string, Labels> label_map;
    std::unordered_map<std::string, unsigned int> reg_map;
    unsigned char memory[512];

    std::size_t inst_index;

    std::unordered_map<std::string, int> labels_link;
    std::vector<Instruction *> instruction_list;

    std::unordered_set<std::string> registers{"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11",
                                              "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21",
                                              "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31",
                                              "$zero", "$at", "$v0", "$v1", "$a0", "$a1",
                                              "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                                              "$s0", "$s1",
                                              "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1",
                                              "$gp", "$sp", "$fp", "$ra", "$lo", "$hi"};
};


#endif



//
// Created by Abs on 3/12/2018.
//

#include"VirtualMachine.h"
#include<sstream>

Labels::Labels() {}

Labels::Labels(types t, int address) {
    m_type = t;
    m_address = address;

}

int VirtualMachine::get_data_add() { return data_add; }

int VirtualMachine::get_label_add2() { return label_address; }

int Labels::address() const {
    return m_address;

}

int VirtualMachine::get_pc() { return pc; }

void VirtualMachine::print(std::string s, bool type) {
    if (type) {
        auto valid_reg = registers.find(s);
        if (!(valid_reg == registers.end())) {
            printf("\n0x%08x\n", VirtualMachine::getVirtualMachine()->get_reg(s));

        } else if (s == "$pc") { printf("\n0x%08x\n", pc); }

        else {


            std::cerr << "Error: invalid reg" << std::endl;

        }
    } else {
        int dec;
        std::stringstream stream;

        int k = std::stoi(s.substr(0, s.length()), nullptr, 16);
        stream << k;
        stream >> std::dec >> dec;
        //std::cout << dec;
        if (k <= 512)
            printf("\n0x%02x\n", memory[dec]);
        else {
            std::cerr << "Error: inalid mem" << std::endl;
        }
    }
}

bool Labels::data_or_text() const {
    if (m_is_data)
        return data;
    else
        return text;

}

std::string Labels::type() const {

    std::string a = ".data";
    std::string b = ".text";
    if (m_type == data)
        return a;
    else
        return b;

}


VirtualMachine::VirtualMachine() {
    label_address = 0;
    data_add = 0;
    pc = 0;

}


void VirtualMachine::reg_init() {
    reg_map["$zero"] = 0;
    reg_map["$t0"] = 0;
    reg_map["$t1"] = 0;
    reg_map["$at"] = 0;
    reg_map["$v0"] = 0;
    reg_map["$v1"] = 0;
    reg_map["$a0"] = 0;
    reg_map["$a1"] = 0;
    reg_map["$a2"] = 0;
    reg_map["$a3"] = 0;
    reg_map["$t0"] = 0;
    reg_map["$t1"] = 0;
    reg_map["$t2"] = 0;
    reg_map["$t3"] = 0;
    reg_map["$t4"] = 0;
    reg_map["$t5"] = 0;
    reg_map["$t6"] = 0;
    reg_map["$t7"] = 0;
    reg_map["$s0"] = 0;
    reg_map["$s1"] = 0;
    reg_map["$s2"] = 0;
    reg_map["$s3"] = 0;
    reg_map["$s4"] = 0;
    reg_map["$s5"] = 0;
    reg_map["$s6"] = 0;
    reg_map["$s7"] = 0;
    reg_map["$t8"] = 0;
    reg_map["$t9"] = 0;
    reg_map["$k0"] = 0;
    reg_map["$k1"] = 0;
    reg_map["$gp"] = 0;
    reg_map["$sp"] = 0;
    reg_map["$fp"] = 0;
    reg_map["$ra"] = 0;
    reg_map["$pc"] = 0;
    reg_map["$hi"] = 0;
    reg_map["$lo"] = 0;

    reg_map["$0"] = 0;
    reg_map["$1"] = 0;
    reg_map["$2"] = 0;
    reg_map["$3"] = 0;
    reg_map["$4"] = 0;
    reg_map["$5"] = 0;
    reg_map["$6"] = 0;
    reg_map["$7"] = 0;
    reg_map["$8"] = 0;
    reg_map["$9"] = 0;
    reg_map["$10"] = 0;
    reg_map["$11"] = 0;
    reg_map["$12"] = 0;
    reg_map["$13"] = 0;
    reg_map["$14"] = 0;
    reg_map["$15"] = 0;
    reg_map["$16"] = 0;
    reg_map["$17"] = 0;
    reg_map["$18"] = 0;

    reg_map["$19"] = 0;
    reg_map["$20"] = 0;
    reg_map["$21"] = 0;
    reg_map["$22"] = 0;
    reg_map["$23"] = 0;
    reg_map["$24"] = 0;
    reg_map["$25"] = 0;
    reg_map["$26"] = 0;
    reg_map["$27"] = 0;
    reg_map["$28"] = 0;
    reg_map["$29"] = 0;
    reg_map["$30"] = 0;
    reg_map["$31"] = 0;


}

//void VirtualMachine::print_inst()
//{
//for(size_t i =0; i< instruction_list.size();i++)
//{
//    std::cout << "[" << i << "]" << instruction_list[i]->type() << std::endl;
//}
//}
int VirtualMachine::return_constant(std::string s) {
    //std::unordered_map::const_iterator pos = constant_list.find(s);
    return constant_list[s];
}

void VirtualMachine::add_constant(std::string key, int val) {
    std::pair<std::string, int> value_pair(key, val);
    constant_list.insert(value_pair);
}

Labels VirtualMachine::return_label(std::string s) {
    return label_map[s];
}

void VirtualMachine::add_label(std::string key, Labels val) {

    std::string s = key.substr(0, key.length() - 1);

    std::pair<std::string, Labels> value_pair(s, val);
    label_map.insert(value_pair);
}

void VirtualMachine::add_inst_label(std::string key, Labels val) {

    std::string s = key.substr(0, key.length() - 1);

    std::pair<std::string, Labels> value_pair(s, val);
    label_map.insert(value_pair);

}

unsigned int VirtualMachine::get_reg(std::string s) {
    reg_change();
    if (s == "$pc") { return pc; }
    else {
        unsigned int k = reg_map[s];
        return k;
    }

}

bool VirtualMachine::write_reg(std::string s, unsigned int t) {
    reg_change();
    if (valid_reg(s)) {
        reg_map[s] = t;
        return true;
    } else
        return false;
}

int VirtualMachine::get_constant(std::string s) {
    return constant_list[s];
}

unsigned int VirtualMachine::at_reg(std::string regid) {
    return reg_map[regid];
}

int VirtualMachine::get_label_addval(std::string label) {
    Labels l = VirtualMachine::getVirtualMachine()->return_label(label);
    return l.address();
}

bool VirtualMachine::valid_reg(std::string s) {
    auto valid_reg = registers.find(s);
    if (!(valid_reg == registers.end()))
        return true;
    else
        return false;
}

bool VirtualMachine::constant_exists(std::string s) {
    if (constant_list.find(s) == constant_list.end()) {
        return false;
    } else {
        return true;
    }
}


void VirtualMachine::add_data(std::string layout, std::string int_value, bool signed_int) {

//std::cout << label_address << std::endl ;
//    std::cout << layout << std::endl;
    if (valid_constant(int_value)) {
        int_value = std::to_string(get_constant(int_value));
    }

    if (layout == ".word") {


        //std::cout << word << std::endl;
        if (!signed_int) {
            unsigned int word = std::stol(int_value);
            unsigned char *p = (unsigned char *) &word;
            for (size_t i = 0; i < sizeof(word); i++) {
                memory[i + data_add] = *(p + i);
                //printf("p[%d] = %x\n", i, memory[i]);
            }

        } else {

            signed int word = std::stol(int_value);
            signed char *p = (signed char *) &word;
            for (size_t i = 0; i < sizeof(word); i++) {
                memory[i + data_add] = *(p + i);
                //printf("p[%d] = %x\n", i, memory[i]);
            }
        }
        data_add += 4;
        //label_address +=4;
    } else if (layout == ".byte") {


        if (!signed_int) {
            int8_t byte = stol(int_value);
            unsigned char num = (unsigned char) byte;
            unsigned char *p = (unsigned char *) &byte;
            for (size_t i = 0; i < sizeof(num); i++) {
                memory[i + data_add] = *(p + i);
                //printf("p[%d] = %x\n", i, memory[i]);
            }
        } else {
            signed int byte = stoi(int_value);
            unsigned char num = (unsigned char) byte;
            unsigned char *p = (unsigned char *) &byte;
            for (size_t i = 0; i < sizeof(num); i++) {
                memory[i + data_add] = *(p + i);
                //printf("p[%d] = %x\n", i, memory[i]);
            }
        }
        data_add += 1;
        //label_address +=1;
    } else if (layout == ".half") {


        unsigned short num = std::stol(int_value);
        unsigned char *p = (unsigned char *) &num;
        if (!signed_int) {
            for (size_t i = 0; i < sizeof(num); i++) {
                memory[i + data_add] = *(p + i);
                //printf("p[%d] = %x\n", i, memory[i]);
            }

        } else {

            signed short num = std::stol(int_value);
            unsigned char *p = (unsigned char *) &num;
            for (size_t i = 0; i < sizeof(num); i++) {
                memory[i + data_add] = *(p + i);
                //printf("p[%d] = %x\n", i, memory[i]);
            }
        }
        data_add += 2;
        // label_address +=2;
    } else if (layout == ".space") {


        int space = stoi(int_value);

        unsigned char num = 0x0;
        // unsigned char *p = (unsigned char *)&num;
        for (auto i = 0; i < space; i++) {
            memory[i + data_add] = num;
            // printf("p[%d] = %x\n", i, mem[i]);
        }
        // label_address +=space;
        data_add += space;
    }

}

bool VirtualMachine::store_mem(unsigned int h, int index) {
    if (index >= 512) {

        std::cerr << "Out of memory bounds" << std::endl;
        return false;
    }
    //memory.insert(memory.begin()+ index, h );
    memory[index] = h;
    return true;
}


//void VirtualMachine::store(int min, int max, int num)
//{
//
//    unsigned char *p = (unsigned char *)&num;
//    for(auto i =0 ;i < max;i++)
//    {
//        memory[i] = *(p+i);
//        // printf("p[%d] = %x\n", i, mem[i]);
//    }
//
//
//}

void VirtualMachine::reset_mem() {
    pc = 0;
    data_add = 0;
    label_address = 0;
    label_map.clear();
    instruction_list.clear();
    reg_map.clear();
    constant_list.clear();
    for (int i = 0; i < 512; i++) {
        memory[i] = 0x0;
        //printf("p[%d] = %x\n", i, memory[i]);
    }


}

//void VirtualMachine::print_label()
//{
// std::string a;
//
//    for(std::unordered_map<std::string,Labels >::const_iterator it = label_map.begin();
//            it != label_map.end(); ++it)
//    {
//        std::cout << it->first << " " << it->second.address() << " " << it->second.type()<< "\n";
//    }
//}

//void VirtualMachine::print_reg()
//{
//    std::string a;
//
//    for(std::unordered_map<std::string,unsigned int >::const_iterator it = reg_map.begin();
//        it != reg_map.end(); ++it)
//    {
//        std::cout << it->first << " " << it->second <<  "\n";
//    }
//}
//void VirtualMachine::print_mem(int min, int max)
//{
//    if(max >=512)
//    {
//        std::cerr << " out of bounds " << std::endl;
//    }
//   for(int i =min; i< max ; i++)
//   {
//       printf("p[%d] = %x\n", i, memory[i]);
//   }
//
//
//}

unsigned int VirtualMachine::load_word(unsigned int index) {

    int Int = memory[index] | ((int) memory[index + 1] << 8) | ((int) memory[index + 2] << 16) |
              ((int) memory[index + 3] << 24);


    return Int;

}

char VirtualMachine::at_mem(const int &index) {
    return memory[index];
}

VirtualMachine *VirtualMachine::s_instance = nullptr;

VirtualMachine *VirtualMachine::getVirtualMachine() {
    static VirtualMachine vm;
    if (!s_instance)
        s_instance = new VirtualMachine;
    return s_instance;

}

bool VirtualMachine::valid_constant(std::string s) {
    if (constant_list.find(s) == constant_list.end()) {
        return false;
    } else {
        return true;
    }
}

bool VirtualMachine::valid_label(std::string s) {
    if (label_map.find(s) == label_map.end()) {
        return false;
    } else {
        return true;
    }
}

void VirtualMachine::add_instruction(std::string operation, std::string dest) {
    if (instruction_list.empty())
        label_address = 0;

    if (operation == "mfhi") {



//    ::LW  ab(dest,OpA);
//    aa= &ab;
        temp = new MFHI(dest);;
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "mflo") {
        temp = new MFLO(dest);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "j") {
        temp = new J(dest);
        instruction_list.push_back(temp);
        label_address++;
    }
}

void VirtualMachine::add_instruction(std::string operation, std::string dest, std::string OpA) {
    if (instruction_list.empty())
        label_address = 0;

    if (operation == "nop") {
        temp = new NOP();
        instruction_list.push_back(temp);
        label_address++;
    }
    if (operation == "lw") {


//    ::LW  ab(dest,OpA);
//    aa= &ab;
        temp = new LW(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "la") {
        temp = new LA(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "li") {
        temp = new LI(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "sw") {
        temp = new SW(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "move") {
        temp = new MOVE(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "not") {
        temp = new NOT(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "mult") {
        temp = new MULT_S(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "multu") {
        temp = new MULT_U(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "div") {
        temp = new DIV2_S(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "divu") {
        temp = new DIV2_U(dest, OpA);
        instruction_list.push_back(temp);
        label_address++;
    }
}

void VirtualMachine::add_instruction(std::string operation, std::string dest, std::string OpA, std::string OpB) {
    if (instruction_list.empty())
        label_address = 0;
    if (operation == "and") {


//    ::LW  ab(dest,OpA);
//    aa= &ab;
        temp = new AND(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "nor") {
        temp = new NOR(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "or") {
        temp = new OR(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "xor") {
        temp = new XOR(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "beq") {
        temp = new BEQ(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "bne") {
        temp = new BNE(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "blt") {
        temp = new BLT(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "ble") {
        temp = new BLE(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "bgt") {
        temp = new BGT(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "bge") {
        temp = new BGE(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "add") {
        temp = new ADD_S(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "addu") {
        temp = new ADD_U(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "sub") {
        temp = new SUB_S(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "subu") {
        temp = new SUB_U(dest, OpA, OpB);
        instruction_list.push_back(temp);
        label_address++;
    }


}


void VirtualMachine::add_instruction_off(std::string operation, std::string dest, std::string OpA, int off) {

    if (instruction_list.empty())
        label_address = 0;
    if (operation == "lw") {


//    ::LW  ab(dest,OpA);
//    aa= &ab;
        temp = new LW(dest, OpA, off);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "la") {
        temp = new LA(dest, OpA, off);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "li") {
        temp = new LI(dest, OpA, off);
        instruction_list.push_back(temp);
        label_address++;
    } else if (operation == "sw") {
        temp = new SW(dest, OpA, off);
        instruction_list.push_back(temp);
        label_address++;
    }


}

void VirtualMachine::set_pc(int k) {
    pc = k;
}

VirtualMachine::~VirtualMachine() {



    // delete s_instance;
    for (size_t i = 0; i < instruction_list.size(); i++) {

        delete instruction_list[i];
    }

    label_map.clear();
    instruction_list.clear();
    reg_map.clear();
    //if(s_instance)
    //delete s_instance;

}

void VirtualMachine::reg_change() {
    reg_map["$0"] = reg_map["$zero"];
    reg_map["$1"] = reg_map["$at"];
    reg_map["$2"] = reg_map["$v0"];
    reg_map["$3"] = reg_map["$v1"];
    reg_map["$4"] = reg_map["$a0"];
    reg_map["$5"] = reg_map["$a1"];
    reg_map["$6"] = reg_map["$a2"];
    reg_map["$7"] = reg_map["$a3"];
    reg_map["$8"] = reg_map["$t0"];
    reg_map["$9"] = reg_map["$t1"];
    reg_map["$10"] = reg_map["$t2"];
    reg_map["$11"] = reg_map["$t3"];
    reg_map["$12"] = reg_map["$t4"];
    reg_map["$13"] = reg_map["$t5"];
    reg_map["$14"] = reg_map["$t6"];
    reg_map["$15"] = reg_map["$t7"];
    reg_map["$16"] = reg_map["$s0"];
    reg_map["$17"] = reg_map["$s1"];
    reg_map["$18"] = reg_map["$s2"];

    reg_map["$19"] = reg_map["$s3"];
    reg_map["$20"] = reg_map["$s4"];
    reg_map["$21"] = reg_map["$s5"];
    reg_map["$22"] = reg_map["$s6"];
    reg_map["$23"] = reg_map["$s7"];
    reg_map["$24"] = reg_map["$t8"];
    reg_map["$25"] = reg_map["$t9"];
    reg_map["$26"] = reg_map["$k0"];
    reg_map["$27"] = reg_map["$k1"];
    reg_map["$28"] = reg_map["$gp"];
    reg_map["$29"] = reg_map["$sp"];
    reg_map["$30"] = reg_map["$fp"];
    reg_map["$31"] = reg_map["$ra"];
}

unsigned long VirtualMachine::inst_size() {
    return instruction_list.size();

}



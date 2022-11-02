

#include "parser.hpp"

Parser::Parser()
        : m_tl(TokenList()), m_it(m_tl.begin()) {

    // do nothing

}

Parser::~Parser() {
    // do nothing
    m_tl.clear();
    //  m_it = nullptr;
}

bool Parser::tokenize(std::string &str) {
    std::istringstream iss(str);
    return tokenize(iss);
}

bool Parser::tokenize(std::istream &ins) {
    m_tl = ::tokenize(ins);
    m_it = m_tl.begin();
    return m_tl.back().type() != ERROR;
}

//bool Parser::tokenize_file( std::string file_name) {
//   // std::ifstream ifs(file_name);
//    std::ifstream ifs;
//    ifs.open (file_name, std::ifstream::in);
//    std::string temp;
//    char c;
//    while(ifs.get(c))
//    {
//        temp.push_back(c);
//    }
//    std::cout << temp;
//    std::istringstream s(temp);
//   // std::istream &s = ifs;
//    return tokenize(s);
//}

const std::string Parser::get_error() {
    if (m_it == m_tl.end())
        return "";
    return "Error:" + std::to_string(m_it->line()) +
           ": " + m_it->value() + "\n";
}

bool Parser::is_char(char c) {
    // <char>          ::= any printable ASCII character (see std::isprint)
    return std::isprint(static_cast<unsigned char>(c));
}

bool Parser::is_alpha(char c) {
    // <alpha>         ::= 'a' | 'b' | ... | 'z' | 'A' | 'B' | ... | 'Z' | '_'
    return (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (c == '_');
}

bool Parser::is_digit(char c) {
    // <digit>         ::= '0' | '1' | ... | '9'
    return ('0' <= c) && (c <= '9');
}

bool Parser::is_section_specifier() {
    if (m_it->type() == STRING &&
        (m_it->value() == ".data" || m_it->value() == ".text"))
        return true;
    return false;
}


bool Parser::is_op(char c) {
    return (c == '+') || (c == '-') || (c == '/') || (c == '*');
}
// data section

bool Parser::parse_string() {
    TokenList::iterator it_start = m_it;

    // <string>        ::= '"' {<char>} '"'
    if (m_it->type() != STRING_DELIM)
        goto bail_out;
    if ((++m_it)->type() != STRING)
        goto bail_out;
    if ((++m_it)->type() != STRING_DELIM)
        goto bail_out;
    ++m_it;
    return true;
    bail_out:
    m_it = it_start;
    return false;
}


bool Parser::parse_constant() {
    //<constant>      ::= <alpha> { <digit> | <alpha> } EQUAL <integer>
    TokenList::iterator it_start = m_it;


    std::size_t i = 0;
    std::size_t n = m_it->value().length();
    std::string constants = m_it->value();
    int value = 0;
    auto c = m_it->value().begin();
    if (!is_alpha(*c)) {
        m_it = it_start;
        return false;
    }
    c++;

    for (; i < n - 1; ++c, ++i) {
        if (!is_alpha(*c) && !is_digit(*c)) {
            m_it = it_start;
            return false;
        }

    }


    if ((++m_it)->type() != EQUAL) {
        m_it = it_start;
        return false;
    }
    ++m_it;
    value = atoi(m_it->value().c_str());


    if (parse_integer()) {
        VirtualMachine::getVirtualMachine()->add_constant(constants, value);
        std::pair<std::string, int> value_pair(constants, value);
        constant_list.insert(value_pair);
    } else {
        goto bail_out;
    }


    return true;


    bail_out:
    m_it = it_start;
    return false;
}


bool Parser::parse_declaration() {
    data_true_text_false = true;
    //<declaration>   ::= <constant> EOL | <label> EOL | [<label>] <layout> EOL
    TokenList::iterator it_start = m_it;
    if (m_it->value() == ".data") {
        m_it++;
        m_it++;
    }
    temp_label = "";
    temp_label = m_it->value();
    if (parse_constant()) {

        if (m_it->type() != EOL)
            goto bail_out;
    } else if (parse_label()) {

        std::string type = m_it->value();
        auto it = m_it;
        it++;


        it++;
        if (it->type() == SEP) {
            while (it->type() == SEP) {

                it++;
                it++;

            }

        } else if (m_it->type() == EOL) {
//            types t = (data_true_text_false) ? t = data : t = text;
//
//             m_label = new Labels(t,VirtualMachine::getVirtualMachine()->get_label_add());
//            VirtualMachine::getVirtualMachine()->add_label(temp_label,*m_label);

            //  label_add+=1;

            ++m_it;
            return true;
        } else if (parse_layout()) {

            //fix when there is more than one int
            //loop that adds from array of ints, until size of ints
            //std::cout << label_add << std::endl;
//                types t = (data_true_text_false) ? t = data : t = text;
//            //FIX THIS




            if ((m_it)->type() != EOL)
                goto bail_out;
        } else {
            goto bail_out;
        }
    } else if (parse_layout()) {

        if ((m_it)->type() != EOL)
            goto bail_out;
    } else {
        goto bail_out;
    }
    ++m_it;
    return true;

    bail_out:
    m_it = it_start;
    return false;
}


bool Parser::parse_label() {
    //<label>         ::= <alpha> { <digit> | <alpha> } ':'
    TokenList::iterator it_start = m_it;

    std::size_t i = 0;
    std::size_t n = m_it->value().length();
    auto c = m_it->value().begin();
    if (!is_alpha(*c))
        goto bail_out;
    for (; i < n - 1; ++c, ++i) {
        if (!is_alpha(*c) && !is_digit(*c))
            goto bail_out;
    }

    if (*c != ':')
        goto bail_out;
    ++m_it;
    return true;


    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_layout() {
    // <layout>        ::= <int_layout> <integer> {',' <integer> } | <string_layout> <string>
    TokenList::iterator it_start = m_it;

    std::string layout = m_it->value();

    if (parse_int_layout()) {
        std::string int_value = m_it->value();
        auto found = constant_list.find(m_it->value());
        if (found != constant_list.end()) {
            ++m_it;

            //FIX THIS
            m_label = new Labels(data, VirtualMachine::getVirtualMachine()->get_data_add());

            VirtualMachine::getVirtualMachine()->add_label(temp_label, *m_label);
            //std::string result = VirtualMachine::getVirtualMachine()->to_hex(int_value);
            VirtualMachine::getVirtualMachine()->add_data(layout, int_value, false);
            return true;
        } else if (parse_integer()) {
            //checking for integer bounds

            bool signed_int = (int_value[0] == '-') || (int_value[0] == '+'); //if negative true, otherwise false
            long
            int i_dec = std::stol(int_value);

            if (layout == ".word") {

                //int32_t word = stoi(int_value);
                // std::cout << word << std::endl;
                if (!signed_int) {

                    if (!(((unsigned long) i_dec <= ULONG_MAX) && (i_dec >= 0))) {
                        std::cerr << "Error: number out of bounds" << std::endl;

                        goto bail_out;
                    }
                } else if (signed_int) {
                    if (!(((long int) i_dec <= LONG_MAX) && ((long int) i_dec >= LONG_MIN))) {
                        std::cerr << "Error: number out of bounds" << std::endl;
                        goto bail_out;
                    }
                }
                //types t = (data_true_text_false) ? t = data : t = text;
                //FIX THIS
                m_label = new Labels(data, VirtualMachine::getVirtualMachine()->get_data_add());

                VirtualMachine::getVirtualMachine()->add_label(temp_label, *m_label);
                //std::string result = VirtualMachine::getVirtualMachine()->to_hex(int_value);
                VirtualMachine::getVirtualMachine()->add_data(layout, int_value, signed_int);


                //  label_add +=4;
            } else if (layout == ".byte") {
                int8_t byte = stoi(int_value);
                if (!signed_int) {
                    if (!((i_dec <= UCHAR_MAX) && (i_dec >= 0))) {
                        std::cerr << "Error: number out of bounds" << std::endl;

                        goto bail_out;
                    }
                } else {
                    if (!((i_dec <= SCHAR_MAX) && (i_dec >= SCHAR_MIN))) {
                        std::cerr << "Error: " << byte << " out of bounds" << std::endl;
                        goto bail_out;
                    }
                }

                // types t = (data_true_text_false) ? t = data : t = text;
                //FIX THIS
                m_label = new Labels(data, VirtualMachine::getVirtualMachine()->get_data_add());

                VirtualMachine::getVirtualMachine()->add_label(temp_label, *m_label);
                VirtualMachine::getVirtualMachine()->add_data(layout, int_value, signed_int);

                //   label_add +=1;
            } else if (layout == ".half") {
                int16_t half = std::stoi(int_value);
                if (!signed_int) {
                    if (!(((unsigned) i_dec <= UINT_MAX) && (i_dec >= 0))) {
                        std::cerr << "Error: " << half << " out of bounds" << std::endl;

                        goto bail_out;
                    }
                } else {
                    if (!(((signed) i_dec <= INT_MAX) && ((signed) i_dec >= INT_MIN))) {
                        std::cerr << "Error: number out of bounds" << std::endl;
                        goto bail_out;
                    }
                }
                //types t = (data_true_text_false) ? t = data : t = text;
                //FIX THIS
                m_label = new Labels(data, VirtualMachine::getVirtualMachine()->get_data_add());

                VirtualMachine::getVirtualMachine()->add_label(temp_label, *m_label);

                VirtualMachine::getVirtualMachine()->add_data(layout, int_value, signed_int);

                // label_add +=2;

            } else if (layout == ".space") {

                // std::string result = VirtualMachine::getVirtualMachine()->to_hex(int_value);

                //types t = (data_true_text_false) ? t = data : t = text;
                //FIX THIS
                m_label = new Labels(data, VirtualMachine::getVirtualMachine()->get_data_add());

                VirtualMachine::getVirtualMachine()->add_label(temp_label, *m_label);
                VirtualMachine::getVirtualMachine()->add_data(layout, int_value, signed_int);

                //  label_add +=space;
            }


            do {


                if ((m_it)->type() == SEP) {
                    ++m_it;
                    int_value = m_it->value();
                    signed_int = (int_value[0] == '-') || (int_value[0] == '+');
                    VirtualMachine::getVirtualMachine()->add_data(layout, int_value, signed_int);


                    //add function  func(stirng type/layout), value of int or stoi)

                    if (!parse_integer()) {
                        goto bail_out;
                    }
//                                        if(layout ==".word")
//                    {
//                        label_add+=4;
//                    }
//                    else if(layout ==".byte")
//                    {
//                        label_add+=1;
//                    }
//                    else if(layout ==".half")
//                    {
//                        label_add+=2;
//                    }


                }


            } while (m_it->type() == SEP);
        } else {
            goto bail_out;
        }


    } else if (parse_string_layout()) {
        if (parse_string()) {

            return true;
        }
    }


    return true;

    bail_out:
    m_it = it_start;
    // std::cout << get_error() << std::endl;
    return false;

}

bool Parser::parse_integer() {
    TokenList::iterator it_start = m_it;
    std::size_t i = 0;
    // <integer>      ::= ['-' | '+'] <digit> { <digit> }
    if (m_it->type() != STRING)
        goto bail_out;
    if (m_it->value()[0] == '+' || m_it->value()[0] == '-') {

        std::size_t n = m_it->value().size();
        std::string new_string = m_it->value();
        for (i = 0; i < n - 1; i++)
            new_string[i] = m_it->value()[i + 1];  //move all element to the left except first one

        //std::cout << new_string << std::endl;
        for (auto c = new_string.begin(); i < new_string.length(); ++c, ++i) {
            if (!is_digit(*c)) {
                // std::memset(static_cast<void*>(m_it->value),signedvar,sizeof(char));
                goto bail_out;
            }
        }
    } else {
        for (auto c = m_it->value().begin(); i < m_it->value().length(); ++c, ++i) {
            if (!is_digit(*c)) {
                // std::memset(static_cast<void*>(m_it->value),signedvar,sizeof(char));
                goto bail_out;
            }
        }
    }
    ++m_it;
    return true;
    bail_out:
    m_it = it_start;

    return false;
}

bool Parser::parse_integer(TokenList::iterator t) {
    TokenList::iterator it_start = t;
    std::size_t i = 0;
    // <integer>      ::= ['-' | '+'] <digit> { <digit> }
    if (t->type() != STRING)
        goto bail_out;
    if (t->value()[0] == '+' || t->value()[0] == '-') {

        std::size_t n = t->value().size();
        std::string new_string = t->value();
        for (i = 0; i < n - 1; i++)
            new_string[i] = t->value()[i + 1];  //move all element to the left except first one

        //std::cout << new_string << std::endl;
        for (auto c = new_string.begin(); i < new_string.length(); ++c, ++i) {
            if (!is_digit(*c)) {
                // std::memset(static_cast<void*>(m_it->value),signedvar,sizeof(char));
                goto bail_out;
            }
        }
    } else {
        for (auto c = t->value().begin(); i < t->value().length(); ++c, ++i) {
            if (!is_digit(*c)) {
                // std::memset(static_cast<void*>(m_it->value),signedvar,sizeof(char));
                goto bail_out;
            }
        }
    }
    ++t;
    return true;
    bail_out:
    t = it_start;

    return false;
}

bool Parser::parse_string_layout() {
    TokenList::iterator it_start = m_it;

    // <string layout> ::= '.ascii' | '.asciiz'
    if (m_it->type() != STRING)
        goto bail_out;
    if (m_it->value() != ".ascii" && m_it->value() != ".asciiz")
        goto bail_out;
    ++m_it;
    return true;
    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_int_layout() {
    TokenList::iterator it_start = m_it;

    // <int_layout>    ::= '.word' | '.half' | '.byte' | '.space'
    if (m_it->type() != STRING)
        goto bail_out;

    if (m_it->value() != ".word" && m_it->value() != ".half" &&
        m_it->value() != ".byte" && m_it->value() != ".space")
        goto bail_out;

    ++m_it;
    return true;
    bail_out:
    m_it = it_start;
    return false;
}




////////////////////////////////////////////////////////////////////////////////////////////////////
// text section

bool Parser::parse_instruction() {
    is_offset = false;
    data_true_text_false = false;
    curr_arg = 0;
    TokenList::iterator it_start = m_it;
    temp_label = m_it->value();

    //  operation = m_it->value();
    if (parse_label()) {
        int label_a = VirtualMachine::getVirtualMachine()->get_label_add2();

        m_label = new Labels(text, label_a);
        VirtualMachine::getVirtualMachine()->add_inst_label(temp_label, *m_label);
        label_add += 1;
        std::string operation = m_it->value();
        if (m_it->type() == EOL) {
            // types t = (data_true_text_false) ? t = data : t = text;

//            m_label = new Labels(text, VirtualMachine::getVirtualMachine()->get_label_add2());
//            VirtualMachine::getVirtualMachine()->add_inst_label(temp_label, *m_label);
//
//            label_add += 1;
            ++m_it;
            return true;
        } else if (parse_operation()) {

            if (m_it->type() != EOL)
                goto bail_out;
        }

    } else if (parse_operation()) {
        if (m_it->type() != EOL)
            goto bail_out;
    } else {
        goto bail_out;
    }

    ++m_it;
    return true;
    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_text_constant() {
    TokenList::iterator it_start = m_it;

    std::size_t i = 0;
    std::size_t n = m_it->value().length();
    auto c = m_it->value().begin();
    if (!is_alpha(*c))
        goto bail_out;
    if (n > 1) {
        c++;
        for (; i < n - 1; ++c, ++i) {
            if (!is_alpha(*c) && !is_digit(*c))
                goto bail_out;
        }
    }

    ++m_it;
    return true;


    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_immediate() {
    TokenList::iterator it_start = m_it;
    if (parse_text_constant()) {

        return true;
    } else if (parse_integer()) {

        return true;

    } else {
        goto bail_out;
    }

    bail_out:
    m_it = it_start;
    return false;
}


bool Parser::parse_source() {
    TokenList::iterator it_start = m_it;
    if (parse_register())
        return true;

    else if (parse_immediate())
        return true;
    else {
        goto bail_out;
    }


    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_op_args() {
    TokenList::iterator it_start = m_it;

    auto opcode = data_mov_inst.find(m_it->value());
    auto opcode_int = int_arithmetic_inst.find(m_it->value());
    auto opcode_logic = logic_inst.find(m_it->value());
    auto opcode_control = control_inst.find(m_it->value());
    if (opcode != data_mov_inst.end()) {
        if (m_it->value() == "lw" || m_it->value() == "lh" || m_it->value() == "sb" || m_it->value() == "la"
            || m_it->value() == "sh" || m_it->value() == "sw" || m_it->value() == "lb") {
            ++m_it;
            num_args = 2;
            arguments[0] = m_it->value();

            curr_arg++;
            if (!parse_register())
                goto bail_out;

            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;


            if (!parse_memref())
                goto bail_out;

            if (!is_offset) {
                if (operation == "lw") {
                    VirtualMachine::getVirtualMachine()->add_instruction("lw", arguments[0], arguments[1]);

                } else if (operation == "la") {
                    VirtualMachine::getVirtualMachine()->add_instruction("la", arguments[0], arguments[1]);

                } else if (operation == "sw") {
                    VirtualMachine::getVirtualMachine()->add_instruction("sw", arguments[0], arguments[1]);

                }
            }


        } else if (m_it->value() == "mfhi" || m_it->value() == "mflo" || m_it->value() == "mthi" ||
                   m_it->value() == "mtlo") {
            ++m_it;
            arguments[0] = m_it->value();
            curr_arg++;
            num_args = 1;
            if (!parse_register())
                goto bail_out;

            if (operation == "mfhi") {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0]);

            } else if (operation == "mflo") {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0]);

            }


        } else if (m_it->value() == "li") {
            ++m_it;
            num_args = 2;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;

            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;

            if (!parse_immediate())
                goto bail_out;

            if (!is_offset) {
                if (operation == "li") {
                    VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1]);

                }
            }


            // VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], var, offset);


        } else if (m_it->value() == "move") {
            ++m_it;
            num_args = 2;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;

            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;

            if (!parse_register())
                goto bail_out;

            if (!is_offset) {
                if (operation == "move") {
                    VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1]);

                }
            }

            //  VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], var, offset);


        }

    } else if (opcode_int != int_arithmetic_inst.end()) {
        if (m_it->value() == "add" || m_it->value() == "addu" || m_it->value() == "sub" || m_it->value() == "subu"
            || m_it->value() == "mul" || m_it->value() == "mulo" || m_it->value() == "mulou"
            || m_it->value() == "rem" || m_it->value() == "remu") {
            ++m_it;
            num_args = 3;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;


            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[2] = m_it->value();
            curr_arg++;

            if (!parse_source())
                goto bail_out;

            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1],
                                                                     arguments[2]);
            }
            //VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], arguments[2],offset);




        } else if (m_it->value() == "mult" || m_it->value() == "multu" || m_it->value() == "abs" ||
                   m_it->value() == "neg" || m_it->value() == "negu") {
            ++m_it;
            num_args = 2;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;

            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;


            if (!parse_register())
                goto bail_out;
            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1]);
            }
            //  VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], arguments[2],offset);



        } else if (m_it->value() == "div" || m_it->value() == "divu") {
            ++m_it;
            num_args = 2;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;

            if (parse_register()) {
                if (m_it->type() == SEP) {
                    ++m_it;
                    if (!parse_source())
                        goto bail_out;
                } else {
                    if (!is_offset) {
                        VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1]);
                        //   VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], arguments[2],offset);
                    }
                    return true;
                }
            }
            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1]);
                //   VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], arguments[2],offset);
            }


        }

    } else if (opcode_logic != logic_inst.end()) {
        if (m_it->value() == "and" || m_it->value() == "nor" || m_it->value() == "or" || m_it->value() == "xor") {
            ++m_it;
            num_args = 3;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[2] = m_it->value();
            curr_arg++;

            if (!parse_source())
                goto bail_out;
            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1],
                                                                     arguments[2]);
            }
            // VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], arguments[2],offset);


        } else if (m_it->value() == "not") {
            ++m_it;
            num_args = 2;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;
            if (!parse_source())
                goto bail_out;
            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1]);
            }
        }

    } else if (opcode_control != control_inst.end()) {
        if (m_it->value() == "beq" || m_it->value() == "bne" || m_it->value() == "blt" || m_it->value() == "ble" ||
            m_it->value() == "bgt" || m_it->value() == "bge") {
            ++m_it;
            num_args = 3;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_register())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[1] = m_it->value();
            curr_arg++;
            if (!parse_source())
                goto bail_out;
            if (m_it->type() != SEP)
                goto bail_out;
            ++m_it;
            arguments[2] = m_it->value();
            curr_arg++;
            if (!parse_text_constant())
                goto bail_out;
            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], arguments[1],
                                                                     arguments[2]);
            }

        } else if (m_it->value() == "j") {
            ++m_it;
            num_args = 1;
            arguments[0] = m_it->value();
            curr_arg++;
            if (!parse_text_constant())
                goto bail_out;
            if (!is_offset) {
                VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0]);
            }
        }
    } else {
        goto bail_out;
    }


    return true;
    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_opcode() {

    TokenList::iterator it_start = m_it;
    auto opcode = valid_op.find(m_it->value());
    if (opcode == valid_op.end())
        goto bail_out;


    operation = m_it->value();
    return true;
    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_operation() {
    TokenList::iterator it_start = m_it;
    operation = m_it->value();
    if (m_it->value() == "nop") {
        VirtualMachine::getVirtualMachine()->add_instruction("nop", "0", ")"); // empty nop
        ++m_it;
    } else if (parse_opcode()) {
        if (!parse_op_args())
            goto bail_out;
    } else {
        goto bail_out;
    }


    return true;
    bail_out:
    m_it = it_start;
    return false;
}

bool Parser::parse_register() {
    TokenList::iterator it_start = m_it;
    auto valid_reg = registers.find(m_it->value());
    if (valid_reg == registers.end())
        goto bail_out;

    ++m_it;
    return true;
    bail_out:
    m_it = it_start;
    return false;
}


bool Parser::parse_memref() {
    //<memref> ::= <label> | <immediate> | <register> | [offset] '(' <label> ')' | [offset] '(' <immediate> ')' | [offset] '(' <register> ')'
    TokenList::iterator it_start = m_it;
    int offset;
    std::string s = m_it->value();
    if (!parse_text_constant()) {
        if (!parse_register()) {
            if (!(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))) {

                offset = std::stoi(s);
            }

        } else {
            return true;
        }


    } else {
        return true;
    }

    if (parse_text_constant()) {

        return true;
    } else if (parse_integer()) {
        if (m_it->type() == EOL) {
            VirtualMachine::getVirtualMachine()->add_instruction(operation, arguments[0], s);
            is_offset = true;
            return true;
        } else if (m_it->type() == OPEN_PAREN) {
            ++m_it;
            is_offset = true;

            // arguments[curr_arg-1] = m_it->value();
            auto it = m_it;
            it--;
            it--;
            std::string reg = arguments[0];
            std::string arg = m_it->value();
            if (parse_register()) {

                if (m_it->type() != CLOSE_PAREN)
                    goto bail_out;
                if (is_offset) {
                    arguments[1] = it->value();
                    if (operation == "div" || operation == "divu" || operation == "move" || operation == "li" ||
                        operation == "lw" || operation == "la" || operation == "sw")
                        VirtualMachine::getVirtualMachine()->add_instruction_off(operation, reg, arg, offset);

                }


            } else if (parse_text_constant()) {

                if (m_it->type() != CLOSE_PAREN)
                    goto bail_out;
                if (is_offset) {
                    arguments[1] = it->value();
                    if (operation == "div" || operation == "divu" || operation == "mult" || operation == "multu" ||
                        operation == "abs" || operation == "neg" || operation == "negu" || operation == "move" ||
                        operation == "li" ||
                        operation == "lw" || operation == "la" || operation == "sw")
                        VirtualMachine::getVirtualMachine()->add_instruction_off(operation, reg, arg, offset);

                }
            } else if (parse_immediate()) {
                ++m_it;
                if (m_it->type() != CLOSE_PAREN)
                    goto bail_out;
                if (is_offset) {
                    arguments[1] = it->value();
                    if (operation == "div" || operation == "divu" || operation == "mult" || operation == "multu" ||
                        operation == "abs" || operation == "neg" || operation == "negu" || operation == "move" ||
                        operation == "li" ||
                        operation == "lw" || operation == "la" || operation == "sw")
                        VirtualMachine::getVirtualMachine()->add_instruction_off(operation, reg, arg, offset);

                }
            }


        }


    } else if (parse_register()) {

        return true;
    } else if (m_it->type() == OPEN_PAREN) {
        ++m_it;
        auto it = m_it;

        if (parse_register()) {
            if (m_it->type() != CLOSE_PAREN)
                goto bail_out;
            arguments[1] = it->value();
            if (operation == "div" || operation == "divu" || operation == "mult" || operation == "multu" ||
                operation == "abs" || operation == "neg" || operation == "negu" || operation == "move" ||
                operation == "li" ||
                operation == "lw" || operation == "la" || operation == "sw")
                VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], 0);

        } else if (parse_immediate()) {
            m_it++;
            if (m_it->type() != CLOSE_PAREN)
                goto bail_out;
            arguments[1] = it->value();
            if (operation == "div" || operation == "divu" || operation == "mult" || operation == "multu" ||
                operation == "abs" || operation == "neg" || operation == "negu" || operation == "move" ||
                operation == "li" ||
                operation == "lw" || operation == "la" || operation == "sw")
                VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], 0);


        } else if (parse_label()) {
            m_it++;
            if (m_it->type() != CLOSE_PAREN)
                goto bail_out;

            arguments[1] = it->value();
            if (operation == "div" || operation == "divu" || operation == "mult" || operation == "multu" ||
                operation == "abs" || operation == "neg" || operation == "negu" || operation == "move" ||
                operation == "li" ||
                operation == "lw" || operation == "la" || operation == "sw")
                VirtualMachine::getVirtualMachine()->add_instruction_off(operation, arguments[0], arguments[1], 0);

        }
    } else if (parse_integer()) {
        ++m_it;
        return true;
    } else {
        goto bail_out;
    }


    ++m_it;
    return true;
    bail_out:
    m_it = it_start;
    return false;
}


//bool Parser::parse_file(const std::string &file_name) {
//    if (!tokenize_file(file_name))
//        return false;
//
//    enum sections {
//        data, text
//    };
//    sections current_section;
//
//
//    if (m_it->value() != ".data") {
//        std::cerr << "The file must start with .data, syntax error" << std::endl;
//        return false;
//    }
//    current_section = data;
//    ++m_it;
//    ++m_it;
//    if (m_it->value() == ".text") {
//        ++m_it;
//        ++m_it;
//        current_section = text;
//    }
//    data:
//
//    while (current_section == data && m_it != m_tl.end()) {
//
//        if (!parse_declaration() && m_it->value() != ".text") {
//            ++m_it;
//            ++m_it;
//            if ((m_it)->value() != ".text")
//                return false;
//        }
//
//        if (m_it->value() == ".text") {
//            ++m_it;
//            ++m_it;
//            current_section = text;
//            break;
//        }
//
//
//    }
//
//    while (current_section == text && m_it != m_tl.end()) {
//
//        if ((!parse_instruction()) && m_it->value() != ".data")
//            return false;
//
//        if (m_it->value() == ".data") {
//            current_section = data;
//            ++m_it;
//            ++m_it;
//            goto data;
//        }
//
//    }
//    if (m_it == m_tl.end())
//        return true;
//
//
//    return false;
//}


bool Parser::parse_text() {
//    if (!tokenize_file(m_text))
//        return false;1
    TokenList::iterator it = m_tl.begin();
    for(;it !=m_tl.end();++it)
    {
        if(it->value() == ".text")
        {
            text_line = it->line();
        }
    }
    enum sections {
        data, text
    };
    sections current_section;


    if (m_it->value() != ".data") {
        std::cerr << "The file must start with .data, syntax error" << std::endl;
        return false;
    }
    current_section = data;
    ++m_it;
    ++m_it;
    if (m_it->value() == ".text") {
        ++m_it;
        ++m_it;
        current_section = text;
    }
    data:

    while (current_section == data && m_it != m_tl.end()) {

        if (!parse_declaration() && m_it->value() != ".text") {
            ++m_it;
            ++m_it;
            if ((m_it)->value() != ".text")
                return false;
        }

        if (m_it->value() == ".text") {
            ++m_it;
            ++m_it;
            current_section = text;
            break;
        }


    }

    while (current_section == text && m_it != m_tl.end()) {

        if ((!parse_instruction()) && m_it->value() != ".data")
            return false;
        instructions++;
        if (m_it->value() == ".data") {
            current_section = data;
            ++m_it;
            ++m_it;
            goto data;
        }


    }
    if (m_it == m_tl.end())
        return true;


    return false;
}

int Parser::get_line_num() {

    return text_line+1;

}
int Parser::get_instructions()
{
    return instructions;
}

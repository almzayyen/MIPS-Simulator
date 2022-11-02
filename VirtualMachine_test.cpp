//
// Created by abs on 3/18/18.
//

#include<string>


#include<istream>
#include<sstream>

#include"catch.hpp"
#include"VirtualMachine.h"
#include"parser.hpp"

TEST_CASE("test VM functions", "[parser]") {

    {
        std::string str_true = R"( .data
        r1:     .space  4
        r2:     .space  12
        r3:     .space  4
        var:    .word   7
        var2:   .byte  2
        var3:   .word  8

                .text
        main:
        la $t0, r2
        lw $t1, var
        lw $t3, var3

        sw $t1, 0
        sw $t1, $t0
        sw $t1, 4($t0)
        sw $t1, 8(r2)
        sw $t1, r3)";

        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        REQUIRE(VirtualMachine::getVirtualMachine()->exec());
        VirtualMachine::getVirtualMachine()->write_reg("$at", 5);
        VirtualMachine::getVirtualMachine()->reg_change();
        REQUIRE(VirtualMachine::getVirtualMachine()->load_word(20) == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_reg("$1") == 5);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$1") == 5);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_label_addval("r1") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_label_addval("r2") == 4);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_label_addval("r3") == 16);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_label_addval("var") == 20);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_label_addval("var2") == 24);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_label_addval("var3") == 25);
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$t1"));
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$t2"));
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$t3"));
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$t4"));
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$a0"));
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$a1"));
        REQUIRE(VirtualMachine::getVirtualMachine()->valid_reg("$s1"));
        VirtualMachine::getVirtualMachine()->add_data(".word", "5", false);
        VirtualMachine::getVirtualMachine()->store_mem(5, 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(0) == 5);
        VirtualMachine::getVirtualMachine()->store_mem(2, 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(1) == 2);
        VirtualMachine::getVirtualMachine()->store_mem(22, 3);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(3) == 22);
        VirtualMachine::getVirtualMachine()->reset_mem();
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(0) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->load_word(0) == 0);
        VirtualMachine::getVirtualMachine()->add_instruction("lw", "$t0", "$t1");
        Labels s = VirtualMachine::getVirtualMachine()->return_label("r1");


        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(VirtualMachine::getVirtualMachine()->at_reg("$t0") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_reg("$sp") == 0);
    }


}

TEST_CASE("test VM testcases", "[parser]") {

    {
        std::string str_true = R"( .data
        r1:     .space  4
        r2:     .space  12
        r3:     .space  4
        var:    .word   7
        var2:   .byte  2
        var3:   .word  8

                .text
        main:
        la $t0, r2
        lw $t1, var
        lw $t3, var3

        sw $t1, 0
        sw $t1, $t0
        sw $t1, 4($t0)
        sw $t1, 8(r2)
        sw $t1, r3)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//       REQUIRE( parser.parse_text());
//      //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {
        std::string str_true = R"(         .data
VALUE = 12
var:    .word 31
        .text
main:
        lw $t0, var
        addu $t1, $t0, VALUE # 31+12=43
        addu $t2, $t1, $t0 # 43+31=74

)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//        REQUIRE(parser.parse_text());
//      //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();


    }


    {
        std::string str_true = R"(         .data
VALUE = 2
var1:   .word 1
var2:   .word 12
var3:   .word -1
        .text
main:
        lw $t0, var1
        lw $t1, var2
        lw $t2, var3
        subu $t3, $t0, VALUE # 1-2 = -1 = 4294967295
        subu $t4, $t1, $t0 # 12-1 = 11
        subu $t5, $t2, VALUE # -1-2 = -3 = 4294967293

)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//        REQUIRE(parser.parse_text());
//       // VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();



    }

    {
        std::string str_true = R"(            .data
        .space 8
var1:   .word 1
var2:   .word -2

        .text
main:
     	la $t0, var1

	lw $t1, 5
	lw $t2, $t0
	lw $t3, 4($t0)
	lw $t4, 4(var1)
	lw $t5, var2



)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//        REQUIRE(parser.parse_text());
//       //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();



    }

    {
        std::string str_true = R"(         .data
r1:     .space 4
r2:     .space 12
r3:     .space 4
var:    .word 7
var2:   .byte 2
var3:   .word 8

        .text
main:
        la $t0, r2
     	lw $t1, var
     	lw $t3, var3

	sw $t1, 0
	sw $t1, $t0
	sw $t1, 4($t0)
	sw $t1, 8(r2)
	sw $t1, r3






)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//        REQUIRE(parser.parse_text());
//        //VirtualMachine::getVirtualMachine()->print_inst();
//       // parser.print_token_list();
//        std::cout <<  VirtualMachine::getVirtualMachine()->get_label_addval("var1")<< std::endl;
//        VirtualMachine::getVirtualMachine()->step();
//        std::cout << "t0" << VirtualMachine::getVirtualMachine()->get_reg("$t0")<< std::endl;
//        VirtualMachine::getVirtualMachine()->step();
//
//        std::cout <<"t1" <<  VirtualMachine::getVirtualMachine()->get_reg("$t1")<< std::endl;
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->print_mem(0,10);
//        std::cout << VirtualMachine::getVirtualMachine()->get_reg("$t2")<< std::endl;
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();



    }


    {
        std::string str_true = R"(        	.data
        n:		.word 10
        sumOfSquares:	.word 0

            #---------------------------------------------------------------
            # the program
            .text
        main:
            lw $t0,n
            li $t1,1
            li $t2,0

        sumLoop:
            mult $t1, $t1
                mflo $t3
            add $t2, $t2, $t3
            add $t1, $t1, 1
            ble $t1, $t0, sumLoop
            sw  $t2, sumOfSquares

        end:
            j end

)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//      //  parser.print_token_list();
//        REQUIRE(parser.parse_text());
//     //   VirtualMachine::getVirtualMachine()->print_inst();
//        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0 );
//        VirtualMachine::getVirtualMachine()->step();
//    //    REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(4) == 81);
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();



    }

    {
        Labels d(data, 0);
        std::string dd = "n:";
        VirtualMachine::getVirtualMachine()->add_label(dd, d);
        std::string s = " lw $t0,n";
        Parser par;
        par.tokenize(s);
//        REQUIRE(VirtualMachine::getVirtualMachine()->valid_label(dd));
        par.parse_instruction();
        // VirtualMachine::getVirtualMachine()->print_inst();
        // VirtualMachine::getVirtualMachine()->add_instruction(op2,dest2,opa2);
    }

    {
        std::string str_true = R"(   	# Example program to compute the sum of squares from Jorgensen [2016]

	#---------------------------------------------------------------
	# data declarations

	.data
n:		.word 10
sumOfSquares:	.word 0

	#---------------------------------------------------------------
	# the program
	.text
main:
	lw $t0,n
	li $t1,1
	li $t2,0

sumLoop:
	mult $t1, $t1
        mflo $t3
	add $t2, $t2, $t3
	add $t1, $t1, 1
	ble $t1, $t0, sumLoop
	sw  $t2, sumOfSquares

end:
	j end





)";
//        Parser parser;
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        VirtualMachine::getVirtualMachine()->reg_init();
//        REQUIRE(parser.tokenize(str_true));
//        //  parser.print_token_list();
//        REQUIRE(parser.parse_text());
//    //    VirtualMachine::getVirtualMachine()->print_inst();
//        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0 );
//        VirtualMachine::getVirtualMachine()->step();
//      int k = VirtualMachine::getVirtualMachine()->get_reg("$t0");
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();
//      VirtualMachine::getVirtualMachine()->step();

        // std::stringstream stream;
        //stream << std::hex << k;
        // // std::string result(  );
        // k =std::stol(stream.str(),nullptr,16);
        //    printf("\n0x%08x\n",k);
//      REQUIRE(k ==  100);
        //    REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(4) == 81);




    }

    {
        std::string str_true = R"(           .data
var0:   .word 0
var1:   .word 1
var2:   .word 2
var3:   .word 3
        .text
main:
        lw $t0, var0
        lw $t1, var1
        lw $t2, var2
        lw $t3, var3

        beq $t0, $t1, check1
        beq $t0, $t0, check1
        nop
check1:
        bne $t0, $t0, check2
        bne $t0, $t1, check2
        nop
check2:
        bgt $t0, $t0, check3
        bgt $t3, $t1, check3
        nop
check3:
        bge $t0, $t1, check4
        bge $t3, $t2, check4
        nop
check4:
        blt $t3, $t1, check5
        blt $t1, $t3, check5
        nop
check5:
        ble $t3, $t1, check6
        ble $t3, $t3, check6
        nop
check6:
        nop
        j check6



        )";
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
//        Parser parser;

//        REQUIRE(parser.tokenize(str_true));
//       REQUIRE(parser.parse_text());
////        printf("\n0x%08x\n", (unsigned char)VirtualMachine::getVirtualMachine()->at_mem(12));
////        printf("\n0x%08x\n", VirtualMachine::getVirtualMachine()->at_mem(13));
////        printf("\n0x%08x\n", VirtualMachine::getVirtualMachine()->at_mem(14));
////        printf("\n0x%08x\n", VirtualMachine::getVirtualMachine()->at_mem(15));
////        VirtualMachine::getVirtualMachine()->print("00000008",false);
////        VirtualMachine::getVirtualMachine()->print("00000009",false);
////        VirtualMachine::getVirtualMachine()->print("0000000a",false);
////        VirtualMachine::getVirtualMachine()->print("0000000c",false);
////        VirtualMachine::getVirtualMachine()->print("0000000d",false);
////        VirtualMachine::getVirtualMachine()->print("0000000e",false);
////        VirtualMachine::getVirtualMachine()->print("0000000f",false);
//     //   VirtualMachine::getVirtualMachine()->print_mem(0,20);
////        //  parser.print_token_list();
////
//       VirtualMachine::getVirtualMachine()->print_label();
//        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0 );
//        VirtualMachine::getVirtualMachine()->step();
////        int k = VirtualMachine::getVirtualMachine()->get_reg("$t0");
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//       // VirtualMachine::getVirtualMachine()->print("$pc",true);
//        VirtualMachine::getVirtualMachine()->step();
//      //  VirtualMachine::getVirtualMachine()->print("$pc",true);
//        VirtualMachine::getVirtualMachine()->step();
//       // VirtualMachine::getVirtualMachine()->print("$pc",true);
//        VirtualMachine::getVirtualMachine()->step();
//
//        VirtualMachine::getVirtualMachine()->step();
//
//        VirtualMachine::getVirtualMachine()->step();



    }
}


TEST_CASE("test VM store memory", "[parser]") {
    {
        std::string str_true = R"(          .data
        .space 8
var1:   .word 1
var2:   .word -2

        .text
main:
     	la $t0, var1

	lw $t1, 0
	lw $t2, $t0
	lw $t3, 4($t0)
	lw $t4, 4(var1)
	lw $t5, var2

)";



//        VirtualMachine::getVirtualMachine()->reset_mem();
//
//        Parser par;
//        VirtualMachine::getVirtualMachine()->reg_init();
//        par.tokenize(str_true);
//        par.parse_text();
//        VirtualMachine::getVirtualMachine()->print_mem(0,20);
//        VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//
//        VirtualMachine::getVirtualMachine()->step();
//        std::cout << "$t2: " << std::hex << VirtualMachine::getVirtualMachine()->get_reg("$t1") << std::endl;
//        std::cout << "$t2: " << std::hex << VirtualMachine::getVirtualMachine()->get_reg("$t2") << std::endl;
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(0)== 0);
//        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(1)== 0);
//        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(2)== 0);
//        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(7)== 0);
//        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(8)== 1);
//        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(11)== 0);
//        unsigned char p =(unsigned char)(VirtualMachine::getVirtualMachine()->at_mem(12));
//        unsigned char c = VirtualMachine::getVirtualMachine()->at_mem(12);
//        REQUIRE((unsigned char)VirtualMachine::getVirtualMachine()->at_mem(12)== 0xfe);
//        REQUIRE((unsigned char)VirtualMachine::getVirtualMachine()->at_mem(13)== 0xff);
//        REQUIRE((unsigned char)VirtualMachine::getVirtualMachine()->at_mem(14)== 0xff);
//        REQUIRE((unsigned char)VirtualMachine::getVirtualMachine()->at_mem(15)== 0xff);
        // VirtualMachine::getVirtualMachine()->print_mem(0,30);
        // VirtualMachine::getVirtualMachine()->print_label();
//        VirtualMachine::getVirtualMachine()->reset_mem();
//        std::string reg_or_label = "adlkf:";
//        std::string s = reg_or_label.substr(0,reg_or_label.length()-1);
//        std::cout << s << std::endl;
//        std::string op2 = "la";
//        std::string dest2 = "$t0";
//        std::string opa2 = "var1";
//        Labels* m_label =   Labels(data,0);
//
//        VirtualMachine::getVirtualMachine()->add_label(opa2,*m_label);
//        VirtualMachine::getVirtualMachine()->add_data(".word","2",false);

//        std::string op =   "lw";
//        std::string dest =  "$t5";
//        std::string opa = "var2";
//
//        std::string op3 =   "li";
//        std::string dest3 =  "$t1";
//        std::string opa3 = "6";
//
//        std::string op4 =   "sw";
//        std::string dest4 =  "$t5";
//        std::string opa4 = "5";
//        std::string op5 =   "sw";
//        std::string dest5 =  "$t5";
//        std::string opa5 = "6";
//
//        std::string op6 =   "sw";
//        std::string dest6 =  "$t5";
//        std::string opa6 = "1";
//       VirtualMachine::getVirtualMachine()->add_instruction(op,dest,opa);
//        VirtualMachine::getVirtualMachine()->add_instruction(op2,dest2,opa2);
//        VirtualMachine::getVirtualMachine()->add_instruction(op3,dest3,opa3);
//        VirtualMachine::getVirtualMachine()->add_instruction(op4,dest4,opa4);
//        VirtualMachine::getVirtualMachine()->add_instruction(op5,dest5,opa5);
//        VirtualMachine::getVirtualMachine()->add_instruction(op6,dest6,opa6);

        // VirtualMachine::getVirtualMachine()->print_inst();
//      VirtualMachine::getVirtualMachine()->print_reg();
//        VirtualMachine::getVirtualMachine()->print_mem(0,10);


    }

}




//TEST_CASE("test VM add_data", "[parser]")
//{
//
//}

TEST_CASE("test functional tests", "[parser]") {

    {//test 00
        std::string str_true = R"(
# simple infinite loop
	        .data
	        .text
             main:	j main
        )";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
        VirtualMachine::getVirtualMachine()->step();
        //   VirtualMachine::getVirtualMachine()->print("$pc",true);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
        VirtualMachine::getVirtualMachine()->step();
        //   VirtualMachine::getVirtualMachine()->print("$pc",true);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
        VirtualMachine::getVirtualMachine()->step();
        //   VirtualMachine::getVirtualMachine()->print("$pc",true);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);

        //  VirtualMachine::getVirtualMachine()->print_inst();

//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 01
        std::string str_true = R"(         .data
        .space 8
var1:   .word 1
var2:   .word -2

        .text
main:
     	la $t0, var1

	lw $t1, 0
	lw $t2, $t0
	lw $t3, 4($t0)
	lw $t4, 4(var1)
	lw $t5, var2)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
        //VirtualMachine::getVirtualMachine()->print_inst();
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(8) == 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(9) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(10) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(11) == 0);
        REQUIRE((unsigned char) VirtualMachine::getVirtualMachine()->at_mem(12) == 254);
        REQUIRE((unsigned char) VirtualMachine::getVirtualMachine()->at_mem(13) == 255);
        REQUIRE((unsigned char) VirtualMachine::getVirtualMachine()->at_mem(14) == 255);
        REQUIRE((unsigned char) VirtualMachine::getVirtualMachine()->at_mem(15) == 255);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 8);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 0);
        VirtualMachine::getVirtualMachine()->step();
        // VirtualMachine::getVirtualMachine()->print("$t2",true);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 3);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t2") == 1);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 4);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t3") == -2);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 5);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t4") == -2);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 6);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t5") == -2);
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 02
        std::string str_true = R"(         .data
r1:     .space 4
r2:     .space 12
r3:     .space 4
var:    .word 7

        .text
main:
        la $t0, r2
     	lw $t1, var

	sw $t1, 0
	sw $t1, $t0
	sw $t1, 4($t0)
	sw $t1, 8(r2)
	sw $t1, r3

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_mem(0,30);


        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 4);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 7);
        VirtualMachine::getVirtualMachine()->step();
        // VirtualMachine::getVirtualMachine()->print_reg();
        //  VirtualMachine::getVirtualMachine()->print_mem(0,30);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 3);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(0) == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(1) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(2) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(3) == 0);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 4);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(4) == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(5) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(6) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(7) == 0);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 5);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(8) == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(9) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(10) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(11) == 0);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 6);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(12) == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(13) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(14) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(15) == 0);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(16) == 7);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(17) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(18) == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->at_mem(19) == 0);

//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 03
        std::string str_true = R"(        .data
A = 0
B = 1
C = 2
D = 4
        .text
main:
        li $t0, 100
        li $t1, A
        li $t2, B
        li $t3, C
        li $t4, D

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 04
        std::string str_true = R"(
        .data
VALUE = -1234

        .text
main:
        li $t0, VALUE
        move $t1, $t0
        move $t2, $t1
        move $t3, $t2
        move $t4, $t3
        move $t5, $t4
        move $t6, $t5
        move $t7, $t6)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//          VirtualMachine::getVirtualMachine()->print_inst();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();

    }


    {//test 04
        std::string str_true = R"(
 .data
VALUE = -1234
var: .word 5
        .text
main:
        li $t0, VALUE
        li $t3, 3
        move $t1, $t3
        move $t2, $t0
        multu $t4, $t0
        move $t5, $t6
        divu $t6, $t5
        move $t7, $t6)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//        VirtualMachine::getVirtualMachine()->print_inst();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();

    }
    {//test 05
        std::string str_true = R"(        .data
VALUE = -1
var:    .word 1
        .text
main:
        lw $t0, var
        add $t1, $t0, VALUE
        add $t2, $t1, $t0

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 0);
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 06
        std::string str_true = R"(         .data
VALUE = 12
var:    .word 31
        .text
main:
        lw $t0, var
        addu $t1, $t0, VALUE # 31+12=43
        addu $t2, $t1, $t0 # 43+31=74

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 07
        std::string str_true = R"(        .data
VALUE = 2
var1:   .word 1
var2:   .word 12
var3:   .word -1
        .text
main:
        lw $t0, var1
        lw $t1, var2
        lw $t2, var3
        sub $t3, $t0, VALUE # 1-2 = -1
        sub $t4, $t1, $t0 # 12-1 = 11
        sub $t5, $t2, VALUE # -1-2 = -3

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 12);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t2") == -1);
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t3") == -1);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t4") == 11);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t5") == -3);

    }

    {//test 08
        std::string str_true = R"(        .data
VALUE = 2
var1:   .word 1
var2:   .word 12
var3:   .word -1
        .text
main:
        lw $t0, var1
        lw $t1, var2
        lw $t2, var3
        sub $t3, $t0, VALUE # 1-2 = -1
        sub $t4, $t1, $t0 # 12-1 = 11
        sub $t5, $t2, VALUE # -1-2 = -3

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());


        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 09
        std::string str_true = R"(             .data
VALUE = 4
VALUE2 = -4
var1:   .word 2
var2:   .word -2
var3:   .word 1073741824 # = 2^30
        .text
main:
        lw $t0, var1
        li $t1, VALUE
        mult $t0, $t1 # 2*4 = 8
        mflo $t8
        mfhi $t9

        lw $t0, var2
        lw $t1, var1
        mult $t0, $t1 # -2*2 = -4
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE
        mult $t0, $t1 # 1073741824*4 = 4294967296 (overflow)
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE2
        mult $t0, $t1 # 1073741824*-4 = -4294967296 (overflow)
        mflo $t8
        mfhi $t9

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->print_inst();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();

        REQUIRE((unsigned int) VirtualMachine::getVirtualMachine()->get_reg("$t0") == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 4);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$lo") == 8);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$hi") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$24") == 8);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t9") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$25") == 0);
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();

        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 4294967294);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$lo") == 4294967292);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$hi") == 4294967295);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$24") == 4294967292);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t9") == 4294967295);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$25") == 4294967295);
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 1073741824);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 4);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$lo") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$hi") == 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$24") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t9") == 1);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$25") == 1);
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 1073741824);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 4294967292);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$lo") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$hi") == 4294967295);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$24") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t9") == 4294967295);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$25") == 4294967295);

        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 10
        std::string str_true = R"(         .data
VALUE = 4
var1:   .word 2
var2:   .word 1073741824 # = 2^30
        .text
main:
        lw $t0, var1
        li $t1, VALUE
        mult $t0, $t1 # 2*4 = 8
        mflo $t8
        mfhi $t9

        lw $t0, var2
        li $t1, VALUE
        mult $t0, $t1 # 1073741824*4 = 4294967296 (overflow)
        mflo $t8
        mfhi $t9


)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 11
        std::string str_true = R"(          .data
VALUE = 4
VALUE2 = -4
var1:   .word 2
var2:   .word -2
var3:   .word 1073741824 # = 2^30
        .text
main:
        lw $t0, var1
        li $t1, VALUE
        div $t0, $t1 # 2/4 = 0 rem 2
        mflo $t8
        mfhi $t9

        lw $t0, var2
        lw $t1, var1
        div $t0, $t1 # -2/2 = -1 rem 0
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE
        div $t0, $t1 # 1073741824/4 = 268435456 rem 0
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE2
        div $t0, $t1 # 1073741824/-4 = -268435456 rem 0
        mflo $t8
        mfhi $t9

        # divide by 0
        div $t0, $0



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//        VirtualMachine::getVirtualMachine()->print_inst();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t0") == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t1") == 4);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$lo") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$hi") == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$24") == 0);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$t9") == 2);
        REQUIRE(VirtualMachine::getVirtualMachine()->get_reg("$25") == 2);
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 12
        std::string str_true = R"(             .data
VALUE = 4
var1:   .word 2
var2:   .word 1073741825 # = 2^30+1
        .text
main:
        lw $t0, var1
        li $t1, VALUE
        div $t0, $t1 # 2/4 = 0 rem 2
        mflo $t8
        mfhi $t9

        lw $t0, var2
        li $t1, VALUE
        div $t0, $t1 # 1073741825/4 = 268435456 rem 1
        mflo $t8
        mfhi $t9

        # divide by 0
        div $t0, $0



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 13
        std::string str_true = R"(           .data
VALUE = 3
var1:   .word 12
var2:   .word 10
        .text
main:
        lw $t0, var1
        lw $t1, var2
        and $t2, $t0, $t1
        and $t2, $t0, VALUE



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 14
        std::string str_true = R"(              .data
VALUE = 3
var1:   .word 12
var2:   .word 10
        .text
main:
        lw $t0, var1
        lw $t1, var2
        nor $t2, $t0, $t1
        nor $t2, $t0, VALUE


)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 15
        std::string str_true = R"(               .data
VALUE = 3
var1:   .word 12
var2:   .word 10
        .text
main:
        lw $t0, var1
        lw $t1, var2
        or $t2, $t0, $t1
        or $t2, $t0, VALUE



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
//        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }


    {//test 16
        std::string str_true = R"(            .data
VALUE = 3
var1:   .word 12
var2:   .word 10
        .text
main:
        lw $t0, var1
        lw $t1, var2
        xor $t2, $t0, $t1
        xor $t2, $t0, VALUE



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 17
        std::string str_true = R"(            .data
VALUE = 3
var1:   .word 12
var2:   .word 10
        .text
main:
        lw $t0, var1
        lw $t1, var2
        not $t2, $t0
        not $t2, $t1
        not $t2, VALUE




)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 18
        std::string str_true = R"(             .data
        .text
main:
        nop
        j next
        nop
next:
        nop
        j main



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
//         VirtualMachine::getVirtualMachine()->print_label();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 1);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 3);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 4);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 0);
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }

    {//test 19
        std::string str_true = R"(                .data
var0:   .word 0
var1:   .word 1
var2:   .word 2
var3:   .word 3
        .text
main:
        lw $t0, var0
        lw $t1, var1
        lw $t2, var2
        lw $t3, var3

        beq $t0, $t1, check1
        beq $t0, $t0, check1
        nop
check1:
        bne $t0, $t0, check2
        bne $t0, $t1, check2
        nop
check2:
        bgt $t0, $t0, check3
        bgt $t3, $t1, check3
        nop
check3:
        bge $t0, $t1, check4
        bge $t3, $t2, check4
        nop
check4:
        blt $t3, $t1, check5
        blt $t1, $t3, check5
        nop
check5:
        ble $t3, $t1, check6
        ble $t3, $t3, check6
        nop
check6:
        nop
        j check6

)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());

////        VirtualMachine::getVirtualMachine()->print_label();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 4);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 5);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 7);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 8);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 10);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 11);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 13);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 14);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 16);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 17);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 19);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 20);
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 22);
        VirtualMachine::getVirtualMachine()->step();
        VirtualMachine::getVirtualMachine()->step();
        REQUIRE(VirtualMachine::getVirtualMachine()->get_pc() == 22);
    }

    {//test 19
        std::string str_true = R"( 	# Example program to compute the sum of squares from Jorgensen [2016]

	#---------------------------------------------------------------
	# data declarations

	.data
n:		.word 10
sumOfSquares:	.word 0

	#---------------------------------------------------------------
	# the program
	.text
main:
	lw $t0,n
	li $t1,1
	li $t2,0

sumLoop:
	mult $t1, $t1
        mflo $t3
	add $t2, $t2, $t3
	add $t1, $t1, 1
	ble $t1, $t0, sumLoop
	sw  $t2, sumOfSquares

end:
	j end



)";
        Parser parser;
        VirtualMachine::getVirtualMachine()->reset_mem();
        VirtualMachine::getVirtualMachine()->reg_init();
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text());
        //  VirtualMachine::getVirtualMachine()->print_inst();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();
//        VirtualMachine::getVirtualMachine()->step();

    }


}
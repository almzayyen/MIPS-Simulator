#include "catch.hpp"

#include "parser.hpp"

#include<string>


TEST_CASE("test other section", "[parser]") {

    {
        std::string str_true = R"(	        .data
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
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true));

        REQUIRE(parser.tokenize(str_true2));

        REQUIRE(parser.is_section_specifier());


    }

    {
        std::string str_true = R"(	        .data
        .space 8
var1:   .byte 65454354545
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
        std::string str_true2 = ".data";
        Parser parser;
        std::string s = "353434343";
        REQUIRE(parser.tokenize(str_true));

        REQUIRE(parser.tokenize(str_true2));

        REQUIRE(parser.is_section_specifier());
        REQUIRE(parser.tokenize(s));
        REQUIRE(parser.parse_integer());
        std::string file = "test09.asm";


    }
    {
        std::string str_true = R"(		# A test file of data declarations only
	.data
var1:	.word              # PARSE ERROR



)";
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text() == false);
        REQUIRE(parser.tokenize(str_true2));


    }

    {
        std::string str_true = R"(		.data
var:	.word 0
	.word 1
	.word 2

	.text

	# lexer error line 10
main:	la $t0, var
	lw $s1, ($t0
)";
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true) == false);
//        REQUIRE(parser.parse_text(str_true)== false);
        REQUIRE(parser.tokenize(str_true2));


    }


    {
        std::string str_true = R"(		# a test for basic data movement
	.data
avar:	.word 	0
bvar:	.half	1
cvar:	.byte	3

	.text
main:
	li $t0, $t1 # Parse Error
	lw $t1, avar
	lh $t2, bvar
	lb $t2, cvar
	sw $t1, avar
	sh $t2, bvar
	sb $t2, cvar

	move $t0, $0


)";
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text() == false);
        REQUIRE(parser.tokenize(str_true2));


    }


    {
        std::string str_true = R"(		# a test of basic arithmetic operations
	.data
x:	.word	1
y:	.word	1
z:	.space 	4

	.text
main:
	lw 	$t0, x
	lw 	$t1, y
	add 	$t2, $t0, $t1
	add 	$t2, $t0, 2
	addu 	$t2, $t0, $t1
	rem 	$t2, $t0	# parse error
	remu 	$t2, $t0, $t1
	remu 	$t2, $t0, 2
	abs	$t0, $t1
	neg	$t0, $t1
	negu	$t0, $t1




)";
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text() == false);
        REQUIRE(parser.tokenize(str_true2));


    }


    {
        std::string str_true = R"(		# test of basic logical instructions
	.data
	TRUE = 1
	FALSE = 0

test1:	.word TRUE
test2:	.word FALSE

	.text
main:
	lw	$t0, test1
	lw	$t1, test2

	and	$t2, $t0, $t1
	and	$t2, $t0, TRUE
	nor	$t2, $t0, $t1
	nor	$t2, $t0, TRUE, FALSE # parse error
	not	$t2, $t0
	not	$t2, $t0
	or	$t2, $t0, $t1
	or	$t2, $t0, TRUE
	xor	$t2, $t0, $t1
	xor	$t2, $t0, TRUE




)";
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true));
        // parser.print_token_list();
        REQUIRE(parser.parse_text() == false);
        //  VirtualMachine::getVirtualMachine()->print_label();
        REQUIRE(parser.tokenize(str_true2));


    }


    {
        std::string str_true = R"(		# test of the basic control instructions
	.data
x:	.word 1
y:	.word 2

	.text
main:
	lw $t0, x
	lw $t1, y

	bgt $t1, next5      # parse error
next5:

	bge $t0, $t0, next6
next6:

end:
	j end

)";
        std::string str_true2 = ".data";
        Parser parser;
        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text() == false);
        REQUIRE(parser.tokenize(str_true2));


    }


}


TEST_CASE("test text section", "[parser]") {

    { // testing parse_memref 3 cases
        std::string str_true11 = " sw $t1, 0";
        std::string str_true2 = "$31";
        std::string s = "sw $t1, 8(r2)";
        std::string str_true3 = "55 ($4)";

        std::string false1 = "label:a";
        std::string false2 = "$35";
        std::string false3 = " ($4";
        Parser parser;
        Parser parser1;
        //  REQUIRE(parser1.tokenize(str_true11));
        // REQUIRE(parser1.parse_instruction() );

        REQUIRE(parser1.tokenize(s));
        //  parser1.print_token_list();
        //  REQUIRE(parser1.parse_instruction() );

        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_memref());

        REQUIRE(parser.tokenize(str_true3));
        REQUIRE(parser.parse_memref());


        REQUIRE(parser.tokenize(false1));
        REQUIRE(parser.parse_memref() == false);
        REQUIRE(parser.tokenize(false2));
        REQUIRE(parser.parse_memref() == false);
        REQUIRE(parser.tokenize(false3));
        REQUIRE(parser.parse_memref() == false);

    }


    { // testing parse opcode
        std::string str_true = "lw";
        std::string str_true2 = "mfhi";
        std::string str_true3 = "move";
        std::string str_true4 = "add";
        std::string str_true5 = "mul";
        std::string str_true6 = "rem";
        std::string str_true7 = "neg";
        std::string str_true8 = "and";
        std::string str_true9 = "j";
        std::string str_true10 = "beq";
        std::string str_true11 = "blt";
        std::string str_fal = "b3eq";
        std::string str_fal2 = "bl1t";

        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true3));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true4));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true5));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true6));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true7));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true8));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true9));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true10));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_true11));
        REQUIRE(parser.parse_opcode());

        REQUIRE(parser.tokenize(str_fal));
        REQUIRE(parser.parse_opcode() == false);

        REQUIRE(parser.tokenize(str_fal2));
        REQUIRE(parser.parse_opcode() == false);


    }

    {
        std::string str_true = "a34343434";
        std::string str_true2 = "53dfasdfasdf";
        std::string str_false = "43dfasdfasdf";
        std::string str_false2 = "adfsdfsf:a";
        std::string str_false3 = "a :a";

        std::string str_false4 = "$331";


        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_text_constant());
        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_text_constant() == false);

        REQUIRE(parser.tokenize(str_false));
        REQUIRE(parser.parse_label() == false);

        REQUIRE(parser.tokenize(str_false2));
        REQUIRE(parser.parse_label() == false);

        REQUIRE(parser.tokenize(str_false3));
        REQUIRE(parser.parse_label() == false);
        REQUIRE(parser.tokenize(str_false4));
        REQUIRE(parser.parse_register() == false);


    }

    {
        std::string str_true = "43432";
        std::string str_true2 = "ads3dsa";
        std::string str_false = "$1";
        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_immediate());
        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_immediate());
        REQUIRE(parser.tokenize(str_false));
        REQUIRE(parser.parse_immediate() == false);

    }


    {
        std::string str_true = "lw $1, x";
        std::string str_true2 = "addu $1, $2, 4"; // addu with source = immediate
        std::string str_true3 = "and $4, $2, $6";
        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true3));
        // parser.print_token_list();
        REQUIRE(parser.parse_op_args());
    }

    {
        std::string str_true = "j x";
        std::string str_true2 = "blt $1, $2, x"; // addu with source = immediate
        std::string str_true3 = "div $4, $2, $6";
        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true3));
        // parser.print_token_list();
        REQUIRE(parser.parse_op_args());
    }


    {
        std::string str_true = "divu $5, $8";
        std::string str_true2 = "divu $1, $2, $8"; // addu with source = immediate
        std::string str_true3 = "div $1, $2, 3";
        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true3));
// parser.print_token_list();
        REQUIRE(parser.parse_op_args());
    }

    {
        std::string str_true = "not $5, $8";
        std::string str_true2 = "mult $1, $2"; // addu with source = immediate
        std::string str_true3 = "abs $1, $2";
        std::string str_true4 = "mfhi $1";
        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true2));
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true3));
// parser.print_token_list();
        REQUIRE(parser.parse_op_args());
        REQUIRE(parser.tokenize(str_true4));
        REQUIRE(parser.parse_op_args());
    }


    {
        std::string str_true = R"(	# a test for basic data movement
	.data
avar:	.word 	0
bvar:	.half	1
cvar:	.byte	3

	.text
main:
	li $t0, 45
	lw $t1, avar
	lh $t2, bvar
	lb $t2, cvar
	sw $t1, avar
	sh $t2, bvar
	sb $t2, cvar

	move $t0, $0

)";

        Parser parser;

        REQUIRE(parser.tokenize(str_true));
        //  parser.print_token_list();
//        REQUIRE(parser.parse_file("/home/abs/CLionProjects/milestone2-almzayyen/build/test00.asm") );

    }


}








//
//TEST_CASE("parse_file", "[parser]") {
//    {
//        std::string str_true = R"(		# a test of basic arithmetic operations
//	.data
//x:	.word	1
//y:	.word	1
//z:	.space 	4
//
//	.text
//main:
//	lw 	$t0, x
//	lw 	$t1, y
//	add 	$t2, $t0, $t1
//	add 	$t2, $t0, 2
//	addu 	$t2, $t0, $t1
//	addu 	$t2, $t0, 2
//	sub 	$t2, $t0, $t1
//	sub 	$t2, $t0, 2
//	subu 	$t2, $t0, $t1
//	subu 	$t2, $t0, 2
//	mul 	$t2, $t0, $t1
//	mul 	$t2, $t0, 2
//	mulo 	$t2, $t0, $t1
//	mulo 	$t2, $t0, 2
//	mulou 	$t2, $t0, $t1
//	mulou 	$t2, $t0, 2
//	mult	$t0, $t1
//	multu	$t0, $t1
//	div 	$t2, $t0, $t1
//	div 	$t2, $t0, 2
//	divu 	$t2, $t0, $t1
//	divu 	$t2, $t0, 2
//	div	$t0, $t1
//	divu	$t0, $t1
//	rem 	$t2, $t0, $t1
//	rem 	$t2, $t0, 2
//	remu 	$t2, $t0, $t1
//	remu 	$t2, $t0, 2
//	abs	$t0, $t1
//	neg	$t0, $t1
//	negu	$t0, $t1
//
// )";
//
//        std::string  s = R"(	.data
//                var:	.word 0
//	          .word 1
//	         .word 2
//
//	          .text
//
//	            # lexer error line 10
//               main:	la $t0, var
//	           lw $s1, ($t0   )";
//        Parser parser;
//
//
//       // REQUIRE(parser.tokenize_file("test01.asm") );
//      //  parser.tokenize_file("test01.asm");
//      //  parser.print_token_list();
//
//        //  REQUIRE(parser.parse_declaration() );
//        // REQUIRE(parser.parse_declaration() );
////        REQUIRE(parser.parse_file("/home/abs/CLionProjects/milestone2-almzayyen/build/test01.asm") );
//
//        REQUIRE(parser.parse_file(s) == false);
//        REQUIRE(parser.parse_string() == false);
//    }
//
//
//    {
//        std::string str_true = R"(	# a test of address modes
//	.data
//str:	.ascii	"hello"
//
//	.text
//main:
//	lb $t8, str
//	la $t0, str
//	lb $t1, ($t0)
//	lb $t2, 1($t0)
//	lb $t3, 2($t0)
//	lb $t4, 3($t0)
//	lb $t5, 4($t0)
//
//end:	nop
//	j end
//
//
//
// )";
//
//
//        Parser parser;
//
//
//        // REQUIRE(parser.tokenize_file("test01.asm") );
//        //  parser.tokenize_file("test01.asm");
//        REQUIRE(parser.tokenize(str_true) );
//          //parser.print_token_list();
//
//        //  REQUIRE(parser.parse_declaration() );
//        // REQUIRE(parser.parse_declaration() );
////        REQUIRE(parser.parse_file("/home/abs/CLionProjects/milestone2-almzayyen/build/test03.asm") );
//
//
//    }
//
//
//    {
//
//
//
//        {
//
//            std::string str_true = R"(
//# A test file of data declarations only
//	.data
//var1:	.word              # PARSE ERROR)";
//            Parser parser;
//            REQUIRE(parser.tokenize(str_true));
//           // parser.print_token_list();
//           // REQUIRE(parser.parse_file(str_true) == false);
//            REQUIRE(parser.parse_declaration() == false);
//        }
//    }
//
//
//
//    {
//        std::string str_true = R"(	# a test of all register names
//	.data
//
//	.text
//main:
//	sw $1, $0
//	sw $at, $zero
//	sw $2, $0
//	sw $v0, $zero
//	sw $3, $0
//	sw $v1, $zero
//	sw $4, $0
//	sw $a0, $zero
//	sw $5, $0
//	sw $a1, $zero
//	sw $6, $0
//	sw $a2, $zero
//	sw $7, $0
//	sw $a3, $zero
//	sw $8, $0
//	sw $t0, $zero
//	sw $9, $0
//	sw $t1, $zero
//	sw $10, $0
//	sw $t2, $zero
//	sw $11, $0
//	sw $t3, $zero
//	sw $12, $0
//	sw $t4, $zero
//	sw $13, $0
//	sw $t5, $zero
//	sw $14, $0
//	sw $t6, $zero
//	sw $15, $0
//	sw $t7, $zero
//	sw $16, $0
//	sw $s0, $zero
//	sw $17, $0
//	sw $s1, $zero
//	sw $18, $0
//	sw $s2, $zero
//	sw $19, $0
//	sw $s3, $zero
//	sw $20, $0
//	sw $s4, $zero
//	sw $21, $0
//	sw $s5, $zero
//	sw $22, $0
//	sw $s6, $zero
//	sw $23, $0
//	sw $s7, $zero
//	sw $24, $0
//	sw $t8, $zero
//	sw $25, $0
//	sw $t9, $zero
//	sw $26, $0
//	sw $k0, $zero
//	sw $27, $0
//	sw $k1, $zero
//	sw $28, $0
//	sw $gp, $zero
//	sw $29, $0
//	sw $sp, $zero
//	sw $30, $0
//	sw $fp, $zero
//	sw $31, $0
//	sw $ra, $zero
//
//end:
//	j end
//)";
//        Parser parser;
//        REQUIRE(parser.tokenize(str_true) );
//        //parser.print_token_list();
//        REQUIRE(parser.parse_text(str_true) );
////        REQUIRE(parser.parse_declaration() );
//
//
//    }
//}
//
//
//
//






TEST_CASE("data section", "[parser]") {
    {
        std::string str_true = R"(	# a test for constants
                .data
        LENGTH = 1024
        arr:	.space LENGTH

                .text
        )";

        std::string str_false = "";
        Parser parser;


        // REQUIRE(parser.tokenize_file("test01.asm") == true);
        //  parser.tokenize_file("test01.asm");
        //parser.print_token_list();

        //  REQUIRE(parser.parse_declaration() == true);
        // REQUIRE(parser.parse_declaration() == true);
//        REQUIRE(parser.parse_file("/home/abs/CLionProjects/milestone2-almzayyen/build/test05.asm"));

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }

    {

        std::string str_true = "   lb $t1, ($t0)";
        Parser parser;
        REQUIRE(parser.tokenize(str_true) == true);
        // parser.print_token_list();
        REQUIRE(parser.parse_instruction() == true);
    }


    {
        std::string str_true = "\"abc\"";
        std::string str_false = "XXX";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        REQUIRE(parser.parse_string() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }

    {
        std::string str_true = "+0123456789";
        std::string str_false = "XXX0123456789";
        Parser parser;

        std::istringstream iss(str_false);
        TokenList tl = tokenize(iss);


        REQUIRE(parser.tokenize(str_true) == true);

//      TokenList::iterator it = tl.begin();
//            for(; it !=tl.end(); ++it)
//    {
//      std::cout << *it << std::endl;
//    }

        REQUIRE(parser.parse_integer() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_integer() == false);
    }

    {
        std::string str_true = ".ascii";
        std::string str_false = "XXX.ascii";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        REQUIRE(parser.parse_string_layout() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string_layout() == false);
    }

    {
        std::string str_true = ".word";
        std::string str_false = "XXX.word";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        REQUIRE(parser.parse_int_layout() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_int_layout() == false);
    }

    { // case for layout
//        std::string str_true = "\"\"";
//        std::string str_false = "XXX.word";
//        Parser parser;
//
//        std::istringstream iss(str_true);
//        TokenList tl = tokenize(iss);
//        TokenList::iterator it = tl.begin();
//        for(;it!=tl.end();++it)
//        {
//            std::cout << *it << std::endl;
//        }
//        it = tl.begin();
//        REQUIRE(*it == Token(STRING,1 , ""));
//        REQUIRE(parser.tokenize(str_true) == true);
//        REQUIRE(parser.parse_int_layout() == true);
//
//        REQUIRE(parser.tokenize(str_false) == true);
//        REQUIRE(parser.parse_int_layout() == false);


    }

    { //declaration with label
        std::string str_true = " label:  \n";
        std::string str_false = "5dfdfd = 5";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        // parser.print_token_list();
        REQUIRE(parser.parse_declaration() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }

    {//layout
        std::string input = ".byte 1, 2, 3, 4, 5, 6, 7, 8  ";
        std::string input2 = " -12345678  ";
        std::istringstream iss(input);
        std::istringstream iss2(input2);
        TokenList tl = tokenize(iss);
        TokenList::iterator it = tl.begin();
        Parser parser;
//          for(;it!=tl.end();++it)
//    {
//      std::cout << *it << std::endl;
//    }
        it = tl.begin();
        REQUIRE(*it == Token(STRING, 1, ".byte"));
        REQUIRE(parser.tokenize(input) == true);
        //REQUIRE(parser.parse_integer());
        REQUIRE(parser.parse_int_layout());
        REQUIRE(parser.parse_layout() == true);


    }

    { //declarationj with int_layout
        std::string str_true = " label: .word 3, 5 \n";

        std::string str_false = "5dfdfd = 5";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        // parser.print_token_list();
        REQUIRE(parser.parse_declaration() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }


    { //declaration with string_layout
        std::string str_true = " label: .ascii \"hello\" \n";
        std::string str_false = "5dfdfd = 5";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        // parser.print_token_list();
        REQUIRE(parser.parse_declaration() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }

    {//layout
        std::string input = ".asciiz  \"dfdfdf\" ";
        std::string input2 = " -12345678  ";
        Parser parser;
        REQUIRE(parser.tokenize(input) == true);


        REQUIRE(parser.parse_layout() == true);

    }

    {//label
        std::string input = "var:   ";
        std::string input2 = " -12345678  ";
        Parser parser;
        REQUIRE(parser.tokenize(input) == true);

        REQUIRE(parser.parse_label() == true);

    }

    {//label
        std::string input = "a3432d:  ";
        std::string input2 = " -12345678  ";
        Parser parser;
        REQUIRE(parser.tokenize(input) == true);


        REQUIRE(parser.parse_label() == true);

    }

    {
        std::string str_true = " h34343 = 3";
        std::string str_false = "5dfdfd = 5";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);
        REQUIRE(parser.parse_constant() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }

    { //declaration with constant
        std::string str_true = " asdfsdafsd = 3 \n";
        std::string str_false = "5dfdfd = 5";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);


        //parser.print_token_list();
        REQUIRE(parser.parse_declaration() == true);

        REQUIRE(parser.tokenize(str_false) == true);
        REQUIRE(parser.parse_string() == false);
    }


}

TEST_CASE("test comment strm", "[parser]") {

    {
        std::string input = R"( # A test file of data declarations only
			.data
			var1 : .word 1024             # int var1 = 1024
			var2 : .half 12               # short var2 = 12
)";
        std::istringstream iss(input);

        TokenList tl = tokenize(iss);
//        TokenList::iterator it = tl.begin();
//    for(;it!=tl.end();++it)
//    {
//      std::cout << *it << std::endl;
//    }
        REQUIRE(tl.size() == 12);
        Parser parser;
        char a = '*';
        REQUIRE(parser.is_op(a));
        a = '/';
        REQUIRE(parser.is_op(a));
        a = '-';
        REQUIRE(parser.is_op(a));
        a = '+';
        REQUIRE(parser.is_op(a));


    }

    {

        std::string str_true = "lw";
        std::string str_false = "lwa";
        Parser parser;

        REQUIRE(parser.tokenize(str_true) == true);


        //parser.print_token_list();
        REQUIRE(parser.parse_opcode() == true);
        REQUIRE(parser.tokenize(str_false) == true);


        //parser.print_token_list();
        REQUIRE(parser.parse_opcode() == false);
    }


}


//
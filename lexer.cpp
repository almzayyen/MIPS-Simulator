#include"parser.hpp"
#include "lexer.hpp"
#include<iostream>
#include<fstream>


/*
 * function that takes a string stream, and tokenizes the string into multiple tokens.
 * token types are:   EOL, SEP, EQUAL, OPEN_PAREN, CLOSE_PAREN, STRING_DELIM, STRING, ERROR
 * and then returns the list of tokens to parsed for MIPS Assembly.
 *
 * @param: string stream &ins
 * @return: TokenList, which is std::list<Token>
 */
TokenList tokenize(std::istream &ins) {
    TokenList tokens;
    bool instring = false;
    bool inParen = false;
    size_t line_num = 1;
    char c;

    int parenNum = 0;

    std::string temp;

    while (ins.get(c)) {



        switch (c) {
            case '#': {
                if(tokens.back().type() != EOL)
                {
                    tokens.emplace_back(Token(EOL, line_num));
                }

                if ( !instring)
                {
                    do {

                        ins.get(c);
                        temp.push_back(c);
                    } while ((c != '\n') && ins.good());


                    temp.clear();
                }

                break;
            }
            case '"': {
                if (!instring) {
                    instring = true;
                    if (inParen) {
                        tokens.emplace_back(Token(ERROR, line_num, "Error: Invalid Parentheses and quotes format "));
                        return tokens;
                    }

                }
                else if (instring) {
                    instring = false;
                    if (inParen) {
                        tokens.emplace_back(Token(ERROR, line_num, "Error: Invalid Parentheses and quotes format "));
                        return tokens;
                    }


                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();
                    //

                }
                tokens.emplace_back(Token(STRING_DELIM, line_num));
                break;
            }
            case '\n': {

                if (parenNum % 2 != 0) {
                    tokens.emplace_back(Token(ERROR, line_num, "Error: unmatched quotes "));
                    return tokens;
                }

                if (instring) {
                    tokens.emplace_back(Token(ERROR, line_num, "Error: unmatched quotes "));
                    return tokens;
                }
                if (inParen && !instring) {
                    tokens.emplace_back(Token(ERROR, line_num, "Error: unmatched parentheses "));
                    return tokens;
                }
                if (!temp.empty() )
                {
                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();

                   }
                if(tokens.size() >0 && tokens.back().type() != EOL)
                tokens.emplace_back(Token(EOL, line_num));



                line_num++;
                break;
            }


            case '(': {  //open Paren case
                parenNum++;
                inParen = true;
                if (!instring && !temp.empty()) { // check for not instring


                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();


                }
                tokens.emplace_back(Token(OPEN_PAREN, line_num)); // add token to list
                break;
            }

            case ')': {  //CLOSE paren case
                parenNum++;
                if (!inParen) {
                    tokens.emplace_back(
                            Token(ERROR, line_num, "Error: unmatched parentheses "));  // checks for dangling paren
                    return tokens;
                }
                inParen = false;
                if (!instring && !temp.empty()) {
                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();

                }

                tokens.emplace_back(Token(CLOSE_PAREN, line_num));
                break;
            }
            case ',': {   // seperator case
                if (!temp.empty() && !instring) {

                    tokens.emplace_back(Token(STRING, line_num, temp));  //adds string between operators
                    temp.clear();
                }
                tokens.emplace_back(Token(SEP, line_num));
                break;
            }
            case '=': {  //equal delimeter case
                if (!temp.empty() && !instring) {

                    tokens.emplace_back(Token(STRING, line_num, temp));  // adds previous string stored
                    temp.clear();
                }
                tokens.emplace_back(Token(EQUAL, line_num)); //adds token
                break;
            }
            case '\t': {  //\t case, adds previous string then skips
                if (!temp.empty() & !instring) {
                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();
                }
                break;

            }
            case ' ': { //empty character case
                if (!temp.empty() && !instring) {
                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();
                }
                if (instring) { //if inside string, it is part of the string
                    temp.push_back(c);
                }
                break;

            }
            case '\r': { // \r case, adds previous string then skips
                if (!temp.empty() & !instring) {
                    tokens.emplace_back(Token(STRING, line_num, temp));
                    temp.clear();
                }
                break;

            }


            default: {  // adds character to previous string.
                temp.push_back(c);
            }

        }


    }
    if(!temp.empty() )
    {
        tokens.emplace_back(Token(STRING, line_num, temp));
        temp.clear();
    }

    if ( tokens.size() > 0 && tokens.back().type() != EOL) {  //final check out of the loop, to add EOL token and last STRING token if applicable.

        tokens.emplace_back(Token(EOL, line_num));

    }



    return tokens;// return list of tokens
}






#include <iostream>
#include "cpu/parser/Lexer.hpp"


int main()
{
    std::string_view source = "MOV R0, 10";

    cpu::lexer::Lexer lexer(source);

    while (true)
    {
        auto token = lexer.advance();

        if (token.kind() == cpu::lexer::Token::Kind::EndOfFile)
            break;

        std::cout << token.text() << '\n';
    }
}
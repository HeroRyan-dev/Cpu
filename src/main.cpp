#include <iostream>
#include <memory>
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include "cpu/Register.hpp"
#include "cpu/RegisterFile.hpp"
#include "cpu/Stack.hpp"
#include "cpu/Memory.hpp"
#include "cpu/Print.hpp"
#include "cpu/Token.hpp"
#include <cassert>
#include <iostream>
#include "cpu/Lexer.hpp"
using namespace cpu;

// ============================================================
// RegisterFile Tests
// ============================================================

void testNumber(
    std::string_view input,
    cpu::parser::Token::Kind expected)
{
    cpu::parser::Lexer lexer(input);
    auto token = lexer.advance();

    assert(token.kind() == expected);
}

int main() {

    
    testNumber("123", cpu::parser::Token::Kind::Number);
    testNumber("0", cpu::parser::Token::Kind::Number);
    testNumber("456", cpu::parser::Token::Kind::Number);
    testNumber("123CCC", cpu::parser::Token::Kind::Invalid);
    testNumber("8283VC", cpu::parser::Token::Kind::Invalid);
    testNumber("456", cpu::parser::Token::Kind::Number);

    return 0;
}
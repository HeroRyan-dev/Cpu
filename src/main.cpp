#include <iostream>
#include <string_view>
#include <initializer_list>
#include <cstddef>

#include "cpu/Token.hpp"
#include "cpu/Lexer.hpp"

using TokenKind = cpu::parser::Token::Kind;

namespace
{
    std::size_t passed = 0;
    std::size_t failed = 0;

    bool testExpression(
        std::string_view input,
        std::initializer_list<TokenKind> expected)
    {
        cpu::parser::Lexer lexer(input);

        std::size_t index = 0;

        for (TokenKind expectedKind : expected)
        {
            const auto token = lexer.advance();

            if (token.kind() != expectedKind)
            {
                std::cerr
                    << "[FAILED] \"" << input << "\"\n"
                    << "  Token index: " << index << '\n'
                    << "  Expected:    " << static_cast<int>(expectedKind) << '\n'
                    << "  Actual:      " << static_cast<int>(token.kind())
                    << "\n\n";

                ++failed;
                return false;
            }

            ++index;
        }

        const auto token = lexer.advance();

        if (token.kind() != TokenKind::EndOfFile)
        {
            std::cerr
                << "[FAILED] \"" << input << "\"\n"
                << "  Unexpected extra token at index: " << index << '\n'
                << "  Actual: " << static_cast<int>(token.kind())
                << "\n\n";

            ++failed;
            return false;
        }

        ++passed;
        return true;
    }
}

int main()
{
    // ========================================================
    // Basic Arithmetic
    // ========================================================

    testExpression("10 + 20", {TokenKind::Number,
                               TokenKind::Plus,
                               TokenKind::Number});

    testExpression("10+20*30", {TokenKind::Number,
                                TokenKind::Plus,
                                TokenKind::Number,
                                TokenKind::Asterisk,
                                TokenKind::Number});

    testExpression("(10 + 20) * 30", {TokenKind::LeftParen,
                                      TokenKind::Number,
                                      TokenKind::Plus,
                                      TokenKind::Number,
                                      TokenKind::RightParen,
                                      TokenKind::Asterisk,
                                      TokenKind::Number});

    testExpression("100 / 5 % 3 - 2 * 8 + 15", {TokenKind::Number,
                                                TokenKind::ForwardSlash,
                                                TokenKind::Number,
                                                TokenKind::Percent,
                                                TokenKind::Number,
                                                TokenKind::Minus,
                                                TokenKind::Number,
                                                TokenKind::Asterisk,
                                                TokenKind::Number,
                                                TokenKind::Plus,
                                                TokenKind::Number});

    // ========================================================
    // Deeply Nested Arithmetic
    // ========================================================

    testExpression("((((10 + 20) * 30) - 40) / 5)", {TokenKind::LeftParen,
                                                     TokenKind::LeftParen,
                                                     TokenKind::LeftParen,
                                                     TokenKind::LeftParen,
                                                     TokenKind::Number,
                                                     TokenKind::Plus,
                                                     TokenKind::Number,
                                                     TokenKind::RightParen,
                                                     TokenKind::Asterisk,
                                                     TokenKind::Number,
                                                     TokenKind::RightParen,
                                                     TokenKind::Minus,
                                                     TokenKind::Number,
                                                     TokenKind::RightParen,
                                                     TokenKind::ForwardSlash,
                                                     TokenKind::Number,
                                                     TokenKind::RightParen});

    testExpression("(10 * (20 + (30 * (40 - (50 / 5))))))", {TokenKind::LeftParen,
                                                             TokenKind::Number,
                                                             TokenKind::Asterisk,
                                                             TokenKind::LeftParen,
                                                             TokenKind::Number,
                                                             TokenKind::Plus,
                                                             TokenKind::LeftParen,
                                                             TokenKind::Number,
                                                             TokenKind::Asterisk,
                                                             TokenKind::LeftParen,
                                                             TokenKind::Number,
                                                             TokenKind::Minus,
                                                             TokenKind::LeftParen,
                                                             TokenKind::Number,
                                                             TokenKind::ForwardSlash,
                                                             TokenKind::Number,
                                                             TokenKind::RightParen,
                                                             TokenKind::RightParen,
                                                             TokenKind::RightParen,
                                                             TokenKind::RightParen,
                                                             TokenKind::RightParen,
                                                             TokenKind::RightParen});

    // ========================================================
    // Unary and Increment / Decrement Operators
    // ========================================================

    testExpression("~10 + !20 - 30", {TokenKind::BitwiseNot,
                                      TokenKind::Number,
                                      TokenKind::Plus,
                                      TokenKind::LogicalNot,
                                      TokenKind::Number,
                                      TokenKind::Minus,
                                      TokenKind::Number});

    testExpression("10++ + 20-- - 5", {TokenKind::Number,
                                       TokenKind::Increment,
                                       TokenKind::Plus,
                                       TokenKind::Number,
                                       TokenKind::Decrement,
                                       TokenKind::Minus,
                                       TokenKind::Number});

    testExpression("-10 + +20", {TokenKind::Minus,
                                 TokenKind::Number,
                                 TokenKind::Plus,
                                 TokenKind::Plus,
                                 TokenKind::Number});

    // ========================================================
    // Comparison and Logical Operators
    // ========================================================

    testExpression("10 >= 5 && 20 != 30", {TokenKind::Number,
                                           TokenKind::GreaterEqual,
                                           TokenKind::Number,
                                           TokenKind::LogicalAnd,
                                           TokenKind::Number,
                                           TokenKind::NotEqual,
                                           TokenKind::Number});

    testExpression("!(10 < 20 || 30 == 40)", {TokenKind::LogicalNot,
                                              TokenKind::LeftParen,
                                              TokenKind::Number,
                                              TokenKind::Less,
                                              TokenKind::Number,
                                              TokenKind::LogicalOr,
                                              TokenKind::Number,
                                              TokenKind::Equal,
                                              TokenKind::Number,
                                              TokenKind::RightParen});

    testExpression(
        "!((R0 >= 100 && R1 <= 200) || (ACC != 0 && R2 == 50))",
        {TokenKind::LogicalNot,
         TokenKind::LeftParen,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::GreaterEqual,
         TokenKind::Number,
         TokenKind::LogicalAnd,
         TokenKind::Identifier,
         TokenKind::LessEqual,
         TokenKind::Number,
         TokenKind::RightParen,
         TokenKind::LogicalOr,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::NotEqual,
         TokenKind::Number,
         TokenKind::LogicalAnd,
         TokenKind::Identifier,
         TokenKind::Equal,
         TokenKind::Number,
         TokenKind::RightParen,
         TokenKind::RightParen});

    // ========================================================
    // Bitwise Operators
    // ========================================================

    testExpression("10 & 20 | 30 ^ 40", {TokenKind::Number,
                                         TokenKind::BitwiseAnd,
                                         TokenKind::Number,
                                         TokenKind::BitwiseOr,
                                         TokenKind::Number,
                                         TokenKind::BitwiseXor,
                                         TokenKind::Number});

    testExpression("10 << 2 >> 1", {TokenKind::Number,
                                    TokenKind::LeftShift,
                                    TokenKind::Number,
                                    TokenKind::RightShift,
                                    TokenKind::Number});

    testExpression("~(0xFF & 0b10101010) | (0x10 << 2)", {TokenKind::BitwiseNot,
                                                          TokenKind::LeftParen,
                                                          TokenKind::Number,
                                                          TokenKind::BitwiseAnd,
                                                          TokenKind::Number,
                                                          TokenKind::RightParen,
                                                          TokenKind::BitwiseOr,
                                                          TokenKind::LeftParen,
                                                          TokenKind::Number,
                                                          TokenKind::LeftShift,
                                                          TokenKind::Number,
                                                          TokenKind::RightParen});

    testExpression(
        "(R0 & R1) | ((R2 ^ R3) << 4) >> (ACC & 0x0F)",
        {TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::BitwiseAnd,
         TokenKind::Identifier,
         TokenKind::RightParen,
         TokenKind::BitwiseOr,
         TokenKind::LeftParen,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::BitwiseXor,
         TokenKind::Identifier,
         TokenKind::RightParen,
         TokenKind::LeftShift,
         TokenKind::Number,
         TokenKind::RightParen,
         TokenKind::RightShift,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::BitwiseAnd,
         TokenKind::Number,
         TokenKind::RightParen});

    // ========================================================
    // Floating-Point Expressions
    // ========================================================

    testExpression("10.5 + 0.25 * 2.0", {TokenKind::Floating,
                                         TokenKind::Plus,
                                         TokenKind::Floating,
                                         TokenKind::Asterisk,
                                         TokenKind::Floating});

    testExpression("(100.125 / 5.5) - (0.5 * 20.25)", {TokenKind::LeftParen,
                                                       TokenKind::Floating,
                                                       TokenKind::ForwardSlash,
                                                       TokenKind::Floating,
                                                       TokenKind::RightParen,
                                                       TokenKind::Minus,
                                                       TokenKind::LeftParen,
                                                       TokenKind::Floating,
                                                       TokenKind::Asterisk,
                                                       TokenKind::Floating,
                                                       TokenKind::RightParen});

    // ========================================================
    // Mixed Numeric Bases
    // ========================================================

    testExpression("10 + 0xFF + 0b1010 + 0o77", {TokenKind::Number,
                                                 TokenKind::Plus,
                                                 TokenKind::Number,
                                                 TokenKind::Plus,
                                                 TokenKind::Number,
                                                 TokenKind::Plus,
                                                 TokenKind::Number});

    testExpression(
        "(0xDEADBEEF & 0b11110000) | (0o777 << 4)",
        {TokenKind::LeftParen,
         TokenKind::Number,
         TokenKind::BitwiseAnd,
         TokenKind::Number,
         TokenKind::RightParen,
         TokenKind::BitwiseOr,
         TokenKind::LeftParen,
         TokenKind::Number,
         TokenKind::LeftShift,
         TokenKind::Number,
         TokenKind::RightParen});

    // ========================================================
    // Addresses
    // ========================================================

    testExpression("@10 + @0xFF", {TokenKind::Address,
                                   TokenKind::Plus,
                                   TokenKind::Address});

    testExpression("@0b1010 * @0o77", {TokenKind::Address,
                                       TokenKind::Asterisk,
                                       TokenKind::Address});

    testExpression("@0x100 + (@0x200 * 4)", {TokenKind::Address,
                                             TokenKind::Plus,
                                             TokenKind::LeftParen,
                                             TokenKind::Address,
                                             TokenKind::Asterisk,
                                             TokenKind::Number,
                                             TokenKind::RightParen});

    testExpression(
        "(@0x100 + @0x20) * (@0b1000 - @0o10)",
        {TokenKind::LeftParen,
         TokenKind::Address,
         TokenKind::Plus,
         TokenKind::Address,
         TokenKind::RightParen,
         TokenKind::Asterisk,
         TokenKind::LeftParen,
         TokenKind::Address,
         TokenKind::Minus,
         TokenKind::Address,
         TokenKind::RightParen});

    // ========================================================
    // Identifiers and Registers
    // ========================================================

    testExpression("R0 + R1 * ACC", {TokenKind::Identifier,
                                     TokenKind::Plus,
                                     TokenKind::Identifier,
                                     TokenKind::Asterisk,
                                     TokenKind::Identifier});

    testExpression("MOV R0, R1", {TokenKind::Identifier,
                                  TokenKind::Identifier,
                                  TokenKind::Comma,
                                  TokenKind::Identifier});

    testExpression(
        "MOV R0, R1, R2, R3",
        {TokenKind::Identifier,
         TokenKind::Identifier,
         TokenKind::Comma,
         TokenKind::Identifier,
         TokenKind::Comma,
         TokenKind::Identifier,
         TokenKind::Comma,
         TokenKind::Identifier});

    testExpression(
        "R0 + (R1 * (R2 - R3)) / (ACC + SP)",
        {TokenKind::Identifier,
         TokenKind::Plus,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::Asterisk,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::Minus,
         TokenKind::Identifier,
         TokenKind::RightParen,
         TokenKind::RightParen,
         TokenKind::ForwardSlash,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::Plus,
         TokenKind::Identifier,
         TokenKind::RightParen});

    // ========================================================
    // Separators and Grouping
    // ========================================================

    testExpression("(10, 20, 30)", {TokenKind::LeftParen,
                                    TokenKind::Number,
                                    TokenKind::Comma,
                                    TokenKind::Number,
                                    TokenKind::Comma,
                                    TokenKind::Number,
                                    TokenKind::RightParen});

    testExpression("{10; 20; 30}", {TokenKind::LeftBrace,
                                    TokenKind::Number,
                                    TokenKind::Semicolon,
                                    TokenKind::Number,
                                    TokenKind::Semicolon,
                                    TokenKind::Number,
                                    TokenKind::RightBrace});

    testExpression(
        "{R0, R1; (R2 + R3), (ACC * SP)}",
        {TokenKind::LeftBrace,
         TokenKind::Identifier,
         TokenKind::Comma,
         TokenKind::Identifier,
         TokenKind::Semicolon,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::Plus,
         TokenKind::Identifier,
         TokenKind::RightParen,
         TokenKind::Comma,
         TokenKind::LeftParen,
         TokenKind::Identifier,
         TokenKind::Asterisk,
         TokenKind::Identifier,
         TokenKind::RightParen,
         TokenKind::RightBrace});

    // ========================================================
    // Special Symbols / Longest Match
    // ========================================================

    testExpression("A <-> B", {TokenKind::Identifier,
                               TokenKind::ArrowBoth,
                               TokenKind::Identifier});

    testExpression("A ... B", {TokenKind::Identifier,
                               TokenKind::Ellipsis,
                               TokenKind::Identifier});

    testExpression("A -> B", {TokenKind::Identifier,
                              TokenKind::ArrowRight,
                              TokenKind::Identifier});

    testExpression("A <- B", {TokenKind::Identifier,
                              TokenKind::ArrowLeft,
                              TokenKind::Identifier});

    testExpression("A ~> B", {TokenKind::Identifier,
                              TokenKind::ArrowTildeRight,
                              TokenKind::Identifier});

    testExpression("A <~ B", {TokenKind::Identifier,
                              TokenKind::ArrowTildeLeft,
                              TokenKind::Identifier});

    testExpression("A <~> B", {TokenKind::Identifier,
                               TokenKind::ArrowTildeBoth,
                               TokenKind::Identifier});

    testExpression("A <> B", {TokenKind::Identifier,
                              TokenKind::ArrowAngleBoth,
                              TokenKind::Identifier});

    // ========================================================
    // Operators Without Whitespace
    // ========================================================

    testExpression("R0+R1*R2-R3/R4%R5", {TokenKind::Identifier,
                                         TokenKind::Plus,
                                         TokenKind::Identifier,
                                         TokenKind::Asterisk,
                                         TokenKind::Identifier,
                                         TokenKind::Minus,
                                         TokenKind::Identifier,
                                         TokenKind::ForwardSlash,
                                         TokenKind::Identifier,
                                         TokenKind::Percent,
                                         TokenKind::Identifier});

    testExpression("10>=5&&20!=30||40<=50", {TokenKind::Number,
                                             TokenKind::GreaterEqual,
                                             TokenKind::Number,
                                             TokenKind::LogicalAnd,
                                             TokenKind::Number,
                                             TokenKind::NotEqual,
                                             TokenKind::Number,
                                             TokenKind::LogicalOr,
                                             TokenKind::Number,
                                             TokenKind::LessEqual,
                                             TokenKind::Number});

    // ========================================================
    // Malformed Numeric Literals
    // ========================================================

    testExpression("10abc + 20", {TokenKind::Invalid,
                                  TokenKind::Plus,
                                  TokenKind::Number});

    testExpression("0xFFG + 1", {TokenKind::Invalid,
                                 TokenKind::Plus,
                                 TokenKind::Number});

    testExpression("0b102 * 2", {TokenKind::Invalid,
                                 TokenKind::Asterisk,
                                 TokenKind::Number});

    testExpression("01234 + 5", {TokenKind::Invalid,
                                 TokenKind::Plus,
                                 TokenKind::Number});

    testExpression("10. + 20", {TokenKind::Invalid,
                                TokenKind::Plus,
                                TokenKind::Number});

    testExpression("0x + 10", {TokenKind::Invalid,
                               TokenKind::Plus,
                               TokenKind::Number});

    testExpression("0b + 10", {TokenKind::Invalid,
                               TokenKind::Plus,
                               TokenKind::Number});

    // ========================================================
    // Malformed Addresses
    // ========================================================

    testExpression("@10.5 + 20", {TokenKind::Invalid,
                                  TokenKind::Plus,
                                  TokenKind::Number});

    testExpression("@0xG + 1", {TokenKind::Invalid,
                                TokenKind::Plus,
                                TokenKind::Number});

    testExpression("@0b102 * 2", {TokenKind::Invalid,
                                  TokenKind::Asterisk,
                                  TokenKind::Number});

    testExpression("@0x + 10", {TokenKind::Invalid,
                                TokenKind::Plus,
                                TokenKind::Number});

    // ========================================================
    // Whitespace
    // ========================================================

    testExpression("   R0    +    R1   ", {TokenKind::Identifier,
                                           TokenKind::Plus,
                                           TokenKind::Identifier});

    testExpression("\t(10 +\n20)\r * 30", {TokenKind::LeftParen,
                                           TokenKind::Number,
                                           TokenKind::Plus,
                                           TokenKind::Number,
                                           TokenKind::RightParen,
                                           TokenKind::Asterisk,
                                           TokenKind::Number});

    // ========================================================
    // Results
    // ========================================================

    std::cout
        << "\nLexer tests completed\n"
        << "Passed: " << passed << '\n'
        << "Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
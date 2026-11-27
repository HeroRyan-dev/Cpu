#include <gtest/gtest.h>

#include <cstddef>
#include <initializer_list>
#include <string>
#include <string_view>
#include <vector>

#include "cpu/parser/Lexer.hpp"
#include "cpu/parser/Token.hpp"

// These tests exercise the lexer on its own: nothing here depends on a parser.

namespace
{
    using cpu::parser::Lexer;
    using cpu::parser::SourceLocation;
    using cpu::parser::Token;
    using cpu::parser::tokenize;
    using Kind = cpu::parser::Token::Kind;

    struct Expected
    {
        Kind kind;
        std::string_view text;
    };

    // gtest would print a Kind as an integer; names make failures readable.
    // There is no default case on purpose: adding a Kind makes -Wswitch point here.
    const char *kindName(Kind kind)
    {
        switch (kind)
        {
            case Kind::Number:           return "Number";
            case Kind::Floating:         return "Floating";
            case Kind::Identifier:       return "Identifier";
            case Kind::Hex:              return "Hex";
            case Kind::Binary:           return "Binary";
            case Kind::Char:             return "Char";
            case Kind::Address:          return "Address";
            case Kind::LeftShift:        return "LeftShift";
            case Kind::RightShift:       return "RightShift";
            case Kind::BitwiseAnd:       return "BitwiseAnd";
            case Kind::BitwiseOr:        return "BitwiseOr";
            case Kind::BitwiseXor:       return "BitwiseXor";
            case Kind::BitwiseNot:       return "BitwiseNot";
            case Kind::LogicalAnd:       return "LogicalAnd";
            case Kind::LogicalOr:        return "LogicalOr";
            case Kind::LogicalNot:       return "LogicalNot";
            case Kind::Equal:            return "Equal";
            case Kind::NotEqual:         return "NotEqual";
            case Kind::Less:             return "Less";
            case Kind::Greater:          return "Greater";
            case Kind::LessEqual:        return "LessEqual";
            case Kind::GreaterEqual:     return "GreaterEqual";
            case Kind::Increment:        return "Increment";
            case Kind::Decrement:        return "Decrement";
            case Kind::Plus:             return "Plus";
            case Kind::Minus:            return "Minus";
            case Kind::Asterisk:         return "Asterisk";
            case Kind::ForwardSlash:     return "ForwardSlash";
            case Kind::Percent:          return "Percent";
            case Kind::LeftParen:        return "LeftParen";
            case Kind::RightParen:       return "RightParen";
            case Kind::LeftBracket:      return "LeftBracket";
            case Kind::RightBracket:     return "RightBracket";
            case Kind::LeftBrace:        return "LeftBrace";
            case Kind::RightBrace:       return "RightBrace";
            case Kind::Comma:            return "Comma";
            case Kind::Semicolon:        return "Semicolon";
            case Kind::Colon:            return "Colon";
            case Kind::ArrowRight:       return "ArrowRight";
            case Kind::ArrowLeft:        return "ArrowLeft";
            case Kind::ArrowBoth:        return "ArrowBoth";
            case Kind::ArrowTildeRight:  return "ArrowTildeRight";
            case Kind::ArrowTildeLeft:   return "ArrowTildeLeft";
            case Kind::ArrowTildeBoth:   return "ArrowTildeBoth";
            case Kind::ArrowAngleBoth:   return "ArrowAngleBoth";
            case Kind::Invalid:          return "Invalid";
            case Kind::MalformedLiteral: return "MalformedLiteral";
            case Kind::Unknown:          return "Unknown";
            case Kind::Ellipsis:         return "Ellipsis";
            case Kind::EndOfFile:        return "EndOfFile";
        }

        return "<unknown kind>";
    }

    std::string describe(Kind kind, std::string_view text)
    {
        return std::string(kindName(kind)) + "(\"" + std::string(text) + "\")";
    }

    // Lexes `source` and checks that it yields exactly `expected`, followed by
    // EndOfFile. It also checks, for every token, that its text is a view INTO
    // `source` (no copies, nothing outside the buffer), and that tokenize()
    // agrees on the token count.
    void expectTokens(std::string_view source, std::initializer_list<Expected> expected)
    {
        SCOPED_TRACE("source: [" + std::string(source) + "]");

        Lexer lexer(source);
        std::size_t index = 0;

        for (const Expected &want : expected)
        {
            const Token got = lexer.advance();

            if (got.kind() != want.kind || got.text() != want.text)
            {
                ADD_FAILURE() << "token " << index << ": expected "
                              << describe(want.kind, want.text) << ", got "
                              << describe(got.kind(), got.text());
                return;
            }

            if (!got.text().empty())
            {
                EXPECT_GE(got.text().data(), source.data()) << "token " << index;
                EXPECT_LE(got.text().data() + got.text().size(),
                          source.data() + source.size())
                    << "token " << index;
            }

            ++index;
        }

        const Token end = lexer.advance();
        EXPECT_TRUE(end.is(Kind::EndOfFile))
            << "unexpected extra token: " << describe(end.kind(), end.text());

        EXPECT_EQ(tokenize(source).size(), expected.size());
    }

    // For inputs that must become exactly one Invalid token covering all of it.
    void expectSingleInvalid(std::string_view source)
    {
        expectTokens(source, {{Kind::Invalid, source}});
    }
}

// ---------------------------------------------------------------------------
// Instructions: the lexer recognizes the pieces; it does not interpret them.
// ---------------------------------------------------------------------------

TEST(LexerInstructions, Move)
{
    expectTokens("MOVE R1 -> R2",
    {
        {Kind::Identifier, "MOVE"}, {Kind::Identifier, "R1"},
        {Kind::ArrowRight, "->"},   {Kind::Identifier, "R2"},
    });
}

TEST(LexerInstructions, AddUsesAnOperatorInsideTheExpression)
{
    expectTokens("ADD R1 + R2 -> R3",
    {
        {Kind::Identifier, "ADD"}, {Kind::Identifier, "R1"}, {Kind::Plus, "+"},
        {Kind::Identifier, "R2"},  {Kind::ArrowRight, "->"}, {Kind::Identifier, "R3"},
    });
}

TEST(LexerInstructions, Sub)
{
    expectTokens("SUB R1 - R2 -> R3",
    {
        {Kind::Identifier, "SUB"}, {Kind::Identifier, "R1"}, {Kind::Minus, "-"},
        {Kind::Identifier, "R2"},  {Kind::ArrowRight, "->"}, {Kind::Identifier, "R3"},
    });
}

TEST(LexerInstructions, ShiftWithRegisterAndWithAddress)
{
    expectTokens("SHIFT R1 << 2 -> R3",
    {
        {Kind::Identifier, "SHIFT"}, {Kind::Identifier, "R1"}, {Kind::LeftShift, "<<"},
        {Kind::Number, "2"},         {Kind::ArrowRight, "->"}, {Kind::Identifier, "R3"},
    });

    expectTokens("SHIFT @0x000 << 2 -> R3",
    {
        {Kind::Identifier, "SHIFT"}, {Kind::Address, "@0x000"}, {Kind::LeftShift, "<<"},
        {Kind::Number, "2"},         {Kind::ArrowRight, "->"},  {Kind::Identifier, "R3"},
    });
}

TEST(LexerInstructions, LoadAndStoreWithAddresses)
{
    expectTokens("LOAD @0x100 -> R0",
    {
        {Kind::Identifier, "LOAD"}, {Kind::Address, "@0x100"},
        {Kind::ArrowRight, "->"},   {Kind::Identifier, "R0"},
    });

    expectTokens("STORE R0 -> @0x100",
    {
        {Kind::Identifier, "STORE"}, {Kind::Identifier, "R0"},
        {Kind::ArrowRight, "->"},    {Kind::Address, "@0x100"},
    });
}

TEST(LexerInstructions, ExpressionsMayHaveManyOperandsAndOperators)
{
    expectTokens("ADD R1 + R2 - R3 << 1 -> R4",
    {
        {Kind::Identifier, "ADD"}, {Kind::Identifier, "R1"}, {Kind::Plus, "+"},
        {Kind::Identifier, "R2"},  {Kind::Minus, "-"},       {Kind::Identifier, "R3"},
        {Kind::LeftShift, "<<"},   {Kind::Number, "1"},      {Kind::ArrowRight, "->"},
        {Kind::Identifier, "R4"},
    });
}

TEST(LexerInstructions, CommasAreStillTokensEvenThoughCryoDoesNotUseThem)
{
    expectTokens("R1, R2",
    {
        {Kind::Identifier, "R1"}, {Kind::Comma, ","}, {Kind::Identifier, "R2"},
    });
}

// ---------------------------------------------------------------------------
// Identifiers
// ---------------------------------------------------------------------------

TEST(LexerIdentifiers, ExistingIdentifierRules)
{
    expectTokens("myLabel", {{Kind::Identifier, "myLabel"}});
    expectTokens("RegisterFile", {{Kind::Identifier, "RegisterFile"}});
    expectTokens("R0_backup", {{Kind::Identifier, "R0_backup"}});
    expectTokens("x_", {{Kind::Identifier, "x_"}});
    expectTokens("A1B2", {{Kind::Identifier, "A1B2"}});
}

TEST(LexerIdentifiers, RegisterNamesAreOrdinaryIdentifiers)
{
    for (int i = 0; i <= 15; ++i)
    {
        const std::string name = "R" + std::to_string(i);
        expectTokens(name, {{Kind::Identifier, name}});
    }
}

TEST(LexerIdentifiers, MustStartWithALetter)
{
    // Characterization of the original behavior: '_' cannot start an identifier.
    expectTokens("_x", {{Kind::Invalid, "_"}, {Kind::Identifier, "x"}});
}

// ---------------------------------------------------------------------------
// Numeric literals
// ---------------------------------------------------------------------------

TEST(LexerNumbers, Decimal)
{
    for (const std::string_view source : {"0", "1", "10", "123", "255", "65535"})
    {
        expectTokens(source, {{Kind::Number, source}});
    }
}

TEST(LexerNumbers, Binary)
{
    for (const std::string_view source : {"0b0", "0b1", "0b1010", "0b11111111"})
    {
        expectTokens(source, {{Kind::Number, source}});
    }
}

TEST(LexerNumbers, Octal)
{
    for (const std::string_view source : {"0o7", "0o10", "0o77", "0o377"})
    {
        expectTokens(source, {{Kind::Number, source}});
    }
}

TEST(LexerNumbers, Hexadecimal)
{
    for (const std::string_view source : {"0x0", "0x10", "0xFF", "0xff", "0xaB", "0x100", "0xFFFF"})
    {
        expectTokens(source, {{Kind::Number, source}});
    }
}

TEST(LexerNumbers, Floating)
{
    for (const std::string_view source : {"0.5", "10.5", "100.25", "0.0"})
    {
        expectTokens(source, {{Kind::Floating, source}});
    }
}

TEST(LexerNumbers, NumbersEndAtOperatorsAndPunctuationWithoutWhitespace)
{
    expectTokens("1+2", {{Kind::Number, "1"}, {Kind::Plus, "+"}, {Kind::Number, "2"}});
    expectTokens("1<<2", {{Kind::Number, "1"}, {Kind::LeftShift, "<<"}, {Kind::Number, "2"}});
    expectTokens("0x10>>1", {{Kind::Number, "0x10"}, {Kind::RightShift, ">>"}, {Kind::Number, "1"}});
    expectTokens("3->R1", {{Kind::Number, "3"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R1"}});
    expectTokens("1.5->R1", {{Kind::Floating, "1.5"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R1"}});
    expectTokens("(0b11)", {{Kind::LeftParen, "("}, {Kind::Number, "0b11"}, {Kind::RightParen, ")"}});
    expectTokens("0;1", {{Kind::Number, "0"}, {Kind::Semicolon, ";"}, {Kind::Number, "1"}});
}

// ---------------------------------------------------------------------------
// Addresses
// ---------------------------------------------------------------------------

TEST(LexerAddresses, ValidAddresses)
{
    for (const std::string_view source :
         {"@0x000", "@0x001", "@0x100", "@255", "@0", "@7", "@0b1010", "@0o17", "@0xFF"})
    {
        expectTokens(source, {{Kind::Address, source}});
    }
}

TEST(LexerAddresses, AnAddressIsNotANumberAndANumberIsNotAnAddress)
{
    expectTokens("100", {{Kind::Number, "100"}});
    expectTokens("@100", {{Kind::Address, "@100"}});
    expectTokens("0x100", {{Kind::Number, "0x100"}});
    expectTokens("@0x100", {{Kind::Address, "@0x100"}});
}

TEST(LexerAddresses, AddressesEndAtOperatorsWithoutWhitespace)
{
    expectTokens("@0x100<<2->R3",
    {
        {Kind::Address, "@0x100"}, {Kind::LeftShift, "<<"}, {Kind::Number, "2"},
        {Kind::ArrowRight, "->"},  {Kind::Identifier, "R3"},
    });
}

TEST(LexerAddresses, MalformedAddresses)
{
    for (const std::string_view source :
         {"@", "@0x", "@0b", "@0o", "@0xGG", "@123abc", "@0b102", "@abc", "@@1", "@012", "@10.5", "@0.5", "@.5"})
    {
        expectSingleInvalid(source);
    }
}

TEST(LexerAddresses, AnAddressDoesNotSpanWhitespace)
{
    expectTokens("@ 0x10", {{Kind::Invalid, "@"}, {Kind::Number, "0x10"}});
}

TEST(LexerAddresses, MalformedAddressStopsAtTheNextOperator)
{
    expectTokens("@0x->R1", {{Kind::Invalid, "@0x"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R1"}});
    expectTokens("@-1", {{Kind::Invalid, "@"}, {Kind::Minus, "-"}, {Kind::Number, "1"}});
}

// ---------------------------------------------------------------------------
// Operators and punctuation
// ---------------------------------------------------------------------------

TEST(LexerOperators, EverySymbolInTheTableIsRecognizedOnItsOwn)
{
    for (const auto &symbol : cpu::parser::detail::symbols)
    {
        expectTokens(symbol.text, {{symbol.kind, symbol.text}});
    }
}

TEST(LexerOperators, EverySymbolBetweenIdentifiersWithoutWhitespace)
{
    for (const auto &symbol : cpu::parser::detail::symbols)
    {
        const std::string source = "R1" + std::string(symbol.text) + "R2";

        expectTokens(source,
        {
            {Kind::Identifier, "R1"}, {symbol.kind, symbol.text}, {Kind::Identifier, "R2"},
        });
    }
}

TEST(LexerOperators, EverySymbolBetweenNumbersWithoutWhitespace)
{
    for (const auto &symbol : cpu::parser::detail::symbols)
    {
        // '.' is not a number boundary, so "1...2" is covered separately below.
        if (symbol.kind == Kind::Ellipsis)
            continue;

        const std::string source = "1" + std::string(symbol.text) + "2";

        expectTokens(source,
        {
            {Kind::Number, "1"}, {symbol.kind, symbol.text}, {Kind::Number, "2"},
        });
    }
}

TEST(LexerOperators, TheSpecifiedOperatorList)
{
    expectTokens("+", {{Kind::Plus, "+"}});
    expectTokens("-", {{Kind::Minus, "-"}});
    expectTokens("<<", {{Kind::LeftShift, "<<"}});
    expectTokens(">>", {{Kind::RightShift, ">>"}});
    expectTokens("->", {{Kind::ArrowRight, "->"}});
    expectTokens("<-", {{Kind::ArrowLeft, "<-"}});
    expectTokens("<->", {{Kind::ArrowBoth, "<->"}});
    expectTokens("<=", {{Kind::LessEqual, "<="}});
    expectTokens(">=", {{Kind::GreaterEqual, ">="}});
    expectTokens("==", {{Kind::Equal, "=="}});
    expectTokens("!=", {{Kind::NotEqual, "!="}});
    expectTokens("&&", {{Kind::LogicalAnd, "&&"}});
    expectTokens("||", {{Kind::LogicalOr, "||"}});
}

TEST(LexerOperators, OperatorsThatSharePrefixesWithLess)
{
    expectTokens("<", {{Kind::Less, "<"}});
    expectTokens("<<", {{Kind::LeftShift, "<<"}});
    expectTokens("<=", {{Kind::LessEqual, "<="}});
    expectTokens("<-", {{Kind::ArrowLeft, "<-"}});
    expectTokens("<->", {{Kind::ArrowBoth, "<->"}});
    expectTokens("<~", {{Kind::ArrowTildeLeft, "<~"}});
    expectTokens("<~>", {{Kind::ArrowTildeBoth, "<~>"}});
    expectTokens("<>", {{Kind::ArrowAngleBoth, "<>"}});
}

TEST(LexerOperators, LongestMatchWinsAndThenTheRestIsLexedAgain)
{
    expectTokens("<<<", {{Kind::LeftShift, "<<"}, {Kind::Less, "<"}});
    expectTokens("<<>", {{Kind::LeftShift, "<<"}, {Kind::Greater, ">"}});
    expectTokens("<--", {{Kind::ArrowLeft, "<-"}, {Kind::Minus, "-"}});
    expectTokens("<-->", {{Kind::ArrowLeft, "<-"}, {Kind::ArrowRight, "->"}});
    expectTokens("<->>", {{Kind::ArrowBoth, "<->"}, {Kind::Greater, ">"}});
    expectTokens("<~~", {{Kind::ArrowTildeLeft, "<~"}, {Kind::BitwiseNot, "~"}});
    expectTokens("<~>>", {{Kind::ArrowTildeBoth, "<~>"}, {Kind::Greater, ">"}});
    expectTokens("<=>", {{Kind::LessEqual, "<="}, {Kind::Greater, ">"}});
    expectTokens("<>>", {{Kind::ArrowAngleBoth, "<>"}, {Kind::Greater, ">"}});
    expectTokens("-->", {{Kind::Decrement, "--"}, {Kind::Greater, ">"}});
    expectTokens("->>", {{Kind::ArrowRight, "->"}, {Kind::Greater, ">"}});
    expectTokens("+++", {{Kind::Increment, "++"}, {Kind::Plus, "+"}});
    expectTokens("&&&", {{Kind::LogicalAnd, "&&"}, {Kind::BitwiseAnd, "&"}});
    expectTokens("|||", {{Kind::LogicalOr, "||"}, {Kind::BitwiseOr, "|"}});
    expectTokens(">>>", {{Kind::RightShift, ">>"}, {Kind::Greater, ">"}});
    expectTokens("~~>", {{Kind::BitwiseNot, "~"}, {Kind::ArrowTildeRight, "~>"}});
    expectTokens("!!=", {{Kind::LogicalNot, "!"}, {Kind::NotEqual, "!="}});
}

TEST(LexerOperators, MultipleOperatorsInARow)
{
    expectTokens("+-*/%",
    {
        {Kind::Plus, "+"}, {Kind::Minus, "-"}, {Kind::Asterisk, "*"},
        {Kind::ForwardSlash, "/"}, {Kind::Percent, "%"},
    });

    expectTokens("&|^~!",
    {
        {Kind::BitwiseAnd, "&"}, {Kind::BitwiseOr, "|"}, {Kind::BitwiseXor, "^"},
        {Kind::BitwiseNot, "~"}, {Kind::LogicalNot, "!"},
    });

    expectTokens("()[]{}",
    {
        {Kind::LeftParen, "("}, {Kind::RightParen, ")"}, {Kind::LeftBracket, "["},
        {Kind::RightBracket, "]"}, {Kind::LeftBrace, "{"}, {Kind::RightBrace, "}"},
    });

    expectTokens("<<>>", {{Kind::LeftShift, "<<"}, {Kind::RightShift, ">>"}});
}

TEST(LexerOperators, IncompleteOperatorsAreInvalidNotSkipped)
{
    // '=' only exists as "==", and '.' only as "...".
    expectSingleInvalid("=");
    expectSingleInvalid(".");
    expectTokens("..", {{Kind::Invalid, "."}, {Kind::Invalid, "."}});
    expectTokens("===", {{Kind::Equal, "=="}, {Kind::Invalid, "="}});
    expectTokens("!==", {{Kind::NotEqual, "!="}, {Kind::Invalid, "="}});
    expectTokens("<<=", {{Kind::LeftShift, "<<"}, {Kind::Invalid, "="}});
    expectTokens("R1 = R2", {{Kind::Identifier, "R1"}, {Kind::Invalid, "="}, {Kind::Identifier, "R2"}});
}

TEST(LexerOperators, EllipsisNeedsExactlyThreeDots)
{
    expectTokens("...", {{Kind::Ellipsis, "..."}});
    expectTokens("....", {{Kind::Ellipsis, "..."}, {Kind::Invalid, "."}});
    expectTokens("R1...R2", {{Kind::Identifier, "R1"}, {Kind::Ellipsis, "..."}, {Kind::Identifier, "R2"}});
}

TEST(LexerOperators, DotIsNotANumberBoundary)
{
    // Characterization of the original rules: only "digits.digits" may contain a '.'.
    expectSingleInvalid("1...2");
    expectSingleInvalid("1.5.3");
    expectSingleInvalid("1..2");
    expectTokens(".5", {{Kind::Invalid, "."}, {Kind::Number, "5"}});
}

// ---------------------------------------------------------------------------
// Token boundaries without whitespace
// ---------------------------------------------------------------------------

TEST(LexerBoundaries, OperatorsNeedNoSurroundingWhitespace)
{
    expectTokens("R1->R2",
    {
        {Kind::Identifier, "R1"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R2"},
    });

    expectTokens("R1+R2->R3",
    {
        {Kind::Identifier, "R1"}, {Kind::Plus, "+"},       {Kind::Identifier, "R2"},
        {Kind::ArrowRight, "->"}, {Kind::Identifier, "R3"},
    });

    expectTokens("R1<<2+R2->R3",
    {
        {Kind::Identifier, "R1"}, {Kind::LeftShift, "<<"}, {Kind::Number, "2"},
        {Kind::Plus, "+"},        {Kind::Identifier, "R2"},
        {Kind::ArrowRight, "->"}, {Kind::Identifier, "R3"},
    });
}

TEST(LexerBoundaries, PunctuationBetweenOperands)
{
    expectTokens("[R1]:{R2};",
    {
        {Kind::LeftBracket, "["}, {Kind::Identifier, "R1"}, {Kind::RightBracket, "]"},
        {Kind::Colon, ":"},       {Kind::LeftBrace, "{"},    {Kind::Identifier, "R2"},
        {Kind::RightBrace, "}"},  {Kind::Semicolon, ";"},
    });

    expectTokens("label:", {{Kind::Identifier, "label"}, {Kind::Colon, ":"}});
}

TEST(LexerBoundaries, ATokenAtTheVeryEndOfTheInputIsComplete)
{
    expectTokens("R1", {{Kind::Identifier, "R1"}});
    expectTokens("123", {{Kind::Number, "123"}});
    expectTokens("0xFF", {{Kind::Number, "0xFF"}});
    expectTokens("10.5", {{Kind::Floating, "10.5"}});
    expectTokens("@0x100", {{Kind::Address, "@0x100"}});
    expectTokens("->", {{Kind::ArrowRight, "->"}});
    expectTokens("<~>", {{Kind::ArrowTildeBoth, "<~>"}});
}

// ---------------------------------------------------------------------------
// Whitespace and multiple lines
// ---------------------------------------------------------------------------

TEST(LexerWhitespace, AmountAndKindOfWhitespaceDoesNotChangeTheTokens)
{
    const auto moveTokens =
    {
        Expected{Kind::Identifier, "MOVE"}, Expected{Kind::Identifier, "R1"},
        Expected{Kind::ArrowRight, "->"},   Expected{Kind::Identifier, "R2"},
    };

    expectTokens("MOVE R1 -> R2", moveTokens);
    expectTokens("MOVE    R1    ->    R2", moveTokens);
    expectTokens("MOVE R1->R2", moveTokens);
    expectTokens("MOVE\tR1\t->\tR2", moveTokens);
    expectTokens("  MOVE R1 -> R2  \n", moveTokens);
    expectTokens("MOVE\nR1\r\n->\n\n R2", moveTokens);
}

TEST(LexerWhitespace, EmptyAndWhitespaceOnlyInput)
{
    expectTokens("", {});
    expectTokens(" ", {});
    expectTokens(" \t\r\n \n\n\t ", {});
}

TEST(LexerWhitespace, MultipleLines)
{
    expectTokens("MOVE R1 -> R2\nADD R1 + R2 -> R3\nLOAD @0x100 -> R0\n",
    {
        {Kind::Identifier, "MOVE"}, {Kind::Identifier, "R1"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R2"},
        {Kind::Identifier, "ADD"},  {Kind::Identifier, "R1"}, {Kind::Plus, "+"},        {Kind::Identifier, "R2"},
        {Kind::ArrowRight, "->"},   {Kind::Identifier, "R3"},
        {Kind::Identifier, "LOAD"}, {Kind::Address, "@0x100"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R0"},
    });

    expectTokens("SUB R1 - R2 -> R3\r\nSTORE R0 -> @0x100\r\n",
    {
        {Kind::Identifier, "SUB"},   {Kind::Identifier, "R1"}, {Kind::Minus, "-"},
        {Kind::Identifier, "R2"},    {Kind::ArrowRight, "->"}, {Kind::Identifier, "R3"},
        {Kind::Identifier, "STORE"}, {Kind::Identifier, "R0"}, {Kind::ArrowRight, "->"},
        {Kind::Address, "@0x100"},
    });
}

// ---------------------------------------------------------------------------
// Invalid input
// ---------------------------------------------------------------------------

TEST(LexerInvalid, MalformedNumbersBecomeOneInvalidToken)
{
    for (const std::string_view source :
         {"0x", "0b", "0o", "0xGG", "0b102", "0o8", "0b2", "12abc", "1.", "0.", "10.", "012",
          "00", "0X10", "0B1", "0O7", "1a", "0xFG", "0b1x"})
    {
        expectSingleInvalid(source);
    }
}

TEST(LexerInvalid, AMalformedLiteralIsNeverSplitIntoAValidPrefixAndLeftovers)
{
    expectTokens("0b102", {{Kind::Invalid, "0b102"}});
    expectTokens("0xGG", {{Kind::Invalid, "0xGG"}});
    expectTokens("12abc", {{Kind::Invalid, "12abc"}});
    expectTokens("0x1G", {{Kind::Invalid, "0x1G"}});
}

TEST(LexerInvalid, AMalformedLiteralEndsAtTheNextBoundaryOnly)
{
    expectTokens("12abc+3", {{Kind::Invalid, "12abc"}, {Kind::Plus, "+"}, {Kind::Number, "3"}});
    expectTokens("0xGG->R1", {{Kind::Invalid, "0xGG"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R1"}});
    expectTokens("0x->R1", {{Kind::Invalid, "0x"}, {Kind::ArrowRight, "->"}, {Kind::Identifier, "R1"}});
    expectTokens("0b102 << 1", {{Kind::Invalid, "0b102"}, {Kind::LeftShift, "<<"}, {Kind::Number, "1"}});
    expectTokens("10 12abc 11", {{Kind::Number, "10"}, {Kind::Invalid, "12abc"}, {Kind::Number, "11"}});
}

TEST(LexerInvalid, UnexpectedCharactersAreReportedOneAtATime)
{
    for (const char unexpected : {'$', '#', '?', '`', '"', '\'', '\\', '_'})
    {
        const std::string source(1, unexpected);
        expectSingleInvalid(source);
    }

    expectTokens("R1 $ R2", {{Kind::Identifier, "R1"}, {Kind::Invalid, "$"}, {Kind::Identifier, "R2"}});
    expectTokens("R1$R2", {{Kind::Identifier, "R1"}, {Kind::Invalid, "$"}, {Kind::Identifier, "R2"}});
    expectTokens("$$", {{Kind::Invalid, "$"}, {Kind::Invalid, "$"}});
    expectTokens("10@", {{Kind::Invalid, "10@"}});
}

TEST(LexerInvalid, BytesAboveAsciiAreInvalidAndNeverReadOutOfRange)
{
    // U+00E9 in UTF-8 is two bytes, each reported on its own.
    expectTokens("\xC3\xA9", {{Kind::Invalid, "\xC3"}, {Kind::Invalid, "\xA9"}});
    expectTokens("R1\xFF", {{Kind::Identifier, "R1"}, {Kind::Invalid, "\xFF"}});
    expectTokens("1\xFF", {{Kind::Invalid, "1\xFF"}});
}

TEST(LexerInvalid, AnEmbeddedNulIsAnInvalidTokenNotTheEndOfTheInput)
{
    expectTokens(std::string_view("R1\0R2", 5),
    {
        {Kind::Identifier, "R1"}, {Kind::Invalid, std::string_view("\0", 1)}, {Kind::Identifier, "R2"},
    });

    expectTokens(std::string_view("12\0", 3), {{Kind::Number, "12"}, {Kind::Invalid, std::string_view("\0", 1)}});
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

TEST(LexerApi, EndOfFileIsEmptyAndRepeatsForever)
{
    Lexer lexer("R1");

    EXPECT_TRUE(lexer.advance().is(Kind::Identifier));

    for (int i = 0; i < 3; ++i)
    {
        const Token end = lexer.advance();

        EXPECT_TRUE(end.is(Kind::EndOfFile));
        EXPECT_TRUE(end.text().empty());
    }
}

TEST(LexerApi, EndOfFileOnEmptySource)
{
    Lexer lexer("");

    EXPECT_TRUE(lexer.advance().is(Kind::EndOfFile));
    EXPECT_TRUE(lexer.advance().is(Kind::EndOfFile));
}

TEST(LexerApi, ResetRestartsFromTheBeginning)
{
    Lexer lexer("ADD R1");

    EXPECT_EQ(lexer.advance().text(), "ADD");
    EXPECT_EQ(lexer.advance().text(), "R1");
    EXPECT_TRUE(lexer.advance().is(Kind::EndOfFile));

    lexer.reset();

    EXPECT_EQ(lexer.advance().text(), "ADD");
    EXPECT_EQ(lexer.advance().text(), "R1");
    EXPECT_TRUE(lexer.advance().is(Kind::EndOfFile));
}

TEST(LexerApi, TokenizeDoesNotIncludeEndOfFile)
{
    const std::vector<Token> tokens = tokenize("MOVE R1 -> R2");

    ASSERT_EQ(tokens.size(), 4u);

    for (const Token &token : tokens)
    {
        EXPECT_FALSE(token.is(Kind::EndOfFile));
    }

    EXPECT_TRUE(tokenize("").empty());
    EXPECT_TRUE(tokenize("  \n ").empty());
}

TEST(LexerApi, TokenTextIsAViewIntoTheSourceNotACopy)
{
    const std::string source = "SHIFT @0x000 << 2 -> R3";
    const std::vector<Token> tokens = tokenize(source);

    ASSERT_EQ(tokens.size(), 6u);

    const std::size_t expectedOffsets[] = {0, 6, 13, 16, 18, 21};

    for (std::size_t i = 0; i < tokens.size(); ++i)
    {
        EXPECT_EQ(tokens[i].text().data(), source.data() + expectedOffsets[i]) << "token " << i;
    }
}

TEST(LexerApi, LocateReportsOffsetLineAndColumn)
{
    const std::string_view source = "MOVE R1 -> R2\nADD R1 + 0xGG -> R3";
    Lexer lexer(source);

    Token token = lexer.advance();
    while (token.isNot(Kind::Invalid) && token.isNot(Kind::EndOfFile))
    {
        token = lexer.advance();
    }

    ASSERT_TRUE(token.is(Kind::Invalid));
    EXPECT_EQ(token.text(), "0xGG");

    const SourceLocation location = lexer.locate(token);
    EXPECT_EQ(location.offset, 23u);
    EXPECT_EQ(location.line, 2u);
    EXPECT_EQ(location.column, 10u);
}

TEST(LexerApi, LocateWorksForTheFirstTokenAndForEndOfFile)
{
    const std::string_view source = "A\nBC";
    Lexer lexer(source);

    const SourceLocation first = lexer.locate(lexer.advance());
    EXPECT_EQ(first.offset, 0u);
    EXPECT_EQ(first.line, 1u);
    EXPECT_EQ(first.column, 1u);

    lexer.advance(); // BC
    const Token end = lexer.advance();
    ASSERT_TRUE(end.is(Kind::EndOfFile));

    const SourceLocation last = lexer.locate(end);
    EXPECT_EQ(last.offset, 4u);
    EXPECT_EQ(last.line, 2u);
    EXPECT_EQ(last.column, 3u);
}

// Lexer.hpp - splits Cryo source text into tokens.
//
// HOW IT WORKS
//
// The lexer holds the source text and a position in it. Each call to advance()
// skips whitespace, then uses the next character to choose what to read:
//
//   First character   What is read                         Result
//   ---------------   -----------------------------------  ------------------
//   (none left)       nothing                              EndOfFile
//   letter            letters, digits and '_'              Identifier
//   digit or '@'      everything up to a "number end"      see classifyNumber
//   anything else     the longest symbol that matches      that symbol's kind
//
// EXAMPLE: "R1 <- [@0x1F];" becomes
//
//   Identifier "R1", ArrowLeft "<-", LeftBracket "[", Address "@0x1F",
//   RightBracket "]", Semicolon ";"
//
// NUMBER END: whitespace, '\0', or a character that can start a symbol. So
// "5;" is two tokens, while "12abc" and "1.5.3" stay whole and are Invalid.
//
// SYMBOLS are matched with a trie: a tree with one branch per character, built
// from detail::symbols at compile time. Part of it:
//
//   start --'<'--> "<" --'-'--> "<-" --'>'--> "<->"
//                   |
//                   +--'<'--> "<<"
//
// The lexer follows branches as far as the input allows and returns the last
// complete symbol it passed (the "longest match"). "<->" is one token, not
// "<-" and ">". For the same reason "a<-1" reads as "a", "<-", "1".
//
// ERRORS: bad input becomes a Token::Kind::Invalid token. Nothing is silently
// dropped, and the lexer carries on with the next token. locate() gives the
// line and column for an error message.
//
// LIFETIME: token text points into the source (no copies), so the source must
// stay alive while the lexer or its tokens are in use.
//
// ADDING AN OPERATOR: add a Token::Kind and one line in detail::symbols.

#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#include "cpu/parser/Token.hpp"

namespace cpu::lexer
{
    namespace detail
    {
        // ---- Character tests ----

        constexpr bool isWhitespace(char c) noexcept { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
        constexpr bool isLetter(char c) noexcept { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
        constexpr bool isDigit(char c) noexcept { return c >= '0' && c <= '9'; }
        constexpr bool isIdentifierChar(char c) noexcept { return isLetter(c) || isDigit(c) || c == '_'; }
        constexpr bool isBinaryDigit(char c) noexcept { return c == '0' || c == '1'; }
        constexpr bool isOctalDigit(char c) noexcept { return c >= '0' && c <= '7'; }
        constexpr bool isHexDigit(char c) noexcept { return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

        // ---- Symbols ----

        struct Symbol
        {
            std::string_view text;
            Token::Kind kind;
        };

        // Every operator and punctuation mark, in any order.
        inline constexpr Symbol symbols[] = {
            // Bitwise
            {"<<", Token::Kind::LeftShift},
            {">>", Token::Kind::RightShift},
            {"&", Token::Kind::BitwiseAnd},
            {"|", Token::Kind::BitwiseOr},
            {"^", Token::Kind::BitwiseXor},
            {"~", Token::Kind::BitwiseNot},
            // Comparison
            {"==", Token::Kind::Equal},
            {"!=", Token::Kind::NotEqual},
            {"<", Token::Kind::Less},
            {">", Token::Kind::Greater},
            {"<=", Token::Kind::LessEqual},
            {">=", Token::Kind::GreaterEqual},
            // Logical
            {"&&", Token::Kind::LogicalAnd},
            {"||", Token::Kind::LogicalOr},
            {"!", Token::Kind::LogicalNot},
            // Arithmetic
            {"+", Token::Kind::Plus},
            {"-", Token::Kind::Minus},
            {"*", Token::Kind::Asterisk},
            {"/", Token::Kind::ForwardSlash},
            {"%", Token::Kind::Percent},
            // Grouping and separators
            {"(", Token::Kind::LeftParen},
            {")", Token::Kind::RightParen},
            {"[", Token::Kind::LeftBracket},
            {"]", Token::Kind::RightBracket},
            {";", Token::Kind::Semicolon},
            // Arrows
            {"->", Token::Kind::ArrowRight},
            {"<-", Token::Kind::ArrowLeft},
            {"<->", Token::Kind::ArrowBoth},
            {"~>", Token::Kind::ArrowTildeRight},
            {"<~", Token::Kind::ArrowTildeLeft},
            {"<~>", Token::Kind::ArrowTildeBoth},
        };

        // ---- The trie ----

        // Each node is the text read so far; node 0 is the start (nothing read).
        struct Trie
        {
            // If the symbols ever need more nodes, compilation fails: raise
            // this (256 at most, because node numbers are stored in a byte).
            static constexpr std::size_t size = 64;

            // next[node][byte] is the node reached, or 0 for "no branch".
            std::array<std::array<std::uint8_t, 256>, size> next{};

            // kind[node] is the symbol ending here, or Invalid if none does.
            std::array<Token::Kind, size> kind{};
        };

        constexpr Trie buildTrie() noexcept
        {
            Trie trie{};

            for (auto &kind : trie.kind)
                kind = Token::Kind::Invalid;

            std::size_t used = 1;

            for (const Symbol &symbol : symbols)
            {
                std::size_t node = 0;

                for (const char c : symbol.text)
                {
                    auto &child = trie.next[node][static_cast<unsigned char>(c)];

                    if (child == 0)
                        child = static_cast<std::uint8_t>(used++);

                    node = child;
                }

                trie.kind[node] = symbol.kind;
            }

            return trie;
        }

        inline constexpr Trie trie = buildTrie();

        // The node reached from `node` by reading `c` (0 if there is none).
        constexpr std::uint8_t step(std::size_t node, char c) noexcept
        {
            // unsigned, so that non-ASCII bytes are not negative indexes
            return trie.next[node][static_cast<unsigned char>(c)];
        }

        // ---- Numbers ----

        constexpr bool isNumberChar(char c) noexcept
        {
            return !isWhitespace(c) && c != '\0' && step(0, c) == 0;
        }

        // True if `text` is not empty and every character passes `test`.
        template <typename Test>
        bool allOf(std::string_view text, Test test) noexcept
        {
            for (const char c : text)
                if (!test(c))
                    return false;

            return !text.empty();
        }

        // Decides what a whole numeric word is. `text` must not be empty.
        //
        //   Number    0   42   0b101   0o17   0xFF
        //   Floating  0.5   10.25
        //   Address   '@' followed by any Number form
        //   Invalid   everything else: 012   1.   0x   @0.5   12abc
        inline Token::Kind classifyNumber(std::string_view text) noexcept
        {
            const bool isAddress = text.front() == '@';
            if (isAddress)
                text.remove_prefix(1);

            const auto integer = isAddress ? Token::Kind::Address : Token::Kind::Number;

            // 0b, 0o and 0x: the rest must be digits of that base.
            const char prefix = text.size() > 1 && text[0] == '0' ? text[1] : '\0';

            if (prefix == 'b')
                return allOf(text.substr(2), isBinaryDigit) ? integer : Token::Kind::Invalid;
            if (prefix == 'o')
                return allOf(text.substr(2), isOctalDigit) ? integer : Token::Kind::Invalid;
            if (prefix == 'x')
                return allOf(text.substr(2), isHexDigit) ? integer : Token::Kind::Invalid;

            // Decimal: digits with no leading zero, then maybe '.' and digits.
            const std::size_t dot = text.find('.');
            const std::string_view whole = text.substr(0, dot);

            if (!allOf(whole, isDigit) || (whole.size() > 1 && whole[0] == '0'))
                return Token::Kind::Invalid;

            if (dot == std::string_view::npos)
                return integer;

            // Addresses cannot have a fraction.
            return !isAddress && allOf(text.substr(dot + 1), isDigit) ? Token::Kind::Floating
                                                                      : Token::Kind::Invalid;
        }
    } // namespace detail

    // Where a token starts. Line and column are 1-based; the column counts bytes.
    struct SourceLocation
    {
        std::size_t offset{};
        std::size_t line{1};
        std::size_t column{1};
    };

    class Lexer
    {
    public:
        explicit Lexer(std::string_view source) noexcept : source_(source) {}

        // Starts again from the beginning.
        void reset() noexcept { position_ = 0; }

        // Returns the next token, or EndOfFile once the input is used up.
        Token advance() noexcept
        {
            skipWhile(detail::isWhitespace);

            if (position_ == source_.size())
                return Token{Token::Kind::EndOfFile};

            const std::size_t start = position_;
            const char first = source_[start];

            if (detail::isLetter(first))
            {
                skipWhile(detail::isIdentifierChar);
                return Token{Token::Kind::Identifier, textFrom(start)};
            }

            if (detail::isDigit(first) || first == '@')
            {
                ++position_; // '@' is not a "number character", so step over it
                skipWhile(detail::isNumberChar);
                return Token{detail::classifyNumber(textFrom(start)), textFrom(start)};
            }

            return readSymbol();
        }

        // The position of a token returned by this lexer, for error messages.
        // EndOfFile maps to the end of the source.
        [[nodiscard]]
        SourceLocation locate(const Token &token) const noexcept
        {
            const char *data = token.text().data();
            const std::size_t offset =
                data ? static_cast<std::size_t>(data - source_.data()) : source_.size();

            const std::string_view before = source_.substr(0, offset);
            const std::size_t newline = before.rfind('\n'); // last one before the token
            const auto newlines = std::count(before.begin(), before.end(), '\n');

            return SourceLocation{
                offset,
                1 + static_cast<std::size_t>(newlines),
                newline == std::string_view::npos ? offset + 1 : offset - newline};
        }

    private:
        std::string_view source_;
        std::size_t position_{};

        // Moves forward while the current character passes `test`.
        template <typename Test>
        void skipWhile(Test test) noexcept
        {
            while (position_ < source_.size() && test(source_[position_]))
                ++position_;
        }

        std::string_view textFrom(std::size_t start) const noexcept
        {
            return source_.substr(start, position_ - start);
        }

        // Walks the trie to find the longest symbol. A character that starts no
        // symbol becomes a one-character Invalid token.
        Token readSymbol() noexcept
        {
            const std::size_t start = position_;
            std::size_t end = start + 1; // end of the best match so far
            Token::Kind kind = Token::Kind::Invalid;
            std::size_t node = 0;

            for (std::size_t i = start; i < source_.size(); ++i)
            {
                node = detail::step(node, source_[i]);

                if (node == 0)
                    break;

                if (detail::trie.kind[node] != Token::Kind::Invalid)
                {
                    end = i + 1;
                    kind = detail::trie.kind[node];
                }
            }

            position_ = end;
            return Token{kind, textFrom(start)};
        }
    };

    // All tokens in `source`, without the final EndOfFile.
    inline std::vector<Token> tokenize(std::string_view source)
    {
        Lexer lexer(source);
        std::vector<Token> tokens;
        tokens.reserve(source.size() / 2);

        for (Token token = lexer.advance(); token.kind() != Token::Kind::EndOfFile; token = lexer.advance())
            tokens.push_back(token);

        return tokens;
    }
} // namespace cpu::lexer
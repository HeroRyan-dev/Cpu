#pragma once

#include <string_view>
#include "cpu/parser/Token.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <cstddef>
#include <expected>

#include <unordered_map>
#include <memory>
#include <optional>

namespace cpu::parser
{

    namespace detail
    {

        struct TrieNode
        {
            std::unordered_map<char, std::unique_ptr<TrieNode>> children;
            std::optional<Token::Kind> kind;
        };

        class SymbolTrie
        {
        private:
            TrieNode root_;

        public:
            void insert(std::string_view symbol, Token::Kind kind)
            {
                TrieNode *node = &root_;

                for (char c : symbol)
                {
                    auto &child = node->children[c];

                    if (!child)
                        child = std::make_unique<TrieNode>();

                    node = child.get();
                }

                node->kind = kind;
            }

            const TrieNode &root() const noexcept
            {
                return root_;
            }
        };
        
        // Character classification

        constexpr bool isWhitespace(char c) noexcept
        {
            return (c == ' ')  ||
                   (c == '\t') ||
                   (c == '\n') ||
                   (c == '\r');
        }
        
        constexpr bool isAlphabetical(char c) noexcept
        {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z');
        }

        constexpr bool isAlphanumerical(char c) noexcept
        {
            return (c >= '0' && c <= '9') ||
                   (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z');
        }
        
        constexpr bool isDecimal(char c) noexcept
        {
            return c >= '0' && c <= '9';
        }

        constexpr bool isBinary(char c) noexcept
        {
            return c == '0' || c == '1';
        }
        
        constexpr bool isOctal(char c) noexcept
        {
            return c >= '0' && c <= '7';
        }

        constexpr bool isHexadecimal(char c) noexcept
        {
            return (c >= '0' && c <= '9') ||
                   (c >= 'a' && c <= 'f') ||
                   (c >= 'A' && c <= 'F');
        }

        // Identifier validation

        constexpr bool isIdentifierChar(char c) noexcept
        {
            return isAlphanumerical(c) || c == '_';
        }

        constexpr bool isIdentifierTerminatorChar(char c) noexcept
        {
            return isWhitespace(c) ||
                   c == '\0' ||
                   c == '+' ||
                   c == '-' ||
                   c == '*' ||
                   c == '/' ||
                   c == '%' ||
                   c == '^';
        }

        // Numeric validation

        constexpr bool isNumberTerminatorChar(char c) noexcept
        {
            return isWhitespace(c) ||
                   c == '\0' ||
                   c == '+' ||
                   c == '-' ||
                   c == '*' ||
                   c == '/' ||
                   c == '%' ||
                   c == '^' ||
                   c == '&' ||
                   c == '|' ||
                   c == '~' ||
                   c == '!' ||
                   c == '=' ||
                   c == '<' ||
                   c == '>' ||
                   c == '(' ||
                   c == ')' ||
                   c == '[' ||
                   c == ']' ||
                   c == '{' ||
                   c == '}' ||
                   c == ',' ||
                   c == ';' ||
                   c == ':';
        }

        struct Symbol
        {
            std::string_view text;
            Token::Kind kind;
        };

        constexpr Symbol symbols[] =
            {
                // BITWISE
                {"<<", Token::Kind::LeftShift},
                {">>", Token::Kind::RightShift},
                {"&",  Token::Kind::BitwiseAnd},
                {"|",  Token::Kind::BitwiseOr},
                {"^",  Token::Kind::BitwiseXor},
                {"~",  Token::Kind::BitwiseNot},

                // COMPARISON
                {"==", Token::Kind::Equal},
                {"!=", Token::Kind::NotEqual},
                {"<",  Token::Kind::Less},
                {">",  Token::Kind::Greater},
                {"<=", Token::Kind::LessEqual},
                {">=", Token::Kind::GreaterEqual},

                // LOGICAL
                {"&&", Token::Kind::LogicalAnd},
                {"||", Token::Kind::LogicalOr},
                {"!",  Token::Kind::LogicalNot},

                // ARITHMETIC
                {"++", Token::Kind::Increment},
                {"--", Token::Kind::Decrement},
                {"+",  Token::Kind::Plus},
                {"-",  Token::Kind::Minus},
                {"*",  Token::Kind::Asterisk},
                {"/",  Token::Kind::ForwardSlash},
                {"%",  Token::Kind::Percent},

                // GROUPING
                {"(",  Token::Kind::LeftParen},
                {")",  Token::Kind::RightParen},
                {"[",  Token::Kind::LeftBracket},
                {"]",  Token::Kind::RightBracket},
                {"{",  Token::Kind::LeftBrace},
                {"}",  Token::Kind::RightBrace},

                // SEPARATOR
                {",",  Token::Kind::Comma},
                {";",  Token::Kind::Semicolon},
                {":",  Token::Kind::Colon},

                // SPECIAL
                {"->",  Token::Kind::ArrowRight},
                {"<-",  Token::Kind::ArrowLeft},
                {"<->", Token::Kind::ArrowBoth},
                {"~>",  Token::Kind::ArrowTildeRight},
                {"<~",  Token::Kind::ArrowTildeLeft},
                {"<~>", Token::Kind::ArrowTildeBoth},
                {"<>",  Token::Kind::ArrowAngleBoth},
                {"...", Token::Kind::Ellipsis}

        };

        inline const SymbolTrie symbolTrie = []
        {
            SymbolTrie trie;

            for (const auto &symbol : symbols)
                trie.insert(symbol.text, symbol.kind);

            return trie;
        }();

    } // namespace detail


    /// @brief null
    class Lexer
    {
        
        private:
            std::string_view source_{};
            std::size_t      position_{};

            /// @brief null
            char current() const noexcept
            {
                return position_ < source_.size()
                    ? source_[position_]
                    : '\0';
            }

            /// @brief null
            char next() const noexcept
            {
                return position_ + 1 < source_.size()
                    ? source_[position_ + 1]
                    : '\0';
            }

            /// @brief null
            char consume() noexcept
            {
                return position_ < source_.size()
                    ? source_[position_++]
                    : '\0';
            }

            // Parsing functions for different token types

            /// @brief null
            Token identifier() noexcept
            {
                const std::size_t start = position_;

                while (detail::isIdentifierChar(current()))
                    consume();

                return Token{
                    Token::Kind::Identifier,
                    source_.substr(start, position_ - start)};
            }

            /// @brief null
            Token number(Token::Kind kind = Token::Kind::Number) noexcept
            {
                const std::size_t start = position_;

                bool valid = true;
                bool floating = false;

                if (current() == '0')
                {
                    consume(); // Consume the '0' character for all non-decimal literals

                    switch (current())
                    {
                        case 'b':
                        {
                            consume();

                            const std::size_t digitStart = position_;

                            while (detail::isBinary(current()))
                                consume();
                            

                            valid = position_ != digitStart;
                            break;
                        }

                        case 'o':
                        {
                            consume();

                            const std::size_t digitStart = position_;

                            while (detail::isOctal(current()))
                                consume();

                            valid = position_ != digitStart;
                            break;
                        }
                        
                        case 'x':
                        {
                            consume();

                            const std::size_t digitStart = position_;

                            while (detail::isHexadecimal(current()))
                                consume();

                            valid = position_ != digitStart;
                            
                            break;
                        }
           
                        case '.':
                        {
                            floating = true;
                            consume();

                            const std::size_t digitStart = position_;

                            while (detail::isDecimal(current()))
                                consume();

                            valid = position_ != digitStart;
                            
                            break;
                        }

                        default:
                        {
                            if (detail::isNumberTerminatorChar(current()))
                                valid = true;
                            break;
                        }
                                      
                    }
                }
                else
                {
                    consume();

                    while (detail::isDecimal(current()))
                        consume();

                    if (current() == '.')
                    {
                        floating = true;
                        consume();

                        const std::size_t digitStart = position_;

                        while (detail::isDecimal(current()))
                            consume();

                        valid = position_ != digitStart;
                    }
                }
                
                if (!detail::isNumberTerminatorChar(current()))
                {
                    valid = false;

                    // Consume the rest of the malformed literals
                    while (current() != '\0' && !detail::isWhitespace(current()))
                        consume();
                }

                return Token
                {
                    valid
                        ? (floating
                            ? Token::Kind::Floating
                            : kind)
                        : Token::Kind::Invalid,
                    source_.substr(start, position_ - start)
                };
            }

            /// @brief null
            Token address() noexcept
            {
                const std::size_t start = position_;

                consume(); // Consume '@'

                while (current() != '\0' &&
                        !detail::isNumberTerminatorChar(current()))
                {
                    consume();
                }

                    return Token{
                        Token::Kind::Invalid,
                        source_.substr(start, position_ - start)};
                

                Token token = number(Token::Kind::Address);

                const bool valid = token.kind() == Token::Kind::Address;

                return Token{
                    valid
                        ? Token::Kind::Address
                        : Token::Kind::Invalid,
                    source_.substr(start, position_ - start)};
            }

            /// @brief null
            Token symbol() noexcept
            {
                const std::size_t start = position_;

                const detail::TrieNode *node = &detail::symbolTrie.root();

                std::optional<Token::Kind> matchedKind;
                std::size_t matchedLength = 0;

                while (position_ < source_.size())
                {
                    auto it = node->children.find(current());

                    if (it == node->children.end())
                        break;

                    node = it->second.get();
                    consume();

                    if (node->kind)
                    {
                        matchedKind = node->kind;
                        matchedLength = position_ - start;
                    }
                }

                if (matchedKind)
                {
                    position_ = start + matchedLength;

                    return Token{
                        *matchedKind,
                        source_.substr(start, matchedLength)};
                }

                position_ = start;
                consume();

                return Token{
                    Token::Kind::Invalid,
                    source_.substr(start, 1)};
            }

        public:
            
            /// @brief null
            void reset() noexcept
            {
                position_ = 0;
            }

            /// @brief null
            explicit Lexer(std::string_view source) noexcept
                : source_(source)
            {
                // N/A
            }

            /// @brief null
            Token advance() noexcept
            {
                while (detail::isWhitespace(current()))
                    consume();
                

                const char c = current();

                if (c == '\0')
                    return Token{Token::Kind::EndOfFile};
                

                if (detail::isAlphabetical(c))
                    return identifier();
                
                
                if (detail::isDecimal(c))
                    return number();
                
                if (c == '@')
                    return address();

                return symbol();
            }
    
    };

    inline std::vector<Token> tokenize(std::string_view source)
    {
        Lexer lexer(source);

        std::vector<Token> tokens;
        tokens.reserve(source.size() / 2);

        while (true)
        {
            Token token = lexer.advance();

            if (token.kind() == Token::Kind::EndOfFile)
                break;

            tokens.push_back(token);
        }

        return tokens;
    }
}

#pragma once

#include <string_view>
#include "Token.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <cstddef>
#include <expected>

namespace cpu
{

    namespace parser
    {

        namespace detail
        {
            
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
                    c == '+'  ||
                    c == '-'  ||
                    c == '*'  ||
                    c == '/'  ||
                    c == '%'  ||
                    c == '^'  ||
                    c == '('  ||
                    c == ')'  ||
                    c == '['  ||
                    c == ']'  ||
                    c == '{'  ||
                    c == '}';
            }
        
        }; // namespace detail


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

                    const bool valid = detail::isIdentifierTerminatorChar(current());

                    if (!valid)
                    {
                        while (!detail::isIdentifierTerminatorChar(current()))
                            consume();
                    }

                    return Token
                    {
                        valid
                            ? Token::Kind::Identifier
                            : Token::Kind::Invalid,
                        
                        source_.substr(start, position_ - start)
                    };
                }

                /// @brief null
                Token number() noexcept
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

                    return Token {
                        valid
                            ? (floating
                                ? Token::Kind::Floating
                                : Token::Kind::Number)
                            : Token::Kind::Invalid,
                            source_.substr(start, position_ - start)
                    };
                }

                /// @brief null
                Token address() noexcept
                {
                    return Token{Token::Kind::Invalid};
                }

            public:
                
                /// @brief null
                explicit Lexer(std::string_view source) noexcept
                    : source_(source)
                {
                    // N/A
                }

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
                    
                    return Token{Token::Kind::Invalid};
                }
        };
    }
}
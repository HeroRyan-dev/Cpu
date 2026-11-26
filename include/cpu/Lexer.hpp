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
        
            constexpr bool isWhitespace(char c) noexcept
            {
                return c == ' ' || c == '\t' || c == '\n' || c == '\r';
            }

            constexpr bool isAlphabetic(char c) noexcept
            {
                return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
            }

            constexpr bool isDecimal(char c) noexcept
            {
                return c >= '0' && c <= '9';
            }

            constexpr bool isOctal(char c) noexcept
            {
                return c >= '0' && c <= '7';
            }

            constexpr bool isHexadecimal(char c) noexcept
            {
                return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
            }
        
            constexpr bool isBinary(char c) noexcept
            {
                return c == '0' || c == '1';
            }

            constexpr bool isAlphaNumeric(char c) noexcept
            {
                return isAlphabetic(c) || isDecimal(c);
            }

            constexpr bool isNumberTerminator(char c) noexcept
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

            constexpr bool isIdentifierTerminator(char c) noexcept
            {
                return isWhitespace(c) ||
                    c == '\0' ||
                    c == '+'  ||
                    c == '-'  ||
                    c == '*'  ||
                    c == '/'  ||
                    c == '%'  ||
                    c == '^';

            }

            constexpr bool isIdentifierChar(char c) noexcept
            {
                return isAlphaNumeric(c) || c == '_';
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

                    consume();

                    while (detail::isIdentifierChar(current()))
                        consume();
                    
                    if (!detail::isIdentifierTerminator(current()))
                    {
                        while (!detail::isIdentifierTerminator(current()))
                            consume();
                        
                    
                        return Token {
                            Token::Kind::Invalid,
                            source_.substr(start, position_ - start)
                        };
                    }

                    return Token {
                        Token::Kind::Identifier,
                        source_.substr(start, position_ - start)
                    };
                }

                /// @brief null
                Token number() noexcept
                {
                    const std::size_t start = position_;

                    bool valid = true;
                    bool floating = false;
                    bool hasDigits = false;
                    bool prefixed = false;

                    if (current() == '0')
                    {
                        prefixed = true;
                        consume(); // Consume the '0' character for all non-decimal literals

                        switch (current())
                        {
                            case 'b':
                            {
                                hasDigits = true;
                                consume();


                                while (detail::isBinary(current()))
                                    consume();
                                
                                break;
                            }

                            case 'o':
                            {
                                hasDigits = true;
                                consume();

                                while (detail::isOctal(current()))
                                    consume();
                                
                                break;
                            }
                            
                            case 'x':
                            {
                                hasDigits = true;
                                consume();
                                
                                while (detail::isHexadecimal(current()))
                                    consume();
                                
                                break;
                            }
               
                            case '.':
                            {
                                floating = true;
                                hasDigits = true;
                                prefixed = true;
                                consume();
                                break;
                            }

                            default:
                            {
                                prefixed = false;

                                hasDigits = true;

                                if (!detail::isDecimal(current()))
                                {
                                    valid = false;
                                
                                    while (detail::isDecimal(current()))
                                        consume();
                                
                                }

                                break;
                            }
                                          
                        }
                    }
                    else
                    {
                        while (detail::isDecimal(current()))
                        {
                            hasDigits = true;
                            consume();

                        }

                        if (current() == '.')
                        {
                            floating = true;
                            consume();
                        
                            while (detail::isDecimal(current()))
                                consume();
                        }
                    }
                    
                    if (prefixed && !hasDigits)
                        valid = false;
                    
                    if (!detail::isNumberTerminator(current()))
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
                    const std::size_t start = position_;

                    consume();


                    return Token{
                        Token::Kind::Address,
                        source_.substr(start, position_ - start)};
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

                    // First char scans
                    
                    while (detail::isWhitespace(current()))
                    {
                        consume();
                    }

                    const char c = current();

                    if (c == '\0')
                    {
                        return Token{Token::Kind::EndOfFile};
                    }

                    if (detail::isAlphabetic(c))
                    {
                        return identifier();
                    }
                    
                    if (detail::isDigit(c))
                    {
                        return number();
                    }

                    if (c == '@')
                    {
                        return address();
                    }
                }

        };
    }
}
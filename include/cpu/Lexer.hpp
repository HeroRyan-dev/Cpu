#pragma once

#include <string_view>
#include "Token.hpp"

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

            constexpr bool isDigit(char c) noexcept
            {
                return c >= '0' && c <= '9';
            }

            constexpr bool isOctalDigit(char c) noexcept
            {
                return c >= '0' && c <= '7';
            }

            constexpr bool isHexDigit(char c) noexcept
            {
                return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
            }
        
            constexpr bool isBinaryDigit(char c) noexcept
            {
                return c == '0' || c == '1';
            }

            constexpr bool isAlphaNumeric(char c) noexcept
            {
                return isAlphabetic(c) || isDigit(c);
            }

            constexpr bool isDigitForBase(char c, unsigned int base) noexcept
            {
                if (base == 2)
                    return c >= '0' && c <= '1';
                if (base == 8)
                    return c >= '0' && c <= '7';
                if (base == 10)
                    return isDigit(c);
                if (base == 16)
                    return isHexDigit(c);

                return false;
            }
        }
    


        /// @brief null
        class Lexer
        {
        
            private:
                std::string_view source_{};
                std::size_t position_{};

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

                    while (false) // Continue consuming characters for the identifier
                    {
                        // Implement logic to continue consuming characters for the identifier
                    }

                    return Token{
                        Token::Kind::Identifier,
                        source_.substr(start, position_ - start)};
                }

                /// @brief null
                Token number() noexcept
                {
                    const std::size_t start = position_;
                    unsigned int base = 10; // Default base is decimal
                    bool valid = true;


                    if (current() == '0')
                    {
                        switch (next())
                        {
                            case 'x': base = 16; break;
                            case 'b': base = 2;  break;
                            case 'o': base = 8;  break;
                            default: break;
                        }
                        
                        if (base != 10)
                        {
                            consume(); // Consume '0'
                            consume(); // Consume the base prefix ('x', 'b', or 'o')
                        }
                    }

                    switch (base)
                    {
                        case 2: // Binary, consume all binary digits
                        {
                            while (detail::isBinaryDigit(current()))
                            {
                                consume();
                            }
                            break;
                        }
                        case 8: // Octal, consume all octal digits
                        {
                            while (detail::isOctalDigit(current()))
                            {
                                consume();
                            }
                            break;
                        }
                        case 10: // Decimal, consume all decimal digits
                        {
                            while (detail::isDigit(current()))
                            {
                                consume();
                            }
                            
                            if (current() == '.')
                            {
                                consume(); // Consume the '.'

                                while (detail::isDigit(current()))
                                {
                                    consume();
                                }
                            }
                            
                            break;
                        }
                        case 16: // Hexadecimal, consume all hexadecimal digits
                        {
                            while (detail::isDigitForBase(current(), base))
                            {
                                consume();
                            }
                            break;
                        }
                        
                        default:
                        {
                            break;
                        }
                    }

                    return Token{
                        Token::Kind::Number,
                        source_.substr(start, position_ - start)};
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


                }

        };
    }
}
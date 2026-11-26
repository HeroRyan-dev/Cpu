#pragma once

#include <string_view>


namespace cpu
{
    namespace lexer
    {
        /// @brief null
        class Token
        {
            public:
                
                /// @brief null
                enum class Kind
                {
                    Number,        // 0 ... 9
                    Floating,      // 0 ... 9 with decimal point
                    Identifier,    // variable names
                    Hex,           // 0x...
                    Binary,        // 0b...
                    Char,          // 'a'
                    Address,       // @variable

                    // Bitwise symbols
                    LeftShift,     // <<
                    RightShift,    // >>
                    BitwiseAnd,    // &
                    BitwiseOr,     // |
                    BitwiseXor,    // ^
                    BitwiseNot,    // ~

                    // Logical symbols
                    LogicalAnd,    // &&
                    LogicalOr,     // ||
                    LogicalNot,    // !

                    // Comparison symbols
                    Equal,         // ==
                    NotEqual,      // !=
                    Less,          // <
                    Greater,       // >
                    LessEqual,     // <=
                    GreaterEqual,  // >=

                    // Arithmetic symbols
                    Increment,     // ++
                    Decrement,     // --
                    Plus,          // +
                    Minus,         // -
                    Asterisk,      // *
                    ForwardSlash,  // /
                    Percent,       // %

                    // Grouping symbols
                    LeftParen,     // (
                    RightParen,    // )
                    LeftBracket,   // [
                    RightBracket,  // ]
                    LeftBrace,     // {
                    RightBrace,    // }

                    Comma,         // ,
                    Semicolon,     // ;
                    Colon,         // :

                    // Arrow symbols
                    ArrowRight,      // ->
                    ArrowLeft,       // <-
                    ArrowBoth,       // <->
                    ArrowTildeRight, // ~>
                    ArrowTildeLeft,  // <~
                    ArrowTildeBoth,  // <~>
                    ArrowAngleBoth,  // <>

                    Invalid,          // N/A
                    MalformedLiteral, // N/A
                    Unknown,          // N/A
                    Ellipsis,         // ...
                    EndOfFile         // N/A
                };

            private:
                
                /// @brief null
                Kind kind_{};
                
                /// @brief null
                std::string_view text_{};

            public:
                
                /// @brief null
                Token(Kind kind)
                    : kind_(kind)
                {
                    // N/A
                }

                /// @brief null
                Token(Kind kind, std::string_view text)
                    : kind_(kind),
                      text_(text)
                {
                    // N/A
                }

                /// @brief null
                Kind kind() const noexcept
                {
                    return kind_;
                }

                /// @brief null
                std::string_view text() const noexcept
                {
                    return text_;
                }

                /// @brief null
                bool is(Kind kind) const noexcept
                {
                    return kind_ == kind;
                }

                /// @brief null
                bool isNot(Kind kind) const noexcept
                {
                    return kind_ != kind;
                }

                /// @brief null
                bool isOneOf(Kind k1, Kind k2) const noexcept
                {
                    return kind_ == k1 || kind_ == k2;
                }

                /// @brief null
                template<typename... Ts>
                bool isOneOf(Kind k1, Kind k2, Ts... ks) const noexcept
                {
                    return is(k1) || isOneOf(k2, ks...);
                }

        };
    
    }

}
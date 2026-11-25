#pragma once

#include <string_view>


namespace cpu
{
    namespace parser
    {
        
        /// @brief null
        class Token
        {
            public:
                
                /// @brief null
                enum class Kind
                {
                    Number,
                    Identifier,
                    Hex,
                    Binary,
                    Char,
                    Address,

                    EndOfFile
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
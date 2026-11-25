#pragma once

#include "cpu/Types.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <expected>

namespace cpu
{

    using namespace boost::multiprecision::literals;

    enum class ALUOperation : uint8_t
    {
        ADD,
        SUB,
        MUL,
        DIV,
        
        AND,
        OR,
        XOR,
        NOT,

        SHL,
        SHR,
        SAR
    };

    enum class ALUError
    {
        DIVIDE_BY_ZERO,
        INVALID_OPERATION
    };

    struct ALUResult
    {
        types::qword_t value{};
        types::qword_t high{};

        bool zero{};
        bool carry{};
        bool overflow{};
        bool negative{};
    };

    class ALU
    {
        private:

            static constexpr unsigned bit_width = 128;
            static constexpr unsigned sign_bit = bit_width - 1;

            static unsigned getShiftAmount(types::qword_t value) noexcept
            {
                return value >= bit_width
                    ? bit_width + 1
                    : static_cast<unsigned>(value);
            }
            
            static bool isNegative(types::qword_t value) noexcept
            {
                return boost::multiprecision::bit_test(value, sign_bit);
            }

            static bool addOverflow(types::qword_t a, types::qword_t b, types::qword_t result)
            {
                return isNegative(a) == isNegative(b)
                    && isNegative(result) != isNegative(a);
            }

            static bool subOverflow(types::qword_t a, types::qword_t b, types::qword_t result)
            {
                return isNegative(a) != isNegative(b)
                    && isNegative(result) != isNegative(a);
            }

            static void updateZeroNegative(ALUResult& result) noexcept
            {
                result.zero = result.value == 0;
                result.negative = isNegative(result.value);
            }

            static bool subCarry(types::qword_t a, types::qword_t b)
            {
                return a < b;
            }
            
            static types::qword_t lowWord(const types::word256_t& value)
            {
                static const types::word256_t mask =
                    (types::word256_t(1) << bit_width) - 1;

                return static_cast<types::qword_t>(value & mask);
            }

        public:
            std::expected<ALUResult, ALUError>
            execute(ALUOperation operation, types::qword_t a, types::qword_t b) const
            {
                ALUResult result{};
                


                switch (operation)
                {
                    case ALUOperation::ADD:
                    {
                        types::word256_t intermediate = a;
                        intermediate += b;

                        result.value = lowWord(intermediate);
                        result.carry = (intermediate >> 128) != 0;
                        result.overflow = addOverflow(a, b, result.value);
                        
                        break;
                    }

                    case ALUOperation::SUB:
                    {
                        types::word256_t intermediate = types::word256_t(1) << bit_width;
                        intermediate += a;
                        intermediate -= b;

                        result.value = lowWord(intermediate);
                        result.carry = subCarry(a, b);
                        result.overflow = subOverflow(a, b, result.value);

                        break;
                    }

                    case ALUOperation::MUL:
                    {
                        types::word256_t intermediate = a;
                        intermediate *= b;

                        result.value = lowWord(intermediate);
                        result.high  = static_cast<types::qword_t>(intermediate >> bit_width);

                        result.carry = result.high != 0;
                        result.overflow = result.carry;

                        break;

                    }

                    case ALUOperation::DIV:
                    {
                        if (b == 0)
                        {
                            return std::unexpected(ALUError::DIVIDE_BY_ZERO);
                        }

                        result.value = a / b;
                        result.high = a % b; // Remainder of the division
                        
                        break;
                    }
                        
                    case ALUOperation::AND:
                    {
                        result.value = a & b;
                        break;
                    }

                    case ALUOperation::OR:
                    {
                        result.value = a | b;
                        break;
                    }

                    case ALUOperation::XOR:
                    {
                        result.value = a ^ b;
                        break;
                    }

                    case ALUOperation::NOT:
                    {
                        result.value = ~a;
                        break;
                    }

                    case ALUOperation::SHL:
                    {
                        auto shift = getShiftAmount(b);

                        result.value = shift >= bit_width ? 0 : a << shift;

                        result.carry = shift > 0 && shift <= bit_width
                            ? boost::multiprecision::bit_test(static_cast<types::word256_t>(a), bit_width - shift)
                            : false;

                        break;
                    }

                    case ALUOperation::SHR:
                    {
                        auto shift = getShiftAmount(b);

                        result.value = shift >= bit_width ? 0 : a >> shift;

                        result.carry = shift > 0 && shift <= bit_width
                            ? boost::multiprecision::bit_test(a, shift - 1)
                            : false;

                        break;
                    }

                    case ALUOperation::SAR:
                    {
                        auto shift = getShiftAmount(b);
                        bool negative = isNegative(a);
                        
                        if (shift == 0)
                        {
                            result.value = a;
                        } else if (shift >= bit_width)
                        {
                            result.value = negative ? ~types::qword_t(0) : types::qword_t(0);
                        }
                        else
                        {
                            result.value = a >> shift;

                            if (negative)
                                result.value |= (~types::qword_t(0)) << (bit_width - shift);
                        }

                        result.carry = shift > 0 && shift <= bit_width
                            ? boost::multiprecision::bit_test(a, shift - 1)
                            : false;

                        break;
                    }
                
                    default:
                        return std::unexpected(ALUError::INVALID_OPERATION);
                }
                
                updateZeroNegative(result);

                return result;


            }




    };
}
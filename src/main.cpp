#include <iostream>
#include <string>
#include <algorithm>
#include <expected>
#include <string_view>
#include <cassert>
#include <optional>
#include <vector>
#include <map>
#include <utility>
#include <cstdint>
#include <bitset>
#include <bit>

#include <boost/multiprecision/cpp_int.hpp>

#include "cpu/RegisterFile.hpp"
#include "cpu/Register.hpp"
#include "cpu/Memory.hpp"
#include "cpu/Stack.hpp"
#include "cpu/ALU.hpp"

int main()
{
    namespace bmp = boost::multiprecision;
    using namespace boost::multiprecision::literals;

    cpu::RegisterFile registers;
    cpu::Memory memory;

    registers.include("R1", std::make_unique<cpu::Register>(16));
    registers.include("R2", std::make_unique<cpu::Register>(32));
    registers.include("R3", std::make_unique<cpu::Register>(64));
    registers.include("R4", std::make_unique<cpu::Register>(128));

    registers.include("SP", std::make_unique<cpu::Register>(16));

    auto locate_result = registers.locate("R1");

    // Attempt to locate the register "R1" in the register file.
    if (!locate_result)
    {
        std::cerr << "Failed to locate R1\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully located R1\n";
    }

    auto* selected_register = *locate_result;

    selected_register->write(42);

    auto locate_result_sp = registers.locate("SP");

    if (!locate_result_sp)
    {
        std::cerr << "Failed to locate SP\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully located SP\n";
    }

    auto* sp_register = *locate_result_sp;

    cpu::Stack stack(*sp_register);

    std::cout << "Stack initialized with SP\n";

    // Stack tests.
    auto push_result = stack.push(10);

    if (!push_result)
    {
        std::cerr << "Failed to push 10 onto the stack\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully pushed 10 onto the stack\n";
    }

    auto push_result_2 = stack.push(12);

    if (!push_result_2)
    {
        std::cerr << "Failed to push 12 onto the stack\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully pushed 12 onto the stack\n";
    }

    auto pop_result = stack.pop();

    if (!pop_result)
    {
        std::cerr << "Failed to pop from the stack\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully popped from the stack\n";
    }

    std::cout << "Value popped from the stack: "
              << pop_result.value() << "\n";

    auto pop_result_2 = stack.pop();

    if (!pop_result_2)
    {
        std::cerr << "Failed to pop from the stack\n";
        return 1;
    }
    else
    {
        std::cout << "Successfully popped from the stack\n";
    }

    std::cout << "Value popped from the stack: "
              << pop_result_2.value() << "\n";

    auto pop_result_3 = stack.pop();

    if (!pop_result_3)
    {
        std::cout << "Failed to pop from the stack as expected\n";
    }
    else
    {
        std::cout << "Unexpectedly succeeded in popping from the stack\n";
    }

    if (pop_result_3)
    {
        std::cout << "Value popped from the stack: "
                  << pop_result_3.value() << "\n";
    }

    // ALU tests.
    cpu::ALU alu;

    auto printResult = [](std::string_view name, const auto& result)
    {
        std::cout << "\n" << name << '\n';

        if (!result)
        {
            std::cout << "ALU error: "
                      << static_cast<int>(result.error()) << '\n';
            return;
        }

        std::cout << "Value:    " << result->value << '\n';
        std::cout << "High:     " << result->high << '\n';
        std::cout << "Zero:     " << result->zero << '\n';
        std::cout << "Carry:    " << result->carry << '\n';
        std::cout << "Overflow: " << result->overflow << '\n';
        std::cout << "Negative: " << result->negative << '\n';
    };

    // Addition.
    printResult("ADD: 5 + 3",
        alu.execute(cpu::ALUOperation::ADD, 5, 3));

    printResult("ADD: Maximum unsigned value + 1",
        alu.execute(cpu::ALUOperation::ADD, ~cpu::qword_t(0), 1));

    printResult("ADD: Signed maximum + 1",
        alu.execute(cpu::ALUOperation::ADD,
            (cpu::qword_t(1) << 127) - 1, 1));

    // Subtraction.
    printResult("SUB: 10 - 5",
        alu.execute(cpu::ALUOperation::SUB, 10, 5));

    printResult("SUB: 5 - 10 (borrow)",
        alu.execute(cpu::ALUOperation::SUB, 5, 10));

    printResult("SUB: Signed minimum - 1",
        alu.execute(cpu::ALUOperation::SUB,
            cpu::qword_t(1) << 127, 1));

    // Multiplication.
    printResult("MUL: 12 * 5",
        alu.execute(cpu::ALUOperation::MUL, 12, 5));

    printResult("MUL: Maximum unsigned value * 2",
        alu.execute(cpu::ALUOperation::MUL,
            ~cpu::qword_t(0), 2));

    // Division.
    printResult("DIV: 17 / 5",
        alu.execute(cpu::ALUOperation::DIV, 17, 5));

    printResult("DIV: 100 / 10",
        alu.execute(cpu::ALUOperation::DIV, 100, 10));

    printResult("DIV: Division by zero",
        alu.execute(cpu::ALUOperation::DIV, 10, 0));

    // Bitwise operations.
    printResult("AND: 12 & 10",
        alu.execute(cpu::ALUOperation::AND, 12, 10));

    printResult("OR: 12 | 10",
        alu.execute(cpu::ALUOperation::OR, 12, 10));

    printResult("XOR: 12 ^ 10",
        alu.execute(cpu::ALUOperation::XOR, 12, 10));

    printResult("NOT: ~0",
        alu.execute(cpu::ALUOperation::NOT, 0, 0));

    // Left shifts.
    printResult("SHL: 1 << 1",
        alu.execute(cpu::ALUOperation::SHL, 1, 1));

    printResult("SHL: Bit 127 shifted out",
        alu.execute(cpu::ALUOperation::SHL,
            cpu::qword_t(1) << 127, 1));

    printResult("SHL: Shift by 128",
        alu.execute(cpu::ALUOperation::SHL, 1, 128));

    // Logical right shifts.
    printResult("SHR: 128 >> 2",
        alu.execute(cpu::ALUOperation::SHR, 128, 2));

    printResult("SHR: Bit 0 shifted out",
        alu.execute(cpu::ALUOperation::SHR, 1, 1));

    // Arithmetic right shifts.
    printResult("SAR: Negative value >> 1",
        alu.execute(cpu::ALUOperation::SAR,
            cpu::qword_t(1) << 127, 1));

    printResult("SAR: Negative value >> 128",
        alu.execute(cpu::ALUOperation::SAR,
            cpu::qword_t(1) << 127, 128));

    return 0;
}
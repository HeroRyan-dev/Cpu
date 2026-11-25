#include <iostream>
#include <memory>
#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include "cpu/Register.hpp"
#include "cpu/RegisterFile.hpp"
#include "cpu/Stack.hpp"
#include "cpu/Memory.hpp"
#include "cpu/Print.hpp"

using namespace cpu;

// ============================================================
// RegisterFile Tests
// ============================================================

void testRegisterFile(storage::RegisterFile& registers)
{
    fmt::print("\n========== REGISTER FILE ==========\n");

    // Include registers.
    auto add1 = registers.include("R1", 32);
    auto add2 = registers.include("R2", 32);
    auto add3 = registers.include("SP", 14);

    fmt::println("Added @Register R1: {}", static_cast<bool>(add1));
    fmt::println("Added @Register R2: {}", static_cast<bool>(add2));
    fmt::println("Added @Register SP: {}", static_cast<bool>(add3));

    // Locate and modify R1.
    std::string register_name = "Rz";
    auto locate_R1 = registers.locate(register_name);
    if (locate_R1)
    {
        auto* R1 = *locate_R1;
        
        fmt::println("[{}] initial value: {}", register_name, R1->read().str());
        
        fmt::println("Writing `100` to [{}]", register_name);
        R1->write(100);
        
        fmt::println("{} new value: {}", register_name, R1->read().str());
    } else
    {
        printError(locate_R1.error(), register_name);
    }

    types::register_id_t id = 66;
    auto locate_R66 = registers.locate(id);
    if (locate_R66)
    {
        auto *R66 = *locate_R66;

        fmt::println("[{}] initial value: {}", std::to_string(id), R66->read().str());
       
        R66->write(100);

        fmt::println("{} new value: {}", std::to_string(id), R66->read().str());
    }
    else
    {
        printError(locate_R66.error(), std::to_string(id));
    }

    // Test duplicate names.
    auto duplicate = registers.include("R1", 32);
    if (!duplicate)
    {
        printError(duplicate.error(), "R1");
    }

    std::string alt_register_name = "R2";
    
    auto removed = registers.remove(alt_register_name);

    if (!removed)
    {
        printError(removed.error(), alt_register_name);
    }
}


// ============================================================
// Memory Tests
// ============================================================

void testMemory(Memory& memory)
{
    std::cout << "\n========== MEMORY ==========\n";

    // Write values.
    auto w1 = memory.write(0, 100);
    auto w2 = memory.write(1, 200);
    auto w3 = memory.write(2, 300);

    std::cout << "Write results: "
              << static_cast<bool>(w1) << ", "
              << static_cast<bool>(w2) << ", "
              << static_cast<bool>(w3) << '\n';

    // Read values.
    for (types::address_t address = 0; address < 3; ++address)
    {
        auto result = memory.read(address);

        if (result)
            std::cout << "Memory[" << address << "] = "
                      << *result << '\n';
    }

    // Overwrite an address.
    auto overwrite = memory.write(1, 500);

    if (overwrite)
    {
        auto updated = memory.read(1);

        if (updated)
            std::cout << "Memory[1] after overwrite: "
                      << *updated << '\n';
    }

    // Test invalid addresses.
    auto invalid_read = memory.read(16384);
    auto invalid_write = memory.write(16384, 999);

    std::cout << "Invalid read rejected: " << !invalid_read << '\n';
    std::cout << "Invalid write rejected: " << !invalid_write << '\n';
}

// ============================================================
// Stack Tests
// ============================================================

void testStack(storage::RegisterFile& registers)
{
    std::cout << "\n========== STACK ==========\n";

    // Retrieve SP from RegisterFile.
    auto sp = registers.locate("SP");

    if (!sp)
    {
        std::cerr << "Failed to locate SP\n";
        return;
    }

    // Stack borrows the SP register.
    storage::Stack stack{**sp};

    std::cout << "Initial SP: " << (*sp)->read() << '\n';

    // Push values.
    for (types::word_t value : {10, 20, 30})
    {
        auto result = stack.push(value);

        if (result)
            std::cout << "Pushed: " << value << '\n';
        else
            std::cout << "Push failed: "
                      << static_cast<int>(result.error()) << '\n';
    }

    std::cout << "SP after pushes: " << (*sp)->read() << '\n';

    // Peek at the top.
    auto top = stack.peek();

    if (top)
        std::cout << "Top value: " << *top << '\n';

    // Pop values.
    while ((*sp)->read() != 0)
    {
        auto result = stack.pop();

        if (result)
            std::cout << "Popped: " << *result << '\n';
        else
            break;
    }

    std::cout << "SP after pops: " << (*sp)->read() << '\n';

    // Test underflow.
    auto underflow = stack.pop();

    std::cout << "Underflow detected: " << !underflow << '\n';

    // Test invalid stack pointer.
    (*sp)->write(4097);

    auto invalid = stack.peek();

    std::cout << "Invalid SP detected: "
              << (!invalid &&
                  invalid.error() == errors::StackError::InvalidStackPointer)
              << '\n';

    // Restore SP.
    (*sp)->write(0);
}

// ============================================================
// Main
// ============================================================

int main()
{
    std::cout << "========== CRYO COMPONENT TESTS ==========\n";

    // Central ownership of registers.
    storage::RegisterFile registers;

    // Shared memory instance.
    Memory memory;

    testRegisterFile(registers);
    testMemory(memory);
    testStack(registers);

    std::cout << "\n========== TESTS COMPLETE ==========\n";

    fmt::print("Hello, Cryo!\n");

    fmt::print("Register {} = {}\n", "R1", 100);

    fmt::print(
        fmt::fg(fmt::color::cyan),
        "Formatting works!\n");

    return 0;
}
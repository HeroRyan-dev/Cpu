#include <iostream>
#include <string>
#include <algorithm>
#include "cpu/RegisterFile.hpp"
#include "cpu/RegisterPrinter.hpp"

int main()
{
    using namespace cpu;

    cpu::RegisterFile registers;

registers[R3] = 123456789; // Example assignment to R3

// Print all registers in decimal.
RegisterPrinter::print(registers, std::cout);

// Print all registers in hexadecimal.
RegisterPrinter::printHex(registers, std::cout);

// Print an individual 128-bit result.
std::cout << "Result in R3: "
          << RegisterPrinter::toDecimal(registers[R3])
          << '\n';

RegisterPrinter::print(registers, std::cout);
RegisterPrinter::printHex(registers, std::cout);
RegisterPrinter::printBinary(registers, std::cout);

// Print individual values.
std::cout << RegisterPrinter::toDecimal(registers[R3]) << '\n';
std::cout << RegisterPrinter::toHex(registers[R3]) << '\n';
std::cout << RegisterPrinter::toBinary(registers[R3]) << '\n';
    return 0;
}
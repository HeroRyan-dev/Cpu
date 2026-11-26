#include <iostream>
#include <string>
#include <algorithm>
#include "cpu/registers/RegisterFile.hpp"
#include "cpu/registers/RegisterPrinter.hpp"

int main()
{
    using namespace cpu::registers;

    RegisterFile registerFile;

registerFile[R3] = 123456789; // Example assignment to R3

// Print all registers in decimal.
RegisterPrinter::print(registerFile, std::cout);

// Print all registers in hexadecimal.
RegisterPrinter::printHex(registerFile, std::cout);

// Print an individual 128-bit result.
std::cout << "Result in R3: "
          << RegisterPrinter::toDecimal(registerFile[R3])
          << '\n';

RegisterPrinter::print(registerFile, std::cout);
RegisterPrinter::printHex(registerFile, std::cout);
RegisterPrinter::printBinary(registerFile, std::cout);

// Print individual values.
std::cout << RegisterPrinter::toDecimal(registerFile[R3]) << '\n';
std::cout << RegisterPrinter::toHex(registerFile[R3]) << '\n';
std::cout << RegisterPrinter::toBinary(registerFile[R3]) << '\n';
    return 0;
}

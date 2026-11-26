#include <cstddef>
#include <expected>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cpu/parser/Token.hpp"
#include "cpu/parser/Lexer.hpp"

int main()
{

    auto tokens = cpu::parser::tokenize("@2+2+0x0000 -> 2x");
    for (const auto &token : tokens)
        std::cout << "Token: " << static_cast<int>(token.kind()) << " \"" << token.text() << "\"" << '\n';

}

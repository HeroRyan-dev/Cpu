#include "cpu/parser/Lexer.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
    // Escape characters that have special meaning in Graphviz labels.
    void writeEscaped(std::ostream &out, unsigned char character)
    {
        switch (character)
        {
        case '"':
            out << "\\\"";
            break;
        case '\\':
            out << "\\\\";
            break;
        case '\n':
            out << "\\n";
            break;
        case '\r':
            out << "\\r";
            break;
        case '\t':
            out << "\\t";
            break;

        default:
            if (character >= 32 && character <= 126)
                out << static_cast<char>(character);
            else
                out << "\\x" << std::hex
                    << static_cast<unsigned>(character)
                    << std::dec;
        }
    }

    void writeSymbolTrie(std::ostream &out)
    {
        using namespace cpu::lexer::detail;

        const auto &automaton = symbolAutomaton;

        out << "digraph SymbolTrie {\n";
        out << "    rankdir=LR;\n";
        out << "    node [shape=circle];\n";
        out << "    edge [fontname=\"Consolas\"];\n\n";

        out << "    n0 [label=\"START\", shape=box];\n";

        // A state is visited only once.
        bool visited[SymbolAutomaton::nodeCapacity] = {};

        auto visit = [&](auto &&self, std::uint8_t state) -> void
        {
            if (visited[state])
                return;

            visited[state] = true;

            // Mark accepting states.
            if (automaton.accepts[state].has_value())
            {
                const auto kind =
                    static_cast<unsigned>(*automaton.accepts[state]);

                out << "    n" << static_cast<unsigned>(state)
                    << " [label=\"State "
                    << static_cast<unsigned>(state)
                    << "\\nToken kind: " << kind
                    << "\", shape=doublecircle];\n";
            }
            else if (state != 0)
            {
                out << "    n" << static_cast<unsigned>(state)
                    << " [label=\"State "
                    << static_cast<unsigned>(state)
                    << "\"];\n";
            }

            // Explore every possible character transition.
            for (unsigned character = 0; character < 256; ++character)
            {
                const std::uint8_t next =
                    automaton.next[state][character];

                // Zero means no transition.
                if (next == 0)
                    continue;

                out << "    n" << static_cast<unsigned>(state)
                    << " -> n" << static_cast<unsigned>(next)
                    << " [label=\"";

                writeEscaped(out, static_cast<unsigned char>(character));

                out << "\"];\n";

                self(self, next);
            }
        };

        visit(visit, SymbolAutomaton::startNode);

        out << "}\n";
    }
}

int main(int argc, char *argv[])
{
    const std::string outputPath =
        argc > 1 ? argv[1] : "symbol_trie.dot";

    std::ofstream output(outputPath);

    if (!output)
    {
        std::cerr << "Failed to open: " << outputPath << '\n';
        return 1;
    }

    writeSymbolTrie(output);

    std::cout << "Generated: " << outputPath << '\n';
}
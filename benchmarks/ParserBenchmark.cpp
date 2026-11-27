
#include <benchmark/benchmark.h>

#include <cstddef>
#include <string_view>

#include "cpu/parser/Lexer.hpp"

using namespace cpu::parser;

// ============================================================
// Tokenization helper
// ============================================================

static std::size_t tokenize(Lexer &lexer)
{
    std::size_t tokenCount = 0;

    while (true)
    {
        auto token = lexer.advance();

        benchmark::DoNotOptimize(token);

        if (token.kind() == Token::Kind::EndOfFile)
            break;

        ++tokenCount;
    }

    return tokenCount;
}

// ============================================================
// Construct + tokenize
// ============================================================

static void runConstructBenchmark(
    benchmark::State &state,
    std::string_view source)
{
    std::size_t totalTokens = 0;

    for (auto _ : state)
    {
        Lexer lexer(source);

        totalTokens += tokenize(lexer);
    }

    state.SetItemsProcessed(totalTokens);
    state.SetBytesProcessed(state.iterations() * source.size());
}

// ============================================================
// Reuse + tokenize
// ============================================================

static void runReuseBenchmark(
    benchmark::State &state,
    std::string_view source)
{
    Lexer lexer(source);

    std::size_t totalTokens = 0;

    for (auto _ : state)
    {
        lexer.reset();

        totalTokens += tokenize(lexer);
    }

    state.SetItemsProcessed(totalTokens);
    state.SetBytesProcessed(state.iterations() * source.size());
}

// ============================================================
// Benchmark registration helpers
// ============================================================

#define DEFINE_LEXER_BENCHMARK(name, source)                       \
    static void BM_Lexer_Construct_##name(benchmark::State &state) \
    {                                                              \
        runConstructBenchmark(state, source);                      \
    }                                                              \
                                                                   \
    static void BM_Lexer_Reuse_##name(benchmark::State &state)     \
    {                                                              \
        runReuseBenchmark(state, source);                          \
    }

// ============================================================
// Test inputs
// ============================================================

DEFINE_LEXER_BENCHMARK(
    SimpleArithmetic,
    "10 + 20 * 30 - 40 / 5")

DEFINE_LEXER_BENCHMARK(
    ComplexExpression,
    "((10 + 20) * (30 - 40)) / "
    "(50 + (60 * 70) - (80 / 4))")

DEFINE_LEXER_BENCHMARK(
    Operators,
    "10++ + --20 * 30 >= 40 && "
    "50 != 60 || 70 <= 80")

DEFINE_LEXER_BENCHMARK(
    Bitwise,
    "~(0xFF & 0b10101010) | "
    "(0x10 << 2) ^ (0xAB >> 1)")

DEFINE_LEXER_BENCHMARK(
    Addresses,
    "MOV R0, @0x1000; "
    "MOV R1, @0b101010; "
    "ADD ACC, R0; "
    "STORE @1024, ACC")

DEFINE_LEXER_BENCHMARK(
    InvalidInput,
    "10abc + 0xFFG * 0b102 + "
    "@10.5 - 01234 + 0x + 10@")

DEFINE_LEXER_BENCHMARK(
    LargeExpression,
    "R0 + R1 * R2 - R3 / R4 + "
    "(R5 << 2) & 0xFF | "
    "(R6 ^ R7) && "
    "(@0x1000 >= 100) || "
    "(~0b10101010 != 0xFF)")

// Cryo's arrow syntax: an expression, "->", then a destination.
DEFINE_LEXER_BENCHMARK(
    CryoProgram,
    "MOVE R1 -> R2\n"
    "ADD R1 + R2 -> R3\n"
    "SUB R1 - R2 -> R3\n"
    "SHIFT R1 << 2 -> R3\n"
    "SHIFT @0x000 << 2 -> R3\n"
    "LOAD @0x100 -> R0\n"
    "STORE R0 -> @0x100\n")

// ============================================================
// Registration
// ============================================================

#define REGISTER_LEXER_BENCHMARK(name)    \
    BENCHMARK(BM_Lexer_Construct_##name); \
    BENCHMARK(BM_Lexer_Reuse_##name);

REGISTER_LEXER_BENCHMARK(SimpleArithmetic)
REGISTER_LEXER_BENCHMARK(ComplexExpression)
REGISTER_LEXER_BENCHMARK(Operators)
REGISTER_LEXER_BENCHMARK(Bitwise)
REGISTER_LEXER_BENCHMARK(Addresses)
REGISTER_LEXER_BENCHMARK(InvalidInput)
REGISTER_LEXER_BENCHMARK(LargeExpression)
REGISTER_LEXER_BENCHMARK(CryoProgram)

BENCHMARK_MAIN();
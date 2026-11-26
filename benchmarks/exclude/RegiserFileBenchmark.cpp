
/*
    Cpu\benchmarks\RegisterFileBench.cpp
    Benchmarks for the RegisterFile class.
*/

#include <benchmark/benchmark.h>

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

#include "cpu/RegisterFile.hpp"
#include "cpu/Types.hpp"

// ============================================================
// Configuration
// ============================================================

using cpu::storage::RegisterFile;
using cpu::types::byte_t;

static constexpr std::size_t register_count = 128;
static constexpr byte_t register_width = 32;

// ============================================================
// Helpers
// ============================================================

static std::string registerName(std::size_t id)
{
    return "R" + std::to_string(id);
}

static void populate(RegisterFile &registers)
{
    for (std::size_t i = 0; i < register_count; ++i)
    {
        auto result = registers.include(registerName(i), register_width);

        if (!result)
            throw std::runtime_error("Failed to populate RegisterFile");
    }
}

// ============================================================
// Lookup by Name
// ============================================================

static void BM_LocateByName(benchmark::State &state)
{
    RegisterFile registers;
    populate(registers);

    const auto name = registerName(
        static_cast<std::size_t>(state.range(0)));

    for (auto _ : state)
    {
        auto result = registers.locate(name);
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================
// Contains
// ============================================================

static void BM_Contains(benchmark::State &state)
{
    RegisterFile registers;
    populate(registers);

    const auto name = registerName(
        static_cast<std::size_t>(state.range(0)));

    for (auto _ : state)
    {
        bool result = registers.contains(name);
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================
// Include
// ============================================================

static void BM_Include(benchmark::State &state)
{
    RegisterFile registers;

    for (auto _ : state)
    {
        auto result = registers.include("TEMP", register_width);
        benchmark::DoNotOptimize(result);

        registers.remove("TEMP");
    }
}

// ============================================================
// Remove
// ============================================================

static void BM_Remove(benchmark::State &state)
{
    RegisterFile registers;

    auto included = registers.include("TEMP", register_width);

    if (!included)
        throw std::runtime_error("Failed to populate RegisterFile");

    for (auto _ : state)
    {
        auto result = registers.remove("TEMP");
        benchmark::DoNotOptimize(result);

        registers.include("TEMP", register_width);
    }
}

// ============================================================
// Include / Remove
// ============================================================

static void BM_IncludeRemove(benchmark::State &state)
{
    RegisterFile registers;

    for (auto _ : state)
    {
        auto included = registers.include("TEMP", register_width);
        benchmark::DoNotOptimize(included);

        auto removed = registers.remove("TEMP");
        benchmark::DoNotOptimize(removed);
    }
}

// ============================================================
// Average Name Lookup
// ============================================================

static void BM_LocateByNameAverage(benchmark::State &state)
{
    RegisterFile registers;
    populate(registers);

    std::array<std::string, register_count> names;

    for (std::size_t i = 0; i < register_count; ++i)
        names[i] = registerName(i);

    std::size_t index = 0;

    for (auto _ : state)
    {
        auto result = registers.locate(names[index++ % register_count]);
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================
// Benchmark Registration
// ============================================================

BENCHMARK(BM_LocateByName)
    ->Arg(0)
    ->Arg(64)
    ->Arg(127)
    ->Arg(128);

BENCHMARK(BM_Contains)
    ->Arg(0)
    ->Arg(64)
    ->Arg(127)
    ->Arg(128);

BENCHMARK(BM_Include);
BENCHMARK(BM_Remove);
BENCHMARK(BM_IncludeRemove);
BENCHMARK(BM_LocateByNameAverage);

BENCHMARK_MAIN();
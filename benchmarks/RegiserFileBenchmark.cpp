/*
    Cpu\benchmarks\RegisterFileBench.cpp
    Benchmarks for the RegisterFile class.
*/

#include <benchmark/benchmark.h>

#include <string>
#include <cstddef>
#include <stdexcept>

#include "cpu/RegisterFile.hpp"
#include "cpu/Types.hpp"

using cpu::storage::RegisterFile;
using cpu::types::byte_t;
using cpu::types::register_id_t;

// ============================================================
// Configuration
// ============================================================

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

    const auto name = registerName(state.range(0));

    for (auto _ : state)
    {
        auto result = registers.locate(name);
        benchmark::DoNotOptimize(result);
    }
}

// ============================================================
// Lookup by ID
// ============================================================

static void BM_LocateById(benchmark::State &state)
{
    RegisterFile registers;
    populate(registers);

    const auto id = static_cast<register_id_t>(state.range(0));

    for (auto _ : state)
    {
        auto result = registers.locate(id);
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

    const auto name = registerName(state.range(0));

    for (auto _ : state)
    {
        bool result = registers.contains(name);
        benchmark::DoNotOptimize(result);
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

static void BM_LocateByNameAverage(benchmark::State &state)
{
    RegisterFile registers;
    populate(registers);

    std::size_t index = 0;

    for (auto _ : state)
    {
        const auto name = registerName(index++ % register_count);

        auto result = registers.locate(name);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_LocateByNameAverage);

// ============================================================
// Benchmark Registration
// ============================================================

// First, middle, last, and missing entries.

BENCHMARK(BM_LocateByName)
    ->Arg(0)
    ->Arg(64)
    ->Arg(127)
    ->Arg(128);

BENCHMARK(BM_LocateById)
    ->Arg(0)
    ->Arg(64)
    ->Arg(127)
    ->Arg(128);

BENCHMARK(BM_Contains)
    ->Arg(0)
    ->Arg(64)
    ->Arg(127)
    ->Arg(128);

BENCHMARK(BM_IncludeRemove);

BENCHMARK_MAIN();
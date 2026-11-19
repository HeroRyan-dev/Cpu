
#include <benchmark/benchmark.h>

#include <cpu/Register.hpp>

#include <array>
#include <cstddef>
#include <string_view>

using Registers = cpu::RegisterFile;
using RegisterId = cpu::register_id_t;

// ============================================
// Direct Write
// ============================================

static void BM_DirectWrite(benchmark::State& state)
{
    Registers registers{};
    cpu::qword value = 0;

    for (auto _ : state)
    {
        registers.R0.set(value++);

        benchmark::DoNotOptimize(registers.R0.value);
    }
}
BENCHMARK(BM_DirectWrite);

// ============================================
// Compile-Time Resolved ID
// ============================================

static void BM_VisitByConstantId(benchmark::State& state)
{
    Registers registers{};
    cpu::qword value = 0;

    constexpr auto id = cpu::resolveRegister<Registers>("R0");
    static_assert(id.has_value());

    constexpr RegisterId registerId = *id;

    for (auto _ : state)
    {
        cpu::visitRegister(registers, registerId, [&](auto& reg)
        {
            reg.set(value++);
            benchmark::DoNotOptimize(reg.value);
        });
    }
}
BENCHMARK(BM_VisitByConstantId);

// ============================================
// Runtime ID
// ============================================

static void BM_VisitByRuntimeId(benchmark::State& state)
{
    Registers registers{};
    cpu::qword value = 0;

    RegisterId id = static_cast<RegisterId>(state.range(0));

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(id);

        cpu::visitRegister(registers, id, [&](auto& reg)
        {
            reg.set(value++);
            benchmark::DoNotOptimize(reg.value);
        });
    }
}
BENCHMARK(BM_VisitByRuntimeId)->Arg(0)->Arg(1)->Arg(2)->Arg(3);

// ============================================
// Direct Read
// ============================================

static void BM_DirectRead(benchmark::State& state)
{
    Registers registers{};
    registers.R0.set(42);

    for (auto _ : state)
    {
        auto result = registers.R0.get();
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_DirectRead);

// ============================================
// Compile-Time Resolved Read
// ============================================

static void BM_VisitReadByConstantId(benchmark::State& state)
{
    Registers registers{};
    registers.R0.set(42);

    constexpr auto id = cpu::resolveRegister<Registers>("R0");
    static_assert(id.has_value());

    constexpr RegisterId registerId = *id;

    for (auto _ : state)
    {
        cpu::visitRegister(registers, registerId, [](auto& reg)
        {
            auto result = reg.get();
            benchmark::DoNotOptimize(result);
        });
    }
}
BENCHMARK(BM_VisitReadByConstantId);

// ============================================
// Runtime ID Read
// ============================================

static void BM_VisitReadByRuntimeId(benchmark::State& state)
{
    Registers registers{};
    registers.R0.set(42);

    RegisterId id = static_cast<RegisterId>(state.range(0));

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(id);

        cpu::visitRegister(registers, id, [](auto& reg)
        {
            auto result = reg.get();
            benchmark::DoNotOptimize(result);
        });
    }
}
BENCHMARK(BM_VisitReadByRuntimeId)->Arg(0)->Arg(1)->Arg(2)->Arg(3);

// ============================================
// Runtime Name Resolution
// ============================================

static void BM_ResolveRegisterRuntimeName(benchmark::State& state)
{
    constexpr std::array<std::string_view, 4> names =
    {
        "R0", "R1", "R2", "R3"
    };

    std::size_t index = 0;

    for (auto _ : state)
    {
        std::string_view name = names[index++ % names.size()];
        benchmark::DoNotOptimize(name);

        auto result = cpu::resolveRegister<Registers>(name);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ResolveRegisterRuntimeName);

// ============================================
// Runtime Name-Based Write
// ============================================

static void BM_VisitByRuntimeName(benchmark::State& state)
{
    Registers registers{};
    cpu::qword value = 0;

    constexpr std::array<std::string_view, 4> names =
    {
        "R0", "R1", "R2", "R3"
    };

    std::size_t index = 0;

    for (auto _ : state)
    {
        std::string_view name = names[index++ % names.size()];
        benchmark::DoNotOptimize(name);

        cpu::visitRegister(registers, name, [&](auto& reg)
        {
            reg.set(value++);
            benchmark::DoNotOptimize(reg.value);
        });
    }
}
BENCHMARK(BM_VisitByRuntimeName);
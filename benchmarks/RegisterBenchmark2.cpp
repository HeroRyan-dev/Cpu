
#include <benchmark/benchmark.h>

#include <cpu/FlatRegisterFile.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace
{

using value_t = cpu::qword;
using RegisterId = cpu::FlatRegisterId;
using RegisterFile = cpu::MaskedFlatRegisterFile;

constexpr std::size_t RegisterCount = RegisterFile::count;

using RawRegisterFile = std::array<value_t, RegisterCount>;
using RegisterPointers = std::array<value_t*, RegisterCount>;

// ============================================================
// Benchmark helpers
// ============================================================

static void initializeIds(volatile std::uint8_t* ids)
{
    for (std::size_t i = 0; i < RegisterCount; ++i)
        ids[i] = static_cast<std::uint8_t>(i);
}

static RegisterPointers makeRegisterPointers(RawRegisterFile& registers)
{
    RegisterPointers pointers{};

    for (std::size_t i = 0; i < RegisterCount; ++i)
        pointers[i] = &registers[i];

    return pointers;
}

// ============================================================
// 1. Raw qword array: runtime write + read
// ============================================================

static void BM_RawArrayRuntimeWriteRead(benchmark::State& state)
{
    RawRegisterFile registers{};

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    std::size_t index = 0;
    value_t value = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<std::size_t>(
            ids[index++ & 15]
        );

        registers[id] = value;

        auto result = registers[id];
        benchmark::DoNotOptimize(result);

        ++value;
    }
}

BENCHMARK(BM_RawArrayRuntimeWriteRead);

// ============================================================
// 2. Raw qword array: runtime write + read with masking
// ============================================================

static void BM_RawArrayMaskedRuntimeWriteRead(benchmark::State& state)
{
    RawRegisterFile registers{};

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    std::size_t index = 0;
    value_t value = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<RegisterId>(
            ids[index++ & 15]
        );

        const auto indexValue = static_cast<std::size_t>(id);

        registers[indexValue] =
            value & RegisterFile::mask(id);

        auto result = registers[indexValue];
        benchmark::DoNotOptimize(result);

        ++value;
    }
}

BENCHMARK(BM_RawArrayMaskedRuntimeWriteRead);

// ============================================================
// 3. MaskedFlatRegisterFile: runtime write + read
// ============================================================

static void BM_MaskedFlatRuntimeWriteRead(benchmark::State& state)
{
    RegisterFile registers{};

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    std::size_t index = 0;
    value_t value = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<RegisterId>(
            ids[index++ & 15]
        );

        registers.set(id, value);

        auto result = registers.get(id);
        benchmark::DoNotOptimize(result);

        ++value;
    }
}

BENCHMARK(BM_MaskedFlatRuntimeWriteRead);

// ============================================================
// 4. Pointer table: runtime write + read
// ============================================================

static void BM_PointerTableRuntimeWriteRead(benchmark::State& state)
{
    RawRegisterFile registers{};
    auto pointers = makeRegisterPointers(registers);

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    std::size_t index = 0;
    value_t value = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<std::size_t>(
            ids[index++ & 15]
        );

        *pointers[id] = value;

        auto result = *pointers[id];
        benchmark::DoNotOptimize(result);

        ++value;
    }
}

BENCHMARK(BM_PointerTableRuntimeWriteRead);

// ============================================================
// 5. Raw array: compile-time R0 write + read
// ============================================================

static void BM_RawArrayCompileTimeWriteRead(benchmark::State& state)
{
    RawRegisterFile registers{};

    value_t value = 0;

    for (auto _ : state)
    {
        registers[0] = value;

        auto result = registers[0];
        benchmark::DoNotOptimize(result);

        ++value;
    }
}

BENCHMARK(BM_RawArrayCompileTimeWriteRead);

// ============================================================
// 6. MaskedFlatRegisterFile: compile-time R0 write + read
// ============================================================

static void BM_MaskedFlatCompileTimeWriteRead(benchmark::State& state)
{
    RegisterFile registers{};

    value_t value = 0;

    for (auto _ : state)
    {
        registers.set<RegisterId::R0>(value);

        auto result = registers.get<RegisterId::R0>();
        benchmark::DoNotOptimize(result);

        ++value;
    }
}

BENCHMARK(BM_MaskedFlatCompileTimeWriteRead);

// ============================================================
// 7. Raw array: runtime read
// ============================================================

static void BM_RawArrayRuntimeRead(benchmark::State& state)
{
    RawRegisterFile registers{};

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    value_t value = 1;

    for (std::size_t i = 0; i < RegisterCount; ++i)
        registers[i] = value++;

    benchmark::ClobberMemory();

    std::size_t index = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<std::size_t>(
            ids[index++ & 15]
        );

        auto result = registers[id];
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_RawArrayRuntimeRead);

// ============================================================
// 8. MaskedFlatRegisterFile: runtime read
// ============================================================

static void BM_MaskedFlatRuntimeRead(benchmark::State& state)
{
    RegisterFile registers{};

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    for (std::size_t i = 0; i < RegisterCount; ++i)
    {
        registers.set(
            static_cast<RegisterId>(i),
            static_cast<value_t>(i + 1)
        );
    }

    benchmark::ClobberMemory();

    std::size_t index = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<RegisterId>(
            ids[index++ & 15]
        );

        auto result = registers.get(id);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_MaskedFlatRuntimeRead);

// ============================================================
// 9. MaskedFlatRegisterFile: compile-time read
// ============================================================

static void BM_MaskedFlatCompileTimeRead(benchmark::State& state)
{
    RegisterFile registers{};

    registers.set<RegisterId::R0>(12345);

    benchmark::ClobberMemory();

    for (auto _ : state)
    {
        auto result = registers.get<RegisterId::R0>();
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_MaskedFlatCompileTimeRead);

// ============================================================
// 10. Register name resolution
// ============================================================

static void BM_FlatResolveRegisterName(benchmark::State& state)
{
    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    std::size_t index = 0;

    for (auto _ : state)
    {
        const auto id = ids[index++ & 15];

        auto name = cpu::RegisterNames[id];

        auto result = cpu::resolveRegister(name);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_FlatResolveRegisterName);

// ============================================================
// 11. Register name resolution + write + read
// ============================================================

static void BM_FlatRegisterNameWriteRead(benchmark::State& state)
{
    RegisterFile registers{};

    volatile std::uint8_t ids[RegisterCount];
    initializeIds(ids);

    std::size_t index = 0;
    value_t value = 0;

    for (auto _ : state)
    {
        const auto id = ids[index++ & 15];

        auto name = cpu::RegisterNames[id];

        auto resolved = cpu::resolveRegister(name);

        if (resolved)
        {
            registers.set(*resolved, value);

            auto result = registers.get(*resolved);
            benchmark::DoNotOptimize(result);
        }

        ++value;
    }
}

BENCHMARK(BM_FlatRegisterNameWriteRead);

} // namespace
#include <array>
#include <cstdint>
#include <memory>
#include <string_view>

#include <benchmark/benchmark.h>

#include "cpu/registers/Register.hpp"
#include "cpu/registers/RegisterFile.hpp"

namespace
{
    constexpr std::size_t RegisterTotal = cpu::registers::RegisterCount;

    constexpr std::array<std::string_view, RegisterTotal> RegisterNames =
    {
        "R0",  "R1",  "R2",  "R3",
        "R4",  "R5",  "R6",  "R7",
        "R8",  "R9",  "R10", "R11",
        "R12", "R13", "R14", "R15"
    };

    // ---------------------------------------------------------
    // Individual Register
    // ---------------------------------------------------------

    static void BM_Register_Read(benchmark::State& state)
    {
        cpu::registers::Register reg;
        reg = 12345;

        for (auto _ : state)
        {
            benchmark::DoNotOptimize(static_cast<cpu::qword>(reg));
        }
    }

    static void BM_Register_Write(benchmark::State& state)
    {
        cpu::registers::Register reg;
        cpu::qword value = 0;

        benchmark::DoNotOptimize(&reg);

        for (auto _ : state)
        {
            reg = value++;
            benchmark::ClobberMemory();
        }
    }

    // ---------------------------------------------------------
    // RegisterFile: Enum Access
    // ---------------------------------------------------------

    static void BM_RegisterFile_EnumRead(benchmark::State& state)
    {
        auto registerFile = std::make_unique<cpu::registers::RegisterFile>();

        for (std::size_t i = 0; i < RegisterTotal; ++i)
        {
            (*registerFile)[static_cast<cpu::registers::RegisterId>(i)] = i + 1;
        }

        benchmark::DoNotOptimize(registerFile.get());

        std::size_t index = 0;

        for (auto _ : state)
        {
            cpu::qword value =
                (*registerFile)[static_cast<cpu::registers::RegisterId>(index)];

            benchmark::DoNotOptimize(value);

            index = (index + 1) % RegisterTotal;
        }
    }

    static void BM_RegisterFile_EnumWrite(benchmark::State& state)
    {
        auto registerFile = std::make_unique<cpu::registers::RegisterFile>();

        benchmark::DoNotOptimize(registerFile.get());

        cpu::qword value = 0;
        std::size_t index = 0;

        for (auto _ : state)
        {
            (*registerFile)[static_cast<cpu::registers::RegisterId>(index)] = value++;

            benchmark::ClobberMemory();

            index = (index + 1) % RegisterTotal;
        }
    }

    // ---------------------------------------------------------
    // RegisterFile: String Access
    // ---------------------------------------------------------

    static void BM_RegisterFile_StringRead(benchmark::State& state)
    {
        auto registerFile = std::make_unique<cpu::registers::RegisterFile>();

        for (std::size_t i = 0; i < RegisterTotal; ++i)
        {
            (*registerFile)[static_cast<cpu::registers::RegisterId>(i)] = i + 1;
        }

        benchmark::DoNotOptimize(registerFile.get());

        std::size_t index = 0;

        for (auto _ : state)
        {
            cpu::qword value = (*registerFile)[RegisterNames[index]];

            benchmark::DoNotOptimize(value);

            index = (index + 1) % RegisterTotal;
        }
    }

    static void BM_RegisterFile_StringWrite(benchmark::State& state)
    {
        auto registerFile = std::make_unique<cpu::registers::RegisterFile>();

        benchmark::DoNotOptimize(registerFile.get());

        cpu::qword value = 0;
        std::size_t index = 0;

        for (auto _ : state)
        {
            (*registerFile)[RegisterNames[index]] = value++;

            benchmark::ClobberMemory();

            index = (index + 1) % RegisterTotal;
        }
    }

    // ---------------------------------------------------------
    // RegisterFile: Arithmetic
    // ---------------------------------------------------------

    static void BM_RegisterFile_EnumArithmetic(benchmark::State& state)
    {
        auto registerFile = std::make_unique<cpu::registers::RegisterFile>();

        (*registerFile)[cpu::registers::R1] = 123;
        (*registerFile)[cpu::registers::R2] = 456;

        benchmark::DoNotOptimize(registerFile.get());

        for (auto _ : state)
        {
            cpu::qword result =
                (*registerFile)[cpu::registers::R1] + (*registerFile)[cpu::registers::R2];

            benchmark::DoNotOptimize(result);
        }
    }

    static void BM_RegisterFile_StringArithmetic(benchmark::State& state)
    {
        auto registerFile = std::make_unique<cpu::registers::RegisterFile>();

        (*registerFile)["R1"] = 123;
        (*registerFile)["R2"] = 456;

        benchmark::DoNotOptimize(registerFile.get());

        for (auto _ : state)
        {
            cpu::qword result =
                (*registerFile)["R1"] + (*registerFile)["R2"];

            benchmark::DoNotOptimize(result);
        }
    }

    // ---------------------------------------------------------
    // Raw Array Baseline
    // Same 32-byte storage footprint as Register:
    // 128-bit mask + 128-bit value.
    // ---------------------------------------------------------

    struct RawRegister
    {
        cpu::qword mask = ~cpu::qword{0};
        cpu::qword value = 0;
    };

    static void BM_RawArray_Read(benchmark::State& state)
    {
        std::array<RawRegister, RegisterTotal> registers{};

        for (std::size_t i = 0; i < RegisterTotal; ++i)
        {
            registers[i].value = i + 1;
        }

        benchmark::DoNotOptimize(registers.data());

        std::size_t index = 0;

        for (auto _ : state)
        {
            benchmark::DoNotOptimize(registers[index].value);

            index = (index + 1) % RegisterTotal;
        }
    }

    static void BM_RawArray_Write(benchmark::State& state)
    {
        std::array<RawRegister, RegisterTotal> registers{};

        benchmark::DoNotOptimize(registers.data());

        cpu::qword value = 0;
        std::size_t index = 0;

        for (auto _ : state)
        {
            registers[index].value = value++;

            benchmark::ClobberMemory();

            index = (index + 1) % RegisterTotal;
        }
    }

    // ---------------------------------------------------------
    // Benchmark Registration
    // ---------------------------------------------------------

    BENCHMARK(BM_Register_Read);
    BENCHMARK(BM_Register_Write);

    BENCHMARK(BM_RegisterFile_EnumRead);
    BENCHMARK(BM_RegisterFile_EnumWrite);

    BENCHMARK(BM_RegisterFile_StringRead);
    BENCHMARK(BM_RegisterFile_StringWrite);

    BENCHMARK(BM_RegisterFile_EnumArithmetic);
    BENCHMARK(BM_RegisterFile_StringArithmetic);

    BENCHMARK(BM_RawArray_Read);
    BENCHMARK(BM_RawArray_Write);
}

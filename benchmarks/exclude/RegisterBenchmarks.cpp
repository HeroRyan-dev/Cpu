#include <benchmark/benchmark.h>

#include <cpu/Register.hpp>

#include <array>
#include <cstddef>
#include <string_view>

using Registers = cpu::RegisterFile;
using RegisterId = cpu::register_id_t;

// ============================================
// Benchmark Helpers
// ============================================

using FlatRegisterFile = std::array<cpu::qword, 16>;

static constexpr std::array<cpu::qword, 16> RegisterMasks =
{
    0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF
};

template<typename Register>
static cpu::qword volatileRead(const Register& reg) noexcept
{
    return *reinterpret_cast<const volatile cpu::qword*>(&reg.value);
}

using RegisterPointers = std::array<cpu::qword*, 16>;

static RegisterPointers makeRegisterPointers(Registers& registers)
{
    return
    {
        &registers.R0.value,
        &registers.R1.value,
        &registers.R2.value,
        &registers.R3.value,
        &registers.R4.value,
        &registers.R5.value,
        &registers.R6.value,
        &registers.R7.value,
        &registers.R8.value,
        &registers.R9.value,
        &registers.R10.value,
        &registers.R11.value,
        &registers.R12.value,
        &registers.R13.value,
        &registers.R14.value,
        &registers.R15.value
    };
}

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
// Compile-Time Resolved ID Write
// ============================================

static void BM_VisitByConstantId(benchmark::State& state)
{
    Registers registers{};
    cpu::qword value = 0;

    const auto id = cpu::resolveRegister<Registers>("R0");
    if (!id)
        return;

    const RegisterId registerId = *id;

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
// Runtime ID Write
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

    registers.R0.set(static_cast<cpu::qword>(state.range(0)));

    cpu::qword result = 0;

    for (auto _ : state)
    {
        result = volatileRead(registers.R0);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_DirectRead)->Arg(42);


// ============================================
// Compile-Time Resolved ID Read
// ============================================

static void BM_VisitReadByConstantId(benchmark::State& state)
{
    Registers registers{};

    registers.R0.set(static_cast<cpu::qword>(state.range(0)));

    constexpr RegisterId registerId = static_cast<RegisterId>(0);

    cpu::qword result = 0;

    for (auto _ : state)
    {
        cpu::visitRegister(registers, registerId, [&](auto& reg)
        {
            result = volatileRead(reg);
        });

        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_VisitReadByConstantId)->Arg(42);


// ============================================
// Runtime ID Read
// ============================================

static void BM_VisitReadByRuntimeId(benchmark::State& state)
{
    Registers registers{};

    const cpu::qword seed =
        static_cast<cpu::qword>(state.range(0));

    for (RegisterId i = 0; i < 16; ++i)
    {
        cpu::visitRegister(registers, i, [&](auto& reg)
        {
            reg.set(seed + static_cast<cpu::qword>(i) + 1);
        });
    }

    RegisterId id = static_cast<RegisterId>(state.range(1));
    cpu::qword result = 0;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(id);

        cpu::visitRegister(registers, id, [&](auto& reg)
        {
            result = volatileRead(reg);
        });

        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_VisitReadByRuntimeId)
    ->Args({ 42, 0 })
    ->Args({ 42, 1 })
    ->Args({ 42, 2 })
    ->Args({ 42, 3 });


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

static void BM_ArrayPointerWrite(benchmark::State& state)
{
    Registers registers{};
    auto pointers = makeRegisterPointers(registers);

    const RegisterId id = static_cast<RegisterId>(state.range(0));
    cpu::qword value = 0;

    for (auto _ : state)
    {
        *pointers[id] = value;

        benchmark::DoNotOptimize(*pointers[id]);

        value = (value + 1) & 0xFFFF;
    }
}
BENCHMARK(BM_ArrayPointerWrite)->Arg(0)->Arg(1)->Arg(2)->Arg(3);

static void BM_ArrayPointerRead(benchmark::State& state)
{
    Registers registers{};
    auto pointers = makeRegisterPointers(registers);

    for (RegisterId i = 0; i < 16; ++i)
    {
        *pointers[i] = 42 + i;
    }

    const RegisterId id = static_cast<RegisterId>(state.range(0));
    cpu::qword result = 0;

    for (auto _ : state)
    {
        result = *reinterpret_cast<const volatile cpu::qword*>(pointers[id]);

        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ArrayPointerRead)->Arg(0)->Arg(1)->Arg(2)->Arg(3);

// ============================================
// Dynamic Runtime ID Write: Visitor
// ============================================

static void BM_DynamicVisitWrite(benchmark::State& state)
{
    Registers registers{};

    volatile RegisterId ids[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15
    };

    std::size_t index = 0;
    cpu::qword value = 0;

    for (auto _ : state)
    {
        const RegisterId id = ids[index++ & 15];

        cpu::visitRegister(registers, id, [&](auto& reg)
        {
            reg.value = value;
            benchmark::DoNotOptimize(reg.value);
        });

        ++value;
    }
}
BENCHMARK(BM_DynamicVisitWrite);


// ============================================
// Dynamic Runtime ID Write: Pointer Table
// ============================================

// ============================================
// Dynamic Indexed Array Write
// ============================================

static void BM_DynamicArrayWrite(benchmark::State& state)
{
    std::array<cpu::qword, 16> registers{};

    volatile RegisterId ids[] =
{
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13, 14, 15
};

    std::size_t index = 0;
    cpu::qword value = 0;

    for (auto _ : state)
    {
        const RegisterId id = ids[index++ & 15];

        registers[id] = value;
        benchmark::DoNotOptimize(registers[id]);

        ++value;
    }
}
BENCHMARK(BM_DynamicArrayWrite);

static void BM_DynamicPointerWrite(benchmark::State& state)
{
    Registers registers{};
    auto pointers = makeRegisterPointers(registers);

    volatile RegisterId ids[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15
    };

    std::size_t index = 0;
    cpu::qword value = 0;

    for (auto _ : state)
    {
        const RegisterId id = ids[index++ & 15];

        *pointers[id] = value;
        benchmark::DoNotOptimize(*pointers[id]);

        ++value;
    }
}
BENCHMARK(BM_DynamicPointerWrite);

static void BM_DynamicMaskedArrayWrite(benchmark::State& state)
{
    FlatRegisterFile registers{};

    volatile RegisterId ids[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15
    };

    std::size_t index = 0;
    cpu::qword value = 0;

    for (auto _ : state)
    {
        const RegisterId id = ids[index++ & 15];

        registers[id] = value & RegisterMasks[id];

        benchmark::DoNotOptimize(registers[id]);

        ++value;
    }
}
BENCHMARK(BM_DynamicMaskedArrayWrite);
#include <optional>
#include <cstdint>

// ============================================
// Flat Register File
// ============================================

enum class FlatRegisterId : std::uint8_t
{
    R0, R1, R2, R3,
    R4, R5, R6, R7,
    R8, R9, R10, R11,
    R12, R13, R14, R15,
    Count
};

static constexpr std::size_t FlatRegisterCount =
    static_cast<std::size_t>(FlatRegisterId::Count);

static constexpr std::array<std::string_view, FlatRegisterCount> FlatRegisterNames =
{
    "R0", "R1", "R2", "R3",
    "R4", "R5", "R6", "R7",
    "R8", "R9", "R10", "R11",
    "R12", "R13", "R14", "R15"
};

static constexpr std::array<cpu::qword, FlatRegisterCount> FlatRegisterMasks =
{
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF,
    0xFFFF,     0xFFFF,     0xFFFF,     0xFFFF
};

class FlatRegisterFile
{
public:
    using value_t = cpu::qword;

private:
    std::array<value_t, FlatRegisterCount> registers{};

public:
    void set(FlatRegisterId id, value_t value) noexcept
    {
        const auto index = static_cast<std::size_t>(id);
        registers[index] = value & FlatRegisterMasks[index];
    }

    [[nodiscard]] value_t get(FlatRegisterId id) const noexcept
    {
        return registers[static_cast<std::size_t>(id)];
    }
};

static constexpr std::optional<FlatRegisterId>
resolveFlatRegister(std::string_view name) noexcept
{
    for (std::size_t i = 0; i < FlatRegisterNames.size(); ++i)
    {
        if (FlatRegisterNames[i] == name)
            return static_cast<FlatRegisterId>(i);
    }

    return std::nullopt;
}
// ============================================
// Flat Register: Runtime Name Resolution
// ============================================

static void BM_FlatResolveRegisterName(benchmark::State& state)
{
    constexpr std::array<std::string_view, 16> names = FlatRegisterNames;

    std::size_t index = 0;

    for (auto _ : state)
    {
        const std::string_view name = names[index++ & 15];

        benchmark::DoNotOptimize(name);

        const auto id = resolveFlatRegister(name);

        benchmark::DoNotOptimize(id);
    }
}
BENCHMARK(BM_FlatResolveRegisterName);
// ============================================
// Flat Register: Runtime Name-Based Write
// ============================================

static void BM_FlatRegisterNameWrite(benchmark::State& state)
{
    FlatRegisterFile registers{};

    constexpr std::array<std::string_view, 16> names = FlatRegisterNames;

    std::size_t index = 0;
    cpu::qword value = 0;

    for (auto _ : state)
    {
        const std::string_view name = names[index++ & 15];

        benchmark::DoNotOptimize(name);

        const auto id = resolveFlatRegister(name);

        if (id)
        {
            registers.set(*id, value);
            benchmark::DoNotOptimize(registers.get(*id));
        }

        ++value;
    }
}
BENCHMARK(BM_FlatRegisterNameWrite);

// ============================================
// Flat Register: Runtime ID Write
// ============================================

static void BM_FlatRegisterIdWrite(benchmark::State& state)
{
    FlatRegisterFile registers{};

    volatile std::uint8_t ids[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15
    };

    std::size_t index = 0;
    cpu::qword value = 0;

    for (auto _ : state)
    {
        const auto id = static_cast<FlatRegisterId>(ids[index++ & 15]);

        registers.set(id, value);
        benchmark::DoNotOptimize(registers.get(id));

        ++value;
    }
}
BENCHMARK(BM_FlatRegisterIdWrite);

// Has bugs.
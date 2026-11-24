
#include "cpu/Register.hpp"

#include <benchmark/benchmark.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int/literals.hpp>

#include <cstdint>

namespace
{
    using namespace boost::multiprecision::literals;

    // ---------------------------------------------------------
    // Test values
    // ---------------------------------------------------------

    template <unsigned Bits>
    constexpr auto testValue()
    {
        if constexpr (Bits == 128)
        {
            return 340282366920938463463374607431768211455_cppui128;
        }
        else
        {
            using value_type = typename cpu::Register<Bits>::value_type;
            return ~value_type{0};
        }
    }

    // ---------------------------------------------------------
    // Register benchmarks
    // ---------------------------------------------------------

    template <unsigned Bits>
    static void BM_Register_Write(benchmark::State& state)
    {
        using register_type = cpu::Register<Bits>;

        register_type reg;
        const auto value = testValue<Bits>();

        for (auto _ : state)
        {
            reg.write(value);
            benchmark::DoNotOptimize(reg);
        }
    }

    template <unsigned Bits>
    static void BM_Register_Read(benchmark::State& state)
    {
        using register_type = cpu::Register<Bits>;

        register_type reg;
        reg.write(testValue<Bits>());

        for (auto _ : state)
        {
            auto&& value = reg.read();
            benchmark::DoNotOptimize(value);
        }
    }

    template <unsigned Bits>
    static void BM_Register_ReadWrite(benchmark::State& state)
    {
        using register_type = cpu::Register<Bits>;

        register_type reg;
        const auto value = testValue<Bits>();

        for (auto _ : state)
        {
            reg.write(value);

            auto&& result = reg.read();
            benchmark::DoNotOptimize(result);
        }
    }

    // ---------------------------------------------------------
    // Register size benchmarks
    // ---------------------------------------------------------

    template <unsigned Bits>
    static void BM_Register_Size(benchmark::State& state)
    {
        using register_type = cpu::Register<Bits>;

        for (auto _ : state)
        {
            benchmark::DoNotOptimize(sizeof(register_type));
        }
    }

    // ---------------------------------------------------------
    // Native integer benchmarks
    // ---------------------------------------------------------

    template <typename T>
    static void BM_Native_Write(benchmark::State& state)
    {
        T value{};
        const T testValue = ~T{0};

        for (auto _ : state)
        {
            value = testValue;
            benchmark::DoNotOptimize(value);
        }
    }

    template <typename T>
    static void BM_Native_Read(benchmark::State& state)
    {
        T value = ~T{0};

        for (auto _ : state)
        {
            auto result = value;
            benchmark::DoNotOptimize(result);
        }
    }

    template <typename T>
    static void BM_Native_ReadWrite(benchmark::State& state)
    {
        T value{};
        const T testValue = ~T{0};

        for (auto _ : state)
        {
            value = testValue;

            auto result = value;
            benchmark::DoNotOptimize(result);
        }
    }

    // ---------------------------------------------------------
    // Register registrations
    // ---------------------------------------------------------

#define REGISTER_BENCHMARKS(Bits)                   \
    BENCHMARK_TEMPLATE(BM_Register_Write, Bits);     \
    BENCHMARK_TEMPLATE(BM_Register_Read, Bits);      \
    BENCHMARK_TEMPLATE(BM_Register_ReadWrite, Bits); \
    BENCHMARK_TEMPLATE(BM_Register_Size, Bits)

    REGISTER_BENCHMARKS(8);
    REGISTER_BENCHMARKS(16);
    REGISTER_BENCHMARKS(32);
    REGISTER_BENCHMARKS(64);
    REGISTER_BENCHMARKS(128);

#undef REGISTER_BENCHMARKS

    // ---------------------------------------------------------
    // Native registrations
    // ---------------------------------------------------------

    using uint128_t = boost::multiprecision::uint128_t;

#define NATIVE_BENCHMARKS(Type)                      \
    BENCHMARK_TEMPLATE(BM_Native_Write, Type);       \
    BENCHMARK_TEMPLATE(BM_Native_Read, Type);        \
    BENCHMARK_TEMPLATE(BM_Native_ReadWrite, Type)

    NATIVE_BENCHMARKS(std::uint8_t);
    NATIVE_BENCHMARKS(std::uint16_t);
    NATIVE_BENCHMARKS(std::uint32_t);
    NATIVE_BENCHMARKS(std::uint64_t);
    NATIVE_BENCHMARKS(uint128_t);

#undef NATIVE_BENCHMARKS
}

BENCHMARK_MAIN();
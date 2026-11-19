#include <benchmark/benchmark.h>

static void BM_Addition(benchmark::State& state)
{
    int a = 10;
    int b = 20;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);

        int result = a + b;

        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_Addition);
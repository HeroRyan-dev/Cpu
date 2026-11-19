#include <benchmark/benchmark.h>

static void BM_Addition(benchmark::State& state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(2 + 2);
    }
}

BENCHMARK(BM_Addition);
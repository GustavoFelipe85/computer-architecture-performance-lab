# Cache Hierarchy Analysis

## Overview

This report documents an experimental analysis of data-cache behavior using Valgrind/Callgrind. The experiments evaluated Bubble Sort and Quick Sort under different compiler optimization levels and simulated L1 data-cache configurations.

The analysis focuses on the relationship between:

- compiler optimization;
- data-memory references;
- L1 data-cache misses;
- cache capacity;
- algorithmic memory-access behavior.

The results were obtained from the definitive Callgrind collection stored in:

`coleta_notebook_definitiva/`

and consolidated in:

`resultados_notebook_definitivos.csv`

## 1. Experimental Methodology

### Workload

The experiments evaluated sorting algorithms operating on a vector containing 50,000 integers, corresponding to approximately 200 KB of data.

The following algorithms were evaluated:

- Bubble Sort
- Quick Sort

Each algorithm was compiled using:

- `-O0`
- `-O3`

### Cache configurations

The simulated L1 data cache (D1) was configured as:

| L1D capacity | Cache line | Associativity |
|---:|---:|---:|
| 8 KB | 64 B | 4-way |
| 64 KB | 64 B | 8-way |

Callgrind was executed with cache simulation enabled.

### Metrics

The principal Callgrind metrics analyzed were:

- `Ir`: instruction references;
- `Dr`: data reads;
- `Dw`: data writes;
- `D1mr`: L1 data-cache read misses;
- `D1mw`: L1 data-cache write misses;
- `DLmr`: last-level data-cache read misses;
- `DLmw`: last-level data-cache write misses.

The L1D miss rate was calculated as:

`D1 miss rate = D1mr / Dr × 100`

Callgrind provides event counts and simulated cache behavior. It should not be interpreted as a direct measurement of processor wall-clock execution time or CPU cycles.

## 2. Experimental Results

| Algorithm | Flag | L1D | Ir | Dr | D1mr | D1 miss rate | DLmr |
|---|---|---:|---:|---:|---:|---:|---:|
| Bubble Sort | `-O0` | 8 KB | 38,389,331,493 | 15,857,793,466 | 64,750,306 | 0.408% | 803 |
| Bubble Sort | `-O0` | 64 KB | 39,725,964,591 | 16,419,723,314 | 67,807,548 | 0.413% | 805 |
| Bubble Sort | `-O3` | 8 KB | 12,507,181,247 | 1,250,905,650 | 78,000,759 | 6.236% | 811 |
| Bubble Sort | `-O3` | 64 KB | 12,498,799,367 | 1,250,905,650 | 68,727,281 | 5.494% | 811 |
| Quick Sort | `-O0` | 8 KB | 30,582,435 | 12,868,669 | 30,213 | 0.235% | 812 |
| Quick Sort | `-O0` | 64 KB | 31,159,785 | 13,148,039 | 14,851 | 0.113% | 812 |
| Quick Sort | `-O3` | 8 KB | 13,139,308 | 2,615,398 | 33,886 | 1.296% | 812 |
| Quick Sort | `-O3` | 64 KB | 12,632,830 | 2,512,383 | 12,007 | 0.478% | 814 |

## 3. Bubble Sort: Effect of Compiler Optimization

The most notable result appears in Bubble Sort.

With an 8 KB L1D cache, changing the compiler optimization level from `-O0` to `-O3` reduced:

- instruction references (`Ir`) from approximately 38.39 billion to 12.51 billion;
- data reads (`Dr`) from approximately 15.86 billion to 1.25 billion.

This corresponds to reductions of approximately:

- 67.4% in instruction references;
- 92.1% in data reads.

At the same time, the measured L1D miss rate increased from 0.408% to 6.236%.

This does not mean that `-O3` necessarily produced worse cache behavior overall. The percentage is a relative metric whose denominator (`Dr`) changed substantially.

Therefore, the increase in miss rate must be interpreted together with the absolute number of data references and cache misses.

The absolute number of L1D read misses increased from 64.75 million to 78.00 million in this particular configuration, while the total number of data reads fell by more than 92%.

This illustrates why cache miss rate should not be analyzed in isolation.

## 4. Effect of L1D Capacity

Increasing the simulated L1D capacity from 8 KB to 64 KB produced different effects depending on the algorithm and optimization level.

### Bubble Sort

With `-O3`:

- 8 KB: 78,000,759 D1 read misses;
- 64 KB: 68,727,281 D1 read misses.

The corresponding miss rate decreased from:

- 6.236%;
- to 5.494%.

This represents an approximately 11.9% reduction in miss rate.

### Quick Sort

With `-O3`:

- 8 KB: 33,886 D1 read misses;
- 64 KB: 12,007 D1 read misses.

The miss rate decreased from:

- 1.296%;
- to 0.478%.

This corresponds to an approximately 63.1% reduction in the L1D miss rate.

The stronger response of Quick Sort indicates that its memory-access behavior was more sensitive to the simulated L1D capacity under the tested workload.

The result should be interpreted specifically as an observation from this experimental configuration rather than as a universal property of Quick Sort.

## 5. Last-Level Cache Behavior

The `DLmr` values remained within a narrow range across all eight scenarios:

- minimum: 803;
- maximum: 814.

These values are several orders of magnitude smaller than the corresponding L1D read-miss counts.

The result indicates that most of the simulated data-cache misses observed at L1D did not propagate to a large number of last-level cache misses.

Because the workload contains approximately 200 KB of integer data, it is substantially smaller than the simulated 6 MB last-level cache used in the experimental configuration.

However, these results should be interpreted as Callgrind simulation results rather than as direct measurements of the physical processor's hardware performance counters.

## 6. Interpretation

The experiments demonstrate three important principles.

### 6.1 Miss rate is not sufficient by itself

A higher miss rate does not automatically imply worse overall performance.

The Bubble Sort `-O3` configuration demonstrates this clearly: the L1D miss rate increased while the compiler simultaneously reduced the total volume of instruction references and data reads substantially.

Therefore, meaningful cache analysis should consider at least:

- total memory references;
- absolute cache misses;
- miss rate;
- instruction volume;
- workload characteristics.

### 6.2 Compiler optimization changes memory behavior

Compiler optimization can substantially alter the generated instruction stream and memory-access pattern.

The transition from `-O0` to `-O3` reduced data reads by approximately 92.1% in the Bubble Sort 8 KB configuration.

This demonstrates that cache behavior cannot be attributed exclusively to the source-level algorithm. The generated machine code also plays a significant role.

### 6.3 Cache capacity interacts with algorithmic access patterns

The Quick Sort experiment showed a much stronger response to increasing L1D capacity than Bubble Sort under `-O3`.

This suggests that the relationship between algorithm and cache is not simply determined by the amount of data processed. The spatial and temporal locality of memory accesses also matters.

## 7. Experimental Limitations

Several limitations must be considered when interpreting these results.

1. Callgrind is a simulation/profiling tool and does not directly measure physical execution time or CPU cycles.

2. The simulated cache configuration does not necessarily reproduce every implementation detail of the physical processor cache hierarchy.

3. The experiments use a specific workload size of 50,000 integers. Different input sizes or data distributions may produce different cache behavior.

4. The results are specific to the tested implementations, compiler version, compilation flags, and experimental configuration.

5. The analysis focuses on cache-related event counts and does not independently measure hardware performance counters, branch misprediction, pipeline stalls, frequency scaling, or thermal effects.

6. Therefore, conclusions about physical execution performance should not be inferred solely from Callgrind event counts.

## 8. Reproducibility

The raw Callgrind output files are stored in:

`coleta_notebook_definitiva/`

The consolidated measurements are stored in:

`resultados_notebook_definitivos.csv`

The collection includes eight experimental scenarios:

1. Bubble Sort — `-O0` — 8 KB;
2. Bubble Sort — `-O0` — 64 KB;
3. Bubble Sort — `-O3` — 8 KB;
4. Bubble Sort — `-O3` — 64 KB;
5. Quick Sort — `-O0` — 8 KB;
6. Quick Sort — `-O0` — 64 KB;
7. Quick Sort — `-O3` — 8 KB;
8. Quick Sort — `-O3` — 64 KB.

The CSV was generated directly from the Callgrind output files by parsing the `cmd`, D1 cache description, and `summary` fields.

## 9. Engineering Takeaways

The experimental results support the following engineering practices:

1. **Do not interpret cache miss rate in isolation.** Always examine absolute references and miss counts.

2. **Compiler optimization must be considered in cache analysis.** Optimization can substantially change the number and pattern of memory references.

3. **Algorithmic locality matters.** Different algorithms can respond very differently to the same cache-capacity change.

4. **Use profiling to validate assumptions.** Source-code intuition alone is insufficient to characterize the generated memory-access behavior.

5. **Distinguish simulated metrics from physical measurements.** Callgrind event counts are useful for controlled analysis, but they should not be presented as direct measurements of CPU cycles or wall-clock execution time.

## Conclusion

The experiment demonstrates that software performance emerges from the interaction between algorithmic structure, compiler optimization, memory-access behavior, and cache hierarchy.

The Bubble Sort results show that a higher relative L1D miss rate can coexist with a substantial reduction in memory references after compiler optimization. The Quick Sort results demonstrate greater sensitivity to L1D capacity under the tested configuration.

These findings reinforce the importance of analyzing performance metrics in context rather than relying on a single indicator.

---

**Keywords:** Computer Architecture · Cache Hierarchy · L1D Cache · Callgrind · Valgrind · Performance Engineering · Compiler Optimization · Bubble Sort · Quick Sort · Memory Locality · Cache Simulation

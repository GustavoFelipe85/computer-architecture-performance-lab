# Cache Hierarchy Analysis

## Overview

This report documents a deterministic cache-profiling experiment using **Valgrind/Callgrind** to analyze the interaction between compiler optimization, algorithmic memory behavior, and the simulated L1 data-cache configuration.

The experiment evaluates **Bubble Sort** and **Quick Sort** under GCC `-O0` and `-O3`, with simulated L1 data caches of **8 KB** and **64 KB**.

The objective is not to measure wall-clock execution time, but to characterize deterministic instruction and memory-reference counts and simulated cache misses.

## 1. Experimental Environment

### Hardware Platform

The definitive cache collection was performed on the notebook platform:

- **CPU:** Intel Core i5-12450H
- **Microarchitecture:** Alder Lake
- **Configuration:** 4 Performance-cores + 4 Efficient-cores
- **Architecture:** x86-64

### Software and Profiling

- Linux
- GCC
- Valgrind 3.26.0
- Callgrind
- Cache simulation enabled with `--cache-sim=yes`

### Workload

A vector containing **50,000 integers**, approximately **200 KB** assuming 4-byte integers, was processed by Bubble Sort and Quick Sort.

## 2. Cache Simulation Configuration

| Configuration | Capacity | Line size | Associativity |
|---|---:|---:|---:|
| L1D-8KB | 8 KB | 64 B | 4-way |
| L1D-64KB | 64 KB | 64 B | 8-way |

The simulated instruction cache was maintained at **32 KB, 64-byte lines, 8-way associative**.

The simulated last-level cache in the definitive Callgrind files was **6 MB, 64-byte lines, 12-way associative**.

These parameters are taken from the `desc:` fields of the Callgrind output files.

## 3. Metrics

| Metric | Meaning |
|---|---|
| `Ir` | Instruction references |
| `Dr` | Data reads |
| `Dw` | Data writes |
| `I1mr` | L1 instruction-cache misses |
| `D1mr` | L1 data-cache read misses |
| `D1mw` | L1 data-cache write misses |
| `ILmr` | Last-level instruction-cache misses |
| `DLmr` | Last-level data-cache read misses |
| `DLmw` | Last-level data-cache write misses |

The L1 data-cache read miss rate is:

\[
\text{D1 miss rate} = \frac{D1mr}{Dr}\times100
\]

The last-level data-cache read miss rate is:

\[
\text{LLd miss rate} = \frac{DLmr}{Dr}\times100
\]

These rates must be interpreted together with their absolute event counts.

> **Methodological note:** Callgrind is used here as a deterministic instruction/cache profiler. The reported `Ir`, `Dr`, `D1mr`, and related counters are not wall-clock execution-time measurements.

## 4. Experimental Results

The following table reproduces the values generated from the definitive dataset `resultados_notebook_definitivos.csv`.

| Algorithm | Flag | L1D | `Ir` | `Dr` | `D1mr` | D1 miss rate | `DLmr` | LLd miss rate |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| Bubble Sort | `-O0` | 8 KB | 38,389,331,493 | 15,857,793,466 | 64,750,306 | **0.408%** | 803 | 0.0000051% |
| Bubble Sort | `-O0` | 64 KB | 39,725,964,591 | 16,419,723,314 | 67,807,548 | **0.413%** | 805 | 0.0000049% |
| Bubble Sort | `-O3` | 8 KB | 12,507,181,247 | 1,250,905,650 | 78,000,759 | **6.236%** | 811 | 0.0000648% |
| Bubble Sort | `-O3` | 64 KB | 12,498,799,367 | 1,250,905,650 | 68,727,281 | **5.494%** | 811 | 0.0000648% |
| Quick Sort | `-O0` | 8 KB | 30,582,435 | 12,868,669 | 30,213 | **0.235%** | 812 | 0.00631% |
| Quick Sort | `-O0` | 64 KB | 31,159,785 | 13,148,039 | 14,851 | **0.113%** | 812 | 0.00618% |
| Quick Sort | `-O3` | 8 KB | 13,139,308 | 2,615,398 | 33,886 | **1.296%** | 812 | 0.03105% |
| Quick Sort | `-O3` | 64 KB | 12,632,830 | 2,512,383 | 12,007 | **0.478%** | 814 | 0.03240% |

## 5. Analysis

### 5.1 Effect of Compiler Optimization on Bubble Sort

For Bubble Sort with an 8 KB L1D:

- `-O0`: `Dr` = **15.858 billion**, `D1mr` = **64.750 million**, D1 miss rate = **0.408%**
- `-O3`: `Dr` = **1.251 billion**, `D1mr` = **78.001 million**, D1 miss rate = **6.236%**

The transition to `-O3` reduces data-read references by approximately **92.1%**, while the absolute number of L1 data-read misses increases.

Therefore, the higher percentage miss rate under `-O3` does **not**, by itself, demonstrate worse cache behavior. The denominator changed substantially.

Compiler transformations can reduce memory traffic through mechanisms such as register allocation, loop transformations, and elimination of redundant memory operations. Consequently, fewer total data references remain in the denominator while a substantial number of misses remain.

The appropriate interpretation is:

> **The percentage miss rate increased, but the generated code performed dramatically fewer data reads.**

### 5.2 Effect of L1D Capacity on Bubble Sort

For Bubble Sort under `-O3`:

| L1D | D1 misses | D1 miss rate |
|---:|---:|---:|
| 8 KB | 78,000,759 | **6.236%** |
| 64 KB | 68,727,281 | **5.494%** |

Increasing L1D from 8 KB to 64 KB reduced the absolute D1 read misses by approximately **11.9%**. Because `Dr` remained identical, the miss-rate reduction is also approximately 11.9%.

### 5.3 Effect of L1D Capacity on Quick Sort

Under `-O3`:

| L1D | D1 misses | D1 miss rate |
|---:|---:|---:|
| 8 KB | 33,886 | **1.296%** |
| 64 KB | 12,007 | **0.478%** |

The absolute D1 read misses decrease by approximately **64.6%**, while the miss rate decreases by approximately **63.1%**.

This demonstrates substantial sensitivity of the selected Quick Sort workload to the simulated L1D configuration.

The result should not be interpreted as proof that the entire recursive working set fits into L1D at every stage. It demonstrates sensitivity to the simulated cache configuration.

### 5.4 Comparing Bubble Sort and Quick Sort

Under `-O3` and 8 KB L1D:

- Bubble Sort: **1.251 billion** data reads
- Quick Sort: **2.615 million** data reads

The difference is several orders of magnitude and illustrates the importance of algorithmic complexity in the generated computational workload.

## 6. Last-Level Cache Behavior

The `DLmr` values remain highly stable:

- Bubble Sort: **803–811**
- Quick Sort: **812–814**

Despite large differences in L1D miss counts, the number of last-level data-cache read misses changes only marginally.

For the selected approximately 200 KB data set and the 6 MB simulated LL cache, the working set is substantially smaller than the simulated last-level capacity.

The experiment therefore shows a strong separation between L1D behavior and last-level cache behavior: a high L1 miss count does not necessarily imply frequent misses in the last-level cache.

## 7. Key Findings

### Finding 1 — Miss rate alone is insufficient

The Bubble Sort `-O3` configuration demonstrates that a higher L1 miss rate can coexist with substantially fewer memory references.

Therefore, analysis should include:

- `Ir`
- `Dr`
- `Dw`
- absolute miss counts
- miss rates

rather than relying exclusively on a percentage.

### Finding 2 — Compiler optimization changes memory behavior

For Bubble Sort with an 8 KB L1D:

\[
15.858\times10^9 \rightarrow 1.251\times10^9
\]

This corresponds to approximately **92.1% fewer data-read references**.

### Finding 3 — Quick Sort is more sensitive to L1D capacity

Under `-O3`:

\[
1.296\% \rightarrow 0.478\%
\]

This corresponds to an approximately **63.1% reduction in D1 miss rate** when increasing the simulated L1D from 8 KB to 64 KB.

### Finding 4 — L1 misses rarely reach the simulated LLC

The last-level data miss counts remain close to 800 events across all configurations. Thus, L1D misses and last-level misses represent different phenomena and should not be conflated.

## 8. Methodological Limitations

### 8.1 Callgrind is not a wall-clock benchmark

Callgrind counters do not directly measure:

- execution time;
- CPU frequency;
- turbo behavior;
- operating-system scheduling;
- DRAM latency;
- hardware prefetcher behavior;
- branch-predictor performance.

Statements about execution speed should therefore be supported by an independent timing experiment.

### 8.2 Simulated cache parameters are experimental parameters

The 8 KB and 64 KB L1D configurations are simulated configurations and should not be presented as the physical L1D size of the Intel Core i5-12450H.

### 8.3 Cache simulation is an abstraction

Callgrind's cache model is useful for controlled comparative experiments, but it is not a cycle-accurate model of the complete Alder Lake memory subsystem.

### 8.4 Results are workload-dependent

The conclusions apply to the selected input size, algorithms, compiler, optimization flags, cache parameters, and program implementations.

## 9. Reproducibility

The definitive result files are stored in:

```text
coleta_notebook_definitiva/
```

The consolidated dataset is:

```text
resultados_notebook_definitivos.csv
```

The Callgrind files contain the cache configuration and raw event counters required to reproduce the calculations.

The D1 miss rate can be independently reproduced with:

```text
D1_miss_rate = D1mr / Dr × 100
```

Example for Quick Sort, `-O3`, 8 KB L1D:

```text
33,886 / 2,615,398 × 100 ≈ 1.296%
```

## 10. Engineering Conclusions

The experiment demonstrates that cache performance cannot be reduced to a single metric.

Three levels of analysis are required:

1. **Algorithmic level** — how much computational and memory work the algorithm generates.
2. **Compiler level** — how source code is transformed into machine instructions and memory operations.
3. **Microarchitectural level** — how those operations interact with cache capacity, associativity, and locality.

The results show that compiler optimization can reduce memory traffic by more than 90% while simultaneously increasing the reported L1 miss rate. Conversely, increasing L1D capacity can significantly reduce misses for Quick Sort without producing the same magnitude of change for Bubble Sort.

The central engineering lesson is:

> **Cache miss rates must be interpreted in the context of absolute references, generated code, working-set behavior, and algorithmic structure.**

## Repository Artifacts

```text
computer-architecture-performance-lab/
├── coleta_notebook_definitiva/
│   ├── callgrind_bubble_O0_8KB.out
│   ├── callgrind_bubble_O0_64KB.out
│   ├── callgrind_bubble_O3_8KB.out
│   ├── callgrind_bubble_O3_64KB.out
│   ├── callgrind_quick_O0_8KB.out
│   ├── callgrind_quick_O0_64KB.out
│   ├── callgrind_quick_O3_8KB.out
│   └── callgrind_quick_O3_64KB.out
├── resultados_notebook_definitivos.csv
└── reports/
    └── cache_hierarchy_analysis.md
```

## Tools

- GCC
- Valgrind
- Callgrind
- Linux
- Git
- GitHub

## Status

**Experimental analysis completed.**

The repository preserves the raw Callgrind outputs and the consolidated CSV dataset to support reproducibility and further analysis.

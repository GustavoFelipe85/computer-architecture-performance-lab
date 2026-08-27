# Computer Architecture & Performance Lab

[![LinkedIn Article](https://img.shields.io/badge/LinkedIn-Technical%20Article-0A66C2?style=flat-square&logo=linkedin)](https://www.linkedin.com/pulse/do-sil%C3%ADcio-ao-algoritmo-o-que-cache-compiladores-e-gustavo-ywaxf/)

> 📖 **LinkedIn Pulse Article:** [From Silicon to Algorithm – What cache, compilers, and parallelism reveal about performance](https://www.linkedin.com/pulse/do-sil%C3%ADcio-ao-algoritmo-o-que-cache-compiladores-e-gustavo-ywaxf/)

🇧🇷 [Leia esta documentação em Português](README.md)

Experimental laboratory focused on the study of **Computer Architecture, Microprocessors, Memory Hierarchy, Cache Memory, and Parallel Computing**, with emphasis on performance analysis, reproducible experimentation, and evaluation of computational strategies.

---

###  Objective

This repository gathers practical experiments developed in the context of advanced studies in **Computer Architecture and Parallel Programming**, investigating the relationship between:

* Processor microarchitecture;
* Algorithmic behavior and memory access patterns;
* Memory hierarchy and cache sensitivity;
* Compiler optimization flags;
* Shared-memory (OpenMP) and distributed-memory (MPI) parallelism;
* Real execution trade-offs (scalability, overhead, and oversubscription).

---

##  Research Areas

- **Computer Architecture**
- **Microprocessors**
- **Memory Hierarchy**
- **CPU Cache**
- **Performance Analysis**
- **Parallel Computing**
- **OpenMP**
- **MPI**
- **Profiling and Performance Analysis**
- **Code Optimization**

---

##  Experiments

### 01 — Cache Hierarchy and Algorithm Performance

Experimental analysis of **Bubble Sort** and **Quick Sort**, considering different compiler optimization levels and L1 cache configurations.

#### Experimental Variables

- Algorithms:
  - Bubble Sort
  - Quick Sort

- Compiler optimization:
  - `-O0`
  - `-O3`

- L1 cache configuration:
  - 8 KB
  - 64 KB

- Vector size:
  - 50,000 integers
  - approximately 200 KB

#### Analyzed Metrics

- Instruction References (`Ir`)
- Data References (`Dr`)
- L1 Data Cache Misses (`D1mr`)
- Last-Level Data Cache Misses (`DLmr`)
- L1 miss rate
- Estimated cycles

---

## 💻 Experimental Platforms

### Intel Core i5-3470

**Ivy Bridge** architecture, used as one of the experimental platforms for analyzing algorithm behavior and memory hierarchy.

### Intel Core i5-12450H

**Alder Lake** architecture, used as a second experimental platform to compare different processor generations.

> Results must be interpreted considering the architectural differences between the platforms and the characteristics of the execution environment.

---

## 🛠️ Tools

- GCC 15.2
- Valgrind 3.26
- Callgrind
- WSL2
- Ubuntu
- C/C++

---

## Research Questions

The experiments investigate questions such as:

1. How do different algorithms interact with the memory hierarchy?
2. How does L1 cache size influence algorithm behavior?
3. What is the effect of `-O3` optimization on executed instructions and cache behavior?
4. How do different memory access patterns affect cache miss rates?
5. Does increasing cache capacity provide equivalent benefits across different algorithms?
6. How do differences between processor generations affect observed performance?

---

##  Analytical Principle

> **"There is no free lunch."**

In Computer Architecture, improvements in one dimension of a system may introduce costs or effects in other dimensions.

An optimization may:

- significantly reduce the number of instructions;
- modify memory access patterns;
- change cache miss rates;
- increase or decrease execution cost.

Therefore, performance should not be evaluated using a single metric.

The purpose of this laboratory is to experimentally observe these relationships.

---

## 🔬 Parallel Computing

A second experimental line addresses **Parallel Computing**, using:

- OpenMP
- MPI

The experiments include:

- Laplace
- N-Body
- K-Means

Metrics include:

- execution time;
- speedup;
- efficiency;
- scalability;
- communication overhead;
- synchronization;
- behavior as the number of threads/processes increases.

> Cache experiments and parallel programming experiments are treated as distinct experimental lines, although both share the goal of understanding factors that influence computational performance.

---

## 📁 Repository Structure

```text
computer-architecture-performance-lab/
│
├── README.md
├── README_en.md
│
├── cache-hierarchy/
│   ├── bubble-sort/
│   ├── quick-sort/
│   ├── callgrind/
│   ├── scripts/
│   └── results/
│
├── parallel-programming/
│   ├── openmp/
│   │   ├── laplace/
│   │   ├── n-body/
│   │   └── k-means/
│   │
│   ├── mpi/
│   │   └── laplace/
│   │
│   └── results/
│
├── hardware-platforms/
│   ├── i5-3470.md
│   └── i5-12450H.md
│
├── reports/
├── presentations/
│
└── docs/
    ├── methodology.md
    ├── reproducibility.md
    └── limitations.md
````

---

## ♻️ Reproducibility

Whenever possible, each experiment will provide:

* source code;
* compiler version;
* compilation parameters;
* execution parameters;
* experimental configuration;
* collected metrics;
* results;
* analysis scripts;
* known limitations.

The documentation aims to allow the experiments to be reproduced and technically audited.

---

## ⚠️ Limitations

Results should be interpreted within the specific conditions of each experiment.

Known limitations may include:

* use of virtualized environments when applicable;
* differences between physical hardware and profiling models;
* number of experimental repetitions;
* dataset size;
* parameters used in simulations and estimations;
* architectural differences between evaluated processors.

Limitations will be documented alongside each experiment.

---

## 🎓 Academic Context

The experiments in this laboratory were developed from academic activities and studies involving:

**Computer Architecture**
**Parallel Programming**
**Algorithm Performance Analysis**

The repository has educational, experimental, and technical portfolio purposes.

---

## 👤 Author

**Gustavo F. Paluch**

Computer Engineer
Cybersecurity • IT Infrastructure • DevOps • IoT

* 🔗 [LinkedIn](https://www.linkedin.com/in/gustavofpaluch)
* 💻 [GitHub](https://github.com/GustavoFelipe85)
* 🧪 [ORCID](https://orcid.org/0009-0007-1926-5139)

---

## 📄 License

This project is licensed under the terms of the MIT License. See the [LICENSE](LICENSE) file for details.

```


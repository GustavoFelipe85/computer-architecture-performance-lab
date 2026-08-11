# Computer Architecture & Performance Lab

🇬🇧 [Read this documentation in English](README_en.md)

Laboratório experimental dedicado ao estudo de **Arquitetura de Computadores, Microprocessadores, Hierarquia de Memória, Memória Cache e Computação Paralela**, com foco em análise de desempenho, experimentação reprodutível e avaliação de diferentes estratégias computacionais.

---

## 🎯 Objetivo

Este repositório reúne experimentos práticos desenvolvidos no contexto de estudos em **Arquitetura de Computadores e Programação Paralela**, investigando a relação entre:

- arquitetura do processador;
- comportamento dos algoritmos;
- hierarquia de memória;
- memória cache;
- otimizações do compilador;
- execução sequencial e paralela;
- desempenho computacional.

A abordagem adotada é experimental, utilizando métricas de desempenho, ferramentas de profiling e documentação dos procedimentos para permitir a reprodução dos experimentos.

---

## 🔬 Áreas de Estudo

- **Arquitetura de Computadores**
- **Microprocessadores**
- **Hierarquia de Memória**
- **Memória Cache**
- **Análise de Desempenho**
- **Programação Paralela**
- **OpenMP**
- **MPI**
- **Profiling e Performance Analysis**
- **Otimização de Código**

---

## 🧪 Experimentos

### 01 — Hierarquia de Cache e Desempenho de Algoritmos

Análise experimental do comportamento de **Bubble Sort** e **Quick Sort**, considerando diferentes níveis de otimização do compilador e configurações de cache L1.

#### Variáveis experimentais

- Algoritmos:
  - Bubble Sort
  - Quick Sort

- Otimização do compilador:
  - `-O0`
  - `-O3`

- Configuração de cache L1:
  - 8 KB
  - 64 KB

- Tamanho do vetor:
  - 50.000 inteiros
  - aproximadamente 200 KB

#### Métricas analisadas

- Instruction References (`Ir`)
- Data References (`Dr`)
- L1 Data Cache Misses (`D1mr`)
- Last-Level Data Cache Misses (`DLmr`)
- Taxa de misses L1
- Ciclos estimados

---

## 💻 Plataformas Experimentais

### Intel Core i5-3470

Arquitetura **Ivy Bridge**, utilizada como uma das plataformas experimentais para análise do comportamento de algoritmos e hierarquia de memória.

### Intel Core i5-12450H

Arquitetura **Alder Lake**, utilizada como segunda plataforma experimental para comparação entre gerações de processadores.

> Os resultados devem ser interpretados considerando as diferenças arquiteturais entre as plataformas e as características do ambiente de execução.

---

## 🛠️ Ferramentas

- GCC 15.2
- Valgrind 3.26
- Callgrind
- WSL2
- Ubuntu
- C/C++

---

## 📊 Principais questões investigadas

Os experimentos procuram responder questões como:

1. Como diferentes algoritmos utilizam a hierarquia de memória?
2. Como o tamanho da cache L1 influencia o comportamento dos algoritmos?
3. Qual é o efeito da otimização `-O3` sobre instruções executadas e comportamento da cache?
4. Como diferentes padrões de acesso à memória afetam a taxa de cache misses?
5. O aumento da capacidade da cache produz benefícios equivalentes para diferentes algoritmos?
6. Como diferenças entre gerações de processadores influenciam o desempenho observado?

---

## 🧠 Princípio de análise

> **"Não existe almoço grátis."**

Em Arquitetura de Computadores, melhorias em uma determinada dimensão do sistema podem introduzir custos ou efeitos em outras dimensões.

Uma otimização pode:

- reduzir significativamente o número de instruções;
- modificar o padrão de acesso à memória;
- alterar a taxa de cache misses;
- aumentar ou reduzir o custo de execução.

Por isso, desempenho não deve ser analisado a partir de uma única métrica.

O objetivo deste laboratório é observar experimentalmente essas relações.

---

## 🔬 Programação Paralela

Uma segunda linha de experimentação aborda **Computação Paralela**, utilizando:

- OpenMP
- MPI

Os experimentos incluem:

- Laplace
- N-Body
- K-Means

São analisadas métricas como:

- tempo de execução;
- speedup;
- eficiência;
- escalabilidade;
- overhead de comunicação;
- sincronização;
- comportamento conforme o número de threads/processos.

> Os experimentos de cache e os experimentos de programação paralela são tratados como linhas experimentais distintas, embora compartilhem o objetivo de compreender fatores que influenciam o desempenho computacional.

---

## 📁 Organização

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

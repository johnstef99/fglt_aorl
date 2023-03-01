# Parallel & Distributed Computer Systems (Jan. 12, 2023) Exercise 3
## Deadline: Feb 26, 2023

Help us accelerate [FGLT](https://github.com/fcdimitr/fglt) with  CUDA.

`FGlT` is a C/C++ multi-threading library for Fast Graphlet Transform of large, sparse, undirected networks/graphs. The graphlets in dictionary Σ16, shown in Table 1, are used as encoding elements to capture topological connectivity quantitatively and transform a graph G=(V,E) into a |V| x 16 array of graphlet frequencies at all vertices. The 16-element vector at each vertex represents the frequencies of induced subgraphs, incident at the vertex, of the graphlet patterns. The transformed data array serves multiple types of network analysis: statistical or/and topological measures, comparison, classification, modeling, feature embedding, and dynamic variation, among others. The library FGlT is distinguished in the following key aspects. (1) It is based on the fast, sparse, and exact transform formulas, which are of the lowest time and space complexities among known algorithms, and, at the same time, in a ready form for globally streamlined computation in matrix-vector operations. (2) It leverages prevalent multi-core processors, with multi-threaded programming in Cilk, and uses sparse graph computation techniques to deliver high-performance network analysis to individual laptops or desktop computers. (3) It has Python, Julia, and MATLAB interfaces for easy integration with, and extension of, existing network analysis software.

More details in [the paper](https://arxiv.org/abs/2007.11111).

```
@inproceedings{floros2020,
  title = {Fast Graphlet Transform of Sparse Graphs},
  booktitle = {{{IEEE High Performance Extreme Computing Conference}}},
  author = {Floros, Dimitris and Pitsianis, Nikos and Sun, Xiaobai},
  year = {2020},
}
```
For this assignment, you will only implement the calculations of $σ_1$, $σ_2$, $σ_3$ and $σ_4$ from this table:

![](https://raw.githubusercontent.com/fcdimitr/fglt/master/figs/table-overview.png)

Input is a sparse matrix $A$.  

Use graphs from SuiteSparse Matrix Collection: i) `auto`, ii) `great-britain_osm` and iii) `delaunay_n22` for your experiments.

## What to submit

- A 4-page report in PDF format 
- Describe the choices you've made for parallelization and streaming
- Compare the CUDA execution times with the times from our multicore parallel code (restrict the computations to only $σ_1$, $σ_2$, $σ_3$ and $σ_4$ by erasing all unrelated codes). 
- Upload the source code on GitHub, BitBucket, Dropbox, Google Drive,
  etc. and add a link to your report.

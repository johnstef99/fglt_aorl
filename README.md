# FGlT and other random letters

[FGLT](https://github.com/fcdimitr/fglt) is a C/C++ multi-threading library for
Fast Graphlet Transform of large, sparse, undirected networks/graphs.

In this repo the frequencies of the 4 first graphlets are calculated using
CUDA.

Based on [this paper](assets/JOSS-FGLT-main.pdf) I need to calculate the
$d_0,d_1,d_2,d_3,d_4$ and I'm only going to use CUDA to calculate 
$d\_4 = c_3 = C_3 e /2$ where $C_3 = A \odot A^2$.

---
Assignment can be found [here](./assignment.md).

[Report from our team](./report.pdf).

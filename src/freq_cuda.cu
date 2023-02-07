extern "C" {
#include "freq_cuda.h"
}

__global__ void C3_kernel(size_t n, size_t *A_com, size_t *A_unc, size_t *c3) {
  size_t i = blockIdx.x * blockDim.x + threadIdx.x;

  if (i < n) {
    int j, k, l, lb, up, clb, cup, llb;
    lb = A_com[i];
    up = A_com[i + 1];
    for (j = lb; j < up; j++) {
      clb = A_com[A_unc[j]];
      cup = A_com[A_unc[j] + 1];
      llb = lb;
      for (k = clb; k < cup; k++) {
        for (l = llb; l < up; l++) {
          if (A_unc[k] == A_unc[l]) {
            c3[i]++;
            llb = l + 1;
            break;
          } else if (A_unc[k] < A_unc[l]) {
            llb = l;
            break;
          } else {
            llb = l + 1;
          }
        }
      }
    }
    c3[i] /= 2;
  }
}

extern "C" {
void cuda_c3(csx A, freq f) {
  // Allocate device memory
  size_t *A_com, *A_unc, *c3;
  cudaMalloc(&A_com, (A->v + 1) * sizeof(size_t));
  cudaMalloc(&A_unc, A->e * sizeof(size_t));
  cudaMalloc(&c3, A->v * sizeof(size_t));
  cudaMemset(c3, 0, A->v * sizeof(size_t));

  // Copy A to device memory
  cudaMemcpy(A_com, A->com, (A->v + 1) * sizeof(size_t),
             cudaMemcpyHostToDevice);
  cudaMemcpy(A_unc, A->unc, A->e * sizeof(size_t), cudaMemcpyHostToDevice);

  // Find necessary number of blocks and number of threads
  dim3 dimGrid(1, 1);
  dim3 dimBlock(A->v);
  if (A->v > 1024) {
    size_t grid = (A->v + 1024 - 1) / 1024;
    dimGrid.x = grid;
    dimBlock.x = 1024;
  }

  // Launch kernel
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  C3_kernel<<<dimGrid, dimBlock>>>(A->v, A_com, A_unc, c3);
  cudaDeviceSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &end);
  f->s4_ms = get_elapsed_ms(start, end);

  // Copy B back to host memory
  cudaMemcpy(f->s4, c3, A->v * sizeof(size_t), cudaMemcpyDeviceToHost);

  // Free device memory
  cudaFree(A_com);
  cudaFree(A_unc);
  cudaFree(c3);
}
}

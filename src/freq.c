#include "freq.h"
#include "freq_cuda.h"

void spmv(csx A, size_t *x, size_t *y);
void c3(csx A, size_t *c3);

freq freq_new(size_t number_of_vertices) {
  freq f = (freq)malloc(sizeof(struct FREQ));

  f->v = number_of_vertices;

  f->s0 = (size_t *)calloc(f->v, sizeof(size_t));
  f->s1 = (size_t *)calloc(f->v, sizeof(size_t));
  f->s2 = (size_t *)calloc(f->v, sizeof(size_t));
  f->s3 = (size_t *)calloc(f->v, sizeof(size_t));
  f->s4 = (size_t *)calloc(f->v, sizeof(size_t));

  return f;
}

void freq_free(freq f) {
  free(f->s0);
  free(f->s1);
  free(f->s2);
  free(f->s3);
  free(f);
}

freq freq_calc(csx A) {
  freq f = freq_new(A->v);

  struct timespec start, end;

  // calculate σ0
  clock_gettime(CLOCK_MONOTONIC, &start);

  cilk_for(size_t i = 0; i < A->v; i++) { f->s0[i] = 1; }

  clock_gettime(CLOCK_MONOTONIC, &end);
  f->s0_ms = get_elapsed_ms(start, end);

  // calculate σ1
  clock_gettime(CLOCK_MONOTONIC, &start);

  cilk_for(size_t i = 0; i < A->v; i++) {
    f->s1[i] = A->com[i + 1] - A->com[i];
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  f->s1_ms = get_elapsed_ms(start, end);

  // calculate σ2
  clock_gettime(CLOCK_MONOTONIC, &start);

  spmv(A, f->s1, f->s2);
  cilk_for(size_t i = 0; i < A->v; i++) { f->s2[i] -= f->s1[i]; }

  clock_gettime(CLOCK_MONOTONIC, &end);
  f->s2_ms = get_elapsed_ms(start, end);

  // calculate σ3
  clock_gettime(CLOCK_MONOTONIC, &start);

  cilk_for(size_t i = 0; i < A->v; i++) {
    f->s3[i] = (f->s1[i] * (f->s1[i] - 1)) / 2;
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  f->s3_ms = get_elapsed_ms(start, end);

  // calculate σ4
  cuda_c3(A, f);

  cilk_for(size_t i = 0; i < A->v; i++) {
    f->s2[i] -= 2 * f->s4[i];
    f->s3[i] -= f->s4[i];
  }

  return f;
}

void freq_print(freq f) {
  printf("  v   σ0  σ1  σ2  σ3  σ4\n");
  for (size_t v = 0; v < f->v; v++) {
    printf("%3zu: %3zu %3zu %3zu %3zu %3zu\n", v, f->s0[v], f->s1[v], f->s2[v],
           f->s3[v], f->s4[v]);
  }
};

/**
 * Caclucaltes y = Ax
 * \\
 * where A is a matrix in CSC/CSR format and x a dense vector
 */
void spmv(csx A, size_t *x, size_t *y) {
  /* initialize result vector */
  cilk_for(int i = 0; i < A->v; i++) { y[i] = 0; }

  /* perform matrix-vector multiplication */
  cilk_for(int j = 0; j < A->v; j++) {
    for (int k = A->com[j]; k < A->com[j + 1]; k++) {
      y[A->unc[k]] += x[j];
    }
  }
}

/**
 * Caclucaltes c3 = (A ○ A^2)e / 2
 * \\
 * where A is a matrix in CSC/CSR format and e a vector with all elements equal
 * 1
 */
void c3(csx A, size_t *c3) {
  int j, k, l, lb, up, clb, cup, llb;
  cilk_for(int i = 0; i < A->v; i++) {
    lb = A->com[i];
    up = A->com[i + 1];
    for (j = lb; j < up; j++) {
      clb = A->com[A->unc[j]];
      cup = A->com[A->unc[j] + 1];
      llb = lb;
      for (k = clb; k < cup; k++) {
        for (l = llb; l < up; l++) {
          if (A->unc[k] == A->unc[l]) {
            c3[i]++;
            llb = l + 1;
            break;
          } else if (A->unc[k] < A->unc[l]) {
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

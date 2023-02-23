#include <cilk/cilk_api.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "csx.h"
#include "freq.h"

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "Usage: %s [martix-filename] [results-file]\n", argv[0]);
    exit(1);
  }

  printf("CILK_NWORKERS: %d\n\n", __cilkrts_get_nworkers());

  struct timespec start, end;

  /* Read mtx file */
  printf("Reading mtx file.. ");
  fflush(stdout);
  clock_gettime(CLOCK_MONOTONIC, &start);
  csx csc = csc_from_file(argv[1]);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("%fms\n", get_elapsed_ms(start, end));
#ifdef DEBUG
  csx_print(csc);
#endif /* ifdef DEBUG */
  printf("Vertices: %zu\n"
         "Edges:    %zu\n"
         "\n",
         csc->v, csc->e);

  printf("Calculating σ0-4..\n");
  freq f = freq_calc(csc);

  printf("σ0: %fms\n", f->s0_ms);
  printf("σ1: %fms\n", f->s1_ms);
  printf("σ2: %fms\n", f->s2_ms);
  printf("σ3: %fms\n", f->s3_ms);
  printf("σ4: %fms\n", f->s4_ms);

  FILE *results = fopen(argv[2], "w");
  freq_print(f, results);
  fclose(results);

  freq_free(f);
  csx_free(csc);

  return 0;
}

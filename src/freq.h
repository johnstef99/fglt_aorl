#ifndef FREQ_H
#define FREQ_H

#include <stddef.h>
#include <stdlib.h>
#include <cilk/cilk.h>

#include "csx.h"
#include "common.h"

struct FREQ {
  /* num of vertices */
  size_t v;

  size_t *s0;
  size_t *s1;
  size_t *s2;
  size_t *s3;
  _Atomic size_t *s4;

  double s0_ms;
  double s1_ms;
  double s2_ms;
  double s3_ms;
  double s4_ms;
};

typedef struct FREQ *freq;

freq freq_new(size_t number_of_vertices);

void freq_free(freq f);

freq freq_calc(csx A);

void freq_print(freq f, FILE* file);

#endif // !FREQ_H

#include <stdio.h>
#include <stdlib.h>

#include "csx.h"
#include "freq.h"

int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
    exit(1);
  }

  /* Read mtx file */
  csx csc = csc_from_file(argv[1]);
  csx_print(csc);

  printf("\n");

  freq f = freq_calc(csc);
  freq_print(f);

  freq_free(f);
  csx_free(csc);

  return 0;
}

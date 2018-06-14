#include <stdio.h>
#include <stdlib.h>

#include "boltzoc.h"
#include "parse.h"

int main(int argc, char **argv)
{

  char *filename;

  // Process command line parameters.
  if (argc > 1) {
    filename = argv[1];
  } else {
    fprintf(stderr, "// Warning: No input file specified, defaulting to /dev/stdin\n");
    filename = "/dev/stdin";
  }

  // Parse grammar, save symbol names, and run oracle.
  char **symbols = NULL;
  comb_sys s = importGrammar(filename, &symbols);
  double z = sing_sys(s, 10e-12, 10e-5);

  // Output singularity
  printf("// === SINGULARITY ===\n");
  printf("#define %-20s %.20lf\n", "Z", z);
  for(int i=0; symbols[i] != NULL; i++) {
    printf("#define %-20s %.10lf\n", symbols[i], sys_val(s, i+1));
  }

  free_sys(s);
  return 0;
}

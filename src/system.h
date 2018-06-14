#ifndef _SYSTEM_H
# define _SYSTEM_H
# include "boltzoc.h"
# ifdef COMPILE

typedef double (*f_eval)(double *, double, int *);

void compile_sys(comb_sys);
# endif

struct comb_sys {
  int len;
  comb_eq *eqs;
  int *refs;
  double *y;
  comb_eq **jacob;
# ifdef COMPILE
  f_eval *f;
  f_eval **fj;
# endif
};

#endif

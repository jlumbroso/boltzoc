#include <stdio.h>
#include <stdlib.h>
#include "../src/combstruct.h"

int main(int argc, char **argv)
{
  comb_sys s;
  double *y;
  double z = 0.5;
  int err = 0;

  s = empty_sys();
  // Y = 1/(1 - Z) + 1/(1 - 4*Z)
  add_sys(s, sum(seq(atom()), seq(prod(sum(sum(epsilon(),epsilon()),sum(epsilon(),epsilon())), atom()))));
  y = malloc(2*sys_len(s)*sizeof(double));
  z = sing_sys(s, 10e-12, 10e-5, y);
  printf("z:%.20lf y:%.20lf\n", z, y[0]);
  eval_point_sys(s, 0.5, 10e-5, y, &err);
  printf("z:0.5 y:%.20lf y':%.20lf err:%d\n", y[0], y[1], err);
  z = mean_sys(s, 500, 10e-12, 10e-5, y);
  printf("z:%.20lf y:%.20lf\n", z, y[0]);
  free(y);
  free_sys(s);
  return 0;
}

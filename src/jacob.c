#include <stdlib.h>
#include "jacob.h"
#include "equation.h"
#include "system.h"
#include "diff.h"

void jacob_sys(comb_sys s)
{
  int i, j, l;
  comb_eq *p;

  if (s->jacob != NULL)
    return;
  l = s->len;
  s->jacob = malloc(l*sizeof(comb_eq *));
  p = malloc(l*l*sizeof(comb_eq));
  for (i = 0; i < l; i++) {
    s->jacob[s->refs[i]] = &p[s->refs[i]*l];
    for (j = 0; j < l; j++) {
      s->jacob[s->refs[i]][s->refs[j]] = diff_eq_ref(s->eqs[i], j+1);
    }
  }
}

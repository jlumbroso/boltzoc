#include <stdlib.h>
#include "system.h"
#include "equation.h"

#define SYS_DEF_SIZE 128

comb_sys sys_new(int size)
{
  comb_sys res;

  res = malloc(sizeof(struct comb_sys));
  res->len = 0;
  res->eqs = malloc(size*sizeof(comb_eq));
  res->refs = malloc(size*sizeof(int));
  res->y = malloc(size*sizeof(double));
  res->jacob = NULL;
#ifdef COMPILE
  res->f = NULL;
  res->fj = NULL;
#endif
  return res;
}

void sys_add_eq(comb_sys s, int r, comb_eq c)
{
  s->eqs[s->len] = c;
  s->refs[r-1] = s->len;
  s->len++;
}

int sys_len(comb_sys s)
{
  return s->len;
}

double sys_val(comb_sys s, int i)
{
  if (i > s->len)
    return 0.0;
  return s->y[s->refs[i-1]];
}

void free_sys(comb_sys s)
{
  int i, j;

  for (i = 0; i < s->len; i++)
    free_eq(s->eqs[i]);
  free(s->eqs);
  free(s->refs);
  free(s->y);
  for (i = 0; i < s->len; i++)
    for (j = 0; j < s->len; j++)
      free_eq(s->jacob[i][j]);
  free(s->jacob[0]);
  free(s->jacob);
#ifdef COMPILE
  free(s->f);
  free(s->fj[0]);
  free(s->fj);
#endif
  free(s);
}

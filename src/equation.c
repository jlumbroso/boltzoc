#include <stdlib.h>
#include "equation.h"

comb_eq eq_init(constr type, comb_eq operand1, comb_eq operand2, int ref, double coef)
{
  comb_eq res;

  res = malloc(sizeof(struct comb_eq));
  res->type = type;
  res->operand1 = operand1;
  res->operand2 = operand2;
  res->ref = ref;
  res->coef = coef;
  return res;
}

comb_eq nil()
{
  return eq_init(Nil, NULL, NULL, -1, 1.0);
}

comb_eq epsilon()
{
  return eq_init(Epsilon, NULL, NULL, -1, 1.0);
}

comb_eq atom()
{
  return eq_init(Atom, NULL, NULL, -1, 1.0);
}

comb_eq sum(comb_eq operand1, comb_eq operand2)
{
  return eq_init(Sum, operand1, operand2, -1, 1.0);
}

comb_eq prod(comb_eq operand1, comb_eq operand2)
{
  return eq_init(Prod, operand1, operand2, -1, 1.0);
}

comb_eq coef(double c, comb_eq operand)
{
  return eq_init(Coef, operand, NULL, -1, c);
}

comb_eq seq(comb_eq operand)
{
  return eq_init(Seq, operand, NULL, -1, 1.0);
}

comb_eq ref(int r)
{
  return eq_init(Ref, NULL, NULL, r, 1.0);
}

void free_eq(comb_eq c)
{
  if (!c)
    return;
  free_eq(c->operand1);
  free_eq(c->operand2);
  free(c);
}

#include <stdlib.h>
#include "diff.h"
#include "common.h"
#include "equation.h"
#include "system.h"

comb_eq copy_eq(comb_eq c)
{
  comb_eq res;

  switch (c->type) {
    case Epsilon:
      res = epsilon();
      break;
    case Atom:
      res = atom();
      break;
    case Ref:
      res = ref(c->ref);
      break;
    case Sum:
      res = sum(copy_eq(c->operand1), copy_eq(c->operand2));
      break;
    case Prod:
      res = prod(copy_eq(c->operand1), copy_eq(c->operand2));
      break;
    case Coef:
      res = coef(c->coef, copy_eq(c->operand1));
      break;
    case Seq:
      res = seq(copy_eq(c->operand1));
      break;
    default:
      my_error("ERROR: copy_eq unknown equation type\n");
      return NULL;
  }
  return res;
}

comb_eq diff_eq_aux(comb_eq c, int d, int l)
{
  comb_eq res;

  switch (c->type) {
    case Nil:
      res = NULL;
      break;
    case Epsilon:
      res = NULL;
      break;
    case Atom:
      if (d == -1)
        res = epsilon();
      else
        res = NULL;
      break;
    case Ref:
      if (d == -1) {
        res = ref(c->ref + l);
      } else if (d == c->ref) {
        res = epsilon();
      } else {
        res = NULL;
      }
      break;
    case Sum:
      {
        comb_eq dop1, dop2;

        dop1 = diff_eq_aux(c->operand1, d, l);
        dop2 = diff_eq_aux(c->operand2, d, l);
        if (dop1 == NULL)
          res = dop2;
        else if (dop2 == NULL)
          res = dop1;
        else
          res = sum(dop1, dop2);
      }
      break;
    case Prod:
      {
        comb_eq dop1, dop2;

        dop1 = diff_eq_aux(c->operand1, d, l);
        dop2 = diff_eq_aux(c->operand2, d, l);
        if (dop1 == NULL && dop2 == NULL)
          res = NULL;
        else if (dop1 == NULL)
          res = prod(copy_eq(c->operand1),dop2);
        else if (dop2 == NULL)
          res = prod(dop1, copy_eq(c->operand2));
        else
          res = sum(prod(dop1,copy_eq(c->operand2)), prod(copy_eq(c->operand1),dop2));
      }
      break;
    case Coef:
      {
        comb_eq dop;

        dop = diff_eq_aux(c->operand1, d, l);
        if (dop == NULL)
          res = NULL;
        else
          res = coef(c->coef, dop);
      }
      break;
    case Seq:
      {
        comb_eq dop;

        dop = diff_eq_aux(c->operand1, d, l);
        if (dop == NULL)
          res = NULL;
        else
          res = prod(copy_eq(c), prod(dop, copy_eq(c)));
      }
      break;
    default:
      my_error("ERROR: diff_eq unknown equation type\n");
      return NULL;
  }
  return res;
}

comb_eq diff_eq_ref(comb_eq c, int r)
{
  comb_eq res;

  res = diff_eq_aux(c, r, 0);
  if (res == NULL)
    return nil();
  return res;
}

comb_eq diff_eq_z(comb_eq c, int l)
{
  comb_eq res;

  res = diff_eq_aux(c, -1, l);
  if (res == NULL)
    return nil();
  return res;
}

comb_sys diff_sys_z(comb_sys s)
{
  comb_sys res;
  int i, l;

  l = s->len;
  res = sys_new(2*l);
  for (i = 0; i < l; i++)
    sys_add_eq(res, i, copy_eq(s->eqs[i]));
  for (i = 0; i < l; i++)
    sys_add_eq(res, l+i, diff_eq_z(s->eqs[i], l));
  return res;
}

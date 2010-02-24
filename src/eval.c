#include "eval.h"
#include "common.h"
#include "equation.h"
#include "system.h"

static comb_sys system;

/* TODO: ajouter mutex ou autre */
void set_sys(comb_sys s)
{
  system = s;
}

double eval_eq(comb_eq c, double *v, double z, int *err)
{
  switch (c->type) {
    case Nil:
      return 0.;
    case Epsilon:
      return 1.;
    case Atom:
      return z;
    case Ref:
      return v[system->refs[c->ref-1]];
    case Sum:
      return eval_eq(c->operand1, v, z, err) + eval_eq(c->operand2, v, z, err);
    case Prod:
      return eval_eq(c->operand1, v, z, err) * eval_eq(c->operand2, v, z, err);
    case Coef:
      return c->coef * eval_eq(c->operand1, v, z, err);
    case Seq:
      {
        double t;

        t = eval_eq(c->operand1, v, z, err);
        if (t >= 1.)
          *err = 1;
        return 1./(1. - t);
      }
    default:
      my_error("ERROR: eval_eq unknown equation type\n");
      return 0.;
  }
}

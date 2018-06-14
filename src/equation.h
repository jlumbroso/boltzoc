#ifndef _EQUATION_H
# define _EQUATION_H
# include "boltzoc.h"

typedef enum { Epsilon = 0x0BEEF0,
               Atom,
               Sum,
               Prod,
               Coef,
               Seq,
               Ref,
               Nil } constr;

struct comb_eq {
  constr type;
  struct comb_eq *operand1;
  struct comb_eq *operand2;
  int ref;
  double coef;
};

#endif

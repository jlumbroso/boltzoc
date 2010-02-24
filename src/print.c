#include <stdio.h>
#include "print.h"
#include "equation.h"
#include "system.h"
#include "jacob.h"

void print_eq(comb_eq c)
{
  switch (c->type) {
    case Nil:
      printf("0");
      break;
    case Epsilon:
      printf("Epsilon");
      break;
    case Atom:
      printf("Z");
      break;
    case Ref:
      printf("Y%d", c->ref);
      break;
    case Sum:
      printf("Sum(");
      print_eq(c->operand1);
      printf(",");
      print_eq(c->operand2);
      printf(")");
      break;
    case Prod:
      printf("Prod(");
      print_eq(c->operand1);
      printf(",");
      print_eq(c->operand2);
      printf(")");
      break;
    case Coef:
      printf("%lf*", c->coef);
      print_eq(c->operand1);
      break;
    case Seq:
      printf("Seq(");
      print_eq(c->operand1);
      printf(")");
      break;
    default:
      my_error("ERROR: print_eq unknown equation type\n");
  }
}

void print_jacob(comb_sys s)
{
  int i, j;

  for (i = 0; i < s->len; i++) {
    for (j = 0; j < s->len; j++) {
      printf("dY%d/dY%d = ", i+1, j+1);
      print_eq(s->jacob[s->refs[i]][s->refs[j]]);
      puts("");
    }
  }
}

void print_sys(comb_sys s)
{
  int i;
  
  for (i = 0; i < s->len; i++) {
    printf("Y%d = ", i+1);
    print_eq(s->eqs[i]);
    puts("");
  }
}

void my_error(char *s)
{
  fprintf(stderr, s);
}

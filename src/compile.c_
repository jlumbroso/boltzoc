#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <lightning.h>
#include "compile.h"
#include "common.h"
#include "equation.h"
// TODO: rendre dynamique la taille du buffer
#define BUFFER_SIZE (1<<28)
#define STACK_SIZE 1024

double zero(double *v, double z, int *err)
{
  return 0.;
}

static int stack_ptr;

// TODO: utiliser les registres libres avant d'aller en mémoire

void stack_push(int reg)
{
  jit_stxi_d (stack_ptr, JIT_FP, reg);
  stack_ptr += sizeof (double);
}

void stack_pop(int reg)
{
  stack_ptr -= sizeof (double);
  jit_ldxi_d (reg, JIT_FP, stack_ptr);
}

void compile_expr(comb_eq c)
{
  switch (c->type) {
    case Epsilon:
      jit_movi_d(JIT_FPR0, 1.0);
      break;
    case Atom:
      jit_movr_d(JIT_FPR0, JIT_FPR2);
      break;
    case Ref:
      // JIT_FPR0 = v[c->ref-1]
      jit_ldxi_d(JIT_FPR0, JIT_R0, sizeof(double) * (c->ref-1));
      break;
    case Sum:
      compile_expr(c->operand1);
      stack_push(JIT_FPR0);
      compile_expr(c->operand2);
      stack_pop(JIT_FPR1);
      jit_addr_d(JIT_FPR0, JIT_FPR1, JIT_FPR0);
      break;
    case Prod:
      compile_expr(c->operand1);
      stack_push(JIT_FPR0);
      compile_expr(c->operand2);
      stack_pop(JIT_FPR1);
      jit_mulr_d(JIT_FPR0, JIT_FPR1, JIT_FPR0);
      break;
    case Coef:
      compile_expr(c->operand1);
      jit_movi_d(JIT_FPR1, c->coef);
      jit_mulr_d(JIT_FPR0, JIT_FPR1, JIT_FPR0);
      break;
    case Seq:
      {
        jit_insn *ref;

        compile_expr(c->operand1);
        // IF JIT_FPR0 >= 1 THEN *err = 1
        jit_floorr_d_i(JIT_R2, JIT_FPR0);
        ref = jit_blti_i(jit_forward(), JIT_R2, 1);
        jit_str_i(JIT_R1, JIT_R2);
        jit_patch(ref);
        jit_movi_d(JIT_FPR1, 1.0);
        jit_subr_d(JIT_FPR0, JIT_FPR1, JIT_FPR0);
        jit_divr_d(JIT_FPR0, JIT_FPR1, JIT_FPR0);
        break;
      }
    default:
      my_error("ERROR: eval_eq unknown equation type\n");
  }
}

f_eval compile_eq(comb_eq c)
{
  f_eval fn;
  int in;

  if (c->type == Nil)
    return zero;
  fn = (f_eval) (jit_get_ip().dptr);
  jit_leaf(3);
  in = jit_arg_p();
  // TODO: verifier que ca ne coute pas trop cher
  stack_ptr = jit_allocai(STACK_SIZE * sizeof (double));

  jit_getarg_p(JIT_R0, in);
  in = jit_arg_d();
  jit_getarg_d(JIT_FPR2, in);
  in = jit_arg_p();
  jit_getarg_p(JIT_R1, in);
  compile_expr(c);
  jit_movr_d(JIT_FPRET, JIT_FPR0);
  jit_ret();
  return fn;
}

void compile_sys(comb_sys s)
{
  int i, j;
  int l;
  static jit_insn codeBuffer[BUFFER_SIZE];
  f_eval *fp;

  if (s->f)
    return;
  jit_set_ip(codeBuffer);
  l = s->len;
  s->f = malloc(l*sizeof(f_eval));
  s->fj = malloc(l*sizeof(f_eval *));
  fp = malloc(l*l*sizeof(f_eval));
  for (i = 0; i < l; i++) {
    s->f[s->refs[i]] = compile_eq(s->eqs[i]);
    s->fj[i] = &fp[s->refs[i]*l];
    for (j = 0; j < l; j++) {
      s->fj[s->refs[i]][s->refs[j]] = compile_eq(s->jacob[i][j]);
    }
  }
  jit_flush_code(codeBuffer, jit_get_ip().ptr);
}

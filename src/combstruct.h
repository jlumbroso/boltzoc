#ifndef _COMBSTRUCT_H
# define _COMBSTRUCT_H

/* Equation */
typedef struct comb_eq *comb_eq;

comb_eq nil();

comb_eq epsilon();

comb_eq atom();

comb_eq sum(comb_eq, comb_eq);

comb_eq prod(comb_eq, comb_eq);

comb_eq coef(double, comb_eq);

comb_eq seq(comb_eq);

comb_eq ref(int);

void free_eq(comb_eq);

/* System */
typedef struct comb_sys *comb_sys;

comb_sys sys_new(int);

void sys_add_eq(comb_sys, int, comb_eq);

int sys_len(comb_sys);

double sys_val(comb_sys, int);

void free_sys(comb_sys);

/* Iter */
// TODO: retourner le max des 1/(1 - y') ?
void eval_sys(comb_sys, double, double, int *);

void eval_point_sys(comb_sys, double, double, int *);

double sing_sys(comb_sys, double, double);

// last argument must be twice the size of the system
double mean_sys(comb_sys, double, double, double);

# ifndef NOPRINT
/* Print */
void print_sys(comb_sys);
# endif
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef BLAS
# include <cblas.h>
#endif
#ifndef COMPILE
# include "eval.h"
#endif
#include "system.h"
#include "jacob.h"
#include "diff.h"

void multm(double **a, double **b, double **c, int n)
{
#ifdef BLAS
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, &b[0][0], n, &c[0][0], n, 0, &a[0][0], n);
#else
  int i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      a[i][j] = 0;
      for (k = 0; k < n; k++)
        a[i][j] += b[i][k]*c[k][j];
    }
#endif
}

void iter_newt(comb_sys s, double *y1, double **dy, double **u1, double z,
               double *y2, double **u2, int *err)
{
  int i, j, k, l;
  double *tmp1;
  double **tmp2;

  l = s->len;
  /* u2 will be filled later on */
  tmp2 = u2;
  // tmp2 = A*U - U + 1
  multm(tmp2, dy, u1, l);
  for (i = 0; i < l; i++) {
    for (j = 0; j < l; j++) {
      tmp2[i][j] += - u1[i][j] + (i==j?1:0);
    }
  }
  // dy = U*tmp2
  multm(dy, u1, tmp2, l);
  // u2 = u1 + dy
  for (i = 0; i < l; i++) {
    for (j = 0; j < l; j++) {
      u2[i][j] = u1[i][j] + dy[i][j];
    }
  }
  /* we don't use the values of u1 in the rest of the function */
  tmp1 = u1[0];
  // tmp1 = H(y1) - y1
  for (i = 0; i < l; i++) {
#ifdef COMPILE
    tmp1[i] = (s->f[i])(y1, z, err) - y1[i];
#else
    set_sys(s);
    tmp1[s->refs[i]] = eval_eq(s->eqs[i], y1, z, err) - y1[s->refs[i]];
#endif
  }
  // y2 = u2*tmp1 + y1
  for (i = 0; i < l; i++) {
    y2[i] = y1[i];
    for (k = 0; k < l; k++) {
      y2[i] += u2[i][k]*tmp1[k];
    }
  }
  // dy = H(y2)
  for (i = 0; i < l; i++) {
    for (j = 0; j < l; j++) {
#ifdef COMPILE
      dy[i][j] = (s->fj[i][j])(y2, z, err);
#else
      set_sys(s);
      dy[i][j] = eval_eq(s->jacob[i][j], y2, z, err);
#endif
    }
  }
}

void eval_sys_noalloc(comb_sys s, double z, double prec, double *y, double *w,
                      double **dy, double **u, double **v, int *err)
{
  // y va contenir le resultat final, w sert de stockage temporaire
  int i, j, l;
  double e;
  double *y1, *y2;
  double **u1, **u2;
  void *tmp;

  l = s->len;
  y2 = y; y1 = w;
  u2 = u; u1 = v;
  // w = 0; y2 = 0
  for (i = 0; i < l; i++) {
    w[i] = 0.;
    y2[i] = 0.;
  }
  l = s->len;
  // dy = H(0); u1 = I
  for (i = 0; i < l; i++) {
    for (j = 0; j < l; j++) {
#ifdef COMPILE
      dy[i][j] = (s->fj[i][j])(y2, z, err);
#else
      set_sys(s);
      dy[i][j] = eval_eq(s->jacob[i][j], y2, z, err);
#endif
      u1[i][j] = (i==j?1.:0.);
      u2[i][j] = 0.;
    }
  }
  do {
    e = 0.;
    iter_newt(s, y1, dy, u1, z, y2, u2, err);
    if (*err)
      break;
    for (i = 0; i < l; i++) {
      if (e < y2[i]-y1[i])
        e = y2[i]-y1[i];
    }
    tmp = y1; y1 = y2; y2 = tmp;
    tmp = u1; u1 = u2; u2 = tmp;
  } while (e > prec);
  if (y != y2)
    for (i = 0; i < l; i++)
      y[i] = y2[i];
}

void init_eval_sys(comb_sys s, double **w, double ***dy, double ***u, double ***v)
{
  int i, l;
  double *p;

  l = s->len;
  jacob_sys(s);
#ifdef COMPILE
  compile_sys(s);
#endif
  *w = malloc(l*sizeof(double));
  *dy = malloc(l*sizeof(double *));
  p = malloc(l*l*sizeof(double));
  for (i = 0; i < l; i++)
    (*dy)[i] = &p[i*l];
  *u = malloc(l*sizeof(double *));
  p = malloc(l*l*sizeof(double));
  for (i = 0; i < l; i++)
    (*u)[i] = &p[i*l];
  *v = malloc(l*sizeof(double *));
  p = malloc(l*l*sizeof(double));
  for (i = 0; i < l; i++)
    (*v)[i] = &p[i*l];
}

void end_eval_sys(comb_sys s, double *w, double **dy, double **u, double **v)
{
  free(w);
  free(dy[0]); free(u[0]); free(v[0]);
  free(dy); free(u); free(v);
}

void eval_sys(comb_sys s, double z, double prec, int *err)
{
  double *w, **dy, **u, **v, *y;

  y = s->y;
  init_eval_sys(s, &w, &dy, &u, &v);
  eval_sys_noalloc(s, z, prec, y, w, dy, u, v, err);
  end_eval_sys(s, w, dy, u, v);
}

double sing_sys(comb_sys s, double prec1, double prec2)
{
  double zmin = 0.;
  double zmax = 1.;
  double z;
  int i;
  double *w, **dy, **u, **v, *y;
  int err;
  
  y = s->y;
  init_eval_sys(s, &w, &dy, &u, &v);
  do {
    z = (zmin + zmax)/2.;
    err = 0;
    eval_sys_noalloc(s, z, prec2, y, w, dy, u, v, &err);
    if (err)
      zmax = z;
    for (i = 0; z != zmax && i < sys_len(s); i++)
      if (y[i] < 0 || y[i] > 1./prec1)
        zmax = z;
    if (zmax != z)
      zmin = z;
  } while (zmax-zmin > prec1);
  eval_sys_noalloc(s, zmin, prec2, y, w, dy, u, v, &err);
  end_eval_sys(s, w, dy, u, v);
  return zmin;
}

/* TODO: mettre à jour */
void eval_point_sys(comb_sys s, double z, double prec, int *err)
{
  comb_sys ss;
  double *w, **dy, **u, **v, *y;

  y = s->y;
  ss = diff_sys_z(s);
  init_eval_sys(ss, &w, &dy, &u, &v);
  eval_sys_noalloc(ss, z, prec, y, w, dy, u, v, err);
  end_eval_sys(ss, w, dy, u, v);
  free_sys(ss);
}

/* TODO: mettre à jour */
double mean_sys(comb_sys s, double mean, double prec1, double prec2)
{
  double zmin = 0.;
  double zmax = 1.;
  double *y;
  double z, m;
  int i;
  comb_sys ss;
  double *w, **dy, **u, **v;
  int err;
  
  y = s->y;
  ss = diff_sys_z(s);
  init_eval_sys(ss, &w, &dy, &u, &v);
  do {
    z = (zmin + zmax)/2.;
    err = 0;
    eval_sys_noalloc(ss, z, prec2, y, w, dy, u, v, &err);
    m = z*y[s->len]/y[0];
    if (err)
      zmax = z;
    for (i = 0; z != zmax && i < sys_len(s); i++)
      if (y[i] < 0 || y[i] > 1./prec1 || m > mean)
        zmax = z;
    if (zmax != z)
      zmin = z;
  } while (zmax-zmin > prec1);
  eval_sys_noalloc(ss, zmin, prec2, y, w, dy, u, v, &err);
  end_eval_sys(ss, w, dy, u, v);
  free_sys(ss);
  return z;
}

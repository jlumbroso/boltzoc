#include <stdio.h>
#include <stdlib.h>
#include "../src/boltzoc.h"

int main(int argc, char **argv)
{
  comb_sys s;
  double z = 4./27.;

  s = sys_new(13);
  // 1: package_uml
  sys_add_eq(s, 1, coef(0.01577, prod(atom(), seq(ref(2)))));
  // 2: packageableElement
  sys_add_eq(s, 2, sum(ref(1), sum(ref(3), ref(6))));
  // 3: class_uml
  sys_add_eq(s, 3, prod(atom(), prod(seq(ref(5)), prod(seq(ref(12)), seq(ref(4)))))),
  // 4: generalization
  sys_add_eq(s, 4, atom());
  // 5: property
  sys_add_eq(s, 5, prod(atom(), coef(3.0, sum(epsilon(), ref(7)))));
  // 6: association
  sys_add_eq(s, 6, atom());
  // 7: valueSpecification
  sys_add_eq(s, 7, sum(ref(8), sum(ref(9), sum(ref(10), ref(11)))));
  // 8: literalBoolean
  sys_add_eq(s, 8, atom());
  // 9: literalNull
  sys_add_eq(s, 9, atom());
  // 10: literalInteger
  sys_add_eq(s, 10, atom());
  // 11: literalString
  sys_add_eq(s, 11, atom());
  // 12: operation
  sys_add_eq(s, 12, prod(atom(), coef(2.0, seq(ref(13)))));
  // 13: parameter
  sys_add_eq(s, 13, prod(atom(), coef(3.0, sum(epsilon(), ref(7)))));
  z = sing_sys(s, 10e-12, 10e-5);
  printf("=== SING ===\n");
  printf("#define X %.20lf\n", z);
  printf("#define PACKAGE_UML %.10lf\n", sys_val(s, 1));
  printf("#define PACKAGEABLE_ELEMENT %.10lf\n", sys_val(s, 2));
  printf("#define CLASS_UML %.10lf\n", sys_val(s, 3));
  printf("#define GENERALIZATION %.10lf\n", sys_val(s, 4));
  printf("#define PROPERTY %.10lf\n", sys_val(s, 5));
  printf("#define ASSOCIATION %.10lf\n", sys_val(s, 6));
  printf("#define VALUE_SPECIFICATION %.10lf\n", sys_val(s, 7));
  printf("#define LITERAL_BOOLEAN %.10lf\n", sys_val(s, 8));
  printf("#define LITERAL_NULL %.10lf\n", sys_val(s, 9));
  printf("#define LITERAL_INTEGER %.10lf\n", sys_val(s, 10));
  printf("#define LITERAL_STRING %.10lf\n", sys_val(s, 11));
  printf("#define OPERATION %.10lf\n", sys_val(s, 12));
  printf("#define PARAMETER %.10lf\n", sys_val(s, 13));
/*
  y = realloc(y, sys_len(s)*2*sizeof(double));
  z = mean_sys(s, 30000, 10e-12, 10e-5, y);
  printf("=== MEAN ===\n");
  printf("#define X %.20lf\n", z);
  printf("#define PACKAGE_UML %.10lf\n", y[0]);
  printf("#define PACKAGEABLE_ELEMENT %.10lf\n", y[1]);
  printf("#define CLASS_UML %.10lf\n", y[2]);
  printf("#define GENERALIZATION %.10lf\n", y[3]);
  printf("#define PROPERTY %.10lf\n", y[4]);
  printf("#define ASSOCIATION %.10lf\n", y[5]);
  printf("#define VALUE_SPECIFICATION %.10lf\n", y[6]);
  printf("#define LITERAL_BOOLEAN %.10lf\n", y[7]);
  printf("#define LITERAL_NULL %.10lf\n", y[8]);
  printf("#define LITERAL_INTEGER %.10lf\n", y[9]);
  printf("#define LITERAL_STRING %.10lf\n", y[10]);
  printf("#define OPERATION %.10lf\n", y[11]);
  printf("#define PARAMETER %.10lf\n", y[12]);
  eval_point_sys(s, z, 10e-3, y, &err);
  printf("Moyennes:\n");
  printf("PACKAGE_UML %.10lf\n", z*y[13]/y[0]);
  printf("PACKAGEABLE_ELEMENT %.10lf\n", z*y[14]/y[1]);
  printf("CLASS_UML %.10lf\n", z*y[15]/y[2]);
  printf("GENERALIZATION %.10lf\n", z*y[16]/y[3]);
  printf("PROPERTY %.10lf\n", z*y[17]/y[4]);
  printf("ASSOCIATION %.10lf\n", z*y[18]/y[5]);
  printf("VALUE_SPECIFICATION %.10lf\n", z*y[19]/y[6]);
  printf("LITERAL_BOOLEAN %.10lf\n", z*y[20]/y[7]);
  printf("LITERAL_NULL %.10lf\n", z*y[21]/y[8]);
  printf("LITERAL_INTEGER %.10lf\n", z*y[22]/y[9]);
  printf("LITERAL_STRING %.10lf\n", z*y[23]/y[10]);
  printf("OPERATION %.10lf\n", z*y[24]/y[11]);
  printf("PARAMETER %.10lf\n", z*y[25]/y[12]); */
  free_sys(s);
  return 0;
}

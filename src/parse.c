#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "combstruct.h"
#include "../combstruct2json.h"
#include "symbtab.h"

// Arbitrary error codes
#define ERROR_FIXME 120
#define ERROR_BAD_TOKEN 86
#define ERROR_UNSUPPORTED 43

/*
 * Adapted from:
 * https://github.com/jlumbroso/combstruct2json/blob/master/examples/jsonOutput.c
 * 
 * This provides a full example of tree traversal of an abstract syntax tree
 * for a parsed combstruct grammar.
 * 
 */

comb_sys importGrammar(char* filename);

// DONE:
void treewalkRestriction(Restriction rest, long long limit)
{
  switch (rest)
  {
  case (NONE):
    // Nothing to do
    return;
  default: ;
  }

  // FIXME: implement restrictions
  perror("FIXME: restriction feature not implemented.\n");
  exit(ERROR_FIXME);

  // switch (rest)
  // {
  // case (LESS):    // card <= limit
  // case (EQUAL):   // card == limit
  // case (GREATER): // card >= limit
  // default:        // error
  // }
}

// DONE:
int treewalkId(const Id* A)
{
  int symbid = symb_lookup(A->name);

  // NOTE: The boltzoc library is 1-index instead of 0-index -_-
  return symbid+1;
}

// DONE:
typedef comb_eq (combineEqFunc)(comb_eq, comb_eq);

// DONE:
comb_eq treewalkExpressionList(const ExpressionList* Elist, combineEqFunc operator);

// DONE:
comb_eq treewalkExpression(const Expression* E)
{
  // CASE 1: type is single entity
  switch (E->type)
  {
  case (ATOM):
    return atom();

  case (EPSILON):
    return epsilon();

  case (Z):
    // FIXME: should this be a reference?
    return atom();

  case (ID): ;
    Id* id = (Id*) E->component;
    return ref(treewalkId(id));

  // CASE 2: type is constructor, but no restrictions can apply
  case UNION: ;
    ExpressionList* elist = (ExpressionList*) E->component;
    comb_eq subexps = treewalkExpressionList(elist, sum);
    return subexps;

  case PROD: ;
    elist = (ExpressionList*) E->component;
    subexps = treewalkExpressionList(elist, prod);
    return subexps;

  // CASE 3: type is constructor, but restrictions can apply
  case SEQUENCE: ;
    Expression* e = (Expression*) E->component;
    treewalkRestriction(E->restriction, E->limit);
    comb_eq subexp = treewalkExpression(e);
    return seq(subexp);

  // Unsupported operations
  case SUBST:
    perror("ERROR: Polya operator are not supported; only algebraic (tree) grammars.\n");
    exit(ERROR_UNSUPPORTED);

  case SET:
  case POWERSET:
  case CYCLE:
    perror("ERROR: Polya operator are not supported; only algebraic (tree) grammars.\n");
    exit(ERROR_UNSUPPORTED);

  default: ;
    perror("ERROR: Bad token.\n");
    exit(ERROR_BAD_TOKEN);
  }
  
  perror("ERROR: Bad token.");
  exit(ERROR_BAD_TOKEN);
  return NULL;
}

// DONE:
comb_eq treewalkExpressionList(const ExpressionList* Elist, combineEqFunc operator)
{
  if (Elist->size >= 1) {
    const Expression** exprs = (const Expression**) Elist->components;

    comb_eq tmp, ret = treewalkExpression(exprs[0]);

    // Combine following values with an operator:
    for(int i=1; i < Elist->size; i++) {
      tmp = treewalkExpression(exprs[i]);
      ret = operator(ret, tmp);
    }

    return ret;
  } else {
    // ERROR:
    return NULL;
  }
}

// DONE:
comb_sys treewalkStatementList(const StatementList* Slist)
{
  comb_sys sys = sys_new(Slist->size);

  const Statement** statements = (const Statement**) Slist->components;
  for(int i=0; i < Slist->size; i++) {
    
    // Retrieve data from object
    const Statement *S = statements[i];
    Id *varstr = S->variable;
    Expression* expr = S->expression;

    // Compute ID and definition
    int symb_id = treewalkId(varstr);
    comb_eq symb_def = treewalkExpression(expr);

    // Add to combinatorial system
    sys_add_eq(sys, symb_id, symb_def);
  }

  return sys;
}

// DONE:
void treewalkError(const Error* error)
{
  int line = error->line;

  switch(error->type)
  {
    case LEXER:  printf("ERROR (lexer, line %d): %s\n", line, error->message);
    case PARSER: printf("ERROR (parser, line #%d): %s\n", line, error->message);
    default: ;
  }

  return;
}

// DONE:
comb_sys treewalkGrammar(const Grammar* grammar)
{
  if (grammar->type == ISERROR) {
    Error* E = (Error*) grammar->component;
    treewalkError(E);
    return NULL;
  } else {
    StatementList* Slist = (StatementList*) grammar->component;
    return treewalkStatementList(Slist);
  }
}

// DONE:
comb_sys importGrammar(char* filename) {
  Grammar *grammar = readGrammar(filename);
  return treewalkGrammar(grammar);
}

//src/system.c src/iter.c src/jacob.c src/diff.c src/eval.c src/print.c src/equation.c src/common.c
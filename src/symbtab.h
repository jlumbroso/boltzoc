#ifndef _SYMBTAB_H
#define _SYMBTAB_H

// Macros for a fast hashmap.
#include "uthash.h"

// Key-value pair prototype.
struct _nameid_pair {
    char *name;
    int     id;

    UT_hash_handle hh;
};

// These fields must be external so a global state can be stored.
extern int _NEXT_ID;
extern int _MAX_TAB;
extern struct _nameid_pair *symbols_by_name;
extern char **symbols_by_id;

// Core methods for alpha-renaming of parameters.
int symb_lookup(char *name);
char *symb_lookup_by_id(int id);

#endif
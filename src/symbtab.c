#include <stdlib.h>  /* malloc */
#include <string.h>  /* strdup */

#include "uthash.h"

#include "symbtab.h"

int _NEXT_ID = -1;
int _MAX_TAB = 64;
struct _nameid_pair *symbols_by_name = NULL;
char **symbols_by_id = NULL;


int symb_lookup(char *name) {
    struct _nameid_pair *record;

    HASH_FIND_STR( symbols_by_name, name, record );

    // Existing symbol
    if (record) {
        return record->id;
    }
    
    // New symbol
    if (_NEXT_ID == -1) {
        symbols_by_id = (char **)malloc(sizeof(char *) * _MAX_TAB);
        _NEXT_ID = 0;
    } else if (_NEXT_ID >= _MAX_TAB) {
        _MAX_TAB = 2*_MAX_TAB;
        symbols_by_id = (char **)realloc(symbols_by_id, sizeof(char *) * _MAX_TAB);
    }
    int newid = _NEXT_ID++;
    record = (struct _nameid_pair *)malloc(sizeof *record);
    record->name = strdup(name);
    record->id = newid;

    HASH_ADD_KEYPTR( hh, symbols_by_name, record->name, strlen(record->name), record );
    symbols_by_id[record->id] = record->name;

    return record->id;
}

char *symb_lookup_by_id(int id) {
    // Check if id is out of bounds.
    if (id < 0 || id >= _MAX_TAB || id >= _NEXT_ID) {
        return NULL;
    }

    return symbols_by_id[id];
}
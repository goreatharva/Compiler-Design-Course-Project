#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

typedef enum {
    VAR_SYM,
    FUNC_SYM,
    PARAM_SYM
} SymType;

typedef struct SymEntry {
    char* name;
    SymType stype;
    VarType dtype;
    struct SymEntry* next;
} SymEntry;

void add_symbol(char* name, SymType stype, VarType dtype);
SymEntry* lookup_symbol(char* name);
void print_symbols();
void enter_scope();
void exit_scope();

#endif

#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 211

typedef struct Scope {
    SymEntry* table[TABLE_SIZE];
    struct Scope* next;
} Scope;

static Scope* current_scope = NULL;

static unsigned int hash(char* str) {
    unsigned int hash = 0;
    for (; *str; str++) {
        hash = (hash << 2) + *str;
    }
    return hash % TABLE_SIZE;
}

void enter_scope() {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    memset(scope->table, 0, sizeof(scope->table));
    scope->next = current_scope;
    current_scope = scope;
}

void exit_scope() {
    if (!current_scope) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        SymEntry* entry = current_scope->table[i];
        while (entry) {
            SymEntry* temp = entry;
            entry = entry->next;
            free(temp->name);
            free(temp);
        }
    }
    Scope* temp = current_scope;
    current_scope = current_scope->next;
    free(temp);
}

void add_symbol(char* name, SymType stype, VarType dtype) {
    if (!current_scope) enter_scope();
    unsigned int h = hash(name);
    SymEntry* entry = (SymEntry*)malloc(sizeof(SymEntry));
    entry->name = strdup(name);
    entry->stype = stype;
    entry->dtype = dtype;
    entry->next = current_scope->table[h];
    current_scope->table[h] = entry;
}

SymEntry* lookup_symbol(char* name) {
    Scope* scope = current_scope;
    while (scope) {
        unsigned int h = hash(name);
        SymEntry* entry = scope->table[h];
        while (entry) {
            if (strcmp(entry->name, name) == 0)
                return entry;
            entry = entry->next;
        }
        scope = scope->next;
    }
    return NULL;
}

void print_symbols() {
    printf("Symbol Table:\n");
    Scope* scope = current_scope;
    int level = 0;
    while (scope) {
        printf("Scope level %d:\n", level++);
        for (int i = 0; i < TABLE_SIZE; i++) {
            SymEntry* entry = scope->table[i];
            while (entry) {
                printf("  Name: %s, SymType: %d, VarType: %d\n", entry->name, entry->stype, entry->dtype);
                entry = entry->next;
            }
        }
        scope = scope->next;
    }
}

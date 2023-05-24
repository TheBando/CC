#ifndef SCOPE_H
#define SCOPE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "symbol.h"


void scope_enter();

void scope_exit();

void scope_level();

void scope_bind(const char *, struct symbol *);

struct symbol* scope_lookup(const char *);

struct symbol* scope_lookup_current(const char *);

bool scope_unbind(const char *);

#endif

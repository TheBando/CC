#include "scope.h"


stack_t* symbol_tables;
void scope_enter() {


    hash_table_t* new_table = ht_create();

    symbol_t* new_symbol = symbol_create();

    stack_push(new_symbol, new_table);

}


void scope_exit() {
    hash_table_t* current_table = stack_pop(symbol_tables);

    ht_destroy(current_table);
}


int scope_level() {

    return stack_size(symbol_tables);

}


void scope_bind(const char* name, struct symbol* sym) {

    hash_table_t* current_table = stack_item(stack_size(symbol_tables));

    ht_set(current_table, name, sym);
}


struct symbol* scope_lookup(const char* name) {
    int num_tables = stack_size(symbol_tables);


    for (int i = num_tables - 1; i >= 0; i--) {
        hash_table_t* table = stack_item(symbol_table,i);

        struct symbol* sym = ht_get(table, name);

        if (sym != NULL) {
            return sym;
        }


    }

    return NULL;
}


struct symbol* scope_lookup_current(const char* name) {


    hash_table_t* current_table = stack_item(stack_size(symbol_tables));

    return ht_get(current_table, name);
}

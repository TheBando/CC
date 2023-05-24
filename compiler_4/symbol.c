#include "symbol.h"


struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ){
  struct symbol *ret = malloc(sizeof(ret));
  ret->kind = kind;
  ret->type = type;
  ret->name = name;
  return ret;
}

void symbol_delete(struct symbol *s){
    if(!s) return;

    type_delete(s->type);

    free(s);
}

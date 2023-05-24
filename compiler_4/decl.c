#include "decl.h"

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ){
  struct decl *ret = malloc(sizeof(ret));
  ret->name = name;
  ret->type = type;
  ret->value = value;
  ret->code = code;
  ret->next = next;
  return ret;
}



void decl_print( struct decl *d, int indent ){
  printf("%s", name);
  type_print(d->type);
  if(d->type != TYPE_FUNCTION){

    if(d->value != NULL){
      printf("=");
      expr_print(d->value);
    }
    printf(";\n");
  }
  else if (d->type == TYPE_FUNCTION) {

    if(d->code != NULL){
      printf(" =\n{\n");
      stmt_print(code, indent + 1);
      printf("}");
    }
    else{
      printf(";");
    }
  }
  printf("\n");

  if (d->next != NULL){
    decl_print(d->next, 0);
  }

}


void decl_resolve( struct decl * d){
    if(!d) return;

    symbol_t kind = scope_level() > 1? SYMBOL_LOCAL : SYMBOL_GLOBAL;

    d->symbol = symbol_create(kind,d->type, d->name);

    expr_resolve(d->value);
    scope_bind(d->name,d->symbol);

    if(d->code) {
        scope_enter();
        param_list_resolve(d->type_params);
        stmt_resolve(d->code);
        scope_exit();
    }

    decl_resolve(d->next);
}

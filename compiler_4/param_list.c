#include "param_list.h"


struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ){
  struct param_list *ret = malloc(sizeof(ret));
  ret->name = name;
  ret->type = type;
  ret->next = next;
}


void param_list_print( struct param_list *a ){
  printf("%s:", a->name);
  type_print(a->type);
  if(a->next != NULL){
    printf(", ");
    param_list_print(a->next);
  }

}


struct param_list * param_list_copy(struct param_list *a ){
    if(!a) return NULL;

    struct param_list* p = malloc(sizeof(struct param_list));
    if (p){
        p->name = a->name;
        p->type = a->type;
        p->symbol = a->symbol;
        p->next = a->next;
    }
    return p;
}

void param_list_delete(struct param_list *a){
    if (a == NULL) return;

    type_delete(a->type);
    param_list_delete(a->next);
    symbol_delete(a->symbol);

    free(a);
}

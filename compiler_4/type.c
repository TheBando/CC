#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ){
  struct type *ret = malloc(sizeof(ret));
  ret->kind = kind;
  ret->subtype = subtype;
  ret->params = params;
  return ret;
}



void type_print( struct type *t ){
  switch(t->kind){
    case TYPE_VOID:
      printf("void");
      break;

    case TYPE_BOOLEAN:
      printf("boolean");
      break;

    case TYPE_CHARACTER:
      printf("char");
      break;

    case TYPE_INTEGER:
      printf("int");
      break;

    case TYPE_STRING:
      printf("string");
      break;

    case TYPE_ARRAY:
      printf("array[] ");
      type_print(t->subtype);
      break;

    case TYPE_FUNCTION:
      printf("function ");
      type_print(t->subtype);
      printf(" (");
      if (t->params != NULL){
        param_list_print(t->params);
      }
      printf(")");
      break;

    default:
      printf("Whoops! in type.c");
      break;
  }
}



bool type_equals ( struct type *a, struct type *b){
    if (a->kind == b->kind ){
        if (a->kind == TYPE_INTEGER ||
            a->kind == TYPE_BOOLEAN ||
            a->kind == TYPE_CHARACTER)
            { return true; }
        else if (a->kind == TYPE_ARRAY)
        {
            if (type_equals(a->subtype, b->subtype))
            { return true; }
        }
        else if (a->kind == TYPE_FUNCTION)
        {
            if(a->subtype->kind == b->subtype->kind)
            { return true; }
        }
    } else { return false; }
}


struct type * type_copy(struct type * t){
    if(!t) return NULL;

    struct type* type = malloc(sizeof(struct type));
    if(type){
        type->kind = t->kind;
        type->subtype = type_copy(t->subtype);
        type->params = param_list_copy(t->params);
    }
    return type;
}

void type_delete( struct type *t){
    if (t == NULL) return;

    type_delete(t->subtype);
    param_list_delete(t->params);

    free(t);
}

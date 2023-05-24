#include "stmt.h"

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ){
  struct stmt *ret = malloc(sizeof(ret));
  ret->kind = kind;
  ret->decl = decl;
  ret->init_expr = init_expr;
  ret->expr = expr;
  ret->next_expr = next_expr
  ret->body = body;
  ret->else_body = else_body;
  ret->next = next;
  return ret;
}


void stmt_print( struct stmt *s, int indent ){
  for (int i = 0; i < indent; i++){
    printf("\t");
  }

  if (s->kind == STMT_DECL){
    decl_print(s->decl, indent);
  }
  if(s->kind == STMT_EXPR) {
    expr_print(s->expr);
    printf(";\n");
  }
  if(s->kind == STMT_IF){
    printf("if (");
    expr_print(s->expr);
    printf(")\n");
    stmt_print(s->body, indent + 1);
  }
  if(s->kind == STMT_IF_ELSE){
    printf("if (");
    expr_print(s->expr);
    printf(")\n");
    stmt_prnt(s->body, indent + 1);
    for (int i = 0; i < indent; i++){
      printf("\t");
    }
    printf("else\n");
    stmt_print(s->else_body, indent + 1);
  }
  if(s->kind == STMT_FOR){
    printf("for (");
    if(s->init_expr != NULL){
      expr_print(s->init_expr);
    }
    printf(";");
    if(s->expr != NULL){
      expr_print(s->expr);
    }
    printf(";");
    if(s->next_expr != NULL){
      expr_print(s->next_expr);
    }
    printf(")\n");
    stmt_print(s->body, indent + 1);
  }
  if(s->kind == STMT_PRINT){
    printf("print ");
    if(s->expr != NULL){
      expr_print(s->expr);
    }
    printf(";\n");
  }
  if(s->kind == STMT_RETURN){
    printf("return ");
    if(s->expr != NULL){
      expr_print(s->expr);
    }
    printf(";\n");
  }
  if(s->kind == STMT_BLOCK){
    printf("{\n");
    stmt_print(s->body, indent + 1);
    printf("}\n");
  }



}

#include "expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ){
  struct expr *ret = malloc(sizeof(ret));
  ret->kind = kind;
  ret->left = left;
  ret->right = right;
  return ret;
}

struct expr * expr_create_name( const char *n ){
  struct expr *ret = expr_create(EXPR_NAME, 0, 0);
  ret->name = n;
  return ret;
}


struct expr * expr_create_integer_literal( int c ){
  struct expr *ret = expr_create(EXPR_INT_LIT, 0, 0);
  ret->literal_value = c;
  return ret;
}

struct expr * expr_create_boolean_literal( int c ){
    struct expr *ret = expr_create(EXPR_BOOL_LIT, 0, 0);
    ret->literal_value = c;
    return ret;
}


struct expr * expr_create_char_literal( char c ){
  struct expr *ret = expr_create(EXPR_CHAR_LIT, 0, 0);
  ret->string_literal = c;
  return ret;
}

struct expr * expr_create_string_literal( const char *str ){
  struct expr *ret = expr_create(EXPR_STRING_LIT, 0, 0);
  ret->string_literal = str;
  return ret;
}

void expr_print( struct expr *e ){
  if (e->left != null){

  }
  if (e->right != null){

  }

  switch(e->kind){
    case EXPR_GROUP:
      if(e->left->kind == EXPR_GROUP) {
        expr_print(e->left);
      }
      else {
        printf("(");
        expr_print(e->left);
        printf(")");
      }
      break;

    case EXPR_MUL:
      printf("*");
      break;

    case EXPR_DIV:
      printf("/");
      break;

    case EXPR_ADD:
      printf("+");
      break;

    case EXPR_SUB:
      printf("-");
      break;

    case EXPR_G:
      printf(">");
      break;

    case EXPR_GE:
      printf(">=");
      break;

    case EXPR_L:
      printf("<");
      break;

    case EXPR_LE:
      printf("<=");
      break;

    case EXPR_E:
      printf("==");
      break;

    case EXPR_NE:
      printf("!=");
      break;

    case EXPR_AND:
      printf("&&");
      break;

    case EXPR_OR:
      printf("||");
      break;

    case EXPR_ASSIGN:
    printf("=");
      break;

    case EXPR_NAME:
      printf("%s", e->name);
      break;

    case EXPR_INT_LIT:
      printf("%d",e->literal_value);
      break;

    case EXPR_BOOL_LIT:
      if(e->literal_value == 0){ printf( "FALSE" ); }
      else { printf( "TRUE" ); }
      break;

    case EXPR_CHAR_LIT:
      printf("%c", e->literal_value);
      break;

    case EXPR_STRING_LIT:
      printf("%s",e->string_literal);
      break;

    defaut:
      printf("???");
      break;


  }


}


void expr_resolve( struct expr *e ){
    if(!e) return;

    if ( e->kind == EXPR_NAME ){
        e->symbol = scope_lookup(e->name);
    } else {
        expr_resolve( e->left );
        expr_resolve( e->right );
    }
}

struct type * expr_typecheck( struct expr *e ){
    if(!e) return 0;


    struct type *lt = expr_typecheck(e->left);
    struct type *rt = expr_typecheck(e->right);

    struct type *result;

    switch (e->kind) {
        case EXPR_STRING_LIT:
            result = type_create(TYPE_INTEGER,0,0);
            break;
        case EXPR_CHAR_LIT:
            result = type_create(TYPE_CHARACTER,0,0);
            break;
        case EXPR_BOOL_LIT:
            result = type_create(TYPE_BOOLEAN,0,0);
            break;
        case EXPR_INT_LIT:
            result = type_create(TYPE_INTEGER,0,0);
            break;
/*        case EXPR_NAME:
        case EXPR_ASSIGN:
        case EXPR_DEC:
        case EXPR_INC:
        case EXPR_NOT:
        case EXPR_NEG:
        case EXPR_OR:
        case EXPR_AND:
        case EXPR_NE:
        case EXPR_E:
        case EXPR_LE:
        case EXPR_L:
        case EXPR_GE:
        case EXPR_G:
        case EXPR_EXPON:
        case EXPR_MOD:
        case EXPR_SUB:
        case EXPR_ADD:
        case EXPR_DIV:
        case EXPR_MUL:
        case EXPR_GROUP:*/
        default:
            break;
    }

    type_delete(lt);
    type_delete(rt);

    return result;

}

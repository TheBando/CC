#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include "type.h"

typedef enum {
	EXPR_GROUP,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MOD,
	EXPR_EXPON,
	EXPR_G,
	EXPR_GE,
	EXPR_L,
	EXPR_LE,
	EXPR_E,
	EXPR_NE,
	EXPR_AND,
	EXPR_OR,
	EXPR_NEG,
	EXPR_NOT,
	EXPR_INC,
	EXPR_DEC,
	EXPR_ASSIGN,
	EXPR_NAME,
	EXPR_INT_LIT,
	EXPR_BOOL_LIT,
	EXPR_CHAR_LIT,
	EXPR_STRING_LIT
} expr_t;



struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );

void expr_print( struct expr *e );

void expr_resolve( struct expr * e);
struct type * expr_typecheck( struct expr *);


#endif

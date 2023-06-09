#ifndef TYPE_H
#define TYPE_H

#include <stdbool.h>

#include "param_list.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_AUTO,
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
void          type_print( struct type *t );
bool 		  type_equals( struct type *, struct type *);
struct type * type_copy(strut type *);
void 		  type_delete(struct type *);

#endif

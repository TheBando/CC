/* Tokens */
%token TOKEN_EOF
%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_BOOLEAN
%token TOKEN_CHAR
%token TOKEN_FALSE
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_IF
%token TOKEN_INTEGER
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING
%token TOKEN_TRUE
%token TOKEN_VOID
%token TOKEN_WHILE
%token TOKEN_CHAR_LIT
%token TOKEN_STRING_LIT
%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_NOT
%token TOKEN_INC
%token TOKEN_DEC
%token TOKEN_IDENT
%token TOKEN_EXP
%token TOKEN_MUL
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_LESS
%token TOKEN_LEQ
%token TOKEN_GREATER
%token TOKEN_GEQ
%token TOKEN_EQ
%token TOKEN_NEQ
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_ASSIGN
%token TOKEN_NUMBER
%token TOKEN_COLON
%token TOKEN_SEMI
%token TOKEN_COMMA
%token TOKEN_LPAREN
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LCURLY
%token TOKEN_RCURLY
%token TOKEN_ERROR

/* Establish precedence for ')' and "else" to resolve dangling else problem */
%nonassoc TOKEN_RPAREN
%nonassoc TOKEN_ELSE

%{

// Includes
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "type.h"

// Flex
extern int yylex();
extern int yyerror(char *str);

%}

%union {
	struct decl *decl;
	struct type *type;
	struct stmt *stmt;
	struct expr *expr;
	struct param_list *param_list;
	char *name;
};

%type <decl> program global_statement_list global_statement
%type <type> type array array_spec formal_array_spec
%type <stmt> statement_list statement block_statement if_statement expr_statement loop_statement decl_statement init_statement func_decl_statement func_def_statement ret_statement print_statement
%type <expr> expr assign_expr or_expr and_expr rel_expr add_expr mul_expr exp_expr postfix_expr primary_expr subscript_expr_list subscript_expr subscript_expr_list_tail init_expr unary_expr
%type <param_list> formal_param_list formal_param_list_tail formal_param param_list param_list_tail
%type <name> name


%%

program
	: global_statement_list {parser_result = $1; }
	;

global_statement_list
	: global_statement global_statement_list
		{ $$ = $1; $1->next = $2; }
	| %empty
		{ $$ = 0; }
	;

global_statement
	: decl_statement { $$ = $1; }
	| init_statement { $$ = $1; }
	| func_decl_statement { $$ = $1; }
	| func_def_statement { $$ = $1; }
	;

statement_list
	: statement statement_list { $$ = $1; $1->next = $2; }
	| %empty { $$ = 0; }
	;

statement
	: block_statement { $$ = $1; }
	| if_statement { $$ = $1; }
	| expr_statement { $$ = $1; }
	| loop_statement { $$ = $1; }
	| decl_statement { $$ = $1; }
	| init_statement { $$ = $1; }
	| func_decl_statement { $$ = $1; }
	| func_def_statement { $$ = $1; }
	| ret_statement { $$ = $1; }
	| print_statement { $$ = $1; }
	;

block_statement
	: TOKEN_LCURLY statement_list TOKEN_RCURLY { SS = expr_create(EXPR_GROUP, $2, 0); }
	;

if_statement
	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement
			{ $$ = stmt_create(STMT_IF, 0, 0, $3, 0, $5, 0, 0); }
	| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement TOKEN_ELSE statement
			{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
	;

expr_statement
	: expr TOKEN_SEMI { $$ = $1; }
	;

expr
	: assign_expr { $$ = $1; }
	;

assign_expr
	: assign_expr TOKEN_ASSIGN or_expr { $$ = expr_create(EXPR_ASSIGN, $1, $3); }
	| or_expr { $$ = $1; }
	;

or_expr
	: or_expr TOKEN_OR and_expr { $$ = expr_create(EXPR_OR, $1, $3); }
	| and_expr { $$ = $1; }
	;

and_expr
	: and_expr TOKEN_AND rel_expr { $$ = expr_create(EXPR_AND, $1, $3); }
	| rel_expr { $$ = $1; }
	;

rel_expr
	: rel_expr TOKEN_LESS add_expr { $$ = expr_create(EXPR_L, $1, $3); }
	| rel_expr TOKEN_LEQ add_expr { $$ = expr_create(EXPR_LE, $1, $3); }
	| rel_expr TOKEN_GREATER add_expr { $$ = expr_create(EXPR_G, $1, $3); }
	| rel_expr TOKEN_GEQ add_expr { $$ = expr_create(EXPR_GE, $1, $3); }
	| rel_expr TOKEN_EQ add_expr { $$ = expr_create(EXPR_E, $1, $3); }
	| rel_expr TOKEN_NEQ add_expr { $$ = expr_crreate(EXPR_NE, $1, $3); }
	| add_expr { $$ = $1; }
	;

add_expr
	: add_expr TOKEN_ADD mul_expr { $$ = expr_create(EXPR_ADD, $1, $3); }
	| add_expr TOKEN_SUB mul_expr { $$ = expr_create(EXPR_SUB, $1, $3); }
	| mul_expr { $$ = $1; }
	;

mul_expr
	: mul_expr TOKEN_MUL exp_expr { $$ = expr_create(EXPR_MUL, $1, $3); }
	| mul_expr TOKEN_DIV exp_expr { $$ = expr_create(EXPR_DIV, $1, $3); }
	| mul_expr TOKEN_MOD exp_expr { $$ = expr_create(EXPR_MOD, $1, $3); }
	| exp_expr { $$ = $1; }
	;

exp_expr
	: exp_expr TOKEN_EXP unary_expr { $$ = expr_create(EXPR_EXPON, $1, $3); }
	| unary_expr { $$ = $1; }
	;

unary_expr
	: TOKEN_SUB unary_expr { $$ = expr_create(EXPR_NEG, $2, 0); }
	| TOKEN_NOT unary_expr { $$ = expr_create(EXPR_NOT, $2, 0); }
	| postfix_expr { $$ = $1; }
	;

postfix_expr
	: postfix_expr TOKEN_INC { $$ = expr_create(EXPR_INC, $1, 0); }
	| postfix_expr TOKEN_DEC { $$ = expr_create(EXPR_DEC, $1, 0); }
	| primary_expr { $$ = $1; }
	;

primary_expr
	: TOKEN_IDENT
	| TOKEN_NUMBER
	| TOKEN_TRUE
	| TOKEN_FALSE
	| TOKEN_CHAR_LIT
	| TOKEN_STRING_LIT
	| TOKEN_LPAREN expr TOKEN_RPAREN
	| TOKEN_IDENT subscript_expr_list
	| TOKEN_IDENT TOKEN_LPAREN param_list TOKEN_RPAREN
	| array { $$ = $1; }
	;

subscript_expr_list
	: subscript_expr subscript_expr_list_tail { $$ = $1; $1->next = $2; }
	;

subscript_expr
	: TOKEN_LBRACKET expr TOKEN_RBRACKET { $$ = $2; }
	;

subscript_expr_list_tail
	: subscript_expr subscript_expr_list_tail
	| %empty { $$ = 0; }
	;

type
	: TOKEN_VOID { $$ = type_create(TYPE_VOID, 0, 0); }
	| TOKEN_INTEGER{ $$ = type_create(TYPE_VOID, 0, 0); }
	| TOKEN_CHAR { $$ = type_create(TYPE_CHARACTER, 0, 0); }
	| TOKEN_STRING { $$ = type_create(TYPE_STRING, 0, 0); }
	| TOKEN_BOOLEAN { $$ = type_create(TYPE_BOOLEAN, 0, 0); }
	| TOKEN_AUTO { $$ = type_create(TYPE_AUTO, 0, 0); }
	;

loop_statement
	: for_loop { $$ = $1; }
	| while_loop { $$ = $1; }
	;

for_loop
	: TOKEN_FOR TOKEN_LPAREN for_loop_param TOKEN_SEMI for_loop_param TOKEN_SEMI for_loop_param TOKEN_RPAREN statement
		{ $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
	;

for_loop_param
	: expr { $$ = $1; }
	| %empty { $$ = 0; }
	;

while_loop
	: TOKEN_WHILE TOKEN_LPAREN expr TOKEN_RPAREN statement
		{ $$ = stmt_create(STMT_WHILE, 0,  0, $3, 0, $5, 0, 0); }
	;

decl_statement
	: decl_expr TOKEN_SEMI { $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
	;

decl_expr
	: TOKEN_IDENT TOKEN_COLON array_spec type
		{ }
	;

array_spec
	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET array_spec
		{ $$ = type_create(TYPE_ARRAY, $5, 0); }
	| %empty { $$ = 0; }
	;

init_statement
	: init_expr TOKEN_SEMI
	{ $$ = $1; }
	;

init_expr
	: decl_expr TOKEN_ASSIGN expr { $$ = expr_create(EXPR_ASSIGN, $1, $3); }
	;

func_decl_expr
	: name TOKEN_COLON TOKEN_FUNCTION type TOKEN_LPAREN formal_param_list TOKEN_RPAREN
		{ $$ = decl_create($1, type_create(TYPE_FUNCTION, $4))}
	;

func_decl_statement
	: func_decl_expr TOKEN_SEMI
		{$$ = $1;}
	;

func_def_statement
	: func_decl_expr TOKEN_ASSIGN block_statement
		{$$ = expr_create(EXPR_ASSIGN, $1, $3); }
	;

formal_param_list
	: formal_param formal_param_list_tail { $$ = $1; $1->next = $2; }
	| %empty { $$ = 0; }
	;

formal_param_list_tail
	: TOKEN_COMMA formal_param formal_param_list_tail
		{ $$ = $2; $2->next = $3; }
	| %empty { $$ = 0; }
	;

formal_param
	: name TOKEN_COLON formal_array_spec type
		{ $$ = param_list_create($1, $3, 0); }
	;

formal_array_spec
	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET formal_array_spec
		{ $$ = type_create(TYPE_ARRAY, $3, 0); }
	| %empty { $$ = 0; }
	;

ret_statement
	: TOKEN_RETURN expr TOKEN_SEMI { $$ = stmt_create(STMT_RETURN, 0, 0, $2, 0, 0); }
	| TOKEN_RETURN TOKEN_SEMI 		 { $$ = stmt_create(STMT_RETURN, 0, 0, 0, 0, 0); }
	;

param_list
	: expr param_list_tail { $$ = $1; }
	| %empty { $$ = 0;}
	;

param_list_tail
	: TOKEN_COMMA expr param_list_tail
		{ $$->$2; $2->next = $3; }
	| %empty
	;

print_statement
	: TOKEN_PRINT expr_list TOKEN_SEMI { $$ = stmt_create(STMT_PRINT, 0, 0, $2, 0, 0, 0, 0); }
	| TOKEN_PRINT TOKEN_SEMI { $$ = stmt_create(STMT_PRINT, 0, 0, 0, 0, 0, 0, 0); }
	;

expr_list
	: expr expr_list_tail { $$ = $1; $1->next = $2; }
	;

expr_list_tail
	: TOKEN_COMMA expr_list { $$ = $2; }
	| %empty { $$ = 0; }
	;

name
	:	TOKEN_IDENT { $$ = strdup(yytext); }

array
	: TOKEN_LCURLY expr_list TOKEN_RCURLY
		{ $$ = $2; }
	;

%%

int yyerror(char *str) {
	printf("[PARSE ERROR]: %s\n",str);
	return 0;
}

%{
#include "ast.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(char *s);
int yylineno;

ASTNode* root = NULL;
%}

%code requires {
    typedef struct ASTNode ASTNode;
    typedef struct SymEntry SymEntry;
}

%union {
    int num;
    char* str;
    ASTNode* node;
    SymEntry* sentry;
}

%token <num> INTEGER
%token <str> IDENTIFIER STRING_LIT
%token INT FLOAT VOID IF ELSE WHILE FOR RETURN SCANF
%token EQ NE LE GE AND OR INC DEC NOT

%type <node> program function_decl block stmt_list stmt expr param_list params arg_list arg_list_nonempty
%type <num> type

%left OR
%left AND
%nonassoc EQ NE
%nonassoc '<' '>' LE GE
%left '+' '-'
%left '*' '/'
%right NOT
%right INC DEC
%nonassoc IF_PREC

%start program

%%

program:
      function_decl {
          root = $1;
      }
    | program function_decl {
          root = create_node(NODE_STMT_LIST, root, $2);
      }
    ;

function_decl:
      type IDENTIFIER '(' params ')' {
          if (lookup_symbol($2) != NULL) {
              fprintf(stderr, "Error at line %d: Function '%s' already declared\n", yylineno, $2);
              YYERROR;
          }
          enter_scope();
          if ($4) { /* params already added to symbol table */ }
      } block {
          exit_scope();
          $$ = create_func_node($1, $2, $4, $7);
          add_symbol($2, FUNC_SYM, $1);
      }
    ;

params:
      /* empty */ { $$ = NULL; }
    | param_list { $$ = $1; }
    ;

param_list:
      type IDENTIFIER {
          if (lookup_symbol($2) != NULL) {
              fprintf(stderr, "Error at line %d: Parameter '%s' already declared\n", yylineno, $2);
              YYERROR;
          }
          $$ = create_param_node($1, $2);
          add_symbol($2, PARAM_SYM, $1);
      }
    | param_list ',' type IDENTIFIER {
          if (lookup_symbol($4) != NULL) {
              fprintf(stderr, "Error at line %d: Parameter '%s' already declared\n", yylineno, $4);
              YYERROR;
          }
          $$ = append_param($1, $3, $4);
          add_symbol($4, PARAM_SYM, $3);
      }
    ;

block:
      '{' {
          enter_scope();
      } stmt_list '}' {
          exit_scope();
          $$ = $3;
      }
    ;

stmt_list:
      /* empty */ { $$ = NULL; }
    | stmt_list stmt { $$ = create_node(NODE_STMT_LIST, $1, $2); }
    ;

stmt:
      expr ';' { $$ = $1; }
    | type IDENTIFIER ';' {
          if (lookup_symbol($2) != NULL) {
              fprintf(stderr, "Error at line %d: Variable '%s' already declared\n", yylineno, $2);
              YYERROR;
          }
          add_symbol($2, VAR_SYM, $1);
          $$ = create_var_decl_node($1, $2);
      }
    | IF '(' expr ')' stmt %prec IF_PREC {
          $$ = create_if_node($3, $5, NULL);
      }
    | IF '(' expr ')' stmt ELSE stmt {
          $$ = create_if_node($3, $5, $7);
      }
    | WHILE '(' expr ')' stmt {
          $$ = create_while_node($3, $5);
      }
    | RETURN expr ';' {
          $$ = create_return_node($2);
      }
    | block
    ;

expr:
      expr '+' expr { $$ = create_op_node('+', $1, $3); }
    | expr '-' expr { $$ = create_op_node('-', $1, $3); }
    | expr '*' expr { $$ = create_op_node('*', $1, $3); }
    | expr '/' expr { $$ = create_op_node('/', $1, $3); }
    | expr '<' expr { $$ = create_op_node('<', $1, $3); }
    | expr '>' expr { $$ = create_op_node('>', $1, $3); }
    | expr LE expr  { $$ = create_op_node(LE, $1, $3); }
    | expr GE expr  { $$ = create_op_node(GE, $1, $3); }
    | expr EQ expr  { $$ = create_op_node(EQ, $1, $3); }
    | expr NE expr  { $$ = create_op_node(NE, $1, $3); }
    | expr AND expr { $$ = create_op_node(AND, $1, $3); }
    | expr OR expr  { $$ = create_op_node(OR, $1, $3); }
    | NOT expr      { $$ = create_op_node(NOT, NULL, $2); }
    | '-' expr %prec INC { $$ = create_op_node('-', NULL, $2); }
    | '+' expr %prec INC { $$ = create_op_node('+', NULL, $2); }
    | IDENTIFIER INC { $$ = create_op_node(INC, create_var_node($1), NULL); }
    | IDENTIFIER DEC { $$ = create_op_node(DEC, create_var_node($1), NULL); }
    | INTEGER { $$ = create_const_node(INT_TYPE, $1); }
    | STRING_LIT { $$ = create_string_node($1); }
    | IDENTIFIER { $$ = create_var_node($1); }
    | IDENTIFIER '=' expr { $$ = create_assign_node($1, $3); }
    | IDENTIFIER '(' ')' { $$ = create_func_call_node($1, NULL); }
    | IDENTIFIER '(' arg_list ')' { $$ = create_func_call_node($1, $3); }
    | '(' expr ')' { $$ = $2; }
    ;

arg_list:
      /* empty */ { $$ = NULL; }
    | arg_list_nonempty { $$ = $1; }
    ;

arg_list_nonempty:
      expr { $$ = create_arg_list_node($1); }
    | arg_list_nonempty ',' expr { $$ = append_arg($1, $3); }
    ;

type:
      INT { $$ = INT_TYPE; }
    | FLOAT { $$ = FLOAT_TYPE; }
    | VOID { $$ = VOID_TYPE; }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error at line %d: %s\n", yylineno, s);
}

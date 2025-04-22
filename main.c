#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtab.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNode* root;
extern void print_ast(ASTNode* node, int indent);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error opening input file");
            return 1;
        }
    } else {
        yyin = stdin;
        printf("Enter your C code (Ctrl+D to end input):\n");
    }

    if (yyparse() == 0) {
        printf("Parsing completed successfully.\n");
        if (root) {
            print_ast(root, 0);
        }
        print_symbols();
    } else {
        printf("Parsing failed.\n");
    }

    if (yyin != stdin) fclose(yyin);
    return 0;
}

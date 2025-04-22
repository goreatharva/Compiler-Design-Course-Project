#include "ast.h"
#include <stdio.h>

void generate_icode(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_FUNC:
            printf("Generating code for function: %s\n", node->name);
            generate_icode(node->left);
            generate_icode(node->right);
            break;
        // Add cases for other node types as needed
        default:
            break;
    }
}

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ASTNode* create_node_internal(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->var_type = INT_TYPE;
    node->name = NULL;
    node->int_val = 0;
    node->left = NULL;
    node->right = NULL;
    node->args = NULL;
    node->next = NULL;
    return node;
}

ASTNode* create_func_node(VarType ret_type, char* name, ASTNode* params, ASTNode* block) {
    ASTNode* node = create_node_internal(NODE_FUNC);
    node->var_type = ret_type;
    node->name = strdup(name);
    node->left = params;
    node->right = block;
    return node;
}

ASTNode* create_param_node(VarType type, char* name) {
    ASTNode* node = create_node_internal(NODE_PARAM);
    node->var_type = type;
    node->name = strdup(name);
    return node;
}

ASTNode* append_param(ASTNode* param_list, VarType type, char* name) {
    ASTNode* new_param = create_param_node(type, name);
    if (!param_list) return new_param;
    ASTNode* temp = param_list;
    while (temp->next) temp = temp->next;
    temp->next = new_param;
    return param_list;
}

ASTNode* create_node(NodeType type, ASTNode* left, ASTNode* right) {
    ASTNode* node = create_node_internal(type);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_var_decl_node(VarType type, char* name) {
    ASTNode* node = create_node_internal(NODE_VAR_DECL);
    node->var_type = type;
    node->name = strdup(name);
    return node;
}

ASTNode* create_if_node(ASTNode* cond, ASTNode* then_stmt, ASTNode* else_stmt) {
    ASTNode* node = create_node_internal(NODE_IF);
    node->left = cond;
    node->right = then_stmt;
    if (else_stmt) then_stmt->next = else_stmt;
    return node;
}

ASTNode* create_while_node(ASTNode* cond, ASTNode* body) {
    ASTNode* node = create_node_internal(NODE_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

ASTNode* create_return_node(ASTNode* expr) {
    ASTNode* node = create_node_internal(NODE_RETURN);
    node->left = expr;
    return node;
}

ASTNode* create_op_node(int op, ASTNode* left, ASTNode* right) {
    ASTNode* node = create_node_internal(NODE_OP);
    node->int_val = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_const_node(VarType type, int val) {
    ASTNode* node = create_node_internal(NODE_CONST);
    node->var_type = type;
    node->int_val = val;
    return node;
}

ASTNode* create_var_node(char* name) {
    ASTNode* node = create_node_internal(NODE_VAR);
    node->name = strdup(name);
    return node;
}

ASTNode* create_assign_node(char* name, ASTNode* expr) {
    ASTNode* node = create_node_internal(NODE_ASSIGN);
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* create_func_call_node(char* name, ASTNode* args) {
    ASTNode* node = create_node_internal(NODE_FUNC_CALL);
    node->name = strdup(name);
    node->args = args;
    return node;
}

ASTNode* create_printf_node(char* str) {
    ASTNode* node = create_node_internal(NODE_PRINTF);
    node->name = strdup(str);
    return node;
}

ASTNode* create_arg_list_node(ASTNode* expr) {
    return expr;
}

ASTNode* append_arg(ASTNode* arg_list, ASTNode* expr) {
    if (!arg_list) return expr;
    ASTNode* temp = arg_list;
    while (temp->next) temp = temp->next;
    temp->next = expr;
    return arg_list;
}

ASTNode* create_string_node(char* str) {
    ASTNode* node = create_node_internal(NODE_STRING);
    node->name = strdup(str);
    node->var_type = VOID_TYPE;
    return node;
}

void free_ast(ASTNode* node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->args);
    free_ast(node->next);
    if (node->name) free(node->name);
    free(node);
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    print_indent(indent);
    switch (node->type) {
        case NODE_FUNC:
            printf("Function: %s ReturnType: %d\n", node->name, node->var_type);
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case NODE_PARAM:
            printf("Param: %s Type: %d\n", node->name, node->var_type);
            print_ast(node->next, indent);
            break;
        case NODE_VAR_DECL:
            printf("VarDecl: %s Type: %d\n", node->name, node->var_type);
            break;
        case NODE_STMT_LIST:
            print_ast(node->left, indent);
            print_ast(node->right, indent);
            break;
        case NODE_IF:
            printf("If:\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            if (node->right && node->right->next) {
                print_indent(indent);
                printf("Else:\n");
                print_ast(node->right->next, indent + 1);
            }
            break;
        case NODE_WHILE:
            printf("While:\n");
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case NODE_RETURN:
            printf("Return:\n");
            print_ast(node->left, indent + 1);
            break;
        case NODE_OP:
            if (node->int_val >= 32 && node->int_val <= 126)
                printf("Op: %c\n", (char)node->int_val);
            else
                printf("Op: %d\n", node->int_val);
            print_ast(node->left, indent + 1);
            print_ast(node->right, indent + 1);
            break;
        case NODE_CONST:
            printf("Const: %d\n", node->int_val);
            break;
        case NODE_VAR:
            printf("Var: %s\n", node->name);
            break;
        case NODE_ASSIGN:
            printf("Assign: %s\n", node->name);
            print_ast(node->left, indent + 1);
            break;
        case NODE_FUNC_CALL:
            printf("FuncCall: %s\n", node->name);
            print_ast(node->args, indent + 1);
            break;
        case NODE_PRINTF:
            printf("Printf: %s\n", node->name);
            break;
        case NODE_ARG_LIST:
            print_ast(node->left, indent);
            print_ast(node->right, indent);
            break;
        case NODE_STRING:
            printf("String: %s\n", node->name);
            break;
        default:
            printf("Unknown node type %d\n", node->type);
            break;
    }
    print_ast(node->next, indent);
}

#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct ASTNode ASTNode;

typedef enum {
    NODE_FUNC,
    NODE_PARAM_LIST,
    NODE_PARAM,
    NODE_STMT_LIST,
    NODE_VAR_DECL,
    NODE_IF,
    NODE_WHILE,
    NODE_RETURN,
    NODE_OP,
    NODE_CONST,
    NODE_VAR,
    NODE_ASSIGN,
    NODE_FUNC_CALL,
    NODE_PRINTF,
    NODE_ARG_LIST,
    NODE_STRING
} NodeType;

typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    VOID_TYPE
} VarType;

struct ASTNode {
    NodeType type;
    VarType var_type;
    char* name;
    int int_val; // For operators or integer constants
    ASTNode* left;
    ASTNode* right;
    ASTNode* args;
    ASTNode* next;
};

ASTNode* create_func_node(VarType ret_type, char* name, ASTNode* params, ASTNode* block);
ASTNode* create_param_node(VarType type, char* name);
ASTNode* append_param(ASTNode* param_list, VarType type, char* name);
ASTNode* create_node(NodeType type, ASTNode* left, ASTNode* right);
ASTNode* create_var_decl_node(VarType type, char* name);
ASTNode* create_if_node(ASTNode* cond, ASTNode* then_stmt, ASTNode* else_stmt);
ASTNode* create_while_node(ASTNode* cond, ASTNode* body);
ASTNode* create_return_node(ASTNode* expr);
ASTNode* create_op_node(int op, ASTNode* left, ASTNode* right);
ASTNode* create_const_node(VarType type, int val);
ASTNode* create_var_node(char* name);
ASTNode* create_assign_node(char* name, ASTNode* expr);
ASTNode* create_func_call_node(char* name, ASTNode* args);
ASTNode* create_printf_node(char* str);
ASTNode* create_arg_list_node(ASTNode* expr);
ASTNode* append_arg(ASTNode* arg_list, ASTNode* expr);
ASTNode* create_string_node(char* str);

void free_ast(ASTNode* node);
void print_ast(ASTNode* node, int indent);

#endif

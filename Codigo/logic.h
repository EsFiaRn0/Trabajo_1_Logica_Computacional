#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>

typedef enum {
    VAR,       // Variable proposicional
    NOT,       // Negación
    AND,       // Conjunción
    OR,        // Disyunción
    IMPL       // Implicación
} NodeType;

// Nodo del árbol lógico
typedef struct Node {
    NodeType type;
    char* var_name; 

    struct Node* left;
    struct Node* right;
} Node;

Node* create_var(const char* name);
Node* create_op(NodeType type, Node* left, Node* right);
Node* create_not(Node* child);
Node* copy_tree(Node* node);

Node* impl_free(Node* node);
Node* to_cnf(Node* node);

void print_formula(Node* node);
void free_tree(Node* node);

int get_num_vars(Node* formula);
bool solve_sat(Node* formula, int num_vars);

#endif

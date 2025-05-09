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

// Estructura que representa un nodo en el árbol lógico
typedef struct Node {
    NodeType type;      // Tipo del nodo (variable, operador lógico)
    char* var_name;     // Nombre de la variable si es un nodo VAR

    struct Node* left;  // Hijo izquierdo del nodo
    struct Node* right; // Hijo derecho del nodo
} Node;

Node* create_var(const char* name);
Node* create_op(NodeType type, Node* left, Node* right);
Node* create_not(Node* child);
Node* copy_tree(Node* node);

void print_formula(Node* node);
void free_tree(Node* node);

int get_num_vars(Node* formula);
bool solve_sat(Node* formula, int num_vars);

#endif

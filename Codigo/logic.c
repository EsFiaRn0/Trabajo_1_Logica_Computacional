
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

Node* create_var(const char* name) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Error al asignar memoria para un nodo VAR\n");
        exit(1);
    }
    node->type = VAR;
    node->var_name = strdup(name);
    node->left = node->right = NULL;
    return node;
}

Node* create_op(NodeType type, Node* left, Node* right) {
    Node* node = (Node*) malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Error al asignar memoria para un nodo de operación\n");
        exit(1);
    }
    node->type = type;
    node->left = left;
    node->right = right;
    node->var_name = NULL;
    return node;
}

Node* create_not(Node* child) {
    Node* node = (Node*) malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Error al asignar memoria para un nodo NOT\n");
        exit(1);
    }
    node->type = NOT;
    node->left = child;
    node->right = NULL;
    node->var_name = NULL;
    return node;
}

Node* impl_free(Node* node) {
    if (!node) return NULL;

    if (node->type == IMPL) {
        Node* left = impl_free(node->left);
        Node* right = impl_free(node->right);
        Node* neg_left = create_not(left);
        return create_op(OR, neg_left, right);
    } else if (node->type == AND || node->type == OR) {
        Node* left = impl_free(node->left);
        Node* right = impl_free(node->right);
        return create_op(node->type, left, right);
    } else if (node->type == NOT) {
        Node* child = impl_free(node->left);
        return create_not(child);
    } else {
        return create_var(node->var_name);
    }
}

Node* copy_tree(Node* node) {
    if (!node) return NULL;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Error al asignar memoria en copy_tree\n");
        exit(1);
    }
    new_node->type = node->type;
    new_node->var_name = node->var_name ? strdup(node->var_name) : NULL;
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    return new_node;
}

Node* distribute(Node* a, Node* b) {
    if (!a || !b) return NULL;

    if (a->type == AND) {
        return create_op(AND,
            distribute(a->left, copy_tree(b)),
            distribute(a->right, copy_tree(b)));
    } else if (b->type == AND) {
        return create_op(AND,
            distribute(copy_tree(a), b->left),
            distribute(copy_tree(a), b->right));
    } else {
        return create_op(OR, copy_tree(a), copy_tree(b));
    }
}

Node* to_cnf(Node* node) {
    if (!node) return NULL;

    switch (node->type) {
        case OR: {
            Node* left = to_cnf(node->left);
            Node* right = to_cnf(node->right);
            return distribute(left, right);
        }
        case AND: {
            return create_op(AND,
                to_cnf(node->left),
                to_cnf(node->right));
        }
        case NOT: {
            return create_not(to_cnf(node->left));
        }
        case VAR: {
            return create_var(node->var_name);
        }
        default:
            return NULL;
    }
}


// Función para imprimir la fórmula.
void print_formula(Node* node) {
    if (!node) return;

    switch (node->type) {
        case VAR:
            printf("%s", node->var_name);
            break;
        case NOT:
            printf("~");
            print_formula(node->left);
            break;
        case AND:
            printf("(");
            print_formula(node->left);
            printf(" & ");
            print_formula(node->right);
            printf(")");
            break;
        case OR:
            printf("(");
            print_formula(node->left);
            printf(" | ");
            print_formula(node->right);
            printf(")");
            break;
        case IMPL:
            printf("(");
            print_formula(node->left);
            printf(" -> ");
            print_formula(node->right);
            printf(")");
            break;
    }
}

// Función para liberar todo el árbol
void free_tree(Node* node) {
    if (!node) return;

    if (node->left) free_tree(node->left);
    if (node->right) free_tree(node->right);

    if (node->var_name) {
        free(node->var_name);
        node->var_name = NULL; 
    }

    free(node);
    node = NULL;
}

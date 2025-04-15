#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

Node* create_var(const char* name) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = VAR;
    node->var_name = strdup(name);
    node->left = node->right = NULL;
    return node;
}

Node* create_op(NodeType type, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->left = left;
    node->right = right;
    node->var_name = NULL;
    return node;
}

Node* create_not(Node* child) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = NOT;
    node->left = child;
    node->right = NULL;
    node->var_name = NULL;
    return node;
}

// Remueve las implicaciones: A → B se convierte en ¬A ∨ B
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

// Convierte a forma normal negativa (NNF)
Node* to_nnf(Node* node) {
    if (!node) return NULL;

    if (node->type == NOT) {
        Node* child = node->left;
        if (child->type == NOT) {
            return to_nnf(child->left); // ~~A => A
        } else if (child->type == AND) {
            return create_op(OR,
                to_nnf(create_not(child->left)),
                to_nnf(create_not(child->right)));
        } else if (child->type == OR) {
            return create_op(AND,
                to_nnf(create_not(child->left)),
                to_nnf(create_not(child->right)));
        } else {
            return create_not(to_nnf(child));
        }
    } else if (node->type == AND || node->type == OR) {
        return create_op(node->type,
            to_nnf(node->left),
            to_nnf(node->right));
    } else {
        return create_var(node->var_name);
    }
}

// Convierte a CNF (distribuyendo OR sobre AND)
Node* distribute(Node* a, Node* b) {
    if (!a || !b) return NULL;

    if (a->type == AND) {
        return create_op(AND,
            distribute(a->left, b),
            distribute(a->right, b));
    } else if (b->type == AND) {
        return create_op(AND,
            distribute(a, b->left),
            distribute(a, b->right));
    } else {
        return create_op(OR, a, b);
    }
}

Node* to_cnf(Node* node) {
    if (!node) return NULL;

    if (node->type == OR) {
        Node* left = to_cnf(node->left);
        Node* right = to_cnf(node->right);
        return distribute(left, right);
    } else if (node->type == AND) {
        return create_op(AND,
            to_cnf(node->left),
            to_cnf(node->right));
    } else if (node->type == NOT || node->type == VAR) {
        return node;
    }
    return NULL;
}

// Imprimir fórmula
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

// Liberar memoria del árbol
void free_tree(Node* node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    if (node->var_name) free(node->var_name);
    free(node);
}

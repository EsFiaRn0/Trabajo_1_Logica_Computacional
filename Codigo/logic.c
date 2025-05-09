
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

/**
 * Crea un nodo de tipo variable (VAR) en el árbol sintáctico.
 *
 * Entrada:
 *   name - Nombre de la variable (por ejemplo, "P")
 *
 * Salida:
 *   Puntero al nodo creado con el nombre y sin hijos
 */
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

/**
 * Crea un nodo de operación binaria (AND, OR, IMPL) en el árbol sintáctico.
 *
 * Entradas:
 *   type  - Tipo de operación lógica
 *   left  - Hijo izquierdo
 *   right - Hijo derecho
 *
 * Salida:
 *   Puntero al nodo de operación creado
 */
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

/**
 * Crea un nodo de negación lógica (NOT) con un único hijo.
 *
 * Entrada:
 *   child - Nodo hijo a negar
 *
 * Salida:
 *   Puntero al nodo NOT creado
 */
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

/**
 * Imprime una fórmula lógica.
 *
 * Entrada:
 *   node - Árbol sintáctico de la fórmula
 */
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

/**
 * Libera la memoria asignada a todo un árbol de nodos.
 *
 * Entrada:
 *   node - Árbol a liberar
 */
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

/**
 * Marca las variables presentes en el árbol lógico usando un arreglo de flags.
 *
 * Entrada:
 *   node - Árbol lógico
 *   vars - Arreglo booleano de tamaño 26 (A-Z) para marcar variables usadas
 */
static void mark_vars(Node* node, bool vars[26]) {
    if (!node) return;

    if (node->type == VAR && node->var_name) {
        char c = node->var_name[0];
        if (c >= 'A' && c <= 'Z')
            vars[c - 'A'] = true;
    }

    mark_vars(node->left, vars);
    mark_vars(node->right, vars);
}

/**
 * Cuenta cuántas variables proposicionales distintas hay en un árbol lógico.
 *
 * Entrada:
 *   node - Árbol lógico
 *
 * Salida:
 *   Cantidad de variables únicas encontradas (basado en A-Z)
 */
int get_num_vars(Node* node) {
    bool vars[26] = {false};

    mark_vars(node, vars);

    int count = 0;
    for (int i = 0; i < 26; i++)
        if (vars[i]) count++;

    return count;
}

/**
 * Evalúa una fórmula lógica con una asignación de valores de verdad.
 *
 * Entradas:
 *   node       - Árbol lógico a evaluar
 *   assignment - Arreglo booleano con valores de verdad para cada variable (A-Z)
 *
 * Salida:
 *   Resultado de la evaluación (true o false)
 */
bool eval_formula(Node* node, bool assignment[26]) {
    if (!node) return false;

    switch (node->type) {
        case VAR:
            return assignment[node->var_name[0] - 'A'];
        case NOT:
            return !eval_formula(node->left, assignment);
        case AND:
            return eval_formula(node->left, assignment) &&
                   eval_formula(node->right, assignment);
        case OR:
            return eval_formula(node->left, assignment) ||
                   eval_formula(node->right, assignment);
        default:
            return false;
    }
}

/**
 * Realiza backtracking para encontrar una asignación que satisfaga la fórmula.
 *
 * Entradas:
 *   node       - Fórmula lógica en forma de árbol
 *   assignment - Arreglo de verdad actual en exploración
 *   idx        - Índice actual (variable A-Z) a probar
 *   max_vars   - Número total de variables relevantes
 *
 * Salida:
 *   true si se encuentra una asignación satisfactoria, false si no
 */
bool solve_rec(Node* node, bool assignment[26], int idx, int max_vars) {
    if (idx == max_vars)
        return eval_formula(node, assignment);

    assignment[idx] = false;
    if (solve_rec(node, assignment, idx + 1, max_vars)) return true;

    assignment[idx] = true;
    if (solve_rec(node, assignment, idx + 1, max_vars)) return true;

    return false;
}

/**
 * Determina si una fórmula en CNF es satisfacible utilizando backtracking.
 *
 * Entrada:
 *   formula   - Árbol de nodos
 *   num_vars  - Número total de variables proposicionales
 *
 * Salida:
 *   true si la fórmula es satisfacible, false si no lo es
 */
bool solve_sat(Node* formula, int num_vars) {
    bool assignment[26] = {false};
    return solve_rec(formula, assignment, 0, num_vars);
}

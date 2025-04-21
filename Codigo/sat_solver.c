#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_satisfied(Node* clause, bool* assignment, int num_vars);
bool backtrack(Node* formula, bool* assignment, int num_vars, int index);

/**
 * Verifica si una cláusula está satisfecha bajo una asignación dada.
 *
 * Entrada:
 *   clause - Nodo que representa la cláusula (puede ser literal o su negación)
 *   assignment - Arreglo de valores booleanos para cada variable (por índice)
 *   num_vars - Número total de variables
 *
 * Salida:
 *   true si la cláusula está satisfecha, false en caso contrario
 */
bool is_satisfied(Node* clause, bool* assignment, int num_vars){

    if (!clause) return false;

    if (clause->type == VAR){
        int var_index = atoi(clause->var_name);
        return assignment[var_index];
    } 
    else if (clause->type == NOT){
        return !assignment[atoi(clause->left->var_name)];
    }
    return false;
}


/**
 * Algoritmo de backtracking que prueba todas las asignaciones posibles
 * para verificar la satisfacibilidad de la fórmula.
 *
 * Entrada:
 *   formula - Árbol lógico en CNF (lista de cláusulas unidas por conjunción)
 *   assignment - Arreglo de valores booleanos para las variables
 *   num_vars - Número total de variables
 *   index - Índice actual de variable que se está asignando
 *
 * Salida:
 *   true si se encuentra una asignación satisfactoria, false en caso contrario
 */
bool backtrack(Node* formula, bool* assignment, int num_vars, int index){
    if (index == num_vars){
        Node* current_clause = formula;
        while (current_clause){
            if (!is_satisfied(current_clause, assignment, num_vars)){
                return false;
            }
            current_clause = current_clause->right;
        }
        return true;
    }

    assignment[index] = true;
    if (backtrack(formula, assignment, num_vars, index + 1)) {
        return true;
    }

    assignment[index] = false;
    if (backtrack(formula, assignment, num_vars, index + 1)) {
        return true;
    }

    return false;
}

/**
 * Determina si una fórmula en CNF es satisfacible utilizando backtracking.
 *
 * Entrada:
 *   formula - Árbol de nodos en forma normal conjuntiva (CNF)
 *   num_vars - Número total de variables proposicionales
 *
 * Salida:
 *   true si la fórmula es satisfacible, false si no lo es
 */
bool solve_sat(Node* formula, int num_vars){
    bool* assignment = malloc(num_vars * sizeof(bool));
    if (!assignment) {
        fprintf(stderr, "Error al asignar memoria.\n");
        return false;
    }

    for (int i = 0; i < num_vars; i++){
        assignment[i] = false;
    }

    bool result = backtrack(formula, assignment, num_vars, 0);

    free(assignment);
    return result;
}
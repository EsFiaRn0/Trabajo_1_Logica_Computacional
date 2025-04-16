#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "postfix_converter.h"

#define MAX_STACK 100
#define MAX_OUTPUT 2048

typedef struct {
    const char* symbol;
    int precedence;
    int right_associative;
} Operator;

Operator operators[] = {
    {"\\neg", 3, 1},
    {"\\wedge", 2, 0},
    {"\\vee", 1, 0},
    {"\\rightarrow", 0, 0},
    {NULL, 0, 0}
};

int precedence(const char* op) {
    for (int i = 0; operators[i].symbol; i++) {
        if (strcmp(op, operators[i].symbol) == 0)
            return operators[i].precedence;
    }
    return -1;
}

int is_right_associative(const char* op) {
    for (int i = 0; operators[i].symbol; i++) {
        if (strcmp(op, operators[i].symbol) == 0)
            return operators[i].right_associative;
    }
    return 0;
}

int is_operator(const char* token) {
    for (int i = 0; operators[i].symbol; i++) {
        if (strcmp(token, operators[i].symbol) == 0)
            return 1;
    }
    return 0;
}

char* convert_to_postfix(const char* input) {
    char output[MAX_OUTPUT] = "";
    const char* stack[MAX_STACK];
    int top = 0;

    char token[256];

    for (int i = 0; input[i];) {
        if (isspace(input[i]) || input[i] == '$') {
            i++;
            continue;
        } else if (input[i] == '(' || input[i] == ')') {
            token[0] = input[i];
            token[1] = '\0';
            i++;
        } else if (input[i] == '\\') {
            int j = 0;
            token[j++] = input[i++];
            while (isalpha(input[i])) token[j++] = input[i++];
            token[j] = '\0';
        } else if (isalnum(input[i])) {
            int j = 0;
            while (isalnum(input[i])) token[j++] = input[i++];
            token[j] = '\0';
        } else {
            token[0] = input[i++];
            token[1] = '\0';
        }

        if (is_operator(token)) {
            while (top > 0 && is_operator(stack[top - 1]) &&
                ((precedence(stack[top - 1]) > precedence(token)) ||
                 (precedence(stack[top - 1]) == precedence(token) && !is_right_associative(token)))) {
                strcat(output, stack[--top]);
                strcat(output, " ");
            }
            stack[top++] = strdup(token);
        } else if (strcmp(token, "(") == 0) {
            stack[top++] = strdup(token);
        } else if (strcmp(token, ")") == 0) {
            while (top > 0 && strcmp(stack[top - 1], "(") != 0) {
                strcat(output, stack[--top]);
                strcat(output, " ");
            }
            if (top == 0) {
                fprintf(stderr, "Error: paréntesis desbalanceados\n");
                exit(1);
            }
            free((void*)stack[--top]);
        } else {
            strcat(output, token);
            strcat(output, " ");
        }
    }

    while (top > 0) {
        if (strcmp(stack[top - 1], "(") == 0) {
            fprintf(stderr, "Error: paréntesis desbalanceados\n");
            exit(1);
        }
        strcat(output, stack[--top]);
        strcat(output, " ");
    }

    return strdup(output);
}
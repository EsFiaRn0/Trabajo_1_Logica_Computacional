#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "postfix_converter.h"

#define MAX_STACK 10000
#define MAX_OUTPUT 16384

static Operator operators[] = {
    {"\\neg", 3, 1},
    {"\\wedge", 2, 0},
    {"\\vee", 1, 0},
    {"\\rightarrow", 0, 0},
    {NULL, 0, 0}
};

int precedence(const char* op) {
    for (int i = 0; operators[i].symbol != NULL; i++) {
        if (strcmp(op, operators[i].symbol) == 0)
            return operators[i].precedence;
    }
    return -1;
}

int is_right_associative(const char* op) {
    for (int i = 0; operators[i].symbol != NULL; i++) {
        if (strcmp(op, operators[i].symbol) == 0)
            return operators[i].right_associative;
    }
    return 0;
}

int is_operator(const char* token) {
    for (int i = 0; operators[i].symbol != NULL; i++) {
        if (strcmp(token, operators[i].symbol) == 0)
            return 1;
    }
    return 0;
}

char* convert_to_postfix(const char* input) {
    char output[MAX_OUTPUT];
    output[0] = '\0';
    int out_pos = 0;

    const char* stack[MAX_STACK];
    int top = 0;

    char token[256];

    for (int i = 0; input[i];) {
        if (isspace(input[i]) || input[i] == '$') {
            i++;
            continue;
        }

        if (input[i] == '(' || input[i] == ')') {
            token[0] = input[i++];
            token[1] = '\0';
        } else if (input[i] == '\\') {
            int j = 0;
            token[j++] = input[i++];
            while (isalpha(input[i])) token[j++] = input[i++];
            token[j] = '\0';
        } else if (isalpha(input[i])) {
            int j = 0;
            token[j++] = input[i++];
            while (isalnum(input[i])) token[j++] = input[i++];
            if (input[i] == '_') {
                token[j++] = input[i++];
                if (input[i] == '{') {
                    token[j++] = input[i++];
                    while (isdigit(input[i])) token[j++] = input[i++];
                    if (input[i] == '}') token[j++] = input[i++];
                } else {
                    while (isdigit(input[i])) token[j++] = input[i++];
                }
            }
            token[j] = '\0';
        } else {
            token[0] = input[i++];
            token[1] = '\0';
        }

        if (is_operator(token)) {
            while (top > 0 && is_operator(stack[top - 1])) {
                const char* top_op = stack[top - 1];
                if ((precedence(top_op) > precedence(token)) ||
                    (precedence(top_op) == precedence(token) && !is_right_associative(token))) {
                    out_pos += snprintf(output + out_pos, MAX_OUTPUT - out_pos, "%s ", stack[--top]);
                    free((void*)stack[top]);
                } else {
                    break;
                }
            }
            stack[top++] = strdup(token);
        } else if (strcmp(token, "(") == 0) {
            stack[top++] = strdup(token);
        } else if (strcmp(token, ")") == 0) {
            int matched = 0;
            while (top > 0) {
                if (strcmp(stack[top - 1], "(") == 0) {
                    free((void*)stack[--top]);
                    matched = 1;
                    break;
                }
                out_pos += snprintf(output + out_pos, MAX_OUTPUT - out_pos, "%s ", stack[--top]);
                free((void*)stack[top]);
            }
            if (!matched) {
                fprintf(stderr, "Error: paréntesis desbalanceados\n");
                while (--top >= 0) free((void*)stack[top]);
                return NULL;
            }
        } else {
            out_pos += snprintf(output + out_pos, MAX_OUTPUT - out_pos, "%s ", token);
        }
    }

    while (top > 0) {
        if (strcmp(stack[top - 1], "(") == 0) {
            fprintf(stderr, "Error: paréntesis desbalanceados\n");
            while (--top >= 0) free((void*)stack[top]);
            return NULL;
        }
        out_pos += snprintf(output + out_pos, MAX_OUTPUT - out_pos, "%s ", stack[--top]);
        free((void*)stack[top]);
    }

    return strdup(output);
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

#define MAX_STACK 1024
Node* stack[MAX_STACK];
int top = 0;

void push(Node* n) {
    if (top < MAX_STACK) {
        stack[top++] = n;
    } else {
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
}

Node* pop() {
    if (top > 0) {
        return stack[--top];
    } else {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
}

void reset_stack() {
    top = 0;
}
%}

%option noyywrap

%%

[ \t\n]+             ;
"("                  ;
"\$"                 ;
")"                  ;

"\\rightarrow"        {
                        if (top < 2) {
                          fprintf(stderr, "Error: operador '\\rightarrow' requiere dos operandos\n");
                          exit(1);
                        }
                        Node* right = pop();
                        Node* left = pop();
                        Node* neg_right = create_not(right);
                        Node* and_node = create_op(AND, left, neg_right);
                        Node* impl_node = create_not(and_node);
                        push(impl_node);
                      }

"\\neg"               {
                        Node* child = pop();
                        push(create_not(child));
                      }

"\\wedge"             {
                        Node* right = pop();
                        Node* left = pop();
                        push(create_op(AND, left, right));
                      }

"\\vee"               {
                        if (top < 2) {
                            fprintf(stderr, "Error: operador '\\vee' requiere dos operandos\n");
                            exit(1);
                        }
                        Node* right = pop();
                        Node* left = pop();
                        Node* neg_left = create_not(left);
                        Node* neg_right = create_not(right);
                        Node* and_node = create_op(AND, neg_left, neg_right);
                        Node* or_node = create_not(and_node);
                        push(or_node);
                     }

[a-zA-Z][a-zA-Z0-9]*(_[0-9]+|_\{[0-9]+\})?  {
                        push(create_var(yytext));
                     }

.                    { printf("Caracter no reconocido: %s\n", yytext); }

%%
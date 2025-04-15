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
        printf("Pushed to stack: %p\n", n);
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

[ \t\n]+              ;
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
                        push(create_op(IMPL, left, right));
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
                        Node* right = pop();
                        Node* left = pop();
                        push(create_op(OR, left, right));
                      }

[a-zA-Z][a-zA-Z0-9]*  {
                        printf("Processing variable: %s\n", yytext);
                        push(create_var(yytext));
                      }

.                    { printf("Caracter no reconocido: %s\n", yytext); }

%%
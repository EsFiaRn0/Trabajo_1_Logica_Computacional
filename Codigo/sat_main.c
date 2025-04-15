#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logic.h"
#include "postfix_converter.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yylex();
extern void yyrestart(FILE*);
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

extern Node* stack[];
extern int top;
extern void reset_stack();

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s formulas.txt\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    char line[1024];
    int line_num = 1;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        printf("Línea %d:\n", line_num++);
        printf("  Fórmula original : %s\n", line);

        reset_stack();

        char* postfix = convert_to_postfix(line);
        printf("  Fórmula postfija : %s\n", postfix);

        YY_BUFFER_STATE buffer = yy_scan_string(postfix);
        yylex();
        yy_delete_buffer(buffer);
        free(postfix);

        printf("Estado de la pila después de procesar la fórmula:\n");
        printf("  Top: %d\n", top);
        if (top > 0) {
            printf("  Pila: ");
            for (int i = 0; i < top; i++) {
                printf("%p ", stack[i]);
            }
            printf("\n");
        } else {
            printf("  Pila vacía.\n");
        }

        if (top != 1) {
            fprintf(stderr, "  ⚠️ Error: fórmula mal formada (top = %d)\n\n", top);
            continue;
        }

        Node* root = stack[0];
        Node* no_impl = impl_free(root);
        Node* nnf = to_nnf(no_impl);
        Node* cnf = to_cnf(nnf);

        printf("  Forma en CNF     : ");
        print_formula(cnf);
        printf("\n\n");

        free_tree(root);
        free_tree(no_impl);
        free_tree(nnf);
        free_tree(cnf);
    }

    fclose(file);
    return 0;
}
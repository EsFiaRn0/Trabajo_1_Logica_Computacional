#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "postfix_converter.h"
#include "logic.h"

extern int yylex();
extern FILE* yyin;
extern Node* stack[];
extern int top;
extern void reset_stack();

int main(int argc, char* argv[]) {
    if (!isatty(fileno(stdin)) && argc == 1) {
        FILE* input_file = stdin;
        char line[2048];

        while (fgets(line, sizeof(line), input_file)) {
            line[strcspn(line, "\n")] = 0;
            if (strlen(line) == 0) continue;

            char* postfix = convert_to_postfix(line);
            if (postfix == NULL) {
                printf("NO-SOLUTION\n");
                continue;
            }

            FILE* temp = fopen("temp_input.txt", "w");
            if (!temp) {
                perror("No se pudo abrir archivo temporal");
                free(postfix);
                return 1;
            }
            fprintf(temp, "%s\n", postfix);
            fclose(temp);

            yyin = fopen("temp_input.txt", "r");
            if (!yyin) {
                perror("No se pudo abrir temp_input.txt");
                free(postfix);
                return 1;
            }

            reset_stack();
            if (yylex() != 0) {
                fclose(yyin);
                remove("temp_input.txt");
                free(postfix);
                continue;
            }

            if (top != 1) {
                fprintf(stderr, "Error: fórmula mal formada (top = %d)\n", top);
                printf("NO-SOLUTION\n");
                fclose(yyin);
                remove("temp_input.txt");
                free(postfix);
                continue;
            }

            Node* root = stack[0];

            int num_vars = get_num_vars(root);

            if (solve_sat(root, num_vars)) {
                printf("\tSATISFACIBLE.\n");
            } else {
                printf("\tNO-SATISFACIBLE.\n");
            }

            printf("\n");

            free(postfix);
            free_tree(root);
            fclose(yyin);
            remove("temp_input.txt");
        }

        return 0;
    } else {
        fprintf(stderr, "Uso correcto: ./tarea1.exe < expresion.txt\n");
        return 1;
    }
}
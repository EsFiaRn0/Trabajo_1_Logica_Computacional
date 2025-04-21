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
            printf("Postfijo: %s\n", postfix);

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
            yylex();

            if (top != 1) {
                fprintf(stderr, "Error: fórmula mal formada (top = %d)\n", top);
                fclose(yyin);
                remove("temp_input.txt");
                free(postfix);
                continue;
            }

            Node* root = stack[0];
            Node* cnf = to_cnf(root);

            printf("Fórmula en la gramática pedida por sat_lineal:\n");
            print_formula(cnf);

            int num_vars = get_num_vars(cnf);

            if (solve_sat(cnf, num_vars)) {
                printf("\tSATISFACIBLE.\n");
            } else {
                printf("\tNO-SATISFACIBLE.\n");
            }
            
            printf("\n");

            free(postfix);
            free_tree(cnf);

            fclose(yyin);
            remove("temp_input.txt");
        }

        return 0;
    } else {
        fprintf(stderr, "Uso correcto: ./tarea1.exe < expresion.txt\n");
        return 1;
    }
}
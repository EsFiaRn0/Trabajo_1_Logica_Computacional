#ifndef POSTFIX_CONVERTER_H
#define POSTFIX_CONVERTER_H

typedef struct {
    const char* symbol;     // Símbolo del operador
    int precedence;         // Precedencia del operador        
    int right_associative;  // 1 si es asociativo por la derecha, 0 si no
} Operator;

int precedence(const char* op);
int is_right_associative(const char* op);
int is_operator(const char* token);
char* convert_to_postfix(const char* input);

#endif

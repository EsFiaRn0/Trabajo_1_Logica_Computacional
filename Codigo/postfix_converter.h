#ifndef POSTFIX_CONVERTER_H
#define POSTFIX_CONVERTER_H

typedef struct {
    const char* symbol;
    int precedence;
    int right_associative;
} Operator;

int precedence(const char* op);
int is_right_associative(const char* op);
int is_operator(const char* token);
char* convert_to_postfix(const char* input);

#endif

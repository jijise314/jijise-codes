#include <stdio.h>
#include "Calculator.h"

int main() {
    char sample[] = "3+4*(5+6)*7";
    char postfix[100];

    infixToPostfix(sample, postfix);
    evalPostfix(postfix);

    return 0;
}
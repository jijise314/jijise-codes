#include <stdio.h>
#include "my_fraction.h"

int main() {
	Fraction f1, f2, result;
	char op;

	printf("수를 입력하세요 (예: 1/2 + 1/4): ");

	f1 = scan_fraction();

	scanf_s(" %c", &op);
    
	f2 = scan_fraction();

    switch (op) {
    case '+': result = add(f1, f2); break;
    case '-': result = minus(f1, f2); break;
    case '*': result = multiply(f1, f2); break;
    case '/': result = divide(f1, f2); break;
    default:
        printf("\n잘못된 연산자입니다.\n");
        return 1;
    }
    
    printf("결과: ");
    print_fraction(result);
    printf("\n");

    return 0;
}

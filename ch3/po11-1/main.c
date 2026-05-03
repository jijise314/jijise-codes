#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "polynomial.h"

int main() {
	polynomial* p1 = createPolynomial(100);
	polynomial* p2 = createPolynomial(100);
	char input1[256], input2[256];

    printf("첫 번째 다항식을 입력하세요 (예: 3X^2+2X^1+1X^0): ");
    scanf("%s", input1);
    parsePolynomial(p1, input1);

    printf("두 번째 다항식을 입력하세요 (예: 1X^1+2X^0): ");
    scanf("%s", input2);
    parsePolynomial(p2, input2);

    printf("\n[입력된 다항식]\n");
    printf("P1 : ");
    printPolynomial(p1);

    printf("P2 : ");
    printPolynomial(p2);

    polynomial* p_add = addPolynomial(p1, p2);
    printf("\n[덧셈 결과]\n");
    printf("P1 + P2 = ");
    printPolynomial(p_add);

    polynomial* p_mul = multiplyPolynomial(p1, p2);
    printf("\n[곱셈 결과]\n");
    printf("P1 * P2 = ");
    printPolynomial(p_mul);

    destroyArrayList(p1);
    destroyArrayList(p2);
    destroyArrayList(p_add);
    destroyArrayList(p_mul);

    return 0;
}
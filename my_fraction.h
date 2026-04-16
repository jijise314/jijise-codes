#ifndef MY_FRACTION_H
#define MY_FRACTION_H

typedef struct {
	int num1; //분자
	int num2; //분모
} Fraction;

// 주요 연산 함수
Fraction create_fraction(int num, int num2);
Fraction reduce(Fraction f);
Fraction add(Fraction f1, Fraction f2);
Fraction minus(Fraction f1, Fraction f2);
Fraction multiply(Fraction f1, Fraction f2);
Fraction divide(Fraction f1, Fraction f2);

// 입출력 변환 함수
Fraction scan_fraction();
void print_fraction(Fraction f);

#endif
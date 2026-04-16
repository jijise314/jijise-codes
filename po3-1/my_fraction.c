#include <stdio.h>
#include <stdlib.h>

#include "my_fraction.h"

//최대공약수 구하기
int get_gcd(int a, int b) {
	a = abs(a);
	b = abs(b);
	while (b) {
		a %= b;
		int temp = a;
		a = b;
		b = temp;
	}
	return a;
}

Fraction reduce(Fraction f) {
	if (f.num2 == 0) return f;
	int gcd = get_gcd(f.num1, f.num2);
	f.num1 /= gcd;
	f.num2 /= gcd;

	return f;
}

Fraction add(Fraction f1, Fraction f2){
	Fraction res = { f1.num1 * f2.num2 + f2.num1 * f1.num2, f1.num2 * f2.num2 };
	return reduce(res);
}

Fraction minus(Fraction f1, Fraction f2) {
	Fraction res = { f1.num1 * f2.num2 - f2.num1 * f1.num2, f1.num2 * f2.num2 };
	return reduce(res);
}

Fraction multiply(Fraction f1, Fraction f2) {
	Fraction res = { f1.num1 * f2.num1, f1.num2 * f2.num2};
	return reduce(res);
}

Fraction divide(Fraction f1, Fraction f2) {
	Fraction res = { f1.num1 * f2.num2, f1.num2 * f2.num1 };
	return reduce(res);
}

Fraction scan_fraction() {
	int n, d;
	scanf_s("%d/%d", &n, &d);
	return reduce((Fraction) { n, d });
}

void print_fraction(Fraction f) {
	printf("%d/%d", f.num1, f.num2);
}

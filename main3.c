#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_fraction.h"
#include "my_math.h"

int main(int c, char** v) {
	if (c != 3) {
		printf("실행 방법: %s <가로> <세로>\n", v[0]);
		return 1;
	}
	Fraction width, height;

	char* slash1 = strchr(v[1], '/');
	if (slash1 != NULL) {
		width.num1 = atoi(v[1]);
		width.num2 = atoi(slash1 + 1);
	}
	else {
		width.num1 = atoi(v[1]);
		width.num2 = 1;  
	}

	char* slash2 = strchr(v[2], '/');
	if (slash2 != NULL) {
		height.num1 = atoi(v[2]);
		height.num2 = atoi(slash2 + 1);
	}
	else {
		height.num1 = atoi(v[2]);
		height.num2 = 1;
	}

	Fraction sum = add(width, height);
	Fraction two = { 2,1 };
	Fraction perimeter = multiply(two, sum);

	Fraction area = multiply(width, height);

	printf("perimeter: ");
	print_fraction(perimeter);
	printf("\t");

	printf("area: ");
	print_fraction(area);

	return 0;
}
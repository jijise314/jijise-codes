#include <stdio.h>
#include "my_math.h"

int main() {
	int n1, n2;

	printf("두 정수를 입력하세요 (예: 18 24): ");
	if (scanf_s("%d %d", &n1, &n2) != 2) {
		printf("잘못 입력 됬습니다. 두 정수를 입력해주세요\n");
		return 1;
	}

	int gcd = get_gcd(n1, n2);
	int lcm = get_lcm(n1, n2);

	printf("--결과--\n");
	printf("GCD: %d, LCM: %d", gcd, lcm);

	return 0;
}

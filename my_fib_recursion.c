#include <stdio.h>
#include <time.h>

#define N 25

unsigned long long fib_recurtive(int n) {
	printf("F(%d)", n);
	if (n == 1 || n == 2) {
		return 1;
	}
	else {
		return fib_recurtive(n - 1) + fib_recurtive(n - 2);
	}
}

int main() {

	clock_t start, finish;
	double duration;

	start = clock();

	int result;

	result = fib_recurtive(N);

	printf("\n==결과값 출력==\n");
	printf("F(%d) = %d\n", N, result);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f 초입니다..\n", duration);
}
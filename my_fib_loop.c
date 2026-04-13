#include <stdio.h>
#include <time.h>

#define N 25	

unsigned long long fib_loop(int n) {
	if (n==1 || n == 2) {
		return 1;
	}
	else {
		unsigned long long fn1 = 1, fn2 = 1, temp = 0;
		for (int i = 3; i <= n; i++) {
			temp = fn1 + fn2;
			fn1 = fn2;
			fn2 = temp;
		}
		return temp;
	}
}

int main() {

	clock_t start, finish;
	double duration;

	start = clock();

	int result;

	result = fib_loop(N);

	printf("\n==결과값 출력==\n");
	printf("F(%d) = %d\n", N, result);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("%f 초입니다..\n", duration);
}
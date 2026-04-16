#include <stdio.h>
#include "my_fibonacci.h"

long long get_fibonacci(int n) {
    if (n <= 1) return n;
    return get_fibonacci(n - 1) + get_fibonacci(n - 2);
}

int main() {
    int n_max = 40;
    printf("결과값 출력 \n");
    printf(" n\t| F(n)\t\t\t| F(n-1)\t\t| GCD\n");
    printf("----------------------------------------------------------------------------------------\n");

    for (int n = 5; n <= n_max; n++) {
        long long fn = get_fibonacci(n);
        long long fn_minus_1 = get_fibonacci(n - 1);

        int result = 0;
        for (int i = 0; i < 1000000; i++) {
            result = get_gcd((int)fn, (int)fn_minus_1);
        }

        printf("%2d\t| %-20lld\t| %-20lld\t| %d\n", n, fn, fn_minus_1, result);
    }

    return 0;
}

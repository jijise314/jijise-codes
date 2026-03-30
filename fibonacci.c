#include <stdio.h>
#include "my_fibonacci.h"

int main() {
    long long fib_1 = 3;
    long long fib_2 = 5;

    int n_max = 92;

    printf(" n\t| F(n)\t\t\t| F(n-1)\t\t| GCD\n");
    printf("----------------------------------------------------------------------------------------\n");

    for (int n = 5; n <= n_max; n++) {
        long long gcd = get_gcd(fib_2, fib_1);

        printf("%2d\t| %-20lld\t| %-20lld\t| %lld\n", n, fib_2, fib_1, gcd);

        long long fib_next = fib_2 + fib_1;
        fib_1 = fib_2;
        fib_2 = fib_next;
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "my_math.h"

long long get_gcd(long long a, long long b) {
    a = llabs(a);
    b = llabs(b);
    while (b) {
        long long temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

long long get_lcm(long long a, long long b) {
    if (a == 0 || b == 0) return 0;

    long long gcd = get_gcd(a, b);
    return llabs((a / gcd) * b);
}
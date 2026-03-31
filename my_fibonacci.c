#include <stdio.h>
#include <stdlib.h>
#include "my_fibonacci.h"

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
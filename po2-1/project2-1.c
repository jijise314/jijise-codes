#include <stdio.h>

int main() {
    unsigned int input;
    printf("실수를 입력하세요 : ");
    scanf_s("%f", &input);

    unsigned int bits = *(unsigned int*)&input;

    printf("\n32비트 패턴 : ");
    for (int i = 31; i >= 0; i--) {
        unsigned int bit = (bits >> i) & 1;
        printf("%u", bit);

        if (i == 31 || i == 23) {
            printf(" / ");
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

int main() {
    int arr[N];
    int sum = 0;
    int max, min;

    // 매번 다른 난수가 나오도록 설정
    srand((unsigned int)time(NULL));

    //배열을 N 이하의 임의의 수로 채우기
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % (N + 1);
    }

    //배열 출력
    printf("앞에서 부터 출력: ");
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("뒤에서부터 출력: ");
    for (int i = N - 1; i >= 0; i--)
    {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    //배열의 총합, 최댓값, 최솟값 출력
    for (int i = 0; i < N; i++)
    {
        sum += arr[i];
    }
    printf("총합: %d\n", sum);

    max = arr[0];
    min = arr[0];

    for (int i = 1; i < N; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    printf("최댓값: %d\n", max);
    printf("최솟값: %d\n", min);

    return 0;
}
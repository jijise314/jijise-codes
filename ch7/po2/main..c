#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

static int random_int(void)
{
    return (rand() << 15 | rand()) % 1000000000;
}

static void print_result(const char* name, long long count, int ok)
{
    printf("%s : %lld 회 비교 %s\n", name, count, ok ? "(정렬 확인 완료)" : "(정렬 오류!)");
}

int main(void)
{
    const int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    srand((unsigned)time(NULL));

    printf("===== 삽입 정렬과 쉘 정렬의 비교 횟수 측정 =====\n\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* original = (int*)malloc(sizeof(int) * n);
        int* work = (int*)malloc(sizeof(int) * n);
        if (original == NULL || work == NULL) {
            printf("메모리 할당 실패\n");
            free(original);
            free(work);
            return 1;
        }

        /* 원본 데이터 생성 (측정 제외) */
        for (int i = 0; i < n; i++)
            original[i] = random_int();

        printf("데이터 크기 : %d\n\n", n);

        /* [1] 삽입 정렬 */
        memcpy(work, original, sizeof(int) * n);
        long long c1 = insertion_sort(work, n);
        print_result("삽입 정렬            ", c1, is_sorted(work, n));

        /* [2] 쉘 정렬 - 기본 간격 */
        memcpy(work, original, sizeof(int) * n);
        long long c2 = shell_sort_original(work, n);
        print_result("쉘 정렬 (기본 간격)  ", c2, is_sorted(work, n));

        /* [3] 쉘 정렬 - Knuth 간격 */
        memcpy(work, original, sizeof(int) * n);
        long long c3 = shell_sort_knuth(work, n);
        print_result("쉘 정렬 (Knuth 간격) ", c3, is_sorted(work, n));

        printf("\n");

        free(original);
        free(work);
    }

    return 0;
}

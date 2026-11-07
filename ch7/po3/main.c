#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quicksort.h"

/* ===================== 비교 횟수 실험 ===================== */
static void run_comparison_test(void)
{
    const int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    const char* names[] = { "퀵 정렬 (첫 번째 원소) ",
                            "퀵 정렬 (무작위 원소)  ",
                            "퀵 정렬 (세 값 중앙값) " };
    int s, t;

    printf("========== 피벗 선택 방법별 비교 횟수 ==========\n\n");

    for (s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int allSorted = 1;
        int* original = (int*)malloc(sizeof(int) * n);
        int* work = (int*)malloc(sizeof(int) * n);
        if (!original || !work) { printf("메모리 할당 실패\n"); exit(1); }

        generate_data(original, n);

        printf("데이터 크기 : %d\n\n", n);
        for (t = 0; t < 3; t++) {
            memcpy(work, original, sizeof(int) * n);

            g_compare = 0;
            quick_sort_recursive(work, 0, n - 1, (PivotType)t);

            printf("%s: %lld 회 비교", names[t], g_compare);
            if (!is_sorted(work, n)) { printf("  [오류: 정렬되지 않음]"); allSorted = 0; }
            printf("\n");
        }
        printf("\n정렬 결과 확인 : %s\n\n", allSorted ? "정상 (오름차순)" : "오류 발생");

        free(original);
        free(work);
    }
}

/* ===================== 재귀 vs 반복 실행시간 ===================== */
static void run_time_test(void)
{
    const int n = 10000;
    const int REPEAT = 1000;
    int* original = (int*)malloc(sizeof(int) * n);
    int* work = (int*)malloc(sizeof(int) * n);
    int* stack = (int*)malloc(sizeof(int) * 2 * n);
    double recTotal = 0.0, itTotal = 0.0, start;
    int r, okRec = 1, okIt = 1;

    if (!original || !work || !stack) { printf("메모리 할당 실패\n"); exit(1); }

    generate_data(original, n);

    for (r = 0; r < REPEAT; r++) {
        /* 재귀적 퀵 정렬 */
        memcpy(work, original, sizeof(int) * n);
        start = now_ms();
        quick_sort_recursive(work, 0, n - 1, PIVOT_MEDIAN3);
        recTotal += now_ms() - start;
        if (!is_sorted(work, n)) okRec = 0;

        /* 반복적 퀵 정렬 */
        memcpy(work, original, sizeof(int) * n);
        start = now_ms();
        quick_sort_iterative(work, n, PIVOT_MEDIAN3, stack);
        itTotal += now_ms() - start;
        if (!is_sorted(work, n)) okIt = 0;
    }

    printf("========== 재귀적 vs 반복적 퀵 정렬 ==========\n\n");
    printf("데이터 크기 : %d\n", n);
    printf("피벗 방식   : 세 값의 중앙값 (Median-of-Three)\n");
    printf("(%d회 측정 평균)\n\n", REPEAT);
    printf("재귀적 퀵 정렬 : %.4f ms%s\n", recTotal / REPEAT, okRec ? "" : "  [오류: 정렬되지 않음]");
    printf("반복적 퀵 정렬 : %.4f ms%s\n", itTotal / REPEAT, okIt ? "" : "  [오류: 정렬되지 않음]");
    printf("\n");

    free(original);
    free(work);
    free(stack);
}

int main(void)
{
    srand((unsigned int)time(NULL));

    run_comparison_test();
    run_time_test();

    return 0;
}

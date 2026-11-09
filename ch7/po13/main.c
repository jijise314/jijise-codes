#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "quicksort.h"
#include "mergesort.h"

#define NUM_ALGO 4
#define REPEAT   50

typedef enum { ALG_MERGE = 0, ALG_QUICK_FIRST, ALG_QUICK_RANDOM, ALG_QUICK_MEDIAN3 } AlgoId;

static const char* ALGO_NAME[NUM_ALGO] = {
    "Merge Sort", "Quick (First)", "Quick (Random)", "Quick (Median3)"
};

static void run_sort(AlgoId id, int a[], int n)
{
    switch (id) {
    case ALG_MERGE:
        if (merge_sort(a, n) != 0) { printf("메모리 할당 실패\n"); exit(1); }
        break;
    case ALG_QUICK_FIRST:   quick_sort_recursive(a, 0, n - 1, PIVOT_FIRST);   break;
    case ALG_QUICK_RANDOM:  quick_sort_recursive(a, 0, n - 1, PIVOT_RANDOM);  break;
    case ALG_QUICK_MEDIAN3: quick_sort_recursive(a, 0, n - 1, PIVOT_MEDIAN3); break;
    }
}

static void run_experiment(int n)
{
    int* original = (int*)malloc(sizeof(int) * n);
    int* work = (int*)malloc(sizeof(int) * n);
    long long comps[NUM_ALGO];
    double    times[NUM_ALGO];
    size_t    memory[NUM_ALGO];
    int       ok[NUM_ALGO];
    int allSorted = 1;
    int id, r;

    if (!original || !work) { printf("메모리 할당 실패\n"); exit(1); }

    /* 모든 알고리즘이 같은 원본 데이터의 복사본을 사용 */
    generate_data(original, n);

    for (id = 0; id < NUM_ALGO; id++) {
        double total = 0.0, start;

        /* (1) 비교 횟수 · 메모리 사용량 · 정렬 결과 확인 */
        memcpy(work, original, sizeof(int) * n);
        g_compare = 0;
        mem_reset();
        run_sort((AlgoId)id, work, n);
        comps[id] = g_compare;
        memory[id] = mem_stack_peak() + g_heap_peak;
        ok[id] = is_sorted(work, n);
        if (!ok[id]) allSorted = 0;

        /* (2) 실행시간: 배열 복사는 측정 구간 밖에서 수행 */
        for (r = 0; r < REPEAT; r++) {
            memcpy(work, original, sizeof(int) * n);
            start = now_ms();
            run_sort((AlgoId)id, work, n);
            total += now_ms() - start;
            if (!is_sorted(work, n)) { ok[id] = 0; allSorted = 0; }
        }
        times[id] = total / REPEAT;
    }

    printf("Data Size : %d\n\n", n);
    printf("%-16s%-15s%-12s%s\n", "Algorithm", "Comparisons", "Time(ms)", "Memory(Bytes)");
    printf("--------------------------------------------------------\n");
    for (id = 0; id < NUM_ALGO; id++) {
        printf("%-16s%-15lld%-12.4f%llu%s\n", ALGO_NAME[id], comps[id], times[id],
            (unsigned long long)memory[id], ok[id] ? "" : "  [오류: 정렬되지 않음]");
    }
    printf("\n정렬 결과 확인 : %s\n\n", allSorted ? "정상 (오름차순)" : "오류 발생");

    free(original);
    free(work);
}

int main(void)
{
    const int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int s;

    srand((unsigned int)time(NULL));

    printf("========== 병합 정렬 vs 퀵 정렬 성능 비교 ==========\n");
    printf("(실행시간: %d회 측정 평균, 메모리: 최대 추가 메모리 = 재귀 스택 + 임시 배열)\n\n", REPEAT);

    for (s = 0; s < numSizes; s++)
        run_experiment(sizes[s]);

    return 0;
}

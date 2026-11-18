#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "quicksort.h"
#include "sorted_array.h"
#include "avl.h"

#define REPEAT       10         /* 실행시간 측정 반복 횟수 (평균) */
#define VALUE_RANGE  100000     /* 데이터 / 삽입·삭제 값의 범위: 0 ~ 99999 */
#define QUICK_PIVOT  PIVOT_MEDIAN3

/* 삽입·삭제 연산 하나 (측정 전에 미리 생성하여 두 자료구조에 동일하게 적용) */
typedef struct {
    int is_insert;              /* 1: 삽입, 0: 삭제 */
    int value;
} Op;

typedef struct {
    double    time_ms;
    long long comps;
    long long moves;
    size_t    memory;
    int       final_count;
    int       deleted;          /* 실제로 삭제된 횟수 */
} Result;

static void fail(void)
{
    printf("메모리 할당 실패\n");
    exit(1);
}

/* 임의의 정수가 짝수이면 삽입, 홀수이면 삭제. 대상 값은 새 난수로 별도 생성 */
static void make_ops(Op ops[], int m)
{
    int k;
    for (k = 0; k < m; k++) {
        int sel = rand_range(VALUE_RANGE);
        ops[k].is_insert = (sel % 2 == 0);
        ops[k].value = rand_range(VALUE_RANGE);
    }
}

/* ===================== 퀵 정렬 + 정렬 배열 ===================== */
static void quick_sort_phase(int a[], int n)
{
    quick_sort_recursive(a, 0, n - 1, QUICK_PIVOT);
}

static int quick_ops_phase(int a[], int* size, const Op ops[], int m)
{
    int k, deleted = 0;
    for (k = 0; k < m; k++) {
        if (ops[k].is_insert) sorted_array_insert(a, size, ops[k].value);
        else                  deleted += sorted_array_delete(a, size, ops[k].value);
    }
    return deleted;
}

/* ===================== AVL 트리 ===================== */
static void avl_sort_phase(AvlTree* t, const int src[], int n)
{
    int i;
    for (i = 0; i < n; i++) avl_insert(t, src[i]);
}

static int avl_ops_phase(AvlTree* t, const Op ops[], int m)
{
    int k, deleted = 0;
    for (k = 0; k < m; k++) {
        if (ops[k].is_insert) avl_insert(t, ops[k].value);
        else                  deleted += avl_delete(t, ops[k].value);
    }
    return deleted;
}

/* ===================== 출력 ===================== */
static void print_row(const char* label, const char* q, const char* v)
{
    printf("%-18s%-16s%s\n", label, q, v);
}

static void print_result(int n, int m, int numInsert, const Result* q, const Result* v, int ok)
{
    char bq[64], bv[64];

    printf("Data Size : %d\n", n);
    printf("Operations: %d\n\n", m);
    print_row("", "Quick Sort", "AVL Tree");
    printf("------------------------------------------------\n");

    snprintf(bq, sizeof bq, "%.4f ms", q->time_ms);            snprintf(bv, sizeof bv, "%.4f ms", v->time_ms);
    print_row("Total Time", bq, bv);
    snprintf(bq, sizeof bq, "%lld", q->comps);                 snprintf(bv, sizeof bv, "%lld", v->comps);
    print_row("Comparisons", bq, bv);
    snprintf(bq, sizeof bq, "%lld", q->moves);                 snprintf(bv, sizeof bv, "%lld", v->moves);
    print_row("Data Movements", bq, bv);
    snprintf(bq, sizeof bq, "%llu Bytes", (unsigned long long)q->memory);
    snprintf(bv, sizeof bv, "%llu Bytes", (unsigned long long)v->memory);
    print_row("Memory Usage", bq, bv);
    printf("\n");
    snprintf(bq, sizeof bq, "%d", q->final_count);             snprintf(bv, sizeof bv, "%d", v->final_count);
    print_row("Final Data Count", bq, bv);

    printf("\n(삽입 %d회, 삭제 시도 %d회 중 실제 삭제 %d회)\n",
        numInsert, m - numInsert, q->deleted);
    printf("검증 결과 : %s\n\n", ok ? "정상 (두 자료구조의 정렬 결과와 최종 데이터 구성이 동일)"
        : "오류 발생");
    printf("================================================\n\n");
}

/* ===================== 실험 ===================== */
static void run_experiment(int n)
{
    const int m = n / 5;                /* 연산 횟수 = n의 20% */
    const int cap = n + m;              /* 배열 최대 용량 (모든 연산이 삽입인 경우) */
    int* original = (int*)malloc(sizeof(int) * n);
    int* sortedRef = (int*)malloc(sizeof(int) * n);    /* 검증용: 퀵 정렬 직후 결과 */
    int* finalRef = (int*)malloc(sizeof(int) * cap);  /* 검증용: 퀵 연산 후 최종 배열 */
    int* buf = (int*)malloc(sizeof(int) * cap);  /* 검증용: AVL 중위 순회 결과 */
    int* work = (int*)malloc(sizeof(int) * cap);  /* 시간 측정용 작업 배열 */
    Op* ops = (Op*)malloc(sizeof(Op) * m);
    Result q, v;
    int ok = 1, numInsert = 0, k, r, cnt;
    double start, total;

    if (!original || !sortedRef || !finalRef || !buf || !work || !ops) fail();

    generate_data(original, n);
    make_ops(ops, m);
    for (k = 0; k < m; k++) numInsert += ops[k].is_insert;

    /* ---------- (1) 퀵 정렬: 비교 / 이동 / 메모리 측정 및 검증 ---------- */
    {
        int* a;
        int size = n;

        mem_reset();
        a = (int*)tracked_malloc(sizeof(int) * cap);
        if (!a) fail();
        memcpy(a, original, sizeof(int) * n);       /* 원본 준비 (이동 횟수 제외) */
        g_compare = 0;
        g_move = 0;

        quick_sort_phase(a, n);
        if (!is_sorted(a, n)) ok = 0;               /* 검증은 LESS를 쓰지 않으므로 계수 없음 */
        memcpy(sortedRef, a, sizeof(int) * n);
        q.deleted = quick_ops_phase(a, &size, ops, m);

        q.comps = g_compare;
        q.moves = g_move;
        q.memory = mem_stack_peak() + g_heap_peak;
        q.final_count = size;
        if (!is_sorted(a, size)) ok = 0;
        memcpy(finalRef, a, sizeof(int) * size);
        tracked_free(a, sizeof(int) * cap);
    }

    /* ---------- (2) AVL 트리: 비교 / 이동 / 메모리 측정 및 검증 ---------- */
    {
        AvlTree t;

        avl_init(&t);
        mem_reset();
        g_compare = 0;
        g_move = 0;

        avl_sort_phase(&t, original, n);
        cnt = avl_inorder(&t, buf);                 /* 중위 순회로 정렬 결과 확인 */
        if (cnt != n || memcmp(buf, sortedRef, sizeof(int) * n) != 0 || !avl_is_valid(&t)) ok = 0;
        v.deleted = avl_ops_phase(&t, ops, m);

        v.comps = g_compare;
        v.moves = g_move;
        v.memory = mem_stack_peak() + g_heap_peak;
        v.final_count = t.size;
        cnt = avl_inorder(&t, buf);
        if (cnt != q.final_count || memcmp(buf, finalRef, sizeof(int) * cnt) != 0 || !avl_is_valid(&t)) ok = 0;
        if (v.deleted != q.deleted) ok = 0;
        avl_free(&t);
    }

    /* ---------- (3) 총 수행시간: 정렬 시작 ~ 모든 삽입/삭제 완료 ---------- */
    total = 0.0;
    for (r = 0; r < REPEAT; r++) {
        int size = n;
        memcpy(work, original, sizeof(int) * n);
        start = now_ms();
        quick_sort_phase(work, n);
        quick_ops_phase(work, &size, ops, m);
        total += now_ms() - start;
    }
    q.time_ms = total / REPEAT;

    total = 0.0;
    for (r = 0; r < REPEAT; r++) {
        AvlTree t;
        avl_init(&t);
        start = now_ms();
        avl_sort_phase(&t, original, n);
        avl_ops_phase(&t, ops, m);
        total += now_ms() - start;
        avl_free(&t);
    }
    v.time_ms = total / REPEAT;

    print_result(n, m, numInsert, &q, &v, ok);

    free(original); free(sortedRef); free(finalRef);
    free(buf); free(work); free(ops);
}

int main(void)
{
    const int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int s;

    srand((unsigned int)time(NULL));

    printf("============ 퀵 정렬 vs AVL 트리 정렬 성능 비교 ============\n");
    printf("(퀵 정렬 피벗: Median-of-3, 실행시간: %d회 측정 평균)\n", REPEAT);
    printf("(메모리: 최대 사용량 = 자료구조(배열 또는 노드) + 호출 스택)\n\n");

    for (s = 0; s < numSizes; s++)
        run_experiment(sizes[s]);

    return 0;
}

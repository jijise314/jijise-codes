#include "common.h"
#include "heapsort.h"

static void swap(int* a, int* b)
{
    int t = *a; *a = *b; *b = t;
}

/* ===================== 힙 속성 복구 ===================== */
static void sift_down(int a[], int k, int n)
{
    int child;

    STACK_PROBE();
    while ((child = 2 * k + 1) < n) {            /* 인덱스 검사: 비교 횟수 제외 */
        /* 두 자식 중 더 큰 자식 선택 (값 비교 1회) */
        if (child + 1 < n && LESS(a[child], a[child + 1]))
            child++;
        /* 부모가 큰 자식보다 작지 않으면 힙 속성 만족 (값 비교 1회) */
        if (!LESS(a[k], a[child]))
            break;
        swap(&a[k], &a[child]);
        k = child;
    }
}

/* ===================== 힙 정렬 ===================== */
void heap_sort(int a[], int n)
{
    int i;

    STACK_PROBE();
    if (n < 2) return;

    /* (1) 힙 구성: 마지막 내부 노드부터 루트까지 sift-down (상향식 힙 구성) */
    for (i = n / 2 - 1; i >= 0; i--)
        sift_down(a, i, n);

    /* (2) 정렬: 루트(최댓값)를 힙의 마지막 원소와 교환하고,
     *     힙 크기를 1 줄인 뒤 힙 속성을 복구한다 */
    for (i = n - 1; i > 0; i--) {
        swap(&a[0], &a[i]);
        sift_down(a, 0, i);
    }
}

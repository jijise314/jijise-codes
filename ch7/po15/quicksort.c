#include "common.h"
#include "quicksort.h"

static void swap(int* a, int* b)
{
    int t = *a; *a = *b; *b = t;
    g_move += 3;
}

/* ===================== 피벗 선택 ===================== */
/* 첫 번째, 가운데, 마지막 원소의 중앙값 위치 반환 */
static int median_of_three(int a[], int low, int high)
{
    int mid = low + (high - low) / 2;

    /* 값을 지역 변수로 복사하지 않고 배열에서 직접 비교 (데이터 이동 없음) */
    if (LESS(a[low], a[mid])) {
        if (LESS(a[mid], a[high])) return mid;
        if (LESS(a[low], a[high])) return high;
        return low;
    }
    else {
        if (LESS(a[low], a[high])) return low;
        if (LESS(a[mid], a[high])) return high;
        return mid;
    }
}

static void select_pivot(int a[], int low, int high, PivotType type)
{
    int p = low;

    switch (type) {
    case PIVOT_FIRST:
        p = low;
        break;
    case PIVOT_RANDOM:
        p = low + rand_range(high - low + 1);
        break;
    case PIVOT_MEDIAN3:
        if (high - low + 1 >= 3)
            p = median_of_three(a, low, high);
        break;
    }
    if (p != low) swap(&a[low], &a[p]);
}

/* ===================== 분할 (공통) ===================== */
static int partition(int a[], int low, int high)
{
    int pivot = a[low];
    int i = low;
    int j = high + 1;

    g_move++;       /* pivot = a[low] 복사 */
    STACK_PROBE();
    while (1) {
        do { i++; } while (i <= high && LESS(a[i], pivot));
        do { j--; } while (LESS(pivot, a[j]));

        if (i >= j) break;
        swap(&a[i], &a[j]);
    }
    swap(&a[low], &a[j]);
    return j;
}

/* ===================== 재귀적 퀵 정렬 ===================== */
void quick_sort_recursive(int a[], int low, int high, PivotType type)
{
    int p;
    STACK_PROBE();
    if (low >= high) return;

    select_pivot(a, low, high, type);
    p = partition(a, low, high);

    quick_sort_recursive(a, low, p - 1, type);
    quick_sort_recursive(a, p + 1, high, type);
}

/* ===================== 반복적 퀵 정렬 (스택) ===================== */
void quick_sort_iterative(int a[], int n, PivotType type, int* stack)
{
    int top = -1;
    int low, high, p;

    if (n < 2) return;
    stack[++top] = 0;
    stack[++top] = n - 1;

    while (top >= 0) {
        high = stack[top--];
        low = stack[top--];
        if (low >= high) continue;

        select_pivot(a, low, high, type);
        p = partition(a, low, high);

        if (p + 1 < high) { stack[++top] = p + 1; stack[++top] = high; }
        if (low < p - 1) { stack[++top] = low;   stack[++top] = p - 1; }
    }
}

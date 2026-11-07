#ifndef QUICKSORT_H
#define QUICKSORT_H

/* 피벗 선택 방법 */
typedef enum { PIVOT_FIRST = 0, PIVOT_RANDOM, PIVOT_MEDIAN3 } PivotType;

/* 데이터 값 사이의 비교 횟수 (정렬 전에 0으로 초기화하여 사용) */
extern long long g_compare;

/* 퀵 정렬 */
void quick_sort_recursive(int a[], int low, int high, PivotType type);
void quick_sort_iterative(int a[], int n, PivotType type, int* stack);

/* 유틸리티 */
int    rand_range(int range);
void   generate_data(int a[], int n);
int    is_sorted(const int a[], int n);
double now_ms(void);

#endif

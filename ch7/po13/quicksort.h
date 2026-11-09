#ifndef QUICKSORT_H
#define QUICKSORT_H

/* 피벗 선택 방법 */
typedef enum { PIVOT_FIRST = 0, PIVOT_RANDOM, PIVOT_MEDIAN3 } PivotType;

/* 재귀적 퀵 정렬: a[low..high]를 오름차순 정렬 */
void quick_sort_recursive(int a[], int low, int high, PivotType type);

/* 반복적 퀵 정렬: stack은 호출자가 준비 (크기 2n 이상의 int 배열) */
void quick_sort_iterative(int a[], int n, PivotType type, int* stack);

#endif

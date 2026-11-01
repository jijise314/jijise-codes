#ifndef SORT_H
#define SORT_H

 /* [1] 삽입 정렬 */
long long insertion_sort(int a[], int n);

/* 간격(gap) 단위 삽입 정렬 */
long long gap_insertion_sort(int a[], int n, int gap);

/* [2] 쉘 정렬 */
long long shell_sort_original(int a[], int n);

/* [3] 쉘 정렬 */
long long shell_sort_knuth(int a[], int n);

/* 오름차순 정렬 여부 확인 (1: 정렬됨, 0: 오류) */
int is_sorted(const int a[], int n);

#endif

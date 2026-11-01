#include "sort.h"

 /* [1] 삽입 정렬 */
long long insertion_sort(int a[], int n)
{
    long long count = 0;
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            count++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                j--;
            }
            else {
                break;
            }
        }
        a[j + 1] = key;
    }
    return count;
}

/* 간격(gap) 단위 삽입 정렬 */
long long gap_insertion_sort(int a[], int n, int gap)
{
    long long count = 0;
    for (int i = gap; i < n; i++) {
        int key = a[i];
        int j = i - gap;
        while (j >= 0) {
            count++;
            if (a[j] > key) {
                a[j + gap] = a[j];
                j -= gap;
            }
            else {
                break;
            }
        }
        a[j + gap] = key;
    }
    return count;
}

/* [2] 쉘 정렬 - 기본 간격 */
long long shell_sort_original(int a[], int n)
{
    long long count = 0;
    for (int gap = n / 2; gap > 0; gap /= 2)
        count += gap_insertion_sort(a, n, gap);
    return count;
}

/* [3] 쉘 정렬 - Knuth 간격 */
long long shell_sort_knuth(int a[], int n)
{
    long long count = 0;
    int h = 1;
    while (3 * h + 1 < n)
        h = 3 * h + 1;
    for (; h >= 1; h = (h - 1) / 3)
        count += gap_insertion_sort(a, n, h);
    return count;
}

/* 오름차순 정렬 여부 확인 */
int is_sorted(const int a[], int n)
{
    for (int i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}

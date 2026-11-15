#include "common.h"
#include "mergesort.h"

static void merge(int a[], int tmp[], int low, int mid, int high)
{
    int i = low, j = mid + 1, k = low;

    STACK_PROBE();
    while (i <= mid && j <= high) {
        if (LESS(a[j], a[i])) tmp[k++] = a[j++];
        else                  tmp[k++] = a[i++];
    }
    while (i <= mid)  tmp[k++] = a[i++];
    while (j <= high) tmp[k++] = a[j++];

    for (k = low; k <= high; k++) a[k] = tmp[k];
}

static void merge_sort_rec(int a[], int tmp[], int low, int high)
{
    int mid;
    STACK_PROBE();
    if (low >= high) return;

    mid = low + (high - low) / 2;
    merge_sort_rec(a, tmp, low, mid);
    merge_sort_rec(a, tmp, mid + 1, high);
    merge(a, tmp, low, mid, high);
}

int merge_sort(int a[], int n)
{
    size_t bytes;
    int* tmp;

    if (n < 2) return 0;
    bytes = sizeof(int) * (size_t)n;
    tmp = (int*)tracked_malloc(bytes);
    if (!tmp) return -1;

    merge_sort_rec(a, tmp, 0, n - 1);

    tracked_free(tmp, bytes);
    return 0;
}

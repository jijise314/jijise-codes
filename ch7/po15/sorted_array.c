#include "common.h"
#include "sorted_array.h"

void sorted_array_insert(int a[], int* size, int x)
{
    int i = 0, k;

    STACK_PROBE();
    /* 삽입 위치 선형 탐색: a[i] < x 인 동안 전진 */
    while (i < *size&& LESS(a[i], x)) i++;

    /* 뒤로 한 칸씩 이동 */
    for (k = *size; k > i; k--) {
        a[k] = a[k - 1];
        g_move++;
    }
    a[i] = x;
    g_move++;
    (*size)++;
}

int sorted_array_delete(int a[], int* size, int x)
{
    int i = 0, k;

    STACK_PROBE();
    /* 삭제 대상 선형 탐색: a[i] < x 인 동안 전진 → a[i] >= x 에서 멈춤 */
    while (i < *size && LESS(a[i], x)) i++;

    /* 끝에 도달했거나 a[i] > x 이면 x가 없음 */
    if (i == *size || LESS(x, a[i])) return 0;

    /* 앞으로 한 칸씩 이동 */
    for (k = i; k < *size - 1; k++) {
        a[k] = a[k + 1];
        g_move++;
    }
    (*size)--;
    return 1;
}

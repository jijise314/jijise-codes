#include <stdlib.h>
#include <time.h>
#include "common.h"

long long g_compare = 0;

/* ===================== 메모리 측정 ===================== */
uintptr_t g_stack_base = 0;
uintptr_t g_stack_min = UINTPTR_MAX;
size_t    g_heap_cur = 0;
size_t    g_heap_peak = 0;

/* 정렬 함수가 호출될 위치의 스택 주소를 기준으로 삼는다 */
void mem_reset(void)
{
    volatile char here = 0;
    g_stack_base = (uintptr_t)&here;
    g_stack_min = g_stack_base;
    g_heap_cur = 0;
    g_heap_peak = 0;
}

size_t mem_stack_peak(void)
{
    return (size_t)(g_stack_base - g_stack_min);
}

void* tracked_malloc(size_t bytes)
{
    void* p = malloc(bytes);
    if (p) {
        g_heap_cur += bytes;
        if (g_heap_cur > g_heap_peak) g_heap_peak = g_heap_cur;
    }
    return p;
}

void tracked_free(void* p, size_t bytes)
{
    if (p) { free(p); g_heap_cur -= bytes; }
}

/* ===================== 유틸리티 ===================== */
/* RAND_MAX가 32767인 환경에서도 넓은 범위를 얻기 위해 rand() 두 번을 이어 붙인다 */
int rand_range(int range)
{
    unsigned int r = ((unsigned int)rand() << 15) | (unsigned int)rand();
    return (int)(r % (unsigned int)range);
}

void generate_data(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        a[i] = rand_range(100000);
}

int is_sorted(const int a[], int n)
{
    int i;
    for (i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}

double now_ms(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1.0e6;
}

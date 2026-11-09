#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

/* ===================== 비교 횟수 ===================== */
/* 데이터 값 사이의 비교 횟수 (정렬 전에 0으로 초기화하여 사용) */
extern long long g_compare;

/* 데이터 값 비교: 호출될 때마다 g_compare 1 증가 (모든 정렬이 공유) */
static inline int LESS(int x, int y)
{
    g_compare++;
    return x < y;
}

/* ===================== 메모리 측정 ===================== */
extern uintptr_t g_stack_base;
extern uintptr_t g_stack_min;
extern size_t    g_heap_cur;
extern size_t    g_heap_peak;

/* 현재 함수 프레임의 지역 변수 주소로 스택 깊이를 기록 (함수 호출 비용 없음) */
#define STACK_PROBE()                                         \
    do {                                                      \
        volatile char probe_ = 0;                             \
        uintptr_t addr_ = (uintptr_t)&probe_;                 \
        if (addr_ < g_stack_min) g_stack_min = addr_;         \
    } while (0)

void   mem_reset(void);
size_t mem_stack_peak(void);
void* tracked_malloc(size_t bytes);
void   tracked_free(void* p, size_t bytes);

/* ===================== 유틸리티 ===================== */
int    rand_range(int range);
void   generate_data(int a[], int n);
int    is_sorted(const int a[], int n);
double now_ms(void);

#endif

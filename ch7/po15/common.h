#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

/* ===================== 비교 횟수 ===================== */
extern long long g_compare;

static inline int LESS(int x, int y)
{
    g_compare++;
    return x < y;
}

/* ===================== 데이터 이동 횟수 ===================== */
extern long long g_move;

/* ===================== 메모리 측정 ===================== */
extern uintptr_t g_stack_base;
extern uintptr_t g_stack_min;
extern size_t    g_heap_cur;
extern size_t    g_heap_peak;

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

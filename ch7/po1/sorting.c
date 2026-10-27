#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"

 /* 1. 난수 생성기 (xorshift32) */
static unsigned int g_rng_state = 20261024u;

unsigned int rng_next(void)
{
    unsigned int x = g_rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    g_rng_state = x;
    return x;
}

int rng_range(int lo, int hi)
{
    return lo + (int)(rng_next() % (unsigned int)(hi - lo + 1));
}

/* 2. 정렬 알고리즘 */
static int g_last_passes = 0;

int  sort_last_passes(void) { return g_last_passes; }
void sort_reset_passes(void) { g_last_passes = 0; }

static void swap_data(Data* a, Data* b, Counter* c)
{
    Data temp = *a;  c->movements++;
    *a = *b;         c->movements++;
    *b = temp;       c->movements++;
}

/* [1] 선택 정렬*/
void selection_sort(Data a[], int n, Counter* c)
{
    int i, j, min;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++) {
            c->comparisons++;
            if (a[j].key1 < a[min].key1)
                min = j;
        }
        if (min != i)
            swap_data(&a[i], &a[min], c);
    }
}

/* [2] 버블 정렬 - 일반 (정해진 n-1번의 순회를 모두 수행)*/
void bubble_sort(Data a[], int n, Counter* c)
{
    int pass, j;
    g_last_passes = 0;
    for (pass = 0; pass < n - 1; pass++) {
        for (j = 0; j < n - 1 - pass; j++) {
            c->comparisons++;
            if (a[j].key1 > a[j + 1].key1)
                swap_data(&a[j], &a[j + 1], c);
        }
        g_last_passes++;
    }
}

/* [2-1] 버블 정렬 - 조기 종료 (한 순회에서 교환이 없으면 종료)*/
void bubble_sort_early(Data a[], int n, Counter* c)
{
    int pass, j, swapped;
    g_last_passes = 0;
    for (pass = 0; pass < n - 1; pass++) {
        swapped = 0;
        for (j = 0; j < n - 1 - pass; j++) {
            c->comparisons++;
            if (a[j].key1 > a[j + 1].key1) {
                swap_data(&a[j], &a[j + 1], c);
                swapped = 1;
            }
        }
        g_last_passes++;
        if (!swapped)
            break;
    }
}

/* [3] 삽입 정렬*/
static void insertion_sort_impl(Data a[], int n, Counter* c, int use_ge)
{
    int i, j, shift;
    Data current;

    for (i = 1; i < n; i++) {
        current = a[i];             c->movements++;
        j = i - 1;
        while (j >= 0) {
            c->comparisons++;
            if (use_ge)
                shift = (a[j].key1 >= current.key1);
            else
                shift = (a[j].key1 > current.key1);
            if (!shift)
                break;
            a[j + 1] = a[j];        c->movements++;
            j--;
        }
        a[j + 1] = current;         c->movements++;
    }
}

void insertion_sort_gt(Data a[], int n, Counter* c) { insertion_sort_impl(a, n, c, 0); }
void insertion_sort_ge(Data a[], int n, Counter* c) { insertion_sort_impl(a, n, c, 1); }

/* 알고리즘 목록*/
const Algorithm PERF_ALGS[NUM_PERF_ALGS] = {
    { "선택 정렬",               selection_sort    },
    { "버블 정렬",               bubble_sort       },
    { "버블 정렬 (조기 종료)",   bubble_sort_early },
    { "삽입 정렬",               insertion_sort_gt },
};

const Algorithm STAB_ALGS[NUM_STAB_ALGS] = {
    { "선택 정렬",               selection_sort    },
    { "버블 정렬",               bubble_sort       },
    { "버블 정렬 (조기 종료)",   bubble_sort_early },
    { "삽입 정렬 (>)",           insertion_sort_gt },
    { "삽입 정렬 (>=)",          insertion_sort_ge },
};

/* 3. 시험 데이터 준비 (비교·이동 횟수에 포함하지 않음) */
void assign_order(Data a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        a[i].order = i;
}

void generate_random(Data a[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        a[i].key1 = rng_range(0, KEY_MAX);
        a[i].key2 = rng_range(0, KEY_MAX);
    }
    assign_order(a, n);
}

void generate_duplicate_heavy(Data a[], int n, int key_range)
{
    int i;
    for (i = 0; i < n; i++) {
        a[i].key1 = rng_range(0, key_range - 1);
        a[i].key2 = rng_range(0, KEY_MAX);
    }
    assign_order(a, n);
}

static int cmp_key1_then_order(const void* p, const void* q)
{
    const Data* a = (const Data*)p, * b = (const Data*)q;
    if (a->key1 != b->key1) return (a->key1 < b->key1) ? -1 : 1;
    return (a->order < b->order) ? -1 : (a->order > b->order);
}

void make_sorted(Data dst[], const Data base[], int n)
{
    memcpy(dst, base, sizeof(Data) * (size_t)n);
    qsort(dst, (size_t)n, sizeof(Data), cmp_key1_then_order);
    assign_order(dst, n);
}

void make_reverse(Data dst[], const Data sorted[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        dst[i] = sorted[n - 1 - i];
    assign_order(dst, n);
}

int make_partial(Data dst[], const Data sorted[], int n)
{
    int k, i, j, swaps = n * PARTIAL_SWAP_PERCENT / 100 / 2;
    Data t;
    memcpy(dst, sorted, sizeof(Data) * (size_t)n);
    for (k = 0; k < swaps; k++) {
        i = rng_range(0, n - 1);
        do { j = rng_range(0, n - 1); } while (j == i);
        t = dst[i]; dst[i] = dst[j]; dst[j] = t;
    }
    assign_order(dst, n);
    return swaps;
}

/* 4. 결과 검증 */
int is_sorted_by_key1(const Data a[], int n)
{
    int i;
    for (i = 1; i < n; i++)
        if (a[i - 1].key1 > a[i].key1)
            return 0;
    return 1;
}

int is_permutation_of(const Data sorted[], const Data original[], int n)
{
    int i, o, ok = 1;
    unsigned char* seen = (unsigned char*)calloc((size_t)n, 1);
    if (!seen) return 0;
    for (i = 0; i < n && ok; i++) {
        o = sorted[i].order;
        if (o < 0 || o >= n || seen[o] ||
            sorted[i].key1 != original[o].key1 ||
            sorted[i].key2 != original[o].key2)
            ok = 0;
        else
            seen[o] = 1;
    }
    free(seen);
    return ok;
}

int count_stability_violations(const Data a[], int n)
{
    int i, v = 0;
    for (i = 1; i < n; i++)
        if (a[i - 1].key1 == a[i].key1 && a[i - 1].order > a[i].order)
            v++;
    return v;
}

int count_equal_adjacent(const Data sorted[], int n)
{
    int i, e = 0;
    for (i = 1; i < n; i++)
        if (sorted[i - 1].key1 == sorted[i].key1)
            e++;
    return e;
}

int count_in_place(const Data a[], const Data sorted[], int n)
{
    int i, c = 0;
    for (i = 0; i < n; i++)
        if (a[i].key1 == sorted[i].key1)
            c++;
    return c;
}

double ascending_pair_ratio(const Data a[], int n)
{
    int i, c = 0;
    for (i = 1; i < n; i++)
        if (a[i - 1].key1 <= a[i].key1)
            c++;
    return 100.0 * c / (n - 1);
}

/* 5. 실험 실행 및 결과 출력 */

#define LABEL_WIDTH 22
#define NAME_WIDTH  22

/* 출력 보조 함수 */
static int hangul_bytes(void)
{
    static const char probe[] = "가";
    return (int)(sizeof(probe) - 1);
}

static int display_width(const char* s)
{
    const unsigned char* p = (const unsigned char*)s;
    int hb = hangul_bytes();
    int w = 0;
    while (*p) {
        if (*p < 0x80) {
            w += 1;
            p += 1;
        }
        else {
            int len = (hb == 2) ? 2 : (*p >= 0xF0) ? 4 : (*p >= 0xE0) ? 3 : 2;
            w += 2;
            while (len-- > 0 && *p) p++;
        }
    }
    return w;
}

static void print_padded(const char* s, int width)
{
    int w = display_width(s);
    printf("%s", s);
    while (w++ < width)
        putchar(' ');
}

static void print_label(const char* label)
{
    printf("  ");
    print_padded(label, LABEL_WIDTH);
    printf(" : ");
}

static const char* success_text(int b) { return b ? "성공" : "실패"; }
static const char* stable_text(int violations) { return violations ? "불안정" : "안정"; }

static void print_line(void)
{
    printf("------------------------------------------------------------\n\n");
}

static void* xmalloc(size_t size)
{
    void* p = malloc(size);
    if (!p) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    return p;
}

/* 한 정렬 알고리즘 실행: 원본의 동일한 복사본으로 정렬 후 검증 */
RunResult run_sort(const Algorithm* alg, const Data original[], Data work[], int n)
{
    RunResult r;
    Counter c = { 0, 0 };

    memcpy(work, original, sizeof(Data) * (size_t)n);
    sort_reset_passes();
    alg->sort(work, n, &c);

    r.cnt = c;
    r.sorted_ok = is_sorted_by_key1(work, n);
    r.perm_ok = is_permutation_of(work, original, n);
    r.violations = count_stability_violations(work, n);
    r.passes = sort_last_passes();
    return r;
}

/* [과제 4] 무작위 데이터 100회 반복 실험 */
void experiment_random(Data* orig, Data* work, int n)
{
    unsigned long long total_cmp[NUM_PERF_ALGS] = { 0 };
    unsigned long long total_mov[NUM_PERF_ALGS] = { 0 };
    unsigned long long min_cmp[NUM_PERF_ALGS], max_cmp[NUM_PERF_ALGS];
    long long total_passes = 0;
    int ok[NUM_PERF_ALGS] = { 0 };
    int unstable_trials[NUM_PERF_ALGS] = { 0 };
    long long dup_pairs = 0;
    int t, a;
    RunResult r;

    for (a = 0; a < NUM_PERF_ALGS; a++) {
        min_cmp[a] = (unsigned long long) - 1;
        max_cmp[a] = 0;
    }

    for (t = 0; t < NUM_TRIALS; t++) {
        fprintf(stderr, "\r  무작위 데이터 실험 진행 중... %3d / %d", t + 1, NUM_TRIALS);
        generate_random(orig, n);

        for (a = 0; a < NUM_PERF_ALGS; a++) {
            r = run_sort(&PERF_ALGS[a], orig, work, n);
            total_cmp[a] += r.cnt.comparisons;
            total_mov[a] += r.cnt.movements;
            if (r.cnt.comparisons < min_cmp[a]) min_cmp[a] = r.cnt.comparisons;
            if (r.cnt.comparisons > max_cmp[a]) max_cmp[a] = r.cnt.comparisons;
            if (r.sorted_ok && r.perm_ok) ok[a]++;
            if (r.violations > 0) unstable_trials[a]++;
            if (a == 2) total_passes += r.passes;
            if (a == 0) dup_pairs += count_equal_adjacent(work, n);
        }
    }
    fprintf(stderr, "\r                                                    \r");

    printf("[무작위 데이터]\n");
    printf("반복 횟수 : %d회\n", NUM_TRIALS);
    printf("(시행당 Key1이 같은 인접 쌍 평균 : %.1f개)\n\n", (double)dup_pairs / NUM_TRIALS);

    for (a = 0; a < NUM_PERF_ALGS; a++) {
        printf("%s\n", PERF_ALGS[a].name);
        print_label("총 비교 횟수");          printf("%llu\n", total_cmp[a]);
        print_label("평균 비교 횟수");        printf("%.1f\n", (double)total_cmp[a] / NUM_TRIALS);
        print_label("최소 / 최대 비교 횟수"); printf("%llu / %llu\n", min_cmp[a], max_cmp[a]);
        print_label("총 이동 횟수");          printf("%llu\n", total_mov[a]);
        print_label("평균 이동 횟수");        printf("%.1f\n", (double)total_mov[a] / NUM_TRIALS);
        if (a == 2) {
            print_label("평균 순회 수");
            printf("%.1f (일반 버블 정렬: %d)\n", (double)total_passes / NUM_TRIALS, n - 1);
        }
        print_label("정렬 성공");             printf("%d / %d\n", ok[a], NUM_TRIALS);
        print_label("안정성 위반 발생 시행"); printf("%d / %d\n\n", unstable_trials[a], NUM_TRIALS);
    }

    printf("[조기 종료 효과 - 무작위 데이터]\n");
    print_label("절약된 비교 횟수");
    printf("정렬 1회당 평균 %.1f회 (%.3f%%)\n\n",
        (double)(total_cmp[1] - total_cmp[2]) / NUM_TRIALS,
        100.0 * (double)(total_cmp[1] - total_cmp[2]) / (double)total_cmp[1]);
    print_line();
}

/* [과제 7] 입력 상태별 성능 비교 (하나의 원본에서 파생) */
static void report_state(const char* title, const Data input[], Data work[], int n)
{
    int a;
    RunResult r, rb = { { 0, 0 }, 0, 0, 0, 0 };
    unsigned long long normal_cmp = 0;

    printf("[%s]\n\n", title);
    for (a = 0; a < NUM_PERF_ALGS; a++) {
        r = run_sort(&PERF_ALGS[a], input, work, n);
        printf("%s\n", PERF_ALGS[a].name);
        print_label("비교 횟수");   printf("%llu\n", r.cnt.comparisons);
        print_label("이동 횟수");   printf("%llu\n", r.cnt.movements);
        if (a == 1 || a == 2) {
            print_label("순회 수"); printf("%d\n", r.passes);
        }
        print_label("정렬 결과");   printf("%s\n\n", success_text(r.sorted_ok && r.perm_ok));
        if (a == 1) normal_cmp = r.cnt.comparisons;
        if (a == 2) rb = r;
    }
    printf("[조기 종료 효과]\n");
    print_label("절약된 비교 횟수");
    printf("%llu회 (%.2f%%)\n\n",
        normal_cmp - rb.cnt.comparisons,
        100.0 * (double)(normal_cmp - rb.cnt.comparisons) / (double)normal_cmp);
    print_line();
}

void experiment_input_states(Data* base, Data* work, int n)
{
    Data* sorted = (Data*)xmalloc(sizeof(Data) * (size_t)n);
    Data* reverse = (Data*)xmalloc(sizeof(Data) * (size_t)n);
    Data* partial = (Data*)xmalloc(sizeof(Data) * (size_t)n);
    int swaps, in_place;

    generate_random(base, n);
    make_sorted(sorted, base, n);
    make_reverse(reverse, sorted, n);
    swaps = make_partial(partial, sorted, n);
    in_place = count_in_place(partial, sorted, n);

    report_state("이미 정렬된 데이터", sorted, work, n);
    report_state("역순으로 정렬된 데이터", reverse, work, n);

    printf("(일부 정렬된 데이터: 정렬된 데이터에 무작위 위치 교환 %d회 적용\n", swaps);
    printf(" -> 전체 %d개 중 %d개(%.1f%%)가 정렬 후의 위치에 그대로 있음,\n",
        n, in_place, 100.0 * in_place / n);
    printf("    인접한 두 원소 쌍의 %.2f%%가 이미 오름차순)\n\n",
        ascending_pair_ratio(partial, n));
    report_state("일부 정렬된 데이터", partial, work, n);

    free(sorted);
    free(reverse);
    free(partial);
}

/* [과제 6] 안정성 검증 */
static void print_small(const Data a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("(%d, %d)#%d ", a[i].key1, a[i].key2, a[i].order);
    printf("\n");
}

void experiment_stability(Data* work, int n)
{
    static const Data small[] = {
        { 527, 100, 0 }, { 527, 200, 1 }, { 125, 300, 2 }, { 892, 400, 3 }
    };

    static const Data small2[] = {
        { 3, 1, 0 }, { 1, 2, 1 }, { 3, 3, 2 }, { 2, 4, 3 },
        { 1, 5, 4 }, { 3, 6, 5 }, { 2, 7, 6 }, { 1, 8, 7 }
    };
    int ns = (int)(sizeof(small) / sizeof(small[0]));
    int ns2 = (int)(sizeof(small2) / sizeof(small2[0]));
    int a;
    Data* dup;
    RunResult r, res[NUM_STAB_ALGS];
    Counter ge_cnt = { 0, 0 }, gt_cnt = { 0, 0 };

    printf("[안정성 검증 상세]\n\n");
    printf("[시험 1] 예시 데이터  (형식: (Key1, Key2)#원래 순서)\n");
    print_padded("입력", NAME_WIDTH);
    printf(": ");
    print_small(small, ns);
    for (a = 0; a < NUM_STAB_ALGS; a++) {
        r = run_sort(&STAB_ALGS[a], small, work, ns);
        print_padded(STAB_ALGS[a].name, NAME_WIDTH);
        printf(": ");
        print_small(work, ns);
        print_padded("", NAME_WIDTH);
        printf("  -> %s\n", r.violations ? "같은 키의 순서가 바뀜" : "같은 키의 순서 유지");
    }
    printf("\n");

    printf("[시험 2] 같은 키가 흩어져 있는 데이터\n");
    print_padded("입력", NAME_WIDTH);
    printf(": ");
    print_small(small2, ns2);
    for (a = 0; a < NUM_STAB_ALGS; a++) {
        r = run_sort(&STAB_ALGS[a], small2, work, ns2);
        print_padded(STAB_ALGS[a].name, NAME_WIDTH);
        printf(": ");
        print_small(work, ns2);
        print_padded("", NAME_WIDTH);
        printf("  -> 위반 %d건\n", r.violations);
    }
    printf("\n");

    dup = (Data*)xmalloc(sizeof(Data) * (size_t)n);
    generate_duplicate_heavy(dup, n, DUP_KEY_RANGE);

    printf("[시험 3] 데이터 %d개, Key1 범위 0 ~ %d (키 하나당 약 %d개)\n",
        n, DUP_KEY_RANGE - 1, n / DUP_KEY_RANGE);
    for (a = 0; a < NUM_STAB_ALGS; a++) {
        res[a] = run_sort(&STAB_ALGS[a], dup, work, n);
        print_padded(STAB_ALGS[a].name, NAME_WIDTH);
        printf(": 정렬 %s, 같은 키 인접 쌍 %d개, 위반 %d건\n",
            success_text(res[a].sorted_ok && res[a].perm_ok),
            count_equal_adjacent(work, n), res[a].violations);
        if (a == 3) gt_cnt = res[a].cnt;
        if (a == 4) ge_cnt = res[a].cnt;
    }
    printf("\n[삽입 정렬 > 와 >= 비교 - 시험 3 데이터]\n");
    printf("  Case A (>)  : 비교 %llu회, 이동 %llu회\n", gt_cnt.comparisons, gt_cnt.movements);
    printf("  Case B (>=) : 비교 %llu회, 이동 %llu회\n", ge_cnt.comparisons, ge_cnt.movements);
    printf("  같은 키까지 밀어내서 늘어난 작업 : 비교 +%llu회, 이동 +%llu회\n\n",
        ge_cnt.comparisons - gt_cnt.comparisons, ge_cnt.movements - gt_cnt.movements);
    print_line();

    /* 요약 출력 */
    printf("[안정성 검증 결과]\n\n");
    for (a = 0; a < NUM_STAB_ALGS; a++) {
        print_padded(STAB_ALGS[a].name, NAME_WIDTH);
        printf(": %s (위반 %d건)\n", stable_text(res[a].violations), res[a].violations);
    }
    printf("\n");

    free(dup);
}

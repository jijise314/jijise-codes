#ifndef SORTING_H
#define SORTING_H

 /* 실험 설정 */
#define DATA_COUNT            10000     /* 데이터 개수                       */
#define KEY_MAX               1000000   /* 키 범위: 0 ~ KEY_MAX              */
#define NUM_TRIALS            100       /* 무작위 데이터 반복 횟수           */
#define PARTIAL_SWAP_PERCENT  5         /* 부분 정렬: 교란할 원소 비율(%)    */
#define DUP_KEY_RANGE         100       /* 안정성 시험: Key1 범위 0 ~ 99     */

/* 자료형 */
typedef struct {
    int key1;   /* 정렬 기준 키                                   */
    int key2;   /* 정렬 기준이 아닌 두 번째 키                    */
    int order;  /* 원래 입력 순서 (안정성 검증용, 정렬 기준 아님) */
} Data;

typedef struct {
    unsigned long long comparisons; /* Key1 비교 횟수   */
    unsigned long long movements;   /* 데이터 이동 횟수 */
} Counter;

typedef struct {
    const char* name;
    void (*sort)(Data a[], int n, Counter* c);
} Algorithm;

typedef struct {
    Counter cnt;
    int sorted_ok;      /* Key1 오름차순 여부                       */
    int perm_ok;        /* 원본 데이터가 손실/변형 없이 보존됐는지  */
    int violations;     /* 안정성 위반 쌍의 수                      */
    int passes;         /* 버블 정렬의 실제 순회 수                 */
} RunResult;

/* 난수 생성기 (xorshift32) */
unsigned int rng_next(void);
int rng_range(int lo, int hi);   /* lo 이상 hi 이하의 정수 */

/* 정렬 알고리즘 */
void selection_sort(Data a[], int n, Counter* c);
void bubble_sort(Data a[], int n, Counter* c);         /* 일반: n-1번 순회   */
void bubble_sort_early(Data a[], int n, Counter* c);   /* 조기 종료          */
void insertion_sort_gt(Data a[], int n, Counter* c);   /* Case A: >          */
void insertion_sort_ge(Data a[], int n, Counter* c);   /* Case B: >=         */

/* 마지막으로 실행한 버블 정렬의 실제 순회 수 (버블 정렬 외에는 0) */
int  sort_last_passes(void);
void sort_reset_passes(void);

/* 성능 실험에 사용하는 4가지 정렬 (삽입 정렬은 Case A '>' 사용) */
#define NUM_PERF_ALGS 4
extern const Algorithm PERF_ALGS[NUM_PERF_ALGS];

/* 안정성 검증에 사용하는 5가지 정렬 */
#define NUM_STAB_ALGS 5
extern const Algorithm STAB_ALGS[NUM_STAB_ALGS];

/* 시험 데이터 준비 */
void assign_order(Data a[], int n);                         /* order = 0,1,2,... */
void generate_random(Data a[], int n);                      /* 무작위 데이터     */
void generate_duplicate_heavy(Data a[], int n, int key_range); /* Key1 중복 데이터 */
void make_sorted(Data dst[], const Data base[], int n);     /* 오름차순          */
void make_reverse(Data dst[], const Data sorted[], int n);  /* 내림차순          */
int  make_partial(Data dst[], const Data sorted[], int n);  /* 부분 정렬, 교환 횟수 반환 */

/* 결과 검증 */
int is_sorted_by_key1(const Data a[], int n);

/* 정렬 결과가 원본의 재배열인지 확인 (두 키가 함께 이동했는지, 누락/중복 없는지) */
int is_permutation_of(const Data sorted[], const Data original[], int n);

/* 안정성 위반 수: Key1이 같은 인접 쌍에서 원래 순서(order)가 뒤집힌 경우 */
int count_stability_violations(const Data a[], int n);

/* 정렬된 배열에서 Key1이 같은 인접 쌍의 수 */
int count_equal_adjacent(const Data sorted[], int n);

/* 정렬된 위치와 같은 Key1을 가진 원소 수 (부분 정렬 정도 확인용) */
int count_in_place(const Data a[], const Data sorted[], int n);

/* 오름차순 인접 쌍의 비율 (%) */
double ascending_pair_ratio(const Data a[], int n);

/* 실험 실행 및 결과 출력 */
RunResult run_sort(const Algorithm* alg, const Data original[], Data work[], int n);

/* [과제 4] 무작위 데이터 NUM_TRIALS회 반복 실험 */
void experiment_random(Data* orig, Data* work, int n);

/* [과제 7] 입력 상태별(정렬/역순/부분 정렬) 성능 비교 */
void experiment_input_states(Data* base, Data* work, int n);

/* [과제 6] 안정성 검증 */
void experiment_stability(Data* work, int n);

#endif

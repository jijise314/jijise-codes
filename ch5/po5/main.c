#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "BST.h"

#define DATA_SIZE     100
#define SEARCH_SIZE   50
#define MAX_VALUE     1000

 /* ---------------- 난수 생성 ---------------- */
int random_value(void)
{
    return rand() % (MAX_VALUE + 1);
}

/* ---------------- 데이터 생성 ---------------- */
void generate_unique_data(int arr[], int n)
{
    int used[MAX_VALUE + 1] = { 0 };
    int count = 0;

    while (count < n) {
        int v = random_value();
        if (used[v]) continue;
        used[v] = 1;
        arr[count++] = v;
    }
}

/* ---------------- 순차 탐색 ---------------- */
int sequential_search(const int arr[], int n, int key, int* cmp)
{
    int i;
    *cmp = 0;
    for (i = 0; i < n; i++) {
        (*cmp)++;
        if (arr[i] == key) return i;
    }
    return -1;
}

void print_line(void)
{
    printf("==================================================================\n");
}

void print_array(const int arr[], int n, int per_line)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%5d", arr[i]);
        if ((i + 1) % per_line == 0 || i == n - 1) printf("\n");
    }
}

int main(void)
{
    int data[DATA_SIZE];
    int keys[SEARCH_SIZE];
    TreeNode* root = NULL;
    long build_cmp = 0;
    int i;

    int found[SEARCH_SIZE];
    int seq_cmp[SEARCH_SIZE];
    int bst_cmp[SEARCH_SIZE];
    long seq_total = 0, bst_total = 0;

    int  succ_cnt = 0, fail_cnt = 0;
    long seq_succ = 0, seq_fail = 0, bst_succ = 0, bst_fail = 0;

    srand((unsigned int)time(NULL));

    /* ---------- 1. 데이터 생성 및 저장 ---------- */
    generate_unique_data(data, DATA_SIZE);
    for (i = 0; i < DATA_SIZE; i++)
        root = bst_insert(root, data[i], &build_cmp);

    print_line();
    printf(" 순차 탐색 vs 이진 탐색 트리(BST) 탐색 비교\n");
    print_line();

    printf("\n[1] 생성된 서로 다른 정수 %d개 (발생 순서, 정렬하지 않음)\n\n", DATA_SIZE);
    print_array(data, DATA_SIZE, 10);

    printf("\n[2] 이진 탐색 트리(BST) 생성\n\n");
    printf("  BST 생성 총 비교 횟수             : %ld\n", build_cmp);
    printf("  삽입 1회당 평균 비교 횟수         : %.2f\n", (double)build_cmp / DATA_SIZE);
    printf("  트리 높이 (레벨 수)               : %d\n", bst_height(root));
    printf("  노드 %d개의 최소 가능 높이       : 7\n", DATA_SIZE);
    printf("  노드 평균 레벨 (루트 = 1)         : %.2f\n",
        (double)bst_level_sum(root, 1) / DATA_SIZE);

    /* ---------- 2. 탐색 데이터 생성 ---------- */
    for (i = 0; i < SEARCH_SIZE; i++)
        keys[i] = random_value();

    printf("\n[3] 생성된 탐색 대상 %d개\n\n", SEARCH_SIZE);
    print_array(keys, SEARCH_SIZE, 10);

    /* ---------- 3. 탐색 수행 ---------- */
    for (i = 0; i < SEARCH_SIZE; i++) {
        int s_idx = sequential_search(data, DATA_SIZE, keys[i], &seq_cmp[i]);
        int b_ok = bst_search(root, keys[i], &bst_cmp[i]);

        if ((s_idx >= 0) != b_ok) {
            fprintf(stderr, "오류: 탐색 키 %d의 두 탐색 결과가 다릅니다\n", keys[i]);
            return 1;
        }
        found[i] = b_ok;
        seq_total += seq_cmp[i];
        bst_total += bst_cmp[i];

        if (found[i]) { succ_cnt++; seq_succ += seq_cmp[i]; bst_succ += bst_cmp[i]; }
        else { fail_cnt++; seq_fail += seq_cmp[i]; bst_fail += bst_cmp[i]; }
    }

    printf("\n[4] 탐색 대상별 결과\n\n");
    printf("  번호   탐색 키   결과   순차 탐색   BST 탐색\n");
    printf("  ----   -------   ----   ---------   --------\n");
    for (i = 0; i < SEARCH_SIZE; i++) {
        printf("  %4d   %7d   %s   %9d   %8d\n",
            i + 1, keys[i], found[i] ? "성공" : "실패", seq_cmp[i], bst_cmp[i]);
    }

    /* ---------- 4. 요약 ---------- */
    printf("\n[5] 결과 요약\n\n");
    printf("탐색 횟수 : %d회  (성공 %d회, 실패 %d회)\n\n", SEARCH_SIZE, succ_cnt, fail_cnt);

    printf("순차 탐색\n");
    printf("  총 비교 횟수       : %ld\n", seq_total);
    printf("  평균 비교 횟수     : %.2f\n", (double)seq_total / SEARCH_SIZE);
    if (succ_cnt) printf("    - 성공 시 평균  : %.2f\n", (double)seq_succ / succ_cnt);
    if (fail_cnt) printf("    - 실패 시 평균  : %.2f\n", (double)seq_fail / fail_cnt);

    printf("\nBST 탐색\n");
    printf("  총 비교 횟수       : %ld\n", bst_total);
    printf("  평균 비교 횟수     : %.2f\n", (double)bst_total / SEARCH_SIZE);
    if (succ_cnt) printf("    - 성공 시 평균  : %.2f\n", (double)bst_succ / succ_cnt);
    if (fail_cnt) printf("    - 실패 시 평균  : %.2f\n", (double)bst_fail / fail_cnt);

    /* ---------- 5. 생성 비용 포함 비교 ---------- */
    printf("\n[6] BST 생성 비용을 포함한 비교\n\n");
    printf("  배열 : 생성 0 + 탐색 %ld = %ld\n", seq_total, seq_total);
    printf("  BST  : 생성 %ld + 탐색 %ld = %ld\n", build_cmp, bst_total, build_cmp + bst_total);

    bst_free(root);
    print_line();
    return 0;
}

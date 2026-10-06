#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AvlTree.h"

static int rand_value(void)
{
    return rand() % (MAX_VALUE + 1);
}

static const char* with_comma(long n, char* buf)
{
    char digits[32];
    int len = 0, i, k = 0;

    if (n < 0) { buf[k++] = '-'; n = -n; }
    do {
        digits[len++] = (char)('0' + n % 10);
        n /= 10;
    } while (n > 0);

    for (i = len - 1; i >= 0; i--) {
        buf[k++] = digits[i];
        if (i > 0 && i % 3 == 0) buf[k++] = ',';
    }
    buf[k] = '\0';
    return buf;
}

static void print_line(void)
{
    printf("============================================================\n");
}

int main(void)
{
    int generated[NUM_DATA];
    int is_dup[NUM_DATA];
    int keys[NUM_SEARCH];

    IntArray arr;
    BSTNode* bst = NULL;
    AVLNode* avl = NULL;
    RotationStats rs;

    long arr_build = 0, bst_build = 0, avl_build = 0;
    long seq_total = 0, bst_total = 0, avl_total = 0;
    int stored = 0, dup = 0, found_cnt = 0;
    int i, ins_a, ins_b, ins_c;
    char b1[32], b2[32], b3[32];

    array_init(&arr);
    srand((unsigned)time(NULL));

    /* ---------------- 1. 데이터 생성 및 삽입 ---------------- */
    for (i = 0; i < NUM_DATA; i++) {
        generated[i] = rand_value();

        ins_a = array_insert(&arr, generated[i], &arr_build);
        ins_b = bst_insert(&bst, generated[i], &bst_build);
        ins_c = avl_insert(&avl, generated[i], &avl_build);

        if (ins_a != ins_b || ins_b != ins_c) {
            fprintf(stderr, "오류: 세 자료구조의 중복 판정이 다릅니다 (%d)\n",
                generated[i]);
            return 1;
        }
        is_dup[i] = !ins_a;
        if (ins_a) stored++; else dup++;
    }

    print_line();
    printf(" 생성된 정수 (%d개)   * = 중복 (삽입되지 않음)\n", NUM_DATA);
    print_line();
    for (i = 0; i < NUM_DATA; i++) {
        printf("%5d%c", generated[i], is_dup[i] ? '*' : ' ');
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    printf("생성된 정수의 수    : %d\n", NUM_DATA);
    printf("저장된 서로 다른 값 : %d\n", stored);
    printf("중복으로 제외된 값  : %d\n\n", dup);

    printf("[생성 과정 숫자 비교 횟수]\n");
    printf("배열     : %6s\n", with_comma(arr_build, b1));
    printf("BST      : %6s\n", with_comma(bst_build, b2));
    printf("AVL 트리 : %6s\n\n", with_comma(avl_build, b3));

    printf("[삽입 완료 후 자료구조]\n");
    printf("배열의 길이     : %d\n", arr.length);
    printf("BST의 높이      : %d\n", bst_height(bst));
    printf("AVL 트리의 높이 : %d\n\n", avl_height(avl));

    rs = avl_rotation_stats();
    printf("AVL 회전 횟수 : LL=%d, RR=%d, LR=%d, RL=%d (총 %d회)\n\n",
        rs.LL, rs.RR, rs.LR, rs.RL, rs.LL + rs.RR + rs.LR + rs.RL);

    /* ---------------- 2. 탐색 키 생성 ---------------- */
    for (i = 0; i < NUM_SEARCH; i++) keys[i] = rand_value();

    print_line();
    printf(" 탐색 대상 (%d개)\n", NUM_SEARCH);
    print_line();
    for (i = 0; i < NUM_SEARCH; i++) {
        printf("%5d ", keys[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    /* ---------------- 3. 탐색 ---------------- */
    print_line();
    printf(" 탐색 결과\n");
    print_line();

    for (i = 0; i < NUM_SEARCH; i++) {
        long c_seq = 0, c_bst = 0, c_avl = 0;
        int r_seq = seq_search(&arr, keys[i], &c_seq);
        int r_bst = bst_search(bst, keys[i], &c_bst);
        int r_avl = avl_search(avl, keys[i], &c_avl);

        seq_total += c_seq;
        bst_total += c_bst;
        avl_total += c_avl;
        if (r_seq) found_cnt++;

        printf("[%2d] 탐색 대상 : %d\n\n", i + 1, keys[i]);
        printf("순차 탐색\n");
        printf("결과      : %s\n", r_seq ? "성공" : "실패");
        printf("비교 횟수 : %ld\n\n", c_seq);
        printf("BST 탐색\n");
        printf("결과      : %s\n", r_bst ? "성공" : "실패");
        printf("비교 횟수 : %ld\n\n", c_bst);
        printf("AVL 트리 탐색\n");
        printf("결과      : %s\n", r_avl ? "성공" : "실패");
        printf("비교 횟수 : %ld\n", c_avl);
        printf("------------------------------------------------------------\n");
    }

    print_line();
    printf(" 탐색 결과 요약표\n");
    print_line();
    printf(" 번호  탐색값  결과   순차   BST   AVL\n");
    for (i = 0; i < NUM_SEARCH; i++) {
        long c_seq = 0, c_bst = 0, c_avl = 0;
        int r = seq_search(&arr, keys[i], &c_seq);
        bst_search(bst, keys[i], &c_bst);
        avl_search(avl, keys[i], &c_avl);
        printf(" %4d  %6d  %s  %5ld %5ld %5ld\n",
            i + 1, keys[i], r ? "성공" : "실패", c_seq, c_bst, c_avl);
    }
    printf("\n");

    /* ---------------- 4. 최종 요약 ---------------- */
    print_line();
    printf(" 최종 요약\n");
    print_line();
    printf("저장된 서로 다른 값 : %d\n\n", stored);

    printf("[생성 과정 숫자 비교 횟수]\n");
    printf("배열     : %6s\n", with_comma(arr_build, b1));
    printf("BST      : %6s\n", with_comma(bst_build, b2));
    printf("AVL 트리 : %6s\n\n", with_comma(avl_build, b3));

    printf("[삽입 완료 후 자료구조]\n");
    printf("배열의 길이     : %d\n", arr.length);
    printf("BST의 높이      : %d\n", bst_height(bst));
    printf("AVL 트리의 높이 : %d\n\n", avl_height(avl));

    printf("탐색 횟수 : %d  (성공 %d, 실패 %d)\n\n",
        NUM_SEARCH, found_cnt, NUM_SEARCH - found_cnt);

    printf("순차 탐색\n");
    printf("총 비교 횟수   : %s\n", with_comma(seq_total, b1));
    printf("평균 비교 횟수 : %.2f\n\n", (double)seq_total / NUM_SEARCH);

    printf("BST 탐색\n");
    printf("총 비교 횟수   : %s\n", with_comma(bst_total, b2));
    printf("평균 비교 횟수 : %.2f\n\n", (double)bst_total / NUM_SEARCH);

    printf("AVL 트리 탐색\n");
    printf("총 비교 횟수   : %s\n", with_comma(avl_total, b3));
    printf("평균 비교 횟수 : %.2f\n", (double)avl_total / NUM_SEARCH);

    bst_free(bst);
    avl_free(avl);
    return 0;
}

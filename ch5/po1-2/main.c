#include <stdio.h>
#include <string.h>

#include "btree.h"

#define MAX_LINE 4096

static void chomp(char* s)
{
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[--n] = '\0';
}

int main(void)
{
    char line[MAX_LINE];
    printf("괄호 표기 이진트리 입력 (예: A(B(D,E),C(,F))) : ");
    if (!fgets(line, sizeof line, stdin)) return 0;
    chomp(line);

    ArrayTree at;
    Node* root;
    const char* err;

    if ((err = arr_build(&at, line)) != NULL) {
        printf("입력 오류: %s\n", err);
        arr_free(&at);
        return 1;
    }
    if ((err = lnk_build(&root, line)) != NULL) {
        printf("입력 오류: %s\n", err);
        arr_free(&at);
        return 1;
    }

    puts("\n==================== 배열 구현 ====================");
    arr_dump(&at);
    puts("\n  [트리 구조]");
    arr_print(&at);
    Stats as = arr_stats(&at);
    Shape ash = arr_shape(&at, &as);
    puts("\n  [트리 정보]");
    print_stats(&as);
    print_shape(&ash, as.nodes);

    puts("\n==================== 연결 구현 ====================");
    puts("  [트리 구조]");
    lnk_print(root);
    Stats ls = lnk_stats(root);
    Shape lsh = lnk_shape(root, &ls);
    puts("\n  [트리 정보]");
    print_stats(&ls);
    print_shape(&lsh, ls.nodes);

    arr_free(&at);
    lnk_free(root);
    return 0;
}

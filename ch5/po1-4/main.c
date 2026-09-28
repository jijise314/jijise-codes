#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "btreeTraversal.h"

int main(void)
{
    char input[256];
    char compact[256];

    printf("===== 이진트리 반복적 순회 프로그램 =====\n");
    printf("괄호 표기법으로 이진트리를 입력하세요. (예: A(B(D,E),C(,F)))\n");
    printf("아무것도 입력하지 않고 Enter를 누르면 종료합니다.\n\n");

    while (1) {
        TreeNode* root;
        int i, j;

        printf("트리 입력 > ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        input[strcspn(input, "\r\n")] = '\0';
        if (input[0] == '\0') break;

        root = parse_tree(input);
        if (root == NULL) {
            printf("[오류] 올바르지 않은 트리 표현입니다.\n\n");
            continue;
        }

        for (i = 0, j = 0; input[i]; i++)
            if (!isspace((unsigned char)input[i])) compact[j++] = input[i];
        compact[j] = '\0';

        printf("\n입력받은 트리 : %s\n", compact);
        printf("\n[트리 구조]\n");
        print_structure(root);

        printf("\n순회 결과\n");
        printf("Preorder  : "); preorder(root);
        printf("Inorder   : "); inorder(root);
        printf("Postorder : "); postorder(root);
        printf("\n");

        free_tree(root);
    }

    printf("프로그램을 종료합니다.\n");
    return 0;
}

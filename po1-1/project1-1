#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int is_valid_tree(const char* tree) {
    if (tree[0] != 'A') return 0;

    int p_count = 0;
    for (int i = 0; tree[i] != '\0'; i++) {
        char ch = tree[i];

        if (!(ch >= 'A' && ch <= 'Z') && ch != '(' && ch != ')' && ch != ',') {
            return 0;
        }

        if (ch == '(') {
            p_count++;
            if (i > 0 && !(tree[i - 1] >= 'A' && tree[i - 1] <= 'Z')) return 0;
        }
        else if (ch == ')') {
            p_count--;
            if (i > 0 && (tree[i - 1] == '(' || tree[i - 1] == ',')) return 0;
        }

        if (p_count < 0) return 0;
    }

    if (p_count != 0) return 0;

    return 1;
}

void nesting_tree(const char* tree) {
    char nodes[100];
    int counts[100];
    int top = -1;

    int total = 0, non_leaf = 0, max_deg = 0, max_h = 0;
    char last_node = 0, parent_c = 0;
    char children_c[100] = { 0 };
    int c_idx = 0;

    printf("\n[트리 출력]\n");

    for (int i = 0; tree[i] != '\0'; i++) {
        char ch = tree[i];

        if (ch >= 'A' && ch <= 'Z') {
            total++;
            last_node = ch;

            if (top >= 0) {
                counts[top]++;
                if (nodes[top] == 'C') children_c[c_idx++] = ch;
            }
            if (ch == 'C' && top >= 0) parent_c = nodes[top];

            for (int j = 1; j <= top; j++) printf("   ");
            if (top < 0) printf("%c\n", ch);
            else printf("+---%c\n", ch);

        }
        else if (ch == '(') {
            non_leaf++;
            top++;
            nodes[top] = last_node;
            counts[top] = 0;

            if (top + 1 > max_h) max_h = top + 1;
        }
        else if (ch == ')') {
            if (counts[top] > max_deg) max_deg = counts[top];
            top--;
        }
    }

    printf("\n[트리 분석 결과]\n");
    printf("전체 노드의 수: %d\n", total);
    printf("단말 노드의 수: %d\n", total - non_leaf);
    printf("비단말 노드의 수: %d\n", non_leaf);
    printf("트리의 높이: %d\n", max_h);
    printf("트리의 차수(최대 차수): %d\n", max_deg);

    printf("노드 C의 부모 노드: %c\n", parent_c ? parent_c : 'X');

    printf("노드 C의 자식 노드: ");
    for (int i = 0; i < c_idx; i++) {
        printf("%c ", children_c[i]);
    }
    if (c_idx == 0) printf("없음");
    printf("\n");
}

int main() {
    char tree_input[100];

    printf("트리를 괄호 표기법으로 입력하세요 (예-대문자로 입력: A(B(E,F),C,D(G))): ");
    scanf("%99s", tree_input);

    if (!is_valid_tree(tree_input)) {
        printf("오류: 올바른 트리의 괄호 표기법이 아닙니다.\n");
        return 1;
    }

    nesting_tree(tree_input);

    return 0;
}

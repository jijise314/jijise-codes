#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Torder.h"

#define MAX_LINE    256
#define MAX_TOKEN   8

static int tokenize(char* line, char* tok[], int maxtok)
{
    int   n = 0;
    char* p = strtok(line, " \t\r\n");

    while (p != NULL) {
        if (n >= maxtok) return maxtok + 1;
        tok[n++] = p;
        p = strtok(NULL, " \t\r\n");
    }
    return n;
}

static int equal_ci(const char* a, const char* b)
{
    while (*a && *b) {
        if (toupper((unsigned char)*a) != toupper((unsigned char)*b)) return 0;
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}

static int is_cmd(const char* s, const char* full)
{
    if (s[0] != '\0' && s[1] == '\0')
        return toupper((unsigned char)s[0]) == full[0];
    return equal_ci(s, full);
}

static int get_data(const char* s, char* out)
{
    if (s[0] == '\0' || s[1] != '\0') return 0;
    if (!isupper((unsigned char)s[0])) return 0;
    *out = s[0];
    return 1;
}

static int get_child(const char* s, char* out)
{
    if (is_cmd(s, "LEFT")) { *out = 'L'; return 1; }
    if (is_cmd(s, "RIGHT")) { *out = 'R'; return 1; }
    return 0;
}

static void cmd_insert(BTree* tree, char* tok[], int ntok)
{
    Status    st = OK;
    TreeNode* parent, * dummy;
    char      value, child;

    if (ntok == 3 && strcmp(tok[1], "/") == 0) {
        if (!get_data(tok[2], &value)) { print_error(ERR_BAD_DATA); return; }
        insert_root(tree, value, &st);
        if (st != OK) print_error(st);
        else          printf("루트 노드 %c 를 생성했습니다.\n", value);
        return;
    }

    if (ntok != 4) { print_error(ERR_ARGC); return; }

    if (!get_child(tok[2], &child)) { print_error(ERR_BAD_CHILD); return; }
    if (!get_data(tok[3], &value)) { print_error(ERR_BAD_DATA);  return; }

    parent = locate(tree, tok[1], &dummy, &st);
    if (parent == NULL) { print_error(st); return; }

    insert_child(tree, parent, child, value, &st);
    if (st != OK) print_error(st);
    else          printf("%s/%c 노드를 추가했습니다.\n", tok[1], value);
}

static void cmd_delete(BTree* tree, char* tok[], int ntok)
{
    Status    st = OK;
    TreeNode* node, * parent;

    if (ntok != 2) { print_error(ERR_ARGC); return; }

    node = locate(tree, tok[1], &parent, &st);
    if (node == NULL) { print_error(st); return; }

    delete_node(tree, node, parent, &st);
    if (st != OK) print_error(st);
    else          printf("%s 노드를 삭제했습니다.\n", tok[1]);
}

static void cmd_update(BTree* tree, char* tok[], int ntok)
{
    Status    st = OK;
    TreeNode* node, * parent;
    char      value, old;

    if (ntok != 3) { print_error(ERR_ARGC); return; }
    if (!get_data(tok[2], &value)) { print_error(ERR_BAD_DATA); return; }

    node = locate(tree, tok[1], &parent, &st);
    if (node == NULL) { print_error(st); return; }

    old = node->data;
    update_value(tree, node, parent, value, &st);
    if (st != OK) print_error(st);
    else          printf("%c 를 %c 로 변경했습니다.\n", old, value);
}

static void cmd_read(BTree* tree, char* tok[], int ntok)
{
    Status    st = OK;
    TreeNode* node, * parent, * l, * r;

    if (ntok != 2) { print_error(ERR_ARGC); return; }

    node = locate(tree, tok[1], &parent, &st);
    if (node == NULL) { print_error(st); return; }

    read_child(tree, node, &l, &r);
    if (l == NULL && r == NULL) {
        printf("단말 노드입니다. (자식 없음)\n");
        return;
    }
    if (l != NULL)                printf("%c(L)", l->data);
    if (l != NULL && r != NULL)   printf(", ");
    if (r != NULL)                printf("%c(R)", r->data);
    printf("\n");
}

int main(void)
{
    BTree* tree = create_btree(MAX_NODES);
    char   line[MAX_LINE];
    char* tok[MAX_TOKEN];
    int    ntok;

    printf("===== 이진트리 조작 프로그램 =====\n");
    printf("Insert / A            : 루트 노드 생성\n");
    printf("Insert /A L B         : 자식 노드 추가\n");
    printf("Delete /A/B           : 단말 노드 삭제\n");
    printf("Update /A/B X         : 노드 데이터 변경\n");
    printf("Read   /A             : 자식 정보 출력\n");
    printf("Print                 : 전체 트리 출력\n");
    printf("Quit                  : 종료\n");

    for (;;) {
        printf("\n명령> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;

        ntok = tokenize(line, tok, MAX_TOKEN);
        if (ntok == 0) continue;
        if (ntok > MAX_TOKEN) { print_error(ERR_ARGC); continue; }

        if (is_cmd(tok[0], "INSERT")) {
            if (ntok < 3) print_error(ERR_ARGC);
            else          cmd_insert(tree, tok, ntok);
        }
        else if (is_cmd(tok[0], "DELETE")) {
            cmd_delete(tree, tok, ntok);
        }
        else if (is_cmd(tok[0], "UPDATE")) {
            cmd_update(tree, tok, ntok);
        }
        else if (is_cmd(tok[0], "READ")) {
            cmd_read(tree, tok, ntok);
        }
        else if (is_cmd(tok[0], "PRINT")) {
            if (ntok != 1) print_error(ERR_ARGC);
            else           print_btree(tree);
        }
        else if (is_cmd(tok[0], "QUIT") || is_cmd(tok[0], "EXIT")) {
            if (ntok != 1) print_error(ERR_ARGC);
            else           break;
        }
        else {
            print_error(ERR_CMD);
        }
    }

    destroy_btree(tree);
    printf("\n프로그램을 종료합니다.\n");
    return 0;
}

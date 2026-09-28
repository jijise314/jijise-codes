#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btreeTraversal.h"

 /* ================= 스택 (TreeNode* 저장) ================= */
typedef struct {
    TreeNode** items;
    int top;
    int capacity;
} Stack;

static void fatal_memory(void)
{
    fprintf(stderr, "메모리 할당에 실패했습니다.\n");
    exit(EXIT_FAILURE);
}

static void stack_init(Stack* s)
{
    s->capacity = 16;
    s->top = -1;
    s->items = (TreeNode**)malloc(sizeof(TreeNode*) * s->capacity);
    if (s->items == NULL) fatal_memory();
}

static void stack_free(Stack* s)
{
    free(s->items);
    s->items = NULL;
    s->top = -1;
    s->capacity = 0;
}

static int stack_empty(const Stack* s)
{
    return s->top < 0;
}

static void push(Stack* s, TreeNode* node)
{
    if (s->top + 1 == s->capacity) {
        TreeNode** tmp = (TreeNode**)realloc(s->items, sizeof(TreeNode*) * s->capacity * 2);
        if (tmp == NULL) fatal_memory();
        s->items = tmp;
        s->capacity *= 2;
    }
    s->items[++s->top] = node;
}

static TreeNode* pop(Stack* s)
{
    if (stack_empty(s)) return NULL;
    return s->items[s->top--];
}

static TreeNode* peek(const Stack* s)
{
    if (stack_empty(s)) return NULL;
    return s->items[s->top];
}

/* ================= 노드 생성 / 해제 ================= */
static TreeNode* create_node(char data)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) fatal_memory();
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(TreeNode* root)
{
    Stack s;
    if (root == NULL) return;
    stack_init(&s);
    push(&s, root);
    while (!stack_empty(&s)) {
        TreeNode* node = pop(&s);
        if (node->left)  push(&s, node->left);
        if (node->right) push(&s, node->right);
        free(node);
    }
    stack_free(&s);
}

/* ================= 괄호 표기법 파서 (반복적) ================= */
typedef enum { TK_START, TK_LABEL, TK_LPAREN, TK_COMMA, TK_RPAREN } TokenType;

typedef struct {
    TreeNode* node;
    int side;
} Frame;

TreeNode* parse_tree(const char* str)
{
    size_t len = strlen(str);
    Frame* frames = (Frame*)malloc(sizeof(Frame) * (len + 1));
    int ftop = -1;
    TreeNode* root = NULL;
    TreeNode* last = NULL;
    TokenType prev = TK_START;
    size_t i = 0;

    if (frames == NULL) fatal_memory();

    while (i < len) {
        char c = str[i];

        if (isspace((unsigned char)c)) { i++; continue; }

        if (isalnum((unsigned char)c)) {
            TreeNode* node;

            if (prev != TK_START && prev != TK_LPAREN && prev != TK_COMMA) goto fail;

            node = create_node(c);
            if (prev == TK_START)        root = node;
            else if (prev == TK_LPAREN)  frames[ftop].node->left = node;
            else                         frames[ftop].node->right = node;

            last = node;
            prev = TK_LABEL;
            i++;
            continue;
        }

        switch (c) {
        case '(':
            if (prev != TK_LABEL) goto fail;
            ftop++;
            frames[ftop].node = last;
            frames[ftop].side = 0;
            prev = TK_LPAREN;
            break;

        case ',':
            if (ftop < 0 || frames[ftop].side == 1 || prev == TK_COMMA) goto fail;
            frames[ftop].side = 1;
            prev = TK_COMMA;
            break;

        case ')':
            if (ftop < 0 || prev == TK_LPAREN) goto fail;
            if (frames[ftop].node->left == NULL && frames[ftop].node->right == NULL) goto fail;
            last = frames[ftop].node;
            ftop--;
            prev = TK_RPAREN;
            break;

        default:
            goto fail;
        }
        i++;
    }

    if (prev == TK_START || ftop >= 0) goto fail;

    free(frames);
    return root;

fail:
    free(frames);
    free_tree(root);
    return NULL;
}

/* ================= 트리 구조 출력 (반복적) ================= */
typedef struct {
    TreeNode* node;
    int depth;
    int tag;
} PrintItem;

static int count_nodes(TreeNode* root)
{
    Stack s;
    int count = 0;
    if (root == NULL) return 0;
    stack_init(&s);
    push(&s, root);
    while (!stack_empty(&s)) {
        TreeNode* node = pop(&s);
        count++;
        if (node->right) push(&s, node->right);
        if (node->left)  push(&s, node->left);
    }
    stack_free(&s);
    return count;
}

void print_structure(TreeNode* root)
{
    int n = count_nodes(root);
    PrintItem* st;
    int top = -1;

    if (root == NULL) return;
    st = (PrintItem*)malloc(sizeof(PrintItem) * n);
    if (st == NULL) fatal_memory();

    st[++top] = (PrintItem){ root, 0, 0 };
    while (top >= 0) {
        PrintItem it = st[top--];
        int d;
        for (d = 0; d < it.depth; d++) printf("    ");
        if (it.tag == 0)      printf("%c (root)\n", it.node->data);
        else if (it.tag == 1) printf("[L] %c\n", it.node->data);
        else                  printf("[R] %c\n", it.node->data);

        if (it.node->right) st[++top] = (PrintItem){ it.node->right, it.depth + 1, 2 };
        if (it.node->left)  st[++top] = (PrintItem){ it.node->left,  it.depth + 1, 1 };
    }
    free(st);
}

/* ================= 방문(출력) ================= */
static void visit(TreeNode* node, int* first)
{
    printf("%s%c", *first ? "" : " ", node->data);
    *first = 0;
}

/* ================= 전위 순회 : Root -> Left -> Right ================= */
void preorder(TreeNode* tree)
{
    Stack s;
    int first = 1;

    if (tree == NULL) { printf("\n"); return; }
    stack_init(&s);
    push(&s, tree);

    while (!stack_empty(&s)) {
        TreeNode* node = pop(&s);
        visit(node, &first);
        if (node->right) push(&s, node->right);
        if (node->left)  push(&s, node->left);
    }
    printf("\n");
    stack_free(&s);
}

/* ================= 중위 순회 : Left -> Root -> Right ================= */
void inorder(TreeNode* tree)
{
    Stack s;
    TreeNode* cur = tree;
    int first = 1;

    stack_init(&s);
    while (cur != NULL || !stack_empty(&s)) {
        while (cur != NULL) {
            push(&s, cur);
            cur = cur->left;
        }
        cur = pop(&s);
        visit(cur, &first);
        cur = cur->right;
    }
    printf("\n");
    stack_free(&s);
}

/* ================= 후위 순회 : Left -> Right -> Root ================= */
void postorder(TreeNode* tree)
{
    Stack s;
    TreeNode* cur = tree;
    TreeNode* lastVisited = NULL;
    int first = 1;

    stack_init(&s);
    while (cur != NULL || !stack_empty(&s)) {
        if (cur != NULL) {
            push(&s, cur);
            cur = cur->left;
        }
        else {
            TreeNode* topNode = peek(&s);
            if (topNode->right != NULL && lastVisited != topNode->right) {
                cur = topNode->right;
            }
            else {
                visit(topNode, &first);
                lastVisited = pop(&s);
            }
        }
    }

    printf("\n");
    stack_free(&s);
}

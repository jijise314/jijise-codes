#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "Torder.h"

void print_error(Status st)
{
    switch (st) {
    case ERR_CMD:       printf("[오류] 알 수 없는 명령입니다.\n");                       break;
    case ERR_ARGC:      printf("[오류] 명령에 필요한 인자의 개수가 맞지 않습니다.\n");   break;
    case ERR_PATH_FORM: printf("[오류] 경로 형식이 올바르지 않습니다.\n");               break;
    case ERR_NO_NODE:   printf("[오류] 지정한 경로의 노드가 존재하지 않습니다.\n");      break;
    case ERR_EMPTY:     printf("[오류] 트리가 비어 있습니다.\n");                        break;
    case ERR_NOT_EMPTY: printf("[오류] 루트 노드가 이미 존재합니다.\n");                 break;
    case ERR_BAD_CHILD: printf("[오류] 자식 위치는 L 또는 R 이어야 합니다.\n");          break;
    case ERR_BAD_DATA:  printf("[오류] 데이터는 영문 대문자 한 글자여야 합니다.\n");     break;
    case ERR_NOT_LEAF:  printf("[오류] 단말 노드가 아닙니다.\n");                        break;
    case ERR_FULL_CHILD:printf("[오류] 해당 위치에 이미 자식 노드가 있습니다.\n");       break;
    case ERR_DUP:       printf("[오류] 같은 부모의 다른 자식과 데이터가 중복됩니다.\n"); break;
    case ERR_FULL:      printf("[오류] 더 이상 노드를 저장할 수 없습니다.\n");           break;
    default:                                                                             break;
    }
}

BTree* create_btree(int size)
{
    BTree* t = (BTree*)malloc(sizeof(BTree));
    if (t == NULL) {
        printf("[오류] 메모리를 할당할 수 없습니다.\n");
        exit(1);
    }
    t->root = NULL;
    t->size = size;
    t->count = 0;
    return t;
}

static TreeNode* make_node(char value)
{
    TreeNode* n = (TreeNode*)malloc(sizeof(TreeNode));
    if (n == NULL) {
        printf("[오류] 메모리를 할당할 수 없습니다.\n");
        exit(1);
    }
    n->data = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

BTree* insert_root(BTree* tree, char value, Status* st)
{
    *st = OK;
    if (tree->root != NULL) { *st = ERR_NOT_EMPTY; return tree; }
    if (tree->count >= tree->size) { *st = ERR_FULL;      return tree; }

    tree->root = make_node(value);
    tree->count++;
    return tree;
}

BTree* insert_child(BTree* tree, TreeNode* parent, char child, char value, Status* st)
{
    TreeNode* sibling;
    *st = OK;

    if (parent == NULL) { *st = ERR_NO_NODE;   return tree; }
    if (child != 'L' && child != 'R') { *st = ERR_BAD_CHILD; return tree; }

#if INSERT_LEAF_ONLY
    if (parent->left != NULL || parent->right != NULL) { *st = ERR_NOT_LEAF; return tree; }
#endif

    if (child == 'L' && parent->left != NULL) { *st = ERR_FULL_CHILD; return tree; }
    if (child == 'R' && parent->right != NULL) { *st = ERR_FULL_CHILD; return tree; }

    sibling = (child == 'L') ? parent->right : parent->left;
    if (sibling != NULL && sibling->data == value) { *st = ERR_DUP; return tree; }

    if (tree->count >= tree->size) { *st = ERR_FULL; return tree; }

    if (child == 'L') parent->left = make_node(value);
    else              parent->right = make_node(value);
    tree->count++;
    return tree;
}

BTree* delete_node(BTree* tree, TreeNode* leaf, TreeNode* parent, Status* st)
{
    *st = OK;
    if (leaf == NULL) { *st = ERR_NO_NODE; return tree; }
    if (leaf->left != NULL || leaf->right != NULL) { *st = ERR_NOT_LEAF; return tree; }

    if (parent == NULL)            tree->root = NULL;
    else if (parent->left == leaf) parent->left = NULL;
    else                           parent->right = NULL;

    free(leaf);
    tree->count--;
    return tree;
}

BTree* update_value(BTree* tree, TreeNode* node, TreeNode* parent, char value, Status* st)
{
    TreeNode* sibling;
    *st = OK;

    if (node == NULL) { *st = ERR_NO_NODE; return tree; }

    if (parent != NULL) {
        sibling = (parent->left == node) ? parent->right : parent->left;
        if (sibling != NULL && sibling->data == value) { *st = ERR_DUP; return tree; }
    }
    node->data = value;
    return tree;
}

void read_child(BTree* tree, TreeNode* parent, TreeNode** l, TreeNode** r)
{
    (void)tree;
    *l = (parent != NULL) ? parent->left : NULL;
    *r = (parent != NULL) ? parent->right : NULL;
}

static void print_rec(TreeNode* node, int depth)
{
    int i;
    if (node == NULL) return;

    for (i = 0; i < depth - 1; i++) printf("%*s", INDENT, "");
    if (depth > 0) printf("+---");
    printf("%c\n", node->data);

    print_rec(node->left, depth + 1);
    print_rec(node->right, depth + 1);
}

void print_btree(BTree* tree)
{
    if (tree->root == NULL) {
        printf("(빈 트리입니다.)\n");
        return;
    }
    print_rec(tree->root, 0);
}

static void destroy_rec(TreeNode* node)
{
    if (node == NULL) return;
    destroy_rec(node->left);
    destroy_rec(node->right);
    free(node);
}

void destroy_btree(BTree* tree)
{
    if (tree == NULL) return;
    destroy_rec(tree->root);
    tree->root = NULL;
    tree->count = 0;
    free(tree);
}

int parse_path(const char* path, char* labels, int maxlen)
{
    int i = 1, n = 0;

    if (path[0] != '/') return -1;
    if (path[1] == '\0') return 0;

    while (path[i] != '\0') {
        if (!isupper((unsigned char)path[i])) return -1;
        if (n >= maxlen) return -1;
        labels[n++] = path[i++];

        if (path[i] == '\0') break;
        if (path[i] != '/') return -1;
        i++;
        if (path[i] == '\0') return -1;
    }
    return n;
}

TreeNode* find_node(BTree* tree, const char* labels, int n, TreeNode** parent_out)
{
    TreeNode* cur, * par = NULL, * next;
    int i;

    *parent_out = NULL;
    if (n <= 0 || tree->root == NULL) return NULL;
    if (tree->root->data != labels[0]) return NULL;

    cur = tree->root;
    for (i = 1; i < n; i++) {
        next = NULL;
        if (cur->left != NULL && cur->left->data == labels[i])  next = cur->left;
        else if (cur->right != NULL && cur->right->data == labels[i])  next = cur->right;

        if (next == NULL) return NULL;
        par = cur;
        cur = next;
    }
    *parent_out = par;
    return cur;
}

TreeNode* locate(BTree* tree, const char* path, TreeNode** parent_out, Status* st)
{
    char labels[MAX_DEPTH];
    int  n;
    TreeNode* node;

    *st = OK;
    n = parse_path(path, labels, MAX_DEPTH);
    if (n < 0) { *st = ERR_PATH_FORM; return NULL; }
    if (n == 0) { *st = ERR_PATH_FORM; return NULL; }
    if (tree->root == NULL) { *st = ERR_EMPTY;     return NULL; }

    node = find_node(tree, labels, n, parent_out);
    if (node == NULL) { *st = ERR_NO_NODE;   return NULL; }
    return node;
}

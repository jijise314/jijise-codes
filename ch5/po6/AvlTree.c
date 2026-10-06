#include <stdio.h>
#include <stdlib.h>
#include "AvlTree.h"

static int max_int(int a, int b) { return a > b ? a : b; }

/* 정수 배열 + 순차 탐색 */
void array_init(IntArray* arr)
{
    arr->length = 0;
}

int seq_search(const IntArray* arr, int key, long* cmp)
{
    int i;
    for (i = 0; i < arr->length; i++) {
        (*cmp)++;
        if (arr->data[i] == key) return 1;
    }
    return 0;
}

int array_insert(IntArray* arr, int value, long* cmp)
{
    if (seq_search(arr, value, cmp)) return 0;
    arr->data[arr->length++] = value;
    return 1;
}

/* 이진 탐색 트리 (BST) */
static BSTNode* bst_new_node(int key)
{
    BSTNode* n = (BSTNode*)malloc(sizeof(BSTNode));
    if (n == NULL) { fprintf(stderr, "메모리 할당 실패\n"); exit(1); }
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

int bst_insert(BSTNode** root, int key, long* cmp)
{
    BSTNode** cur = root;

    while (*cur != NULL) {
        (*cmp)++;
        if (key == (*cur)->key) return 0;
        else if (key < (*cur)->key) cur = &(*cur)->left;
        else                        cur = &(*cur)->right;
    }
    *cur = bst_new_node(key);
    return 1;
}

int bst_search(const BSTNode* root, int key, long* cmp)
{
    while (root != NULL) {
        (*cmp)++;
        if (key == root->key) return 1;
        else if (key < root->key) root = root->left;
        else                      root = root->right;
    }
    return 0;
}

int bst_height(const BSTNode* root)
{
    if (root == NULL) return 0;
    return 1 + max_int(bst_height(root->left), bst_height(root->right));
}

void bst_free(BSTNode* root)
{
    if (root == NULL) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

/* AVL 트리 */
static RotationStats rot_stats = { 0, 0, 0, 0 };

RotationStats avl_rotation_stats(void)
{
    return rot_stats;
}

static int avl_h(const AVLNode* n) { return n ? n->height : 0; }

static void avl_update(AVLNode* n)
{
    n->height = 1 + max_int(avl_h(n->left), avl_h(n->right));
}

static int avl_balance(const AVLNode* n)
{
    return n ? avl_h(n->left) - avl_h(n->right) : 0;
}

static AVLNode* avl_new_node(int key)
{
    AVLNode* n = (AVLNode*)malloc(sizeof(AVLNode));
    if (n == NULL) { fprintf(stderr, "메모리 할당 실패\n"); exit(1); }
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

static AVLNode* rotate_right(AVLNode* y)
{
    AVLNode* x = y->left;
    y->left = x->right;
    x->right = y;
    avl_update(y);
    avl_update(x);
    return x;
}

static AVLNode* rotate_left(AVLNode* x)
{
    AVLNode* y = x->right;
    x->right = y->left;
    y->left = x;
    avl_update(x);
    avl_update(y);
    return y;
}

static AVLNode* avl_rebalance(AVLNode* n)
{
    int bf;

    avl_update(n);
    bf = avl_balance(n);

    if (bf > 1) {
        if (avl_balance(n->left) >= 0) {        /* LL */
            rot_stats.LL++;
            return rotate_right(n);
        }
        else {                                /* LR */
            rot_stats.LR++;
            n->left = rotate_left(n->left);
            return rotate_right(n);
        }
    }
    if (bf < -1) {
        if (avl_balance(n->right) <= 0) {       /* RR */
            rot_stats.RR++;
            return rotate_left(n);
        }
        else {                                /* RL */
            rot_stats.RL++;
            n->right = rotate_right(n->right);
            return rotate_left(n);
        }
    }
    return n;
}

static AVLNode* avl_insert_rec(AVLNode* node, int key, long* cmp, int* inserted)
{
    if (node == NULL) {
        *inserted = 1;
        return avl_new_node(key);
    }

    (*cmp)++;
    if (key == node->key) {
        *inserted = 0;
        return node;
    }
    else if (key < node->key) {
        node->left = avl_insert_rec(node->left, key, cmp, inserted);
    }
    else {
        node->right = avl_insert_rec(node->right, key, cmp, inserted);
    }

    if (!*inserted) return node;
    return avl_rebalance(node);
}

int avl_insert(AVLNode** root, int key, long* cmp)
{
    int inserted = 0;
    *root = avl_insert_rec(*root, key, cmp, &inserted);
    return inserted;
}

int avl_search(const AVLNode* root, int key, long* cmp)
{
    while (root != NULL) {
        (*cmp)++;
        if (key == root->key) return 1;
        else if (key < root->key) root = root->left;
        else                      root = root->right;
    }
    return 0;
}

int avl_height(const AVLNode* root)
{
    if (root == NULL) return 0;
    return 1 + max_int(avl_height(root->left), avl_height(root->right));
}

void avl_free(AVLNode* root)
{
    if (root == NULL) return;
    avl_free(root->left);
    avl_free(root->right);
    free(root);
}

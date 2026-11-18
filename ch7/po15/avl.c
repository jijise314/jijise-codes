#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "avl.h"

/* ===================== 높이 / 회전 ===================== */
static int height(const AvlNode* t)
{
    return t ? t->height : 0;
}

static void update_height(AvlNode* t)
{
    int hl = height(t->left), hr = height(t->right);
    t->height = (hl > hr ? hl : hr) + 1;
}

static AvlNode* rotate_right(AvlNode* y)
{
    AvlNode* x = y->left;
    y->left = x->right;
    x->right = y;
    update_height(y);
    update_height(x);
    return x;
}

static AvlNode* rotate_left(AvlNode* x)
{
    AvlNode* y = x->right;
    x->right = y->left;
    y->left = x;
    update_height(x);
    update_height(y);
    return y;
}

static AvlNode* rebalance(AvlNode* t)
{
    int bf;

    update_height(t);
    bf = height(t->left) - height(t->right);

    if (bf > 1) {
        if (height(t->left->left) < height(t->left->right))
            t->left = rotate_left(t->left);         /* LR */
        return rotate_right(t);                     /* LL */
    }
    if (bf < -1) {
        if (height(t->right->right) < height(t->right->left))
            t->right = rotate_right(t->right);      /* RL */
        return rotate_left(t);                      /* RR */
    }
    return t;
}

/* ===================== 삽입 ===================== */
static AvlNode* new_node(int x)
{
    AvlNode* p = (AvlNode*)tracked_malloc(sizeof(AvlNode));
    if (!p) { printf("메모리 할당 실패\n"); exit(1); }
    p->key = x;
    g_move++;
    p->count = 1;
    p->height = 1;
    p->left = p->right = NULL;
    return p;
}

static AvlNode* insert_rec(AvlNode* t, int x)
{
    STACK_PROBE();
    if (!t) return new_node(x);

    if (LESS(x, t->key))
        t->left = insert_rec(t->left, x);
    else if (LESS(t->key, x))
        t->right = insert_rec(t->right, x);
    else {
        t->count++;
        return t;
    }
    return rebalance(t);
}

void avl_insert(AvlTree* t, int x)
{
    t->root = insert_rec(t->root, x);
    t->size++;
}

/* ===================== 삭제 ===================== */
static AvlNode* remove_min(AvlNode* t)
{
    AvlNode* r;

    STACK_PROBE();
    if (!t->left) {
        r = t->right;
        tracked_free(t, sizeof(AvlNode));
        return r;
    }
    t->left = remove_min(t->left);
    return rebalance(t);
}

static AvlNode* delete_rec(AvlNode* t, int x, int* removed)
{
    AvlNode* child, * s;

    STACK_PROBE();
    if (!t) return NULL;

    if (LESS(x, t->key))
        t->left = delete_rec(t->left, x, removed);
    else if (LESS(t->key, x))
        t->right = delete_rec(t->right, x, removed);
    else {
        *removed = 1;
        if (t->count > 1) {
            t->count--;
            return t;
        }
        if (!t->left || !t->right) {
            child = t->left ? t->left : t->right;
            tracked_free(t, sizeof(AvlNode));
            return child;
        }
        s = t->right;
        while (s->left) s = s->left;
        t->key = s->key;
        g_move++;
        t->count = s->count;
        t->right = remove_min(t->right);
    }
    return rebalance(t);
}

int avl_delete(AvlTree* t, int x)
{
    int removed = 0;
    t->root = delete_rec(t->root, x, &removed);
    if (removed) t->size--;
    return removed;
}

/* ===================== 순회 / 검증 / 해제 ===================== */
static int inorder_rec(const AvlNode* t, int out[], int idx)
{
    int c;
    if (!t) return idx;
    idx = inorder_rec(t->left, out, idx);
    for (c = 0; c < t->count; c++) out[idx++] = t->key;
    return inorder_rec(t->right, out, idx);
}

int avl_inorder(const AvlTree* t, int out[])
{
    return inorder_rec(t->root, out, 0);
}

static int check_rec(const AvlNode* t, int* ok)
{
    int hl, hr, h;
    if (!t) return 0;
    hl = check_rec(t->left, ok);
    hr = check_rec(t->right, ok);
    h = (hl > hr ? hl : hr) + 1;
    if (t->height != h || hl - hr > 1 || hr - hl > 1 || t->count < 1) *ok = 0;
    return h;
}

int avl_is_valid(const AvlTree* t)
{
    int ok = 1;
    check_rec(t->root, &ok);
    return ok;
}

static void free_rec(AvlNode* t)
{
    if (!t) return;
    free_rec(t->left);
    free_rec(t->right);
    tracked_free(t, sizeof(AvlNode));
}

void avl_init(AvlTree* t)
{
    t->root = NULL;
    t->size = 0;
}

void avl_free(AvlTree* t)
{
    free_rec(t->root);
    t->root = NULL;
    t->size = 0;
}

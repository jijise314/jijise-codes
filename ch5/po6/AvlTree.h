#ifndef AVLTREE_H
#define AVLTREE_H

#define NUM_DATA    100
#define NUM_SEARCH  50
#define MAX_VALUE   1000

typedef struct {
    int data[NUM_DATA];
    int length;
} IntArray;

void array_init(IntArray* arr);

int  seq_search(const IntArray* arr, int key, long* cmp);

int  array_insert(IntArray* arr, int value, long* cmp);

/* 이진 탐색 트리 (BST) */
typedef struct BSTNode {
    int key;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

int  bst_insert(BSTNode** root, int key, long* cmp);
int  bst_search(const BSTNode* root, int key, long* cmp);
int  bst_height(const BSTNode* root);
void bst_free(BSTNode* root);

/* AVL 트리 */
typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

typedef struct {
    int LL, RR, LR, RL;
} RotationStats;

int  avl_insert(AVLNode** root, int key, long* cmp);
int  avl_search(const AVLNode* root, int key, long* cmp);
int  avl_height(const AVLNode* root);
void avl_free(AVLNode* root);
RotationStats avl_rotation_stats(void);

#endif

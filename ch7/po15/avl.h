#ifndef AVL_H
#define AVL_H

typedef struct AvlNode {
    int key;
    int count;
    int height;
    struct AvlNode* left;
    struct AvlNode* right;
} AvlNode;

typedef struct {
    AvlNode* root;
    int size;
} AvlTree;

void avl_init(AvlTree* t);
void avl_insert(AvlTree* t, int x);
int  avl_delete(AvlTree* t, int x);
int  avl_inorder(const AvlTree* t, int out[]);
int  avl_is_valid(const AvlTree* t);
void avl_free(AvlTree* t);

#endif

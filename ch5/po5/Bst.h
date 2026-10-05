#ifndef BST_H
#define BST_H

typedef struct TreeNode {
    int key;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* bst_create_node(int key);

TreeNode* bst_insert(TreeNode* root, int key, long* cmp);

int bst_search(TreeNode* root, int key, int* cmp);

int bst_height(TreeNode* root);

long bst_level_sum(TreeNode* root, int level);

void bst_free(TreeNode* root);

#endif

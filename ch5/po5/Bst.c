#include <stdio.h>
#include <stdlib.h>
#include "BST.h"

TreeNode* bst_create_node(int key)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* bst_insert(TreeNode* root, int key, long* cmp)
{
    TreeNode* node = bst_create_node(key);
    TreeNode* cur;

    if (root == NULL) return node;

    cur = root;
    while (1) {
        (*cmp)++;
        if (key < cur->key) {
            if (cur->left == NULL) { cur->left = node; break; }
            cur = cur->left;
        }
        else {
            if (cur->right == NULL) { cur->right = node; break; }
            cur = cur->right;
        }
    }
    return root;
}

int bst_search(TreeNode* root, int key, int* cmp)
{
    TreeNode* cur = root;
    *cmp = 0;

    while (cur != NULL) {
        (*cmp)++;
        if (key == cur->key) return 1;
        else if (key < cur->key) cur = cur->left;
        else cur = cur->right;
    }
    return 0;
}

int bst_height(TreeNode* root)
{
    int lh, rh;
    if (root == NULL) return 0;
    lh = bst_height(root->left);
    rh = bst_height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

long bst_level_sum(TreeNode* root, int level)
{
    if (root == NULL) return 0;
    return level
        + bst_level_sum(root->left, level + 1)
        + bst_level_sum(root->right, level + 1);
}

void bst_free(TreeNode* root)
{
    if (root == NULL) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

#ifndef BTREE_TRAVERSAL_H
#define BTREE_TRAVERSAL_H

 /* ================= 트리 노드 ================= */
typedef struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* parse_tree(const char* str);

/* 트리 전체 메모리 해제 */
void free_tree(TreeNode* root);

/* 트리 구조 출력 */
void print_structure(TreeNode* root);

/* 반복적(iterative) 순회 */
void preorder(TreeNode* tree);
void inorder(TreeNode* tree);
void postorder(TreeNode* tree);

#endif

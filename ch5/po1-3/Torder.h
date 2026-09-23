#define TORDER_H

#define INSERT_LEAF_ONLY   0

#define MAX_NODES   100
#define MAX_DEPTH   64
#define INDENT      4

typedef struct TreeNode {
    char             data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct {
    TreeNode* root;
    int       size;
    int       count;
} BTree;

typedef enum {
    OK = 0,
    ERR_CMD,            /* 알 수 없는 명령 */
    ERR_ARGC,           /* 인자 개수 오류 */
    ERR_PATH_FORM,      /* 경로 형식 오류 */
    ERR_NO_NODE,        /* 경로에 해당하는 노드 없음 */
    ERR_EMPTY,          /* 빈 트리 */
    ERR_NOT_EMPTY,      /* 이미 루트가 존재 */
    ERR_BAD_CHILD,      /* L/R 이 아님 */
    ERR_BAD_DATA,       /* 영문 대문자 한 글자가 아님 */
    ERR_NOT_LEAF,       /* 단말 노드가 아님 */
    ERR_FULL_CHILD,     /* 해당 위치에 이미 자식이 존재 */
    ERR_DUP,            /* 형제 노드와 데이터가 중복 */
    ERR_FULL            /* 트리 용량 초과 */
} Status;

BTree* create_btree(int size);
BTree* insert_root(BTree* tree, char value, Status* st);
BTree* insert_child(BTree* tree, TreeNode* parent, char child, char value, Status* st);
BTree* delete_node(BTree* tree, TreeNode* leaf, TreeNode* parent, Status* st);
BTree* update_value(BTree* tree, TreeNode* node, TreeNode* parent, char value, Status* st);
void   read_child(BTree* tree, TreeNode* parent, TreeNode** l, TreeNode** r);
void   print_btree(BTree* tree);
void   destroy_btree(BTree* tree);

int       parse_path(const char* path, char* labels, int maxlen);
TreeNode* find_node(BTree* tree, const char* labels, int n, TreeNode** parent_out);
TreeNode* locate(BTree* tree, const char* path, TreeNode** parent_out, Status* st);
void      print_error(Status st);

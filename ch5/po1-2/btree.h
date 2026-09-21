#include <stddef.h>

#define MAX_LEVEL 20
#define EMPTY '\0'

typedef struct {
    int nodes;
    int leaves;
    int internal;
    int height;
    int degree;
} Stats;

typedef struct {
    int full;
    int complete;
    int skew;
} Shape;

typedef struct {
    char* data;
    size_t cap;
} ArrayTree;

const char* arr_build(ArrayTree* t, const char* expr);
void        arr_free(ArrayTree* t);
Stats       arr_stats(const ArrayTree* t);
Shape       arr_shape(const ArrayTree* t, const Stats* s);
void        arr_print(const ArrayTree* t);
void        arr_dump(const ArrayTree* t);

typedef struct Node {
    char         data;
    struct Node* left;
    struct Node* right;
} Node;

const char* lnk_build(Node** root, const char* expr);
void        lnk_free(Node* root);
Stats       lnk_stats(const Node* root);
Shape       lnk_shape(const Node* root, const Stats* s);
void        lnk_print(const Node* root);

void print_stats(const Stats* s);
void print_shape(const Shape* sh, int nodes);

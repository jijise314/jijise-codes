#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

#define V        100
#define SPARSE_E 100
#define DENSE_E  4000
#define MAX_E    (V * (V - 1) / 2)

void rng_init(unsigned int s);
unsigned int rng_next(void);
int rng_range(int n);

typedef struct { int u, v; } Edge;

void generate_edges(Edge* out, int E);

typedef struct {
    int n;
    int edges;
    int adj[V][V];
} AdjMatrix;

AdjMatrix* matrix_create(void);
void   matrix_destroy(AdjMatrix* g);
int    matrix_has_edge(const AdjMatrix* g, int u, int v, long* cmp);
int    matrix_insert_edge(AdjMatrix* g, int u, int v, long* cmp);
int    matrix_delete_edge(AdjMatrix* g, int u, int v, long* cmp);
int    matrix_neighbors(const AdjMatrix* g, int u, int* out, long* cmp);
size_t matrix_memory(const AdjMatrix* g);

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    int n;
    int edges;
    int nodes;
    Node* head[V];
} AdjList;

AdjList* list_create(void);
void   list_destroy(AdjList* g);
int    list_find(const AdjList* g, int u, int v, long* cmp);
int    list_insert_edge(AdjList* g, int u, int v, long* cmp);
int    list_delete_edge(AdjList* g, int u, int v, long* cmp);
int    list_neighbors(const AdjList* g, int u, int* out, long* cmp);
size_t list_memory(const AdjList* g);

typedef struct { double ins, del, conn, adj; } Average;

Average average_matrix(const AdjMatrix* g);
Average average_list(const AdjList* g, const AdjMatrix* ref);

#endif

#define NUM_V      10
#define NUM_E      20
#define MIN_W      1
#define MAX_W      100
#define INF        1000000000
#define NO_VERTEX  (-1)

typedef struct {
    int u;
    int v;
    int w;
} Edge;

typedef struct {
    int adj[NUM_V][NUM_V];
    Edge edges[NUM_E];
    int edgeCount;

    int dist[NUM_V][NUM_V];
    int next[NUM_V][NUM_V];
} Graph;

void generateGraph(Graph* g);

void floydWarshall(Graph* g);

int reconstructPath(const Graph* g, int u, int v, int path[]);

void printGraph(const Graph* g);

void printShortestPaths(const Graph* g);

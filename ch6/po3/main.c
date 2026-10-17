#include <stdlib.h>
#include <time.h>
#include "mst.h"

int main(void)
{
    Graph g;

    srand((unsigned int)time(NULL));

    generateGraph(&g);
    printGraph(&g);

    floydWarshall(&g);
    printShortestPaths(&g);

    return 0;
}

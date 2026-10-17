#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mst.h"

static void print_case(int no, const char* title, int E, const MSTResult* r,
    MemInfo m, const char* gname, const char* aname, double us)
{
    printf("Case %d : %s\n", no, title);
    printf("정점 수        : %d\n", NUM_V);
    printf("간선 수        : %d\n", E);
    printf("MST 간선 수    : %d\n", r->count);
    printf("MST 전체 비용  : %lld\n", r->cost);
    printf("메모리 사용량  : %zu Bytes\n", m.total);
    printf("  - %s : %zu Bytes\n", gname, m.graph);
    printf("  - %s : %zu Bytes\n", aname, m.aux);
    printf("  - MST 결과 배열 : %zu Bytes\n", m.result);
    printf("실행 시간      : %.3f μs (%d회 반복 평균)\n", us, REPEAT);
    printf("MST 간선 목록 (정점-정점:가중치)\n");
    for (int i = 0; i < r->count; i++) {
        printf("  %2d-%-2d:%-3d", r->edges[i].u, r->edges[i].v, r->edges[i].w);
        if ((i + 1) % 7 == 0 || i == r->count - 1) printf("\n");
    }
    printf("\n");
}

static void print_check(const char* name, const MSTResult* k, const MSTResult* p)
{
    bool ok = k->cost == p->cost && k->count == NUM_V - 1 && p->count == NUM_V - 1;
    printf("%s : Kruskal 비용 %lld / Prim 비용 %lld -> %s\n",
        name, k->cost, p->cost, ok ? "일치 (정상)" : "불일치 (오류)");
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    srand((unsigned)time(NULL));

    static Edge sparseE[SPARSE_E], denseE[DENSE_E];
    generate_graph(sparseE, SPARSE_E);
    generate_graph(denseE, DENSE_E);

    static AdjList   sparseL, denseL;
    static AdjMatrix sparseM, denseM;
    build_list(&sparseL, sparseE, SPARSE_E);
    build_list(&denseL, denseE, DENSE_E);
    build_matrix(&sparseM, sparseE, SPARSE_E);
    build_matrix(&denseM, denseE, DENSE_E);

    MSTResult r1, r2, r3, r4;
    double t1 = time_kruskal(&sparseL, &r1);
    double t2 = time_kruskal(&denseL, &r2);
    double t3 = time_prim(&sparseM, &r3);
    double t4 = time_prim(&denseM, &r4);

    print_case(1, "희소 그래프 - 인접 리스트 - Kruskal", SPARSE_E, &r1,
        mem_kruskal(&sparseL), "인접 리스트", "간선 배열 + Union-Find", t1);
    print_case(2, "밀집 그래프 - 인접 리스트 - Kruskal", DENSE_E, &r2,
        mem_kruskal(&denseL), "인접 리스트", "간선 배열 + Union-Find", t2);
    print_case(3, "희소 그래프 - 인접 행렬 - Prim", SPARSE_E, &r3,
        mem_prim(&sparseM), "인접 행렬", "key / parent / 방문 배열", t3);
    print_case(4, "밀집 그래프 - 인접 행렬 - Prim", DENSE_E, &r4,
        mem_prim(&denseM), "인접 행렬", "key / parent / 방문 배열", t4);

    printf("[결과 검증]\n");
    print_check("희소 그래프", &r1, &r3);
    print_check("밀집 그래프", &r2, &r4);

    free_list(&sparseL);
    free_list(&denseL);
    return 0;
}

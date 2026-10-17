#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

typedef struct {
    size_t memory;
    long ins, del, conn, adj;
} Result;

static int cmp_int(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}

static void print_vertices(int u, const int* arr, int k)
{
    printf("정점 %d의 인접 정점 (%d개) : ", u, k);
    if (k == 0) { printf("(없음)\n"); return; }
    for (int i = 0; i < k; i++) {
        if (i > 0 && i % 20 == 0) printf("\n    ");
        printf("%d ", arr[i]);
    }
    printf("\n");
}

static void print_case(int no, const char* gname, const char* rname, const Result* r)
{
    printf("Case %d : %s - %s\n", no, gname, rname);
    printf("메모리 사용량             : %zu Bytes\n", r->memory);
    printf("간선 삽입 비교 횟수       : %ld\n", r->ins);
    printf("간선 삭제 비교 횟수       : %ld\n", r->del);
    printf("연결 여부 확인 비교 횟수  : %ld\n", r->conn);
    printf("인접 정점 출력 비교 횟수  : %ld\n", r->adj);
}

static void run_experiment(const char* gname, int E, int case_no,
    Average* avg_m, Average* avg_l)
{
    static Edge edges[MAX_E];
    generate_edges(edges, E);

    AdjMatrix* gm = matrix_create();
    AdjList* gl = list_create();
    long dummy = 0;
    for (int i = 0; i < E; i++) {
        matrix_insert_edge(gm, edges[i].u, edges[i].v, &dummy);
        list_insert_edge(gl, edges[i].u, edges[i].v, &dummy);
    }

    Result rm = { 0, 0, 0, 0, 0 }, rl = { 0, 0, 0, 0, 0 };
    rm.memory = matrix_memory(gm);
    rl.memory = list_memory(gl);

    *avg_m = average_matrix(gm);
    *avg_l = average_list(gl, gm);

    int iu, iv;
    do { iu = rng_range(V); iv = rng_range(V); } while (iu == iv || gm->adj[iu][iv]);

    Edge de = edges[rng_range(E)]; 
    int du = de.u, dv = de.v;

    int cu, cv;
    do { cu = rng_range(V); cv = rng_range(V); } while (cu == cv);

    int au = rng_range(V);

    printf("============================================================\n");
    printf("%s\n", gname);
    printf("정점 수 : %d\n", gm->n);
    printf("간선 수 : %d\n", gm->edges);
    printf("------------------------------------------------------------\n");
    printf("측정 대상 (두 표현 방식에 동일하게 사용)\n");
    printf("  삽입할 간선      : (%d, %d)\n", iu, iv);
    printf("  삭제할 간선      : (%d, %d)\n", du, dv);
    printf("  연결 여부 확인   : (%d, %d)\n", cu, cv);
    printf("  인접 정점 출력   : 정점 %d\n", au);
    printf("============================================================\n\n");

    int nb_m[V], nb_l[V], km, kl;
    int ok_ins_m, ok_ins_l, ok_del_m, ok_del_l, con_m, con_l;

    ok_ins_m = matrix_insert_edge(gm, iu, iv, &rm.ins);
    ok_del_m = matrix_delete_edge(gm, du, dv, &rm.del);
    con_m = matrix_has_edge(gm, cu, cv, &rm.conn);
    km = matrix_neighbors(gm, au, nb_m, &rm.adj);

    print_case(case_no, gname, "인접 행렬", &rm);
    printf("연결 여부 (%d, %d) : %s\n", cu, cv, con_m ? "연결됨" : "연결되지 않음");
    print_vertices(au, nb_m, km);
    printf("\n");

    ok_ins_l = list_insert_edge(gl, iu, iv, &rl.ins);
    ok_del_l = list_delete_edge(gl, du, dv, &rl.del);
    con_l = list_find(gl, cu, cv, &rl.conn);
    kl = list_neighbors(gl, au, nb_l, &rl.adj);

    print_case(case_no + 1, gname, "인접 리스트", &rl);
    printf("연결 여부 (%d, %d) : %s\n", cu, cv, con_l ? "연결됨" : "연결되지 않음");
    print_vertices(au, nb_l, kl);
    printf("\n");

    int same = (ok_ins_m == ok_ins_l) && (ok_del_m == ok_del_l) &&
        (con_m == con_l) && (km == kl) && (gm->edges == gl->edges) &&
        (gl->nodes == 2 * gl->edges);
    if (same) {
        int s[V];
        memcpy(s, nb_l, sizeof(int) * kl);
        qsort(s, kl, sizeof(int), cmp_int);
        for (int i = 0; i < km; i++) if (s[i] != nb_m[i]) same = 0;
    }
    printf("검증 : %s (인접 행렬과 인접 리스트가 같은 그래프를 저장, 간선 %d개)\n\n",
        same ? "성공" : "실패", gm->edges);

    matrix_destroy(gm);
    list_destroy(gl);
}

int main(void)
{
    rng_init((unsigned int)time(NULL) ^ ((unsigned int)clock() << 16));

    printf("인접 행렬 vs 인접 리스트 성능 비교\n");
    printf("sizeof(int) = %zu, sizeof(Node) = %zu, sizeof(AdjMatrix) = %zu, sizeof(AdjList) = %zu\n\n",
        sizeof(int), sizeof(Node), sizeof(AdjMatrix), sizeof(AdjList));

    Average sm, sl, dm, dl;
    run_experiment("희소 그래프", SPARSE_E, 1, &sm, &sl);
    run_experiment("밀집 그래프", DENSE_E, 3, &dm, &dl);

    printf("============================================================\n");
    printf("가능한 모든 대상에 대한 평균 비교 횟수 (원본 그래프 기준)\n");
    printf("============================================================\n");
    printf("%18s %10s %10s %10s %10s\n", "", "      삽입", "      삭제", " 연결 확인", " 인접 정점");
    printf("희소 - 인접 행렬   %10.2f %10.2f %10.2f %10.2f\n", sm.ins, sm.del, sm.conn, sm.adj);
    printf("희소 - 인접 리스트 %10.2f %10.2f %10.2f %10.2f\n", sl.ins, sl.del, sl.conn, sl.adj);
    printf("밀집 - 인접 행렬   %10.2f %10.2f %10.2f %10.2f\n", dm.ins, dm.del, dm.conn, dm.adj);
    printf("밀집 - 인접 리스트 %10.2f %10.2f %10.2f %10.2f\n", dl.ins, dl.del, dl.conn, dl.adj);
    return 0;
}

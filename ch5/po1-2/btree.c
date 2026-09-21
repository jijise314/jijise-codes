#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "btree.h"

typedef struct {
    const char* s;
    size_t      pos;
    const char* err;
} Parser;

static char peek(Parser* p)
{
    while (p->s[p->pos] && isspace((unsigned char)p->s[p->pos]))
        p->pos++;
    return p->s[p->pos];
}

static void fail(Parser* p, const char* msg)
{
    if (!p->err) p->err = msg;
}

static int is_label(char c) { return isalnum((unsigned char)c); }

static int level_of(size_t i)
{
    int l = 0;
    while (i) { l++; i >>= 1; }
    return l;
}

static int arr_reserve(ArrayTree* t, size_t idx)
{
    if (idx < t->cap) return 1;
    size_t nc = t->cap ? t->cap : 2;
    while (nc <= idx) nc *= 2;
    char* nd = realloc(t->data, nc * sizeof(char));
    if (!nd) return 0;
    memset(nd + t->cap, EMPTY, nc - t->cap);
    t->data = nd;
    t->cap = nc;
    return 1;
}

static char arr_get(const ArrayTree* t, size_t i)
{
    return (i > 0 && i < t->cap) ? t->data[i] : EMPTY;
}

static void arr_parse(Parser* p, ArrayTree* t, size_t idx, int level)
{
    char c = peek(p);
    if (p->err || c == ',' || c == ')' || c == '\0') return;
    if (!is_label(c)) { fail(p, "노드 이름은 영문자/숫자 한 글자여야 합니다"); return; }
    if (level > MAX_LEVEL) { fail(p, "트리 높이가 허용 범위(20)를 넘습니다"); return; }
    if (!arr_reserve(t, idx)) { fail(p, "메모리가 부족합니다"); return; }

    t->data[idx] = c;
    p->pos++;

    if (peek(p) != '(') return;
    p->pos++;
    arr_parse(p, t, 2 * idx, level + 1);
    if (peek(p) == ',') {
        p->pos++;
        arr_parse(p, t, 2 * idx + 1, level + 1);
    }
    if (p->err) return;
    if (peek(p) != ')') { fail(p, "')' 가 필요합니다"); return; }
    p->pos++;
}

const char* arr_build(ArrayTree* t, const char* expr)
{
    Parser p = { expr, 0, NULL };
    t->data = NULL;
    t->cap = 0;
    if (!arr_reserve(t, 1)) return "메모리가 부족합니다";
    arr_parse(&p, t, 1, 1);
    if (!p.err && peek(&p) != '\0') fail(&p, "해석할 수 없는 문자가 남아 있습니다");
    return p.err;
}

void arr_free(ArrayTree* t)
{
    free(t->data);
    t->data = NULL;
    t->cap = 0;
}

Stats arr_stats(const ArrayTree* t)
{
    Stats s = { 0, 0, 0, 0, 0 };
    for (size_t i = 1; i < t->cap; i++) {
        if (t->data[i] == EMPTY) continue;
        int ch = (arr_get(t, 2 * i) != EMPTY) + (arr_get(t, 2 * i + 1) != EMPTY);
        s.nodes++;
        if (ch == 0) s.leaves++; else s.internal++;
        if (ch > s.degree) s.degree = ch;
        int lv = level_of(i);
        if (lv > s.height) s.height = lv;
    }
    return s;
}

Shape arr_shape(const ArrayTree* t, const Stats* s)
{
    Shape sh = { 0, 0, 0 };
    size_t max_idx = 0;
    int has_l = 0, has_r = 0, has_two = 0;

    for (size_t i = 1; i < t->cap; i++) {
        if (t->data[i] == EMPTY) continue;
        int l = arr_get(t, 2 * i) != EMPTY;
        int r = arr_get(t, 2 * i + 1) != EMPTY;
        max_idx = i;
        has_l |= l;
        has_r |= r;
        if (l && r) has_two = 1;
    }

    sh.complete = s->nodes > 0 && max_idx == (size_t)s->nodes;

    sh.full = s->nodes > 0 && s->nodes == (1 << s->height) - 1;

    if (s->nodes >= 2 && !has_two) {
        if (has_l && !has_r) sh.skew = -1;
        else if (has_r && !has_l) sh.skew = 1;
    }
    return sh;
}

static void arr_print_rec(const ArrayTree* t, size_t i, char* prefix, size_t plen)
{
    size_t      kid[2];
    const char* tag[2];
    int n = 0;

    if (arr_get(t, 2 * i) != EMPTY) { kid[n] = 2 * i;     tag[n++] = "L"; }
    if (arr_get(t, 2 * i + 1) != EMPTY) { kid[n] = 2 * i + 1; tag[n++] = "R"; }

    for (int j = 0; j < n; j++) {
        int last = (j == n - 1);
        printf("%s%s[%s] %c\n", prefix, last ? "`--" : "+--", tag[j], t->data[kid[j]]);
        strcpy(prefix + plen, last ? "    " : "|   ");
        arr_print_rec(t, kid[j], prefix, plen + 4);
        prefix[plen] = '\0';
    }
}

void arr_print(const ArrayTree* t)
{
    if (arr_get(t, 1) == EMPTY) { puts("  (공백 트리)"); return; }
    char prefix[4 * MAX_LEVEL + 8] = "  ";
    printf("  %c\n", t->data[1]);
    arr_print_rec(t, 1, prefix, 2);
}

void arr_dump(const ArrayTree* t)
{
    printf("  배열 내용 (칸 %zu개, '-' = 빈 칸)\n  ", t->cap);
    if (t->cap > 64) { puts("  (칸이 많아 생략)"); return; }
    for (size_t i = 1; i < t->cap; i++)
        printf("[%zu]%c ", i, t->data[i] ? t->data[i] : '-');
    putchar('\n');
}

static Node* node_new(char c)
{
    Node* n = malloc(sizeof * n);
    if (!n) return NULL;
    n->data = c;
    n->left = n->right = NULL;
    return n;
}

void lnk_free(Node* n)
{
    if (!n) return;
    lnk_free(n->left);
    lnk_free(n->right);
    free(n);
}

static Node* lnk_parse(Parser* p, int level)
{
    char c = peek(p);
    if (p->err || c == ',' || c == ')' || c == '\0') return NULL;
    if (!is_label(c)) { fail(p, "노드 이름은 영문자/숫자 한 글자여야 합니다"); return NULL; }
    if (level > MAX_LEVEL) { fail(p, "트리 높이가 허용 범위(20)를 넘습니다"); return NULL; }

    Node* n = node_new(c);
    if (!n) { fail(p, "메모리가 부족합니다"); return NULL; }
    p->pos++;

    if (peek(p) != '(') return n;
    p->pos++;
    n->left = lnk_parse(p, level + 1);
    if (peek(p) == ',') {
        p->pos++;
        n->right = lnk_parse(p, level + 1);
    }
    if (p->err) return n;
    if (peek(p) != ')') { fail(p, "')' 가 필요합니다"); return n; }
    p->pos++;
    return n;
}

const char* lnk_build(Node** root, const char* expr)
{
    Parser p = { expr, 0, NULL };
    *root = lnk_parse(&p, 1);
    if (!p.err && peek(&p) != '\0') fail(&p, "해석할 수 없는 문자가 남아 있습니다");
    if (p.err) { lnk_free(*root); *root = NULL; }
    return p.err;
}

static void lnk_stats_rec(const Node* n, int depth, Stats* s)
{
    if (!n) return;
    int ch = (n->left != NULL) + (n->right != NULL);
    s->nodes++;
    if (ch == 0) s->leaves++; else s->internal++;
    if (ch > s->degree) s->degree = ch;
    if (depth > s->height) s->height = depth;
    lnk_stats_rec(n->left, depth + 1, s);
    lnk_stats_rec(n->right, depth + 1, s);
}

Stats lnk_stats(const Node* root)
{
    Stats s = { 0, 0, 0, 0, 0 };
    lnk_stats_rec(root, 1, &s);
    return s;
}

typedef struct {
    unsigned long max_idx;
    int has_l, has_r, has_two;
} ShapeInfo;

static void lnk_shape_rec(const Node* n, unsigned long idx, ShapeInfo* si)
{
    if (!n) return;
    if (idx > si->max_idx) si->max_idx = idx;
    si->has_l |= (n->left != NULL);
    si->has_r |= (n->right != NULL);
    if (n->left && n->right) si->has_two = 1;
    lnk_shape_rec(n->left, 2 * idx, si);
    lnk_shape_rec(n->right, 2 * idx + 1, si);
}

Shape lnk_shape(const Node* root, const Stats* s)
{
    Shape sh = { 0, 0, 0 };
    ShapeInfo si = { 0, 0, 0, 0 };
    lnk_shape_rec(root, 1, &si);
    sh.complete = s->nodes > 0 && si.max_idx == (unsigned long)s->nodes;
    sh.full = s->nodes > 0 && s->nodes == (1 << s->height) - 1;
    if (s->nodes >= 2 && !si.has_two) {
        if (si.has_l && !si.has_r) sh.skew = -1;
        else if (si.has_r && !si.has_l) sh.skew = 1;
    }
    return sh;
}

static void lnk_print_rec(const Node* n, char* prefix, size_t plen)
{
    const Node* kid[2];
    const char* tag[2];
    int k = 0;

    if (n->left) { kid[k] = n->left;  tag[k++] = "L"; }
    if (n->right) { kid[k] = n->right; tag[k++] = "R"; }

    for (int j = 0; j < k; j++) {
        int last = (j == k - 1);
        printf("%s%s[%s] %c\n", prefix, last ? "`--" : "+--", tag[j], kid[j]->data);
        strcpy(prefix + plen, last ? "    " : "|   ");
        lnk_print_rec(kid[j], prefix, plen + 4);
        prefix[plen] = '\0';
    }
}

void lnk_print(const Node* root)
{
    if (!root) { puts("  (공백 트리)"); return; }
    char prefix[4 * MAX_LEVEL + 8] = "  ";
    printf("  %c\n", root->data);
    lnk_print_rec(root, prefix, 2);
}

void print_stats(const Stats* s)
{
    printf("  전체 노드 수     : %d\n", s->nodes);
    printf("  단말 노드 수     : %d\n", s->leaves);
    printf("  비단말 노드 수   : %d\n", s->internal);
    printf("  트리의 높이      : %d\n", s->height);
    printf("  트리의 차수      : %d\n", s->degree);
}

void print_shape(const Shape* sh, int nodes)
{
    if (nodes == 0) { puts("  트리 종류        : 공백 트리"); return; }
    printf("  포화 이진트리    : %s\n", sh->full ? "예" : "아니오");
    printf("  완전 이진트리    : %s\n", sh->complete ? "예" : "아니오");
    printf("  편향 이진트리    : %s\n",
        sh->skew < 0 ? "예 (왼쪽 편향)" : sh->skew > 0 ? "예 (오른쪽 편향)" : "아니오");
}

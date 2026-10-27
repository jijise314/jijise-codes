#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

int main(void)
{
    Data* orig, * work;

    orig = (Data*)malloc(sizeof(Data) * DATA_COUNT);
    work = (Data*)malloc(sizeof(Data) * DATA_COUNT);
    if (!orig || !work) {
        fprintf(stderr, "메모리 할당 실패\n");
        free(orig);
        free(work);
        return 1;
    }

    printf("데이터 개수 : %d\n", DATA_COUNT);
    printf("키 범위     : 0 ~ %d\n\n", KEY_MAX);

    experiment_random(orig, work, DATA_COUNT);
    experiment_input_states(orig, work, DATA_COUNT);
    experiment_stability(work, DATA_COUNT);

    free(orig);
    free(work);
    return 0;
}

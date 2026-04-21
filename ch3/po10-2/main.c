#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sparseMatrix.h"

int main() {
	srand((unsigned int)time(NULL));

	sparseMatrix* sm = createSparseMatrix(10, 10);

	printf("10x10 의소행렬에 0이 아닌 원소 20개 무작위 초기화\n");
	int count = 0;
	while (count < 20) {
		int r = rand() % 10;
		int c = rand() % 10;
		int v = (rand() % 99) + 1;

		int Duplicate = 0;
		for (int i = 0; i < sizeArrayList(sm); i++) {
			elementArrayList* item = getItemArrayList(sm, i);
			if (item->row == r && item->col == c) {
				Duplicate = 1;
				break;
			}
		}

		if (!Duplicate) {
			addElementSparseMatrix(sm, r, c, v);
			count++;
		}
	}

	printSparseMatrix(sm);
	printf("\n");

	int moveCount = 0;
	sparseMatrix* trans = transposeSparseMatrix(sm, &moveCount);

	printf("만들어진 희소행렬의 전치행렬\n");
	printSparseMatrix(trans);
	printf("=> 전치행렬 생성 간 총 데이터 이동 횟수: %d번\n", moveCount);

	destroyArrayList(sm);
	destroyArrayList(trans);
}
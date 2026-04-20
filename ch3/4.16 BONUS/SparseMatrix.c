#include <stdio.h>
#include <stdlib.h>

#include "sparseMatrix.h"

sparseMatrix* createSparseMatrix(int row, int col) {
	return createArrayList(row * col);
}

int addElementSparseMatrix(sparseMatrix* sm, int row, int col, int value) {
	int size;
	size = sizeArrayList(sm);

	insertArrayList(sm, size, (elementArrayList) {
		row, col, value
	});
}

int printSparseMatrix(sparseMatrix* sm) {
	printf("희소행렬입니다.\n");

	printArrayList(sm);
}

sparseMatrix* addSparseMatrix(sparseMatrix* sm1, sparseMatrix* sm2) {
	sparseMatrix* smResult = createArrayList(100);

	// i <= sizeArrayList(sm1),j <= sizeArrayList(sm2) 에서 i < sizeArrayList(sm1),j < sizeArrayList(sm2)로 수정
	for (int i = 0; i < sizeArrayList(sm1); i++) {
		elementArrayList nonZeroOfSM1 = getItemArrayList(sm1, i);

		int j;
		for (j = 0; j < sizeArrayList(sm2); j++) {
			elementArrayList nonZeroOfSM2 = getItemArrayList(sm2, j);

			if (nonZeroOfSM1.col == nonZeroOfSM2.col &&
				nonZeroOfSM1.row == nonZeroOfSM2.row) {
				insertArrayList(smResult, sizeArrayList(smResult), (elementArrayList) {
					nonZeroOfSM1.row,
						nonZeroOfSM1.col,
						nonZeroOfSM1.value + nonZeroOfSM2.value
				});

				break;
			}
		}

		if (j == sizeArrayList(sm2)) {
			insertArrayList(smResult, sizeArrayList(smResult), nonZeroOfSM1);
		}
	}

	return smResult;
}
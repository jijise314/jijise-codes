#include <stdio.h>
#include <stdlib.h>

#include "sparseMatrix.h"

sparseMatrix* createSparseMatrix(int maxRow, int maxCol) {
	return createArrayList(maxRow * maxCol, maxRow, maxCol);
}

int addElementSparseMatrix(sparseMatrix* sm, int row, int col, int value) {
	if (value == 0) return 0;
	
	int size = sizeArrayList(sm);
	elementArrayList item = { row,col,value };
	return insertArrayList(sm, size, item);
}

int printSparseMatrix(sparseMatrix* sm) {
	printArrayList(sm);
}

sparseMatrix* transposeSparseMatrix(sparseMatrix* sm, int* moveCount) {
	sparseMatrix* trans = createSparseMatrix(sm->maxCol, sm->maxRow);
	*moveCount = 0;

	if (sm->size > 0) {
		for (int c = 0; c < sm->maxCol; c++) {
			for (int i = 0; i < sm->size; i++) {
				elementArrayList* curr = getItemArrayList(sm, i);
				if (curr->col == c) {
					addElementSparseMatrix(trans, curr->col, curr->row, curr->value);
					(*moveCount)++;
				}
			}
		}
	}
	return trans;
}


sparseMatrix* addSparseMatrix(sparseMatrix* sm1, sparseMatrix* sm2) {
	sparseMatrix* smResult = createSparseMatrix(sm1->maxRow, sm1->maxCol);

	for (int i = 0; i < sizeArrayList(sm1); i++) {
		elementArrayList* nonZeroOfSM1 = getItemArrayList(sm1, i);

		int j;
		for (j = 0; j < sizeArrayList(sm2); j++) {
			elementArrayList* nonZeroOfSM2 = getItemArrayList(sm2, j);

			if (nonZeroOfSM1->col == nonZeroOfSM2->col &&
				nonZeroOfSM1->row == nonZeroOfSM2->row) {
				addElementSparseMatrix(smResult, nonZeroOfSM1->row, nonZeroOfSM1->col,
					nonZeroOfSM1->value + nonZeroOfSM2->value);
				break;
			}
		}
		if (j == sizeArrayList(sm2)) {
			addElementSparseMatrix(smResult, nonZeroOfSM1->row,
				nonZeroOfSM1->col, nonZeroOfSM1->value);
		}
	}
	return smResult;
}
#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

typedef arrayList sparseMatrix;

extern sparseMatrix* createSparseMatrix(int maxRow, int maxCol);
extern int addElementSparseMatrix(sparseMatrix* sm, int row, int col, int value);
extern int printSparseMatrix(sparseMatrix* sm);
extern sparseMatrix* addSparseMatrix(sparseMatrix* sm1, sparseMatrix* sm2);
extern sparseMatrix* transposeSparseMatrix(sparseMatrix* sm, int* moveCount);
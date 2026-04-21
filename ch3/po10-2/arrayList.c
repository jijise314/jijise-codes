#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

arrayList* createArrayList(int capacity, int maxRow, int maxCol) {
	arrayList* al;
	al = (arrayList*)malloc(sizeof(arrayList));

	al->data = (elementArrayList**)malloc(
		sizeof(elementArrayList*) * capacity);
	for (int i = 0; i < capacity; i++) {
		al->data[i] = NULL;
	}
	al->size = 0;
	al->capacity = capacity;
	al->maxRow = maxRow;
	al->maxCol = maxCol;

	return al;
}

void destroyArrayList(arrayList* al) {
	for (int i = 0; i < al->size; i++) {
		if (al->data[i] != NULL) free(al->data[i]);
	}
	free(al->data);
	free(al);
}

int isEmptyArrayList(arrayList* al) {
	if (al->size == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

int isFullArrayList(arrayList* al) {
	if (al->size == al->capacity) {
		return 1;
	}
	else {
		return 0;
	}
}

int sizeArrayList(arrayList* al) {
	return al->size;
}

int insertArrayList(arrayList* al,
	int pos, elementArrayList item) {
	if (pos < 0 || pos > al->size) {
		return 0;
	}

	for (int i = al->size - 1; i >= pos; i--) {
		al->data[i + 1] = al->data[i];
	}

	al->data[pos] = (elementArrayList*)malloc(sizeof(elementArrayList));
	al->data[pos]->row = item.row;
	al->data[pos]->col = item.col;
	al->data[pos]->value = item.value;
	al->size++;

	return 1;
}

elementArrayList deleteArrayList(
	arrayList* al, int pos) {
	elementArrayList item = { 0,0,0 };
	if (pos < 0 || pos > al->size) {
		return item;
	}

	item = *(al->data[pos]);
	free(al->data[pos]);

	for (int i = pos; i < al->size - 1; i++) {
		al->data[i] = al->data[i + 1];
	}

	al->data[al->size - 1] = NULL;
	al->size--;

	return item;
}

int initArrayList(arrayList* al) {
	while (al->size > 0) {
		deleteArrayList(al, al->size - 1);
	}
	return 1;
}

elementArrayList* getItemArrayList(
	arrayList* al, int pos) {
	if (pos < 0 || pos >= al->size) return NULL;
	return al->data[pos];
}

int replaceItemArrayList(arrayList* al,
	int pos, elementArrayList item) {
	if (pos < 0 || pos >= al->size) {
		return 0;
	}
	al->data[pos]->row = item.row;
	al->data[pos]->col = item.col;
	al->data[pos]->value = item.value;
	return 1;
}

void printArrayList(arrayList* al) {
	printf("크기: %dx%d, 원소 개수: %d\n", al->maxRow, al->maxCol, al->size);

	for (int i = 0; i < al->size; i++) {
		printf("(%d, %d, %d) ",
			al->data[i]->row, al->data[i]->col, al->data[i]->value);
		if ((i + 1) % 5 == 0) printf("\n");
	}
	printf("\n");
}

#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

arrayList* createArrayList(int size) {
	arrayList* al;
	al = (arrayList*)malloc(sizeof(arrayList));

	al->data = (elementArrayList*)malloc(
		sizeof(elementArrayList) * size);
	al->size = 0;
	al->capacity = size;
	return al;
}

void destroyArrayList(arrayList* al) {
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
	if (isFullArrayList(al)) {
		printf("Error: 리스트가 꽉 찼습니다.\n");
		return 0;
	}
	if (pos<0 || pos>al->size || item <= 0) {
		return 0;
	}
	for (int i = al->size - 1; i >= pos; i--) {
		al->data[i + 1] = al->data[i];
	}

	al->data[pos] = item;
	al->size++;

	return 1;
}

elementArrayList deleteArrayList(
	arrayList* al, int pos) {
	if (pos < 0 || pos >= al->size) {
		return -1;
	}

	elementArrayList item = al->data[pos];

	for (int i = pos; i < al->size - 1; i++) {
		al->data[i] = al->data[i + 1];
	}

	al->size--;

	return item;
}

int initArrayList(arrayList* al) {
	al->size = 0;
	return 1;
}

elementArrayList getItemArrayList(
	arrayList* al, int pos) {
	return al->data[pos];
}

int replaceItemArrayList(arrayList* al,
	int pos, elementArrayList item) {
	if (pos < 0 || pos > al->size - 1) {
		return 0;
	}

	al->data[pos] = item;

	return 1;
}

void printArrayList(arrayList* al) {
	printf("=== 양의 정수 리스트 출력 ===\n");
	if (al->size == 0) {
		printf("리스트가 비어있습니다.\n");
		return;
	}
	for (int i = 0; i < al->size; i++) {
		printf("위치[%d] : %d\n", i, al->data[i]);
	}
}

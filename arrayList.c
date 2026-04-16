#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

arrayList* createArrayList(int size) {
	arrayList* al;
	al = (arrayList*)malloc(sizeof(arrayList));

	al->data = (elementArrayList*)malloc(sizeof(elementArrayList) * size);
	al->size = 0;
	al->capacity = size;

	return al;
}

void destroyArrayList(arrayList* al) {
	free(al->data);
	free(al);
}

int isEmptyArrayList(arrayList* al) {
	if (al->size == al->capacity) {
		return 1;
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

int insertArrayList(arrayList* al, int pos, elementArrayList item){
	if (pos<0 || pos>al->size) {
		return 0;
	}

	if (isFullArrayList(al)) {
		int newCapacity = (al->capacity == 0) ? 1 : al->capacity * 2;

		elementArrayList* newData = (elementArrayList*)realloc(al->data, sizeof(elementArrayList) * newCapacity);

		if (newData == NULL) {
			printf("메모리 realloc 실패!\n");
			return 0;
		}

		al->data = newData;
		al->capacity = newCapacity;

		printf("배열의 capacity를 %d로 확장했습니다.\n", al->capacity);
	}

	for (int i = al->size - 1; i >= pos; i--) {
		al->data[i + 1] = al->data[i];
	}

	al->data[pos] = item;
	al->size++;

	return 1;
}

elementArrayList deletArrayList(arrayList* al, int pos) {
	if (pos<0 || pos>al->size - 1) {
		return 0;
	}

	elementArrayList item = al->data[pos];

	for (int i = pos; i < al->size - 1; i++) {
		al->data[i] = al->data[i + 1];
	}

	al->size--;

	return item;
}

int initArrayList(arrayList* al) {
	for (int i = al->size - 1; i >= 0; i--) {
		deletArrayList(al, i);
	}
}

elementArrayList getItemArrayList(arrayList* al, int pos) {
	return al->data[pos];
}

int replaceItemArrayList(arrayList* al, int pos, elementArrayList item) {
	if (pos<0 || pos>al->size - 1) {
		return 0;
	}

	al->data[pos] = item;

	return 1;
}

void printArrayList(arrayList* al) {
	printf("ArrayList (size: %d, capacity: %d) : ", al->size, al->capacity);

	for (int i = 0; i < al->size; i++) {
		printf("%d ", al->data[i]);
	}
}
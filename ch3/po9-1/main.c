#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

int main() {
	arrayList* myArrayList;
	myArrayList = createArrayList(2);

	if (sizeArrayList(myArrayList) == 0) {
		printf("create success! (capacity: %d)\n", myArrayList->capacity);
	}

	insertArrayList(myArrayList, 0, 10);
	insertArrayList(myArrayList, 1, 20);
	insertArrayList(myArrayList, 2, 30);
	insertArrayList(myArrayList, 0, 5);

	printArrayList(myArrayList);
	printf("\n");

	deletArrayList(myArrayList, 0);
	deletArrayList(myArrayList, 1);
	deletArrayList(myArrayList, 2);

	printArrayList(myArrayList);
}

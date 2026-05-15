#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

int main() {
	LinkedList* myList = createLinkedList();

	printf("is myList1 empty: %s\n",
		(isEmptyLinkedList(myList) == 1) ? "Yes" : "No");
	printf("size of myList: %d\n", sizeLinkedList(myList));

	insertFirstLinkedList(myList, (PointType) { 10, 20, 0 });
	insertLastLinkedList(myList, (PointType) { 20, 30, 0 });
	insertItemLinkedList(myList, NULL, (PointType) { 30, 40, 0 });
	insertAtLinkedList(myList, 3, (PointType) { 10, 20, 0 });
	printLinkedList(myList);

	insertFirstLinkedList(myList, (PointType) { 10, 20, 0 });
	deleteFirstLinkedList(myList);
	deleteLastLinkedList(myList);
	deleteItemLinkedList(myList, 1);
	deleteAtLinkedList(myList, 1);
	printLinkedList(myList);

	PointType Item = getItemLinkedList(myList, 0);
	printf("x: %d y: %d\n", Item.x, Item.y);

	replaceItemLinkedList(myList, 0, (PointType) { 10, 20, 0 });
	printLinkedList(myList);

	insertLastLinkedList(myList, (PointType) { 20, 30, 0 });
	printLinkedList(myList);
	PointType next = nextItemLinkedList(myList, 0);
	printf("x: %d y: %d\n", next.x, next.y);
}
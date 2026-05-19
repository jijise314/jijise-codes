#include "CircularList.h"

// 생성/소멸
LinkedList* createLinkedList() {
	LinkedList* CL = (LinkedList*)malloc(sizeof(LinkedList));

	CL->head.prev = &(CL->head);
	CL->head.next = &(CL->head);
	CL->head.x = 0;
	CL->head.y = 0;

	CL->size = 0;
	return CL;
}

int destroyLinkedList(LinkedList* CL) {
	while (CL->size > 0) {
		deleteFirstLinkedList(CL);
	}
	free(CL);
	return 1;
}

// 상태확인
int isEmptyLinkedList(LinkedList* CL) {
	return (CL->size == 0);
}

int sizeLinkedList(LinkedList* CL) {
	return CL->size;
}

// 추가
LinkedList* insertFirstLinkedList(LinkedList* CL, PointType item) {
	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	nPtr->prev = &(CL->head);
	nPtr->next = CL->head.next;

	CL->head.next->prev = nPtr;
	CL->head.next = nPtr;

	CL->size++;

	return CL;
}

LinkedList* insertLastLinkedList(LinkedList* CL, PointType item) {
	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	nPtr->prev = CL->head.prev;
	nPtr->next = &(CL->head);

	CL->head.prev->next = nPtr;
	CL->head.prev = nPtr;

	CL->size++;

	return CL;
}

LinkedList* insertItemLinkedList(LinkedList* CL, PointType* pre, PointType item) {
	if (pre == NULL) return CL;

	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	nPtr->prev = pre;
	nPtr->next = pre->next;

	pre->next->prev = nPtr;
	pre->next = nPtr;

	CL->size++;

	return CL;
}

LinkedList* insertAtLinkedList(LinkedList* CL, int at, PointType item) {
	if (at < 0 || at > CL->size) return CL;

	PointType* cur = &(CL->head);
	for (int i = 0; i < at; i++) {
		cur = cur->next;
	}

	return insertItemLinkedList(CL, cur, item);
}

// 삭제
PointType deleteFirstLinkedList(LinkedList* CL) {
	PointType empty = { 0, 0, NULL, NULL };
	if (CL->size == 0) return empty;

	PointType* dPtr = CL->head.next;
	PointType temp = *dPtr;

	CL->head.next = dPtr->next;
	dPtr->next->prev = &(CL->head);

	free(dPtr);
	CL->size--;

	return temp;
}

PointType deleteLastLinkedList(LinkedList* CL) {
	PointType empty = { 0, 0, NULL, NULL };
	if (CL->size == 0) return empty;

	PointType* dPtr = CL->head.prev;
	PointType temp = *dPtr;

	dPtr->prev->next = &(CL->head);
	CL->head.prev = dPtr->prev;

	free(dPtr);
	CL->size--;

	return temp;
}

LinkedList* deleteItemLinkedList(LinkedList* CL, int pos) {
	return deleteAtLinkedList(CL, pos);
}

LinkedList* deleteAtLinkedList(LinkedList* CL, int at) {
	if (at < 0 || at >= CL->size) return CL;

	PointType* cur = CL->head.next;
	for (int i = 0; i < at; i++) {
		cur = cur->next;
	}

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;

	free(cur);
	CL->size--;

	return CL;
}

// 데이터 접근
PointType getItemLinkedList(LinkedList* CL, int pos) {
	PointType empty = { 0, 0, NULL, NULL };
	if (pos >= 0 && pos < CL->size) {
		PointType* cur = CL->head.next;
		for (int i = 0; i < pos; i++) {
			cur = cur->next;
		}
		return *cur;
	}

	printf("범위 오류\n");
	return empty;
}

PointType replaceItemLinkedList(LinkedList* CL, int pos, PointType item) {
	PointType empty = { 0, 0, NULL, NULL };
	if (pos >= 0 && pos < CL->size) {
		PointType* cur = CL->head.next;
		for (int i = 0; i < pos; i++) {
			cur = cur->next;
		}

		PointType oldData = *cur;
		cur->x = item.x;
		cur->y = item.y;

		return oldData;
	}

	printf("범위 오류\n");
	return empty;
}

PointType nextItemLinkedList(LinkedList* CL, int pre) {
	PointType empty = { 0, 0, NULL, NULL };
	if (pre >= 0 && pre + 1 < CL->size) {
		PointType* cur = CL->head.next;
		for (int i = 0; i <= pre; i++) {
			cur = cur->next;
		}
		return *cur;
	}

	printf("다음 데이터가 존재하지 않습니다.\n");
	return empty;
}

// 출력
int printLinkedList(LinkedList* CL) {
	if (CL->size == 0) {
		printf("리스트가 비어있습니다.\n");
		return 0;
	}

	PointType* current = CL->head.next;
	printf("LinkedList\n");

	for (int i = 0; i < CL->size; i++) {
		printf("[%d] x:%d, y:%d, prev:%p, next:%p\n",
			i, current->x, current->y, current->prev, current->next);
		current = current->next;
	}
	return 1;
}
#include "LinkedList.h"

// 생성/소멸
LinkedList* createLinkedList() {
	struct linkedList* r;
	r = (struct linkedList*)malloc(sizeof(struct linkedList));

	r->head = (struct pointType*)NULL;
	r->size = 0;

	return r;
}

int destroyLinkedList(LinkedList* li) {
	struct pointType* nptr, * mptr;
	for (nptr = li->head; nptr != NULL; nptr = mptr) {
		mptr = nptr->next;
		free(nptr);
	}

	free(li);

	return 1;
}

// 상태확인
int isEmptyLinkedList(LinkedList* li) {
	return (li->size == 0);
}

int sizeLinkedList(LinkedList* li) {
	return li->size;
}

// 추가
LinkedList* insertFirstLinkedList(LinkedList* li, PointType item) {
	PointType* ptr = (PointType*)malloc(sizeof(PointType));
	*ptr = item;

	ptr->next = li->head;
	li->head = ptr;
	li->size++;

	return li;
}

LinkedList* insertLastLinkedList(LinkedList* li, PointType item) {

	if (li->head == NULL) {
		return insertFirstLinkedList(li, item);
	}

	PointType* ptr = li->head;
	while (ptr->next != NULL) {
		ptr = ptr->next;
	}

	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;
	nPtr->next = NULL;

	ptr->next = nPtr;

	li->size++;

	return li;
}

LinkedList* insertItemLinkedList(LinkedList* li, PointType* pre, PointType item) {
	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;
	if (pre != NULL) {
		nPtr->next = pre->next;
		pre->next = nPtr;
	}
	else {
		nPtr->next = li->head;
		li->head = nPtr;
	}

	li->size++;
}

LinkedList* insertAtLinkedList(LinkedList* li, int at, PointType item) {
	if (at >= 0 && at <= li->size) {
		PointType* nPtr = li->head;

		for (int i = 0; i < at - 1; i++) {
			nPtr = nPtr->next;
		}

		if (at == 0) {
			return insertFirstLinkedList(li, item);
		}
		else {
			PointType* newNode = (PointType*)malloc(sizeof(PointType));
			*newNode = item;

			newNode->next = nPtr->next;
			nPtr->next = newNode;
		}

		li->size++;
	}
	return li;
}

// 삭제
PointType deleteFirstLinkedList(LinkedList* li) {
	if (li->size != 0)
	{
		PointType* tPtr = li->head;
		li->head = tPtr->next;

		PointType temp = *tPtr;
		free(tPtr);

		li->size--;
		return temp;
	}
	else {
		return (PointType) { 0, 0, 0 };
	}
}

PointType deleteLastLinkedList(LinkedList* li) {
	PointType* pre;
	PointType* iPtr;

	pre = li->head;

	if (pre == NULL) {
		return (PointType) { 0, 0, 0, };
	}

	iPtr = pre->next;

	if (iPtr == NULL) {
		li->head = NULL;

		PointType temp;
		temp = *pre;

		free(pre);
		li->size--;

		return temp;
	}

	while (iPtr->next != NULL) {
		pre = iPtr;
		iPtr = iPtr->next;
	}

	PointType temp;
	temp = *iPtr;

	pre->next = NULL;
	free(iPtr);
	li->size--;

	return temp;
}

LinkedList* deleteItemLinkedList(LinkedList* li, int pos) {
	if (pos >= 0 && pos < li->size) {
		PointType* pre = NULL;

		if (pos > 0) {
			pre = li->head;
			for (int i = 0; i < pos - 1; i++) {
				pre = pre->next;
			}
		}

		if (pre != NULL) {
			PointType* dPtr = pre->next;
			pre->next = dPtr->next;
			free(dPtr);
		}
		else {
			PointType* dPtr = li->head;
			pre->next = dPtr->next;
			free(dPtr);
		}
		li->size--;
	}
	return li;
}

LinkedList* deleteAtLinkedList(LinkedList* li, int at) {
	if (at >= 0 && at < li->size) {
		PointType* nPtr = li->head;

		for (int i = 0; i < at - 1; i++) {
			nPtr = nPtr->next;
		}

		if (at == 0) {
			PointType* fPtr = li->head;
			li->head = fPtr->next;
			free(fPtr);
		}
		else {
			PointType* fPtr = nPtr->next;
			nPtr->next = fPtr->next;
			free(fPtr);
		}

		li->size--;
	}
	return li;
}

PointType getItemLinkedList(LinkedList* li, int pos) {
	if (pos >= 0 && pos < li->size) {
		PointType* current = li->head;

		for (int i = 0; i < pos; i++) {
			current = current->next;
		}
		return *current;
	}

	PointType empty = { 0, 0, NULL };
	printf("범위 오류\n");
	return empty;
}

// 데이터 접근
PointType replaceItemLinkedList(LinkedList* li, int pos, PointType item) {
	PointType empty = { 0, 0, NULL };

	if (pos >= 0 && pos < li->size) {
		PointType* cur = li->head;

		for (int i = 0; i < pos; i++) {
			cur = cur->next;
		}

		PointType Data = *cur;

		cur->x = item.x;
		cur->y = item.y;

		return Data;
	}

	printf("범위 오류\n");
	return empty;
}

PointType nextItemLinkedList(LinkedList* li, int pre) {
	PointType empty = { 0, 0, NULL };

	if (pre >= 0 && pre + 1 < li->size) {
		PointType* cur = li->head;

		for (int i = 0; i <= pre; i++) {
			cur = cur->next;
		}

		return *cur;
	}

	printf("다음 데이터가 존재하지 않습니다.\n");
	return empty;
}

// 출력
int printLinkedList(LinkedList* li) {
	PointType* current = li->head;
	printf("LinkedList\n");

	for (int i = 0; i < li->size; i++) {
		printf("[%d] x:%d, y:%d, next:%x\n",
			i, current->x, current->y, current->next);

		current = current->next;
	}
}
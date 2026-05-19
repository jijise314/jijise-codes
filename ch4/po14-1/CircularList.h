#include <stdio.h>
#include <stdlib.h>

typedef struct pointType {
	int x;
	int y;
	struct pointType* prev;
	struct pointType* next;
} PointType;

typedef struct CLnkedList {
	PointType head;
	int size;
} LinkedList;

extern LinkedList* createLinkedList();
extern int destroyLinkedList(LinkedList* CL);

extern int isEmptyLinkedList(LinkedList* CL);
extern int sizeLinkedList(LinkedList* CL);

extern LinkedList* insertFirstLinkedList(LinkedList* CL, PointType item);
extern LinkedList* insertLastLinkedList(LinkedList* CL, PointType item);
extern LinkedList* insertItemLinkedList(LinkedList* CL, PointType* pre, PointType item);
extern LinkedList* insertAtLinkedList(LinkedList* CL, int at, PointType item);

extern PointType deleteFirstLinkedList(LinkedList* CL);
extern PointType deleteLastLinkedList(LinkedList* CL);
extern LinkedList* deleteItemLinkedList(LinkedList* CL, int pos);
extern LinkedList* deleteAtLinkedList(LinkedList* CL, int at);

extern PointType getItemLinkedList(LinkedList* CL, int pos);

extern PointType replaceItemLinkedList(LinkedList* CL, int pos, PointType item);
extern PointType nextItemLinkedList(LinkedList* CL, int pre);

extern int printLinkedList(LinkedList* CL);
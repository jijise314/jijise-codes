#include "linkedStack.h"

LinkedStack* createLinkedStack() {
	LinkedStack* re = (LinkedStack*)malloc(sizeof(LinkedStack));
	re->head = NULL;
	re->size = 0;

	return re;
 }

 void destroyLinkedStack(LinkedStack* s) {
	 while (!emptyLinkedStack(s)) {
		 stackElement temp = popLinkedStack(s);
		 free(temp);
	 }
	 free(s);
 }

 int emptyLinkedStack(LinkedStack* s) {
	 return s->size == 0;
 }

 int fullLinkedStack(LinkedStack* s) {
	 return 0;
 }

 int pushLinkedStack(LinkedStack* s, stackElement item) {
	 Node* temp = (Node*)malloc(sizeof(Node));

	 temp->data = (char*)malloc(strlen(item) + 1);
	 strcpy(temp->data, item);

	 temp->next = s->head;
	 s->head = temp;
	 s->size++;

	 return 1;
 }

 stackElement popLinkedStack(LinkedStack* s) {
	 if (emptyLinkedStack(s)) return NULL;

	 Node* temp = s->head;
	 stackElement Data = temp->data;

	 s->head = temp->next;
	 free(temp);
	 s->size--;

	 return Data;
 }

 stackElement peekLinkedStack(LinkedStack* s) {
	 if (emptyLinkedStack(s)) return NULL;
	 return s->head->data;
 }

 void printLinkedStack(LinkedStack* s) {
	 printf("Stack\n");
	 printf("Size: %d\n", s->size);
	 Node* temp = s->head;
	 while (temp != NULL) {
		 printf("s\n", temp->data);
		 temp = temp->next;
	 }
 }
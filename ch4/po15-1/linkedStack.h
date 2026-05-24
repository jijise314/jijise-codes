#include <stdio.h>
#include <stdlib.h>

typedef char* stackElement;

typedef struct node {
	stackElement data;
	struct node* next;
} Node;

typedef struct linkedStack {
	Node* head;
	int size;
} LinkedStack;

extern LinkedStack* createLinkedStack();
extern void destroyLinkedStack(LinkedStack* s);

extern int emptyLinkedStack(LinkedStack* s);
extern int fullLinkedStack(LinkedStack* s);

extern int pushLinkedStack(LinkedStack* s, stackElement item);
extern stackElement popLinkedStack(LinkedStack* s);

extern stackElement peekLinkedStack(LinkedStack* s);

extern void printLinkedStack(LinkedStack* s);
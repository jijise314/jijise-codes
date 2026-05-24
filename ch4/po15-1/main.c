#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "linkedStack.h"

int main() {
	LinkedStack* myStack = createLinkedStack();
	int choice;
	char Url[2048];

	while (1) {
		scanf("%d", &choice);

		if (choice == 1) {
			printf("Url 입력 : ");
			scanf("%s", Url);

			pushLinkedStack(myStack, Url);
			printf("현재 창: %s\n", peekLinkedStack(myStack));
		}
		else if (choice == 2) {
			if (!emptyLinkedStack(myStack)) {
				stackElement poppedUrl = popLinkedStack(myStack);
				printf("이탈: %s\n", poppedUrl);
				free(poppedUrl);

				if (emptyLinkedStack(myStack)) {
					printf("브라우저가 종료되었습니다.\n");
					break;
				}
				else {
					printf("현재 창: %s\n", peekLinkedStack(myStack));
				}
			}
		}
		else if (choice == 3) {
			if (!emptyLinkedStack(myStack)) {
				printf("현재 창: %s\n", peekLinkedStack(myStack));
			}
		}
	}
	destroyLinkedStack(myStack);

	return 0;
}
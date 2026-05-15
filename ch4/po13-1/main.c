#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

int main() {
	int menu;
	int value;
	int pos;

	LinkedList* myList = createLinkedList();

	do {
		printf("\n연결 리스트 실행\n");

		printf("정수를 입력하세요 : 1. 맨 앞에 추가, 2. 맨 뒤에 추가, 3. 특정 위치에 추가, 4. 특정 위치 삭제, 5. 전체 목록 출력, 6. 리스트 크기 확인, 0.종료 ");
		scanf("%d", &menu);

		switch (menu) {
		case 0: printf("\n모든 메모리를 해제(destroy)하고 프로그램을 종료합니다.");  destroyLinkedList(myList);
			break;

		case 1: printf("\n맨 앞에 추가할 정수(value)를 입력하세요 : ");
			scanf("%d", &value);
			insertFirstLinkedList(myList, (PointType) { value, value, NULL });
			break;

		case 2: printf("\n맨 뒤에 추가할 정수를 입력하세요 : ");
			scanf("%d", &value);
			insertLastLinkedList(myList, (PointType) { value, value, NULL });
			break;

		case 3: printf("\n추가할 위치와 정수를 입력하세요(순서-> 위치 정수) : ");
			scanf("%d %d", &pos, &value);
			insertAtLinkedList(myList, pos, (PointType) { value, value, NULL });
			break;

		case 4: printf("\n삭제할 위치를 입력하세요 : ");
			scanf("%d", &pos);
			if (pos >= 0 && pos < sizeLinkedList(myList)) {
				deleteAtLinkedList(myList, pos);
			}
			else {
				printf("잘못된 위치입니다.");
			}
			break;

		case 5: printf("\n전체 목록 출력\n"); printLinkedList(myList);
			break;

		case 6: printf("\n리스트 크기 확인\n리스트 크기 : %d\n", sizeLinkedList(myList));
			break;
		}
	} while (menu != 0);

}
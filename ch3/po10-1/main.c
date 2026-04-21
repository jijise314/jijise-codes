#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

void Menu() {
	printf("1.추가 2.삭제 3.변경 4.출력 5.전체 삭제 0.종료\n");
	printf("선택: ");
}

int main() {
	arrayList* myArrayList;
	myArrayList = createArrayList(100);
	int choice, pos, item;

	if (sizeArrayList(myArrayList) == 0) {
		printf("Create ok\n");
	}
	while (1) {
		Menu();
		if (scanf("%d", &choice) != 1) {
			while (getchar() != '\n');
			continue;
		}

		switch (choice) {
		case 1:
			printf("추가할 양의 정수와 위치를 입력허세요.(예: 10 0): ");
			scanf("%d %d", &item, &pos);
			if (item <= 0) {
				printf("=> 양의 정수를 입력하세요!");
			}
			else if(insertArrayList(myArrayList, pos, item)){
				printf("=> %d에 %d가 추가되었습니다.\n", pos, item);
			}
			else {
				printf("=> 잘못된 위치입니다.");
			}
			break;

		case 2:
			printf("삭제할 위치를 입력하세요.: ");
			scanf("%d", &pos);
			item = deleteArrayList(myArrayList, pos);
			if (item < 0) {
				printf("=> 잘못된 위치입니다.\n");
			}
			else {
				printf("=> %d의 %d가 삭제되었습니다.\n", pos, item);
			}
			break;

		case 3:
			printf("변경할 양의 정수와 위치를 입력하세요.(예: 20 0)");
			scanf("%d %d", &item, &pos);
			if (item <= 0) {
				printf("=> 양의 정수를 입력하세요!\n");
			}
			else if (replaceItemArrayList(myArrayList, pos, item)) {
				printf("=> %d의 값이 %d로 변경되었습니다.\n", pos, item);
			}
			else {
				printf("=> 잘못된 위치입니다.");
			}
			break;
			
		case 4:
			printArrayList(myArrayList);
			break;
			
		case 5:
			initArrayList(myArrayList);
			printf("=> 리스트의 모든 데이터가 삭제되었습니다.\n");
			break;

		case 0:
			destroyArrayList(myArrayList);
			printf("프로그램 종료!");
			return 0;

		default:
			printf("=> 잘못된 입력입니다. 다시 선택하세요.\n");
		}
	}
	return 0;
}
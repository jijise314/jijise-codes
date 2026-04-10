#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct info {
	char name[50];
	int id;
	int score;
};

int main() {

	int N;

	printf("학생수(N)를 입력하세요.: ");
	if (scanf("%d", &N) != 1 || N <= 0) {
		printf("1 이상의 정수를 입력하세요");
		return 1;
	}

	struct info* arr = (struct info*)malloc(N * sizeof(struct info));
	if (arr == NULL) {
		printf("메모리 할당 실패\n");
		return 1;
	}

	FILE* fp = fopen("name_list.txt", "r");
	if (fp == NULL) {
		printf("name_list.txt 파일을 열 수 없습니다. 파일 위치를 확인해주세요.\n");
		return 1;
	}

	char name_pool[200][50];
	int pool_size = 0;

	while (fgets(*(name_pool + pool_size), sizeof(*(name_pool + 0)), fp) != NULL) {
		*(*(name_pool + pool_size) + strcspn(*(name_pool + pool_size), "\n")) = '\0';
		pool_size++;
		if (pool_size >= 200) break;
	}
	fclose(fp);

	if (N > pool_size) {
		printf("중복 방지를 위해 학생 수를 %d명으로 제한합니다.\n", pool_size);
		N = pool_size;
	}

	srand((unsigned int)time(NULL));

	int* idx = (int*)malloc(pool_size * sizeof(int));
	for (int i = 0; i < pool_size; i++) {
		*(idx + i) = i;
	}

	for (int i = pool_size - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = *(idx + i);
		*(idx + i) = *(idx + j);
		*(idx + j) = temp;
	}
	for (int i = 0; i < N; i++) {
		strcpy((*(arr + i)).name, *(name_pool + *(idx + i)));
		(*(arr + i)).id = i + 1;
		(*(arr + i)).score = (rand() % 100) + 1;
	}
	free(idx);

	printf("==학생정보 출력==\n");
	for (int i = 0; i < N; i++) {
		printf("이름: %-20s | 학번: %3d | 점수: %3d\n", (*(arr + i)).name, (*(arr + i)).id, (*(arr + i)).score);
	}
	printf("\n");

	if (N > 0) {
		int max_idx = 0;
		int min_idx = 0;
		int sum = 0;

		for (int i = 0; i < N; i++) {
			if ((*(arr + i)).score > (*(arr + max_idx)).score) {
				max_idx = i;
			}
			if ((*(arr + i)).score < (*(arr + min_idx)).score) {
				min_idx = i;
			}
			sum += (*(arr + i)).score;
		}

		double average = (double)sum / N;

		printf("==최고점, 최저점 학생정보 및 학생 평균점수==\n");
		printf("최고점 학생: %s (학번: %d, 점수: %d)\n", (*(arr + max_idx)).name, (*(arr + max_idx)).id, (*(arr + max_idx)).score);
		printf("최저점 학생: %s (학번: %d, 점수: %d)\n", (*(arr + min_idx)).name, (*(arr + min_idx)).id, (*(arr + min_idx)).score);
		printf("학생 평균점수: %.1f\n", average);

	}

	free(arr);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int N;

	printf("정수 N을 입력하세요: ");
	if (scanf_s("%d", &N) != 1 || N <= 0)
	{
		printf("1 이상의 정수를 입력하세요.");
		return 1;
	}

	int *arr = (int *)malloc(N * sizeof(int));
	if (arr == NULL)
	{
		printf("메모리 할당 실패\n");
		return 1;
	}

	srand((unsigned int)time(NULL));

	int *ptr = arr;
	for (int i = 0; i < N; i++)
	{
		*ptr = rand() % (N + 1);
		ptr++;
	}

	printf("생성된 배열 앞에서 부터 출력\n");
	ptr = arr;
	for (int i = 0; i < N; i++)
	{
		printf("%d ", *ptr);
		ptr++;
	}
	printf("\n");

	printf("생성된 배열 뒤에서 부터 출력\n");
	ptr = arr + N - 1;
	for (int i = N - 1; i >= 0; i--)
	{
		printf("%d ", *ptr);
		ptr--;
	}
	printf("\n");

	int sum = 0;
	int max = arr[0];
	int min = arr[0];

	ptr = arr;
	for (int i = 0; i < N; i++)
	{
		sum += *ptr;

		if (*ptr > max)
		{
			max = *ptr;
		}
		if (*ptr < min)
		{
			min = *ptr;
		}
		ptr++;
	}
	printf("총합: %d\n", sum);
	printf("최댓값: %d\n", max);
	printf("최솟값: %d\n", min);

	free(arr);

	return 0;
}

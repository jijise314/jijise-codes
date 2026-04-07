#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main() {

	int N;

	printf("마방진의 크기(홀수N)를 입력하세요: ");
	if (scanf("%d", &N) != 1 || N % 2 == 0 || N <= 0)
	{
		printf("홀수를 입력하세요.");
		return 1;
	}

	int** square = (int **)malloc(N * sizeof(int *));
	for (int i = 0; i < N; i++)
	{
		square[i] = (int*)malloc(N * sizeof(int));
		for (int j = 0; j < N; j++)
		{
			square[i][j] = 0;
		}
	}

	int row = 0;
	int col = N / 2;

	for (int i = 1; i <= N * N; i++)
	{
		square[row][col] = i;

		int next_row = (row - 1 + N) % N;
		int next_col = (col + 1) % N;

		if (square[next_row][next_col] != 0)
		{
			row = (row + 1) % N;
		}
		else
		{
			row = next_row;
			col = next_col;
		}
	}

	printf("\n== %d * %d 마방진==\n", N, N);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%4d|", square[i][j]);
		}
		printf("\n");
	}
	
	int total_sets = 0;

	printf("--- 가로/세로/대각선 합 ---\n");

	for (int i = 0; i < N; i++)
	{
		int sum = 0;
		for (int j = 0; j < N; j++)
		{
			sum += square[i][j];
		}
		printf("가로 %d행 합계: %d\n", i + 1, sum);
		total_sets++;
	}
	printf("\n");

	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int i = 0; i < N; i++)
		{
			sum += square[i][j];
		}
		printf("세로 %d열 합계: %d\n", j + 1, sum);
		total_sets++;
	}
	printf("\n");

	int diagnol_sum1 = 0;
	int diagnol_sum2 = 0;
	for (int i = 0; i < N; i++)
	{
		diagnol_sum1 += square[i][i];
		diagnol_sum2 += square[i][N - 1 - i];
	}

	printf("좌측대각선 합: %d\n", diagnol_sum1);
	total_sets++;
	printf("우측대각선 합: %d\n", diagnol_sum2);
	total_sets++;

	printf("세로 %d개, 가로 %d개, 대각선 2개로 총 세트는 %d개 입니다.", N, N, total_sets);
	
	free(square);

	return 0;
}
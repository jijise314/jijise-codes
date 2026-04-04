#include <stdio.h>
#include <stdlib.h>

#define N 5

int main()
{
    int arr[N][N] = { 0 };

    int row = 0;
    int col = N / 2;

    for (int i = 1; i <= N * N; i++)
    {
        arr[row][col] = i;

        int next_row = (row - 1 + N) % N;
        int next_col = (col + 1) % N;

        if (arr[next_row][next_col] != 0)
        {
            row = (row + 1) % N;
        }
        else
        {
            row = next_row;
            col = next_col;
        }
    }

    printf("--마방진 출력--\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("|%2d", arr[i][j]);
        }
        printf("|\n");
    }
    printf("\n");

    int total_sets = 0; // 출력된 총 세트 수를 세기 위한 변수

    printf("--- 가로/세로/대각선 합---\n");

    // 1. 가로 세트 합 출력 (N개 세트)
    for (int i = 0; i < N; i++)
    {
        int sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum += arr[i][j];
        }
        printf("가로 %d행 합계: %d\n", i + 1, sum);
        total_sets++;
    }
    printf("\n");

    // 2. 세로 세트 합 출력 (N개 세트)
    for (int j = 0; j < N; j++)
    {
        int sum = 0;
        for (int i = 0; i < N; i++)
        {
            sum += arr[i][j];
        }
        printf("세로 %d열 합계: %d\n", j + 1, sum);
        total_sets++;
    }
    printf("\n");

    // 3. 대각선 세트 합 출력 (2개 세트)
    int diagnol_sum1 = 0; 
    int diagnol_sum2 = 0;
    for (int i = 0; i < N; i++)
    {
        diagnol_sum1 += arr[i][i];           // 좌측 대각선
        diagnol_sum2 += arr[i][N - 1 - i];   // 우측 대각선
    }

    printf("좌측대각선 합: %d\n", diagnol_sum1);
    total_sets++;
    printf("우측대각선 합: %d\n", diagnol_sum2);
    total_sets++;

    printf("\n");
    printf("세로 %d개, 가로 %d개, 대각선 2개로 총 세트는 %d개 입니다.", N, N, total_sets);
}
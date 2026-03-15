#include <stdio.h>

int main()
{
	float F;
	int s;

	printf("실수 입력 : ");
	scanf_s("%f", &F);

	s = (F * 256);

	for (int i = 15; i >= 0; i--)
	{
		printf("%d", (s >> i) & 1);
		if (i == 8) printf(".");
	}
}
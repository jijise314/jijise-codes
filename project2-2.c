/*
#include <stdio.h>

int main()
{
	int i;
	char str[50];

	printf("문자열을 입력 : ");
	gets(str);

	printf("입력된 문자열 : ");
	for (i = 0; str[i]; i++)
	{
		printf("%c", str[i]);
	}
	printf("\n");
	printf("문자열 변경 후 출력 : ");

	for (i = 0; str[i]; i++)
	{
		char c = str[i];

		if (c >= 'A' && c <= 'Z')
		{
			printf("%c", c + 32);
		}
		else if (c >= 'a' && c <= 'z')
		{
			printf("%c", c - 32);
		}
		else
		{
			printf("%c", c);
		}
	}

	return 0;
}
*/
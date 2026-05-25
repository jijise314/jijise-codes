#include "calculator.h"
#include "ArrayStack.h"

int infixToPostfix(char* infix, char* postfix) {
	int j = 0;
	ArrayStack* opStack = createArrayStack(100, CHARACTER);
	stackElement item;

	for (int i = 0; infix[i] != NULL; i++) {
		if (infix[i] >= '0' && infix[i] <= '9') {
			postfix[j] = infix[i];
			j++;

			postfix[j] = NULL;
			printf("- Postfix: %s | Stack: ", postfix);
			printStack(opStack);
			printf("\n");
		}
		else if (infix[i] == '+' || infix[i] == '-' ||
			infix[i] == '*' || infix[i] == '/') {

			while (!emptyArrayStack(opStack)) {
				stackElement prevOp = peekArrayStack(opStack);

				if (precedence(infix[i]) <= precedence(prevOp.operator)) {
					postfix[j] = popArrayStack(opStack).operator;
					j++;
				}
				else {
					break;
				}
			}

			item.operator=infix[i];
			pushArrayStack(opStack, item);


			postfix[j] = NULL;
			printf("- Postfix: %s | Stack: ", postfix);
			printStack(opStack);
			printf("\n");
		}
		else if (infix[i] == '(') {
			item.operator=infix[i];
			pushArrayStack(opStack, item);


			postfix[j] = NULL;
			printf("- Postfix: %s | Stack: ", postfix);
			printStack(opStack);
			printf("\n");
		}
		else if (infix[i] == ')') {
			while(!emptyArrayStack(opStack)) {
				stackElement prevOp = popArrayStack(opStack);

				if (prevOp.operator != '(') {
					postfix[j] = prevOp.operator;
					j++;
				}
				else {
					break;
				}
			}
			postfix[j] = NULL;
			printf("- Postfix: %s | Stack: ", postfix);
			printStack(opStack);
			printf("\n");			
		}
	}

	while (!emptyArrayStack(opStack)) {
		postfix[j] = popArrayStack(opStack).operator;
		j++;
	}

	postfix[j] = NULL;
	printf("- Postfix: %s | Stack: ", postfix);
	printStack(opStack);
	printf("\n\n- Result: %s\n\n", postfix);

	free(opStack->data);
	free(opStack);

	return 0;
}

int evalPostfix(char* postfix) {
	ArrayStack* valueStack = createArrayStack(100, INTEGER);
	int i = 0;
	stackElement item, res;

	while (postfix[i] != NULL) {
		if (postfix[i] >= '0' && postfix[i] <= '9') {
			item.value = postfix[i] - '0';
			pushArrayStack(valueStack, item);
		}
		else if (postfix[i] == '*' || postfix[i] == '/' ||
			postfix[i] == '+' || postfix[i] == '-') {
			int i2 = popArrayStack(valueStack).value;
			int i1 = popArrayStack(valueStack).value;

			if (postfix[i] == '*') res.value = i1 * i2;
			else if (postfix[i] == '/') res.value = i1 / i2;
			else if (postfix[i] == '+') res.value = i1 + i2;
			else if (postfix[i] == '-') res.value = i1 - i2;

			pushArrayStack(valueStack, res);
		}

		printf("- Current: %c | Stack: ", postfix[i]);
		printStack(valueStack);
		printf("\n");

		i++;
	}
	int result = popArrayStack(valueStack).value;
	printf("\n- Result: %d\n", result);

	free(valueStack->data);
	free(valueStack);

	return result;
}

int precedence(char op) {
	switch (op)
	{
	case ')':return 3;
	case '*':
	case '/':return 2;
	case '+':
	case '-':return 1;
	case '(':return 0;
	default:return 0;
	}
}
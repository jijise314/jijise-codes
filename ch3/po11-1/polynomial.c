#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "polynomial.h"

polynomial* createPolynomial(int capacity) {
	polynomial* po = (polynomial*)createArrayList(capacity);
	return po;
}

polynomial* addItemPolynomial(polynomial* po, elementType item) {
	int i;
	for (i = 0; i < sizeArrayList(po); i++) {
		if (po->data[i].expo <= item.expo) {
			break;
		}
	}

	if (i == sizeArrayList(po)) {
		insertArrayList(po, i, item);
	}
	else if (po->data[i].expo == item.expo) {
		po->data[i].coef += item.coef;
	}
	else {
		insertArrayList(po, i, item);
	}

	return po;
}

void parsePolynomial(polynomial* po, const char* str) {
	int coef, expo;
	const char* ptr = str;
	int offset;

	while (*ptr != '\0') {
		while (sscanf(ptr, "%dX^%d%n", &coef, &expo, &offset) == 2) {
			addItemPolynomial(po, (elementType) { coef, expo });
			ptr += offset;

			if (*ptr == '+') {
				ptr++;
			}
			else if (*ptr == '\0') {
				break;
			}
		}
	}
}

polynomial* addPolynomial(polynomial* p1, polynomial* p2) {
	polynomial* r = createPolynomial(100);

	int i = 0, j = 0;

	while (i < sizeArrayList(p1) && j < sizeArrayList(p2)) {
		if (p1->data[i].expo < p2->data[j].expo) {
			addItemPolynomial(r, p2->data[j]);
			j++;
		}
		else if (p1->data[i].expo > p2->data[j].expo) {
			addItemPolynomial(r, p1->data[i]);
			i++;
		}
		else {
			addItemPolynomial(r, (elementType) {
				p1->data[i].coef + p2->data[j].coef,
					p1->data[i].expo
			});

			i++;
			j++;
		}
	}
	
	while (i < sizeArrayList(p1)) {
		addItemPolynomial(r, p1->data[i]);
		i++;
	}

	while (j < sizeArrayList(p2)) {
		addItemPolynomial(r, p2->data[j]);
		j++;
	}

	return r;
}

polynomial* multiplyPolynomial(polynomial* p1, polynomial* p2) {
	polynomial* r = createPolynomial(100);

	for (int i = 0; i < sizeArrayList(p1); i++) {
		for (int j = 0; j < sizeArrayList(p2); j++) {
			elementType temp;
			temp.coef = p1->data[i].coef * p2->data[j].coef;
			temp.expo = p1->data[i].expo + p2->data[j].expo;

			addItemPolynomial(r, temp);
		}
	}

	return r;
}

void printPolynomial(polynomial* po) {
	//printf("Polynomial: ");
	printArrayList(po);
	printf("\n");
}
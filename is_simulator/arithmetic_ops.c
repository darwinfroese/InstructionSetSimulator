#include "operations.h"
#include "registers.h"

void StoreAnswer(unsigned char answer, unsigned char dest) {
	switch(dest) {
	case 0:
		currentRegisters->registerZero = answer;
		break;
	case 1:
		currentRegisters->registerOne = answer;
		break;
	case 2:
		currentRegisters->registerTwo = answer;
		break;
	case 3:
		currentRegisters->registerThree = answer;
		break;
	case 4:
		currentRegisters->registerFour = answer;
		break;
	}
}

void Add(void *data) {
	ThreeArgs_s *args = (ThreeArgs_s *)data;
	unsigned char sum;

	sum = args->argTwo + args->argThree;
	StoreAnswer(sum, args->argOne);
}

void Subtract(void *data) {
	ThreeArgs_s *args = (ThreeArgs_s *)data;
	unsigned char difference;

	difference = args->argTwo - args->argThree;
	StoreAnswer(difference, args->argOne);
}

void Multiply(void *data) {
	ThreeArgs_s *args = (ThreeArgs_s *)data;
	unsigned char product;

	product = args->argTwo * args->argThree;
	StoreAnswer(product, args->argOne);
}

// Check for divide by 0 and error accordingly
void Divide(void *data) {
	ThreeArgs_s *args = (ThreeArgs_s *)data;
	unsigned char quotient;

	quotient = args->argTwo / args->argThree;
	StoreAnswer(quotient, args->argOne);
}
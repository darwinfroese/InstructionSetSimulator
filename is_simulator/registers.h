#ifndef __REGISTERS_H_
#define __REGISTERS_H_

typedef struct {
	unsigned char registerZero;
	unsigned char registerOne;
	unsigned char registerTwo;
	unsigned char registerThree;
	unsigned char registerFour;

	unsigned char programCounter;
}RegisterState;

extern RegisterState *currentRegisters;

#endif
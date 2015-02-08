#ifndef __OPERATIONS_H_
#define __OPERATIONS_H_

typedef struct {
	unsigned char argOne;
	unsigned char argTwo;
	unsigned char argThree;
}ThreeArgs_s;

typedef struct {
	unsigned char argOne;
	unsigned char argTwo;
}TwoArgs_s;

typedef struct { 
	unsigned char argOne;
}OneArg_s;

void Jump(void *);
void Not(void *);
void Add(void *);
void Subtract(void *);
void Multiply(void *);
void Divide(void *);
void Or(void *);
void And(void *);
void Xor(void *);
void PrintRegisters(void *);
void NotImplemented(void *);

#endif
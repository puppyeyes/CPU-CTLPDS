/*
 * abpds.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include <stdio.h>
#include "stdlib.h"
#ifndef ABPDS_H_
#define ABPDS_H_


struct FromConfig{
	short int controlLocation;
	int stack;
};

typedef struct ToConfig{
	short int controlLocation;
	int stack1;
	int stack2;
}Config;

struct TransitionRule{
	short int to_config_size;
	FromConfig from;
	ToConfig *to;
	TransitionRule *next;

};

typedef struct ABPDS{
	int *finalState;
	int finalStateSize;
}ABPDS;

typedef struct RuleHead{
	TransitionRule *next;
}*Delta;

struct ABPDSInfo{
	int state_size;
	int stack_size;
	int finalStateSize;
};


extern ABPDSInfo *abpds_info;

extern Delta delta;

extern int *finalStateArray;

void initTMP();

void deleteTMP();

void initABPDSInfo();

void initDelta(int delta_size);

void addRuleToDelta(TransitionRule *t);

__device__ void printRule(TransitionRule *rule);



#endif /* ABPDS_H_ */

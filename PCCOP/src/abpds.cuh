/*
 * abpds.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */

#ifndef ABPDS_H_
#define ABPDS_H_


typedef struct FromConfig{
	short int controlLocation;
	int stack;
}FromConfig;

typedef struct ToConfig{
	short int controlLocation;
	int stack1;
	int stack2;
}Config;

struct TransitionRule{
	bool tag;
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
};

extern ABPDSInfo *abpds_info;

extern Delta delta;

void initABPDSInfo();

void initDelta(int delta_size,Delta delta);

void addRuleToDelta(TransitionRule t,Delta delta);



#endif /* ABPDS_H_ */

/*
 * abpds.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */

#ifndef ABPDS_H_
#define ABPDS_H_


struct FromConfig{
	short int controlLocation;
	int stack;
};

struct ToConfig{
	short int controlLocation;
	int stack1;
	int stack2;
};

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
};

extern ABPDSInfo *abpds_info;

extern Delta delta;

extern int *finalStateArray;

void initABPDSInfo();

void initDelta(int delta_size);

void addRuleToDelta(TransitionRule *t);



#endif /* ABPDS_H_ */

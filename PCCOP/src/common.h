/*
 * common.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */

#ifndef COMMON_H_
#define COMMON_H_

struct ABPDSInfo{
	int state_size;
	int stack_size;
};

struct InitConfig{
	int initState;
	int *stack;
};

struct Trans{
	int fromState;
	int stack;
	int toState;
};


#endif /* COMMON_H_ */

/*
 * common.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include <cuda_util.h>
#include <stdio.h>
#include "stdlib.h"

#ifndef COMMON_H_
#define COMMON_H_

struct InitConfig{
	int initState;
	int *stack;
};

typedef struct Trans{
	int fromState;
	int stack;
	int toState;
}*TransQueue;

struct Gqueue{
	unsigned short head;
	unsigned short tail;
	TransQueue queue;
};

void initGQueue(int queue_size);
#endif /* COMMON_H_ */

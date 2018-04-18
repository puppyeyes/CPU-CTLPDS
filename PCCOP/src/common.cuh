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
	unsigned short int fromState;
	int stack;
	unsigned short int toState;
}*TransQueue;

struct Gqueue{
	unsigned short head;
	unsigned short tail;
	int mutex;
	TransQueue queue;
};

extern Gqueue *gqueue;

void initGQueue(int queue_size);
__device__  void get_Gqueue_Mutex(Gqueue *gqueue);
__device__  void free_Gqueue_Mutex(Gqueue *gqueue);
#endif /* COMMON_H_ */

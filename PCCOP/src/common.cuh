/*
 * common.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include <cuda_util.h>
#include <stdio.h>
#include "stdlib.h"
#include "utility.cuh"

#ifndef COMMON_H_
#define COMMON_H_

struct InitConfig{
	int initState;
	int *stack;
};

typedef struct Trans{
	unsigned short int fromState;
	int stack;
	short int toState;
}*TransQueue;

struct Gqueue{
	int head;
	int tail;
	short size;
	int mutex;
	TransQueue queue;
};

extern Gqueue *gqueue;

void initGQueue(int queue_size);

void add_one_to_queue(Trans t);
__device__ void d_add_one_to_queue(Trans t,Gqueue *gqueue);
__device__  void get_Gqueue_Mutex(Gqueue *gqueue);
__device__  void free_Gqueue_Mutex(Gqueue *gqueue);
__device__ __host__ void printTrans(Trans t);
__device__ __host__ void printGQueue(Gqueue *gqueue);


#endif /* COMMON_H_ */

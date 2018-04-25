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

#define STATEMASK 0x3ff
#define SPUPERSCRIPTMASK 0xffffffc00
#define STATEBIT 10

struct InitConfig{
	int initState;
	int *stack;
};

typedef struct Trans{
	short int fromState;
	int stack;
	short int toState;
}*TransQueue;

struct Gqueue{
	int head;
	int tail;
	short size;
	int mutex;
	unsigned long long int *queue;
};

extern Gqueue *gqueue;

void initGQueue(int queue_size);

__device__ __host__ unsigned long long int encode_trans_to_long(Trans t);
__device__ __host__ Trans decode_long_to_trans(unsigned long long int t);
void add_one_to_queue(Trans t);
__device__ void d_add_one_to_queue(Trans t,Gqueue *gqueue);
__device__  void get_Gqueue_Mutex(Gqueue *gqueue);
__device__  void free_Gqueue_Mutex(Gqueue *gqueue);
__device__ __host__ void printTrans(Trans t);
__device__ __host__ void printGQueue(Gqueue *gqueue);

__host__ __device__ short int decode_state_superScript(int state);
__device__ int encode_state_superScript(int state,short int recursion);

#endif /* COMMON_H_ */

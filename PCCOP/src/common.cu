#include "common.cuh"

#define FROMSTATEMASK 0xffff000000000000
#define STACKMASK 0x0000ffffffff0000
#define TOSTATEMASK 0x000000000000ffff
Gqueue *gqueue;


void initGQueue(int queue_size) {
	CUDA_SAFE_CALL(cudaMallocManaged(&(gqueue), sizeof(Gqueue)));
	gqueue->mutex = 0;
	gqueue->size = queue_size;
	CUDA_SAFE_CALL(
			cudaMallocManaged(&(gqueue->queue),
					sizeof(unsigned long long int) * queue_size));
}

void add_one_to_queue(Trans t) {
	int pos = gqueue->head;
	if (gqueue->head < gqueue->size) {
		gqueue->queue[pos]=encode_trans_to_long(t);
		gqueue->head++;

	} else {
		printf("queue full\n");
	}
}

__device__ __host__ unsigned long long int encode_trans_to_long(Trans t) {
	unsigned long long int res = 0;
	res=t.fromState;
	res=res<<32;
	res=res+t.stack;
	res=res<<16;
	res=res+(unsigned short)t.toState;
	return res;
}

__device__ __host__ Trans decode_long_to_trans(unsigned long long int t){
	Trans res;
	res.fromState=(short int)((t&FROMSTATEMASK)>>48);
	res.stack=(int)((t&STACKMASK)>>16);
	res.toState=(short int)(t&TOSTATEMASK);
	return res;
}

__device__ void d_add_one_to_queue(Trans t, Gqueue *gqueue) {
	int pos = atomicAdd(&(gqueue->head), 1);
	if (gqueue->head < gqueue->size) {
		gqueue->queue[pos] = encode_trans_to_long(t);
	} else {
		printf("one queue full\n");
	}
}

__device__ void get_Gqueue_Mutex(Gqueue *gqueue) {
	while (true) {
		if (atomicCAS(&(gqueue->mutex), 0, 1) == 0) {
			break;
		}
	}
}
__device__ void free_Gqueue_Mutex(Gqueue *gqueue) {
	atomicCAS(&(gqueue->mutex), 1, 0);
}

__device__ int encode_state_superScript(int state, short int recursion) {
	int res = (state&STATEMASK) | (recursion << STATEBIT);
	return res;
}

__host__ __device__ short int decode_state_superScript(int state) {
	int res = state >> STATEBIT;
	return res;
}

__device__ __host__ void printTrans(Trans t) {
	if(t.toState!=-1){
		int superScript = decode_state_superScript(t.toState);
		int toState = t.toState & STATEMASK;
		printf("%d %d --> %d[%d](%d)\n", t.fromState, t.stack, toState,
				superScript,t.toState);
	}else{
		printf("%d %d --> %d\n", t.fromState, t.stack, t.toState);
	}

}

__device__ __host__ void printGQueue(Gqueue *gqueue) {
	printf("打印queue\n");
	for (int i = 0; i < gqueue->head; i++) {
		Trans t=decode_long_to_trans(gqueue->queue[i]);
		printTrans(t);
	}
	printf("打印queue结束\n");
}

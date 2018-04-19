#include "common.cuh"

Gqueue *gqueue;

void initGQueue(int queue_size) {
	CUDA_SAFE_CALL(cudaMallocManaged(&(gqueue), sizeof(Gqueue)));
	gqueue->mutex = 0;
	gqueue->size = queue_size;
	CUDA_SAFE_CALL(
			cudaMallocManaged(&(gqueue->queue),
					sizeof(TransQueue) * queue_size));
}

void add_one_to_queue(Trans t) {
	int pos = gqueue->head;
	if (gqueue->head < gqueue->size) {
		gqueue->queue[pos] = t;
		pos++;
	} else {
		printf("queue full\n");
	}
}

__device__ void d_add_one_to_queue(Trans t, Gqueue *gqueue) {
	int pos = atomicAdd(&(gqueue->head), 1);
	if (gqueue->head < gqueue->size) {
		gqueue->queue[pos] = t;
	} else {
		printf("queue full\n");
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

__device__ __host__ void printTrans(Trans t){
	printf("%d %d --> %d\n",t.fromState,t.stack,t.toState);
}

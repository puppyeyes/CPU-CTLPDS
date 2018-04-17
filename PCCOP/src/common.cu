#include "common.cuh"

Gqueue *gqueue;
void initGQueue(int queue_size){
	CUDA_SAFE_CALL(cudaMallocManaged(&(gqueue), sizeof(Gqueue)));
	CUDA_SAFE_CALL(cudaMallocManaged(&(gqueue->queue), sizeof(TransQueue)*queue_size));
}

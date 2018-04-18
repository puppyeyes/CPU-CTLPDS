#include "common.cuh"

Gqueue *gqueue;

void initGQueue(int queue_size){
	CUDA_SAFE_CALL(cudaMallocManaged(&(gqueue), sizeof(Gqueue)));
	gqueue->mutex=0;
	CUDA_SAFE_CALL(cudaMallocManaged(&(gqueue->queue), sizeof(TransQueue)*queue_size));
}

__device__  void get_Gqueue_Mutex(Gqueue *gqueue){
	while(true){
		if(atomicCAS(&(gqueue->mutex),0,1)==0){
			break;
		}
	}
}
__device__  void free_Gqueue_Mutex(Gqueue *gqueue){
	atomicCAS(&(gqueue->mutex),1,0);
}

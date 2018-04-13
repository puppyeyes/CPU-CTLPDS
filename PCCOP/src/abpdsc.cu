#include "abpds.cuh"

Delta delta;

void initDelta(int delta_size,Delta delta){
	cudaMallocManaged(&delta, sizeof(int) *delta_size);
}

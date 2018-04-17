#include "abpdspre.cuh"
#include <cooperative_groups.h>

using namespace std;
using namespace cooperative_groups;
__global__ void compute_pre_on_pds(int*finish, Delta delta, AMA *ama1,
		AMA *ama2, int *finalStateArray, Gqueue *gqueue, ABPDSInfo abpds_info) {
	grid_group grid = this_grid();
	int thread_num = threadIdx.x;

	__shared__ Trans queue[32][64];
	__shared__ int count[32];
	__shared__ int start[32];
	__shared__ bool isDone;

	while (true) {
		if (thread_num == 0) {
			isDone = true;
		}






		__syncthreads();
		if ((gqueue->head - gqueue->tail == 0) && isDone) {
			atomicExch(finish, 1);
		}
		if(!isDone){
			start[0]=0;
			for(int i=1;i<32;i++)
			{
				count[i]=count[i-1]+start[0];
			}
		}
		__syncthreads();

		int thread_start_offset=start[thread_num];
		int thread_count=count[thread_num];

		for(int i=0;i<thread_count;i++){
			gqueue->queue[gqueue->head+thread_start_offset+i]=queue[thread_num][i];
		}

		if (*finish == 1) {
			grid.sync();
			if ((gqueue->head - gqueue->tail == 0)) {
				break;
			} else {
				atomicExch(finish, 0);
			}
		}

	}

}


#include "abpdspre.cuh"
#include <cooperative_groups.h>

using namespace std;
using namespace cooperative_groups;

//extern __shared__ Trans queue[32][64];

__device__ inline void compute(Delta delta, AMA *latest_ama, Pool *pool,
		Trans t, ABPDSInfo *abpds_info, int thread_num, Trans queue[32][64]) {
	TransitionRule *tmp_rule = delta[t.fromState * abpds_info->stack_size
			+ t.stack].next;
	int queue_count = 0;
	while (tmp_rule != NULL) {

		if (tmp_rule->to_config_size == 1) {
			if (tmp_rule->to->stack1 == 0) {
				//<p,r>--><p,*>跳过不做
				continue;
			} else if (tmp_rule->to->stack2 == 0) {
				Trans new_t = { tmp_rule->from.controlLocation,
						tmp_rule->from.stack, t.toState };
				//<p,r>--><p,r>
				if (!isTransInAMA(new_t, latest_ama,abpds_info)) {
					d_insertTransToAMA(new_t, latest_ama, pool,abpds_info);
					printTrans(new_t);
					queue[thread_num][queue_count] = new_t;
					queue_count++;
				}
			} else {
				//<p,r>--><p,r,r>
				int tmp_state = t.toState;
				AMANode *tmp_node =
						latest_ama->list[tmp_state * abpds_info->state_size
								+ tmp_rule->to->stack2].head.next;
				while (tmp_node != NULL) {
					Trans new_t = { tmp_rule->from.controlLocation,
							tmp_rule->from.stack, tmp_node->state };
					if (!isTransInAMA(new_t, latest_ama,abpds_info)) {
						d_insertTransToAMA(new_t, latest_ama, pool,abpds_info);
						printTrans(new_t);
						queue[thread_num][queue_count] = new_t;
						queue_count++;
					}
					tmp_node = tmp_node->next;
				}
			}
		} else {
			//<p,r>--><p,r>&<p,r>...
			int isSatify = true;
			for (int i = 0; i < tmp_rule->to_config_size; i++) {
				if (!latest_ama->list[tmp_rule->to[i].controlLocation
						* abpds_info->state_size + tmp_rule->to[i].stack1].head.next
						!= NULL) {
					isSatify = false;
					break;
				}
			}
			if (isSatify) {
				for (int i = 0; i < tmp_rule->to_config_size; i++) {
					AMANode *tmp_node =
							latest_ama->list[tmp_rule->to[i].controlLocation
									* abpds_info->state_size
									+ tmp_rule->to[i].stack1].head.next;
					while (tmp_node != NULL) {
						int new_to_state = tmp_node->state;
						Trans new_t = { tmp_rule->from.controlLocation,
								tmp_rule->from.stack, new_to_state };
						if (!isTransInAMA(new_t, latest_ama,abpds_info)) {
							d_insertTransToAMA(new_t, latest_ama, pool,abpds_info);
							printTrans(new_t);
							queue[thread_num][queue_count] = new_t;
							queue_count++;
						}
						tmp_node = tmp_node->next;
					}
				}
			}
		}

		tmp_rule = tmp_rule->next;
	}
}
;

__global__ void compute_pre_on_pds(int*finish, Delta delta, AMA *latest_ama,
		int *finalStateArray, Gqueue *gqueue, ABPDSInfo *abpds_info,
		Pool *pool) {
	grid_group grid = this_grid();
	int thread_num = threadIdx.x;

	__shared__ Trans queue[32][64];
	__shared__ int count[32];
	__shared__ int offset[32];
	__shared__ bool isDone;

	while (true) {
		if (thread_num == 0) {
			isDone = true;
			get_Gqueue_Mutex(gqueue);
		}
		int active_thread_num = gqueue->head - gqueue->tail;
		//初始化 offset[] count[]
		count[thread_num] = 0;
		offset[thread_num] = 0;
		if (thread_num < active_thread_num) {
			//取出一条边
			Trans t = gqueue->queue[gqueue->head-thread_num-1];
//			printf("取出");
//			printTrans(t);
			if (thread_num == 0) {
				atomicSub(&(gqueue->head), active_thread_num);
				free_Gqueue_Mutex(gqueue);
			}
			//计算
			compute(delta, latest_ama, pool, t, abpds_info, thread_num, queue);
		}

		//计算完成之后进行同步
		__syncthreads();
		if ((gqueue->head - gqueue->tail == 0) && isDone) {
			atomicExch(finish, 1);
		}
		if (!isDone) {
			offset[0] = 0;
			for (int i = 1; i < 32; i++) {
				count[i] = count[i - 1] + offset[0];
			}
		}
		__syncthreads();
		if (!isDone) {
			if(thread_num==0){
				get_Gqueue_Mutex(gqueue);
			}
			int thread_offset = offset[thread_num];
			int thread_count = count[thread_num];

			//将计算结果加入queue
			for (int i = 0; i < thread_count; i++) {
				gqueue->queue[gqueue->head + thread_offset + i] =
						queue[thread_num][i];
			}
			if(thread_num==0){
				atomicAdd(&(gqueue->head),count[31]+offset[31]);
				free_Gqueue_Mutex(gqueue);
						}
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

__global__ void compute_epsilon(Delta delta, AMA *ama, Pool *pool,
		ABPDSInfo *abpds_info, Gqueue *gqueue) {
	int thread_num = threadIdx.x + blockIdx.x * blockDim.x;
	if (thread_num < abpds_info->state_size) {
		TransitionRule *r_h = delta[thread_num * abpds_info->stack_size].next;
		while (r_h != NULL) {
			if (r_h->to_config_size == 1) {
				Trans new_t = { r_h->from.controlLocation, r_h->from.stack,
						r_h->to[0].controlLocation };
				d_add_one_to_queue(new_t, gqueue);
				if (!isTransInAMA(new_t, ama,abpds_info)) {
					d_insertTransToAMA(new_t, ama, pool,abpds_info);
					//printTrans(new_t);
				}
			}
			r_h = r_h->next;
		}
	}
}


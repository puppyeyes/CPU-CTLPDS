#include "abpdspre.cuh"
#include "common.cuh"
#include <cooperative_groups.h>

using namespace cooperative_groups;
#define QUEUEBASESIZE 5
#define DEFAULT_XML_FILE "abpds.xml"
#define ARGSNUM 7
#define THREADPERNUM 32
#define BLOCKSIZE 1
AMA *ama_1, *ama_2;
void add_initTrans_to_GQueue_AMA(AMA *ama, Pool *pool) {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 1; j < abpds_info->stack_size; j++) {
			Trans new_t = { finalStateArray[i], j, -1 };
			add_one_to_queue(new_t);
			if (!isTransInAMA(new_t, ama, abpds_info)) {
				insertTransToAMA(new_t, ama, pool);
			}
		}
	}
}

void add_Epsilon_to_queue(AMA *ama) {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 0; j < abpds_info->state_size; j++) {
			int pos = i * abpds_info->stack_size + j;
			AMANode *node = ama->list[pos].head.next;
			while (node != NULL) {
				Trans new_t = { i, j, node->state };
				add_one_to_queue(new_t);
				node = node->next;
			}
		}
	}
}

int main() {

	char * file_name = DEFAULT_XML_FILE;
	if (parse_abpds_xml(file_name) != 0) {
		printf("Failed to parse abpds\n ");
	} else {
		printf("parse abpds compelet\n");
	}

	print_parse_result();
	printStateMap();
	printStackMap();

	AMA *ama_1, *ama_2;
	Pool *pool_1, *pool_2;
	CUDA_SAFE_CALL(cudaMallocManaged(&ama_1, sizeof(AMA)));
	CUDA_SAFE_CALL(cudaMallocManaged(&pool_1, sizeof(Pool));)
	CUDA_SAFE_CALL(cudaMallocManaged(&ama_2, sizeof(AMA)));
	CUDA_SAFE_CALL(cudaMallocManaged(&pool_2, sizeof(Pool)));
	initGQueue(QUEUEBASESIZE * abpds_info->stack_size);

	initAMA(ama_1, pool_1);
	initAMA(ama_2, pool_2);

	short int *recursion;
	CUDA_SAFE_CALL(cudaMallocManaged(&recursion, sizeof(short int)));

	dim3 dimBlock(THREADPERNUM, 1, 1); //一个块中开threadsPerBlock个线程
	dim3 dimGrid(BLOCKSIZE, 1, 1); //一个gird里开blockSize个块

	int *finish;
	CUDA_SAFE_CALL(cudaMallocManaged(&finish, sizeof(int)));

	void **kernelArgs = NULL;
	kernelArgs = (void**) malloc(ARGSNUM * sizeof(*kernelArgs));

	kernelArgs[0] = malloc(sizeof(finish));
	memcpy(kernelArgs[0], &finish, sizeof(finish));

	kernelArgs[1] = malloc(sizeof(delta));
	memcpy(kernelArgs[1], &delta, sizeof(delta));

	kernelArgs[2] = malloc(sizeof(ama_1));
	memcpy(kernelArgs[2], &ama_1, sizeof(ama_1));

	kernelArgs[3] = malloc(sizeof(recursion));
	memcpy(kernelArgs[3], &recursion, sizeof(recursion));

	kernelArgs[4] = malloc(sizeof(gqueue));
	memcpy(kernelArgs[4], &gqueue, sizeof(gqueue));

	kernelArgs[5] = malloc(sizeof(abpds_info));
	memcpy(kernelArgs[5], &abpds_info, sizeof(abpds_info));

	kernelArgs[6] = malloc(sizeof(pool_1));
	memcpy(kernelArgs[6], &pool_1, sizeof(pool_1));
	int i = 0;
	//向queue中添加初始化数据
	add_initTrans_to_GQueue_AMA(ama_1, pool_1);
	i++;
	bool isEqual = false;
	int epsilion_thread_num = abpds_info->state_size / 32 + 1;
	compute_epsilon<<<epsilion_thread_num, 32>>>(delta, ama_2, pool_2, abpds_info,
			gqueue,recursion);
	cudaDeviceSynchronize();
	//printGQueue(gqueue);
	add_Epsilon_to_queue(ama_1);
	cudaLaunchCooperativeKernel((void*) compute_pre_on_pds, dimGrid, dimBlock,
			kernelArgs);
	cudaDeviceSynchronize();
	printAMA(ama_1);
	/*	while (!(i > 2 && isEqual)) {
	 //计算epsilon
	 int epsilion_thread_num = abpds_info->state_size / 32 + 1;
	 compute_epsilon<<<epsilion_thread_num, 32>>>(delta, ama_1, pool,
	 abpds_info, gqueue);
	 cudaDeviceSynchronize();
	 //插入p epsilon -->p
	 add_Epsilon_to_queue(ama_1);
	 //计算pre*
	 cudaLaunchCooperativeKernel((void*) compute_pre_on_pds, dimGrid,
	 dimBlock, kernelArgs);
	 cudaDeviceSynchronize();
	 //更新ama，对比ama
	 i++;
	 }*/

	return 0;
}

#include "abpdspre.cuh"
#include "common.cuh"
#include <cooperative_groups.h>

using namespace cooperative_groups;
#define QUEUEBASESIZE 5
#define DEFAULT_XML_FILE "/home/chuancy/git/GPU-CTLPDS/xmlAPDSGenerate/abpds.xml"
#define ARGSNUM 10
#define THREADPERNUM 32
#define BLOCKSIZE 1
AMA *ama_1, *ama_2;
void add_initTrans_to_GQueue_AMA(AMA *ama, Pool *pool) {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 0; j < abpds_info->stack_size; j++) {
			Trans new_t = { finalStateArray[i], j, -1 };
			//add_one_to_queue(new_t);
			insertTransToAMA(new_t, ama, pool);
		}
	}
}

bool isReach(AMA *ama, Config init_config) {
	int pos1 = init_config.controlLocation * abpds_info->stack_size
			+ init_config.stack1;
	AMANode *node = ama->list[pos1].head.next;
	if (init_config.stack2 == 0 && node != NULL) {
		return true;
	}
	while (node != NULL) {
		if (node->state == -1) {
			return true;
		}
		node->state = node->state & STATEMASK;
		int pos2 = node->state * abpds_info->stack_size + init_config.stack2;
		AMANode *node2 = ama->list[pos2].head.next;
		if (node2 != NULL) {
			return true;
		}
		node = node->next;
	}
	return false;
}

void add_Epsilon_to_queue(AMA *ama) {
	add_to_TMP(ama);
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 0; j < abpds_info->stack_size; j++) {
			int pos = i * abpds_info->stack_size + j;
			AMANode *node = ama->list[pos].head.next;
			while (node != NULL) {
				Trans new_t = { finalStateArray[i], j, node->state };
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
	//print_parse_result();
	//printStateMap();
	//printStackMap();
	//添加初始格局
	map<string, int>::iterator it_find;
	string init_state, init_stack1, init_stack2;
	int init_state_id, init_stack1_id, init_stack2_id;
	init_state = "p0";
	init_stack1 = "r0";
	init_stack2 = "r3";
	it_find = state_mp.find(init_state);
	if (it_find != state_mp.end()) {
		init_state_id = it_find->second;
	}
	it_find = stack_mp.find(init_stack1);
	if (it_find != stack_mp.end()) {
		init_stack1_id = it_find->second;
	}
	it_find = stack_mp.find(init_stack2);
	if (it_find != stack_mp.end()) {
		init_stack2_id = it_find->second;
	}
	Config init_config = { init_state_id, init_stack1_id, init_stack2_id };

	AMA *ama_1, *ama_2;
	Pool *pool_1, *pool_2;
	CUDA_SAFE_CALL(cudaMallocManaged(&ama_1, sizeof(AMA)));
	CUDA_SAFE_CALL(cudaMallocManaged(&pool_1, sizeof(Pool));)
	CUDA_SAFE_CALL(cudaMallocManaged(&ama_2, sizeof(AMA)));
	CUDA_SAFE_CALL(cudaMallocManaged(&pool_2, sizeof(Pool)));
	initGQueue(abpds_info->rule_size);

	initAMA(ama_1, pool_1);
	initAMA(ama_2, pool_2);
	initTMP();

	short int *recursion;
	CUDA_SAFE_CALL(cudaMallocManaged(&recursion, sizeof(short int)));

	dim3 dimBlock(THREADPERNUM, 1, 1); //一个块中开threadsPerBlock个线程
	dim3 dimGrid(BLOCKSIZE, 1, 1); //一个gird里开blockSize个块

	int *finish;
	CUDA_SAFE_CALL(cudaMallocManaged(&finish, sizeof(int)));

	void **kernelArgs_1 = NULL;
	kernelArgs_1 = (void**) malloc(ARGSNUM * sizeof(*kernelArgs_1));

	kernelArgs_1[0] = malloc(sizeof(finish));
	memcpy(kernelArgs_1[0], &finish, sizeof(finish));

	kernelArgs_1[1] = malloc(sizeof(delta));
	memcpy(kernelArgs_1[1], &delta, sizeof(delta));

	kernelArgs_1[2] = malloc(sizeof(ama_1));
	memcpy(kernelArgs_1[2], &ama_1, sizeof(ama_1));

	kernelArgs_1[3] = malloc(sizeof(pool_1));
	memcpy(kernelArgs_1[3], &pool_1, sizeof(pool_1));

	kernelArgs_1[4] = malloc(sizeof(ama_2));
	memcpy(kernelArgs_1[4], &ama_2, sizeof(ama_2));

	kernelArgs_1[5] = malloc(sizeof(pool_2));
	memcpy(kernelArgs_1[5], &pool_2, sizeof(pool_2));

	kernelArgs_1[6] = malloc(sizeof(recursion));
	memcpy(kernelArgs_1[6], &recursion, sizeof(recursion));

	kernelArgs_1[7] = malloc(sizeof(gqueue));
	memcpy(kernelArgs_1[7], &gqueue, sizeof(gqueue));

	kernelArgs_1[8] = malloc(sizeof(abpds_info));
	memcpy(kernelArgs_1[8], &abpds_info, sizeof(abpds_info));

	kernelArgs_1[9] = malloc(sizeof(tmp_ama));
	memcpy(kernelArgs_1[9], &tmp_ama, sizeof(tmp_ama));

	/*偶数次计算*/

	void **kernelArgs_2 = NULL;
	kernelArgs_2 = (void**) malloc(ARGSNUM * sizeof(*kernelArgs_2));

	kernelArgs_2[0] = malloc(sizeof(finish));
	memcpy(kernelArgs_2[0], &finish, sizeof(finish));

	kernelArgs_2[1] = malloc(sizeof(delta));
	memcpy(kernelArgs_2[1], &delta, sizeof(delta));

	kernelArgs_2[2] = malloc(sizeof(ama_2));
	memcpy(kernelArgs_2[2], &ama_2, sizeof(ama_2));

	kernelArgs_2[3] = malloc(sizeof(pool_2));
	memcpy(kernelArgs_2[3], &pool_2, sizeof(pool_2));

	kernelArgs_2[4] = malloc(sizeof(ama_1));
	memcpy(kernelArgs_2[4], &ama_1, sizeof(ama_1));

	kernelArgs_2[5] = malloc(sizeof(pool_1));
	memcpy(kernelArgs_2[5], &pool_1, sizeof(pool_1));

	kernelArgs_2[6] = malloc(sizeof(recursion));
	memcpy(kernelArgs_2[6], &recursion, sizeof(recursion));

	kernelArgs_2[7] = malloc(sizeof(gqueue));
	memcpy(kernelArgs_2[7], &gqueue, sizeof(gqueue));

	kernelArgs_2[8] = malloc(sizeof(abpds_info));
	memcpy(kernelArgs_2[8], &abpds_info, sizeof(abpds_info));

	kernelArgs_2[9] = malloc(sizeof(tmp_ama));
	memcpy(kernelArgs_2[9], &tmp_ama, sizeof(tmp_ama));

	float elapsedTime = 0.0;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	*recursion = 0;
	//向queue中添加初始化数据
	add_initTrans_to_GQueue_AMA(ama_1, pool_1);
	(*recursion)++;
	int epsilion_thread_num = abpds_info->state_size / 32 + 1;
//	int update_block_num=abpds_info->state_size;
	int update_thread_num = abpds_info->stack_size * abpds_info->state_size;
	while (true) {
		if ((*recursion) % 2 == 0) {
			printf("%d:\n", (*recursion));
			compute_epsilon<<<epsilion_thread_num, 32>>>(delta, ama_1, pool_1,
					abpds_info, gqueue, recursion);
			cudaDeviceSynchronize();

			add_Epsilon_to_queue(ama_2);
			//printGQueue(gqueue);
			cudaLaunchCooperativeKernel((void*) compute_pre_on_pds, dimGrid,
					dimBlock, kernelArgs_2);
			cudaDeviceSynchronize();
			cudaEventRecord(stop, 0);
			cudaEventSynchronize(stop);

			cudaEventElapsedTime(&elapsedTime, start, stop);

			cout << "calculate Time :" << elapsedTime << "ms" << endl;
			ama_1->count = 0;
			updateAMA<<<1, update_thread_num>>>(ama_1, *recursion, pool_1,
					abpds_info);
			cudaDeviceSynchronize();
			cudaEventRecord(stop, 0);
			cudaEventSynchronize(stop);

			cudaEventElapsedTime(&elapsedTime, start, stop);

			cout << "update Time :" << elapsedTime << "ms" << endl;
			printAMA(ama_1);
		} else {
			printf("%d:\n", (*recursion));
			compute_epsilon<<<epsilion_thread_num, 32>>>(delta, ama_2, pool_2,
					abpds_info, gqueue, recursion);
			cudaDeviceSynchronize();

			add_Epsilon_to_queue(ama_1);
			//printGQueue(gqueue);
			cudaLaunchCooperativeKernel((void*) compute_pre_on_pds, dimGrid,
					dimBlock, kernelArgs_1);
			cudaDeviceSynchronize();
			ama_2->count = 0;
			cudaEventRecord(stop, 0);
			cudaEventSynchronize(stop);

			cudaEventElapsedTime(&elapsedTime, start, stop);

			cout << "calculate Time :" << elapsedTime << "ms" << endl;
			updateAMA<<<1, update_thread_num>>>(ama_2, *recursion, pool_2,
					abpds_info);
			cudaDeviceSynchronize();
			cudaEventRecord(stop, 0);
			cudaEventSynchronize(stop);

			cudaEventElapsedTime(&elapsedTime, start, stop);

			cout << "update Time :" << elapsedTime << "ms" << endl;
			printAMA(ama_2);
		}

		if ((*recursion) >= 3 && isEqual(ama_1, ama_2)) {
			break;
		}
		deleteTMP();
		if ((*recursion) % 2 == 0) {
			deleteAMA(ama_2, pool_2);
		} else {
			deleteAMA(ama_1, pool_1);
		}
		(*recursion)++;
	}
/*	if (isReach(ama_2, init_config)) {
		printf(
				"The ABPDS has an accepting run from the initial configuration\n");
	} else {
		printf(
				"The ABPDS has not an accepting run from the initial configuration\n");
	}*/

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	cudaEventElapsedTime(&elapsedTime, start, stop);

	cout << "Time :" << elapsedTime << "ms" << endl;
	return 0;
}

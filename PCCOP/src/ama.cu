#include"ama.cuh"

TMPAMA tmp_ama;
#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)
static void CheckCudaErrorAux(const char *, unsigned, const char *,
		cudaError_t);
/**
 * Check the return value of the CUDA runtime API call and exit
 * the application if the call has failed.
 */
static void CheckCudaErrorAux(const char *file, unsigned line,
		const char *statement, cudaError_t err) {
	if (err == cudaSuccess)
		return;
	std::cerr << statement << " returned " << cudaGetErrorString(err) << "("
			<< err << ") at " << file << ":" << line << std::endl;
	exit(1);
}

void initTMP() {
	//存放临时的边，数组大小为abpds_info->state_size* abpds_info->stack_size
	CUDA_SAFE_CALL(
			cudaMallocManaged(&tmp_ama,
					sizeof(TMPINDEX) * abpds_info->state_size
							* abpds_info->stack_size));
}

void deleteTMP() {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 0; j < abpds_info->stack_size; j++) {
			tmp_ama[finalStateArray[i] * abpds_info->stack_size + j].next =
			NULL;
			tmp_ama[finalStateArray[i] * abpds_info->stack_size + j].nodeFlag =
					0;
		}
	}
}

void add_to_TMP(AMA *ama) {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 0; j < abpds_info->stack_size; j++) {
			tmp_ama[finalStateArray[i] * abpds_info->stack_size + j].nodeFlag =
					ama->list[finalStateArray[i] * abpds_info->stack_size + j].nodeFlag;
			tmp_ama[finalStateArray[i] * abpds_info->stack_size + j].next =
					ama->list[finalStateArray[i] * abpds_info->stack_size + j].next;
		}
	}
}

bool insertTransToAMA(Trans t, AMA *ama, Pool *pool) {
	//if (!isTransInAMA(t, ama, abpds_info)) {
	int insertPosition = t.fromState * abpds_info->stack_size + t.stack;
	if (!isExist(ama->list[insertPosition].nodeFlag, t.toState)) {
		//插入
		int pool_position = pool->tail++;
		if (pool_position > pool->size) {
			printf("pool exceed \n");
		}
		pool->item[pool_position].state = t.toState;
		pool->item[pool_position].next = NULL;
		pool->item[pool_position].next = ama->list[insertPosition].next;
		ama->list[insertPosition].next = &(pool->item[pool_position]);
		ama->list[insertPosition].nodeFlag |= (1
				<< ((t.toState & STATEMASK) + 1));
		return true;
	}
	return false;
}

__device__ __host__ bool isExist(unsigned long long int nodeFlag,
		short int state) {
	int res = 0;
	if (state == -1) {
		res = (nodeFlag) & 0x01;
	} else {
		res = (nodeFlag >> ((state & STATEMASK) + 1)) & 0x01;
	}
	if (res == 0) {
		return false;
	} else {
		return true;
	}
}

__device__ bool d_insertTransToAMA(Trans t, AMA *ama, Pool *pool,
		ABPDSInfo *abpds_info) {
	int insertPosition = t.fromState * abpds_info->stack_size + t.stack;
	bool next = true;
	unsigned long long int nodeFlag;
	while (next) {
		int v = atomicCAS(&(ama->list[insertPosition].mutex), 0, 1);
		if (v == 0) {
			//在此放置你的临界区
			if (!isExist(ama->list[insertPosition].nodeFlag, t.toState)) {
				if((nodeFlag) & 0x01){
					nodeFlag = ama->list[insertPosition].nodeFlag
												| (1 << ((t.toState & STATEMASK) + 1));
					atomicExch(&(ama->list[insertPosition].nodeFlag), nodeFlag);
					atomicExch(&(ama->list[insertPosition].mutex), 0);
					return false;
				}
				//插入
				int pool_position = atomicAdd(&(pool->tail), 1);
				if (pool_position > pool->size) {
					printf("pool exceed \n");
				}
				pool->item[pool_position].state = t.toState;
				pool->item[pool_position].next = NULL;
				pool->item[pool_position].next = ama->list[insertPosition].next;
				ama->list[insertPosition].next = &(pool->item[pool_position]);
				if (t.toState != -1) {
					nodeFlag = ama->list[insertPosition].nodeFlag
							| (1 << ((t.toState & STATEMASK) + 1));
				} else {
					nodeFlag = ama->list[insertPosition].nodeFlag | 1;
				}
				atomicExch(&(ama->list[insertPosition].nodeFlag), nodeFlag);
				atomicExch(&(ama->list[insertPosition].mutex), 0);
				return true;
			} else {
				atomicExch(&(ama->list[insertPosition].mutex),0);
				return false;
			}
			//临界区结束
		}  //此处是安全的汇聚点
	}  //此处是安全的汇聚点2
	return false;
}

void deleteAMA(AMA *ama, Pool *pool) {
	ama->count = 0;
	for (int i = 0; i < abpds_info->state_size * abpds_info->stack_size; i++) {
		//尾指针指向头结点
		ama->list[i].next = NULL;
		ama->list[i].mutex = 0;
		ama->list[i].nodeFlag = 0;
	}
	pool->tail = 0;

}
bool isEqual(AMA *ama_1, AMA *ama_2) {
	if (ama_1->count == ama_2->count) {
		return true;
	}
	return false;
}
bool isFinalState(int state) {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		if (state == finalStateArray[i]) {
			return true;
		}
	}
	return false;
}

__global__ void updateAMA(AMA *ama, int recursion, Pool *pool,
		ABPDSInfo *abpds_info) {
//需要一个数组存储每个AMAList的所有元素  这个数组空间在哪开？
	int amaListPosition = threadIdx.x + blockIdx.x * blockDim.x;
//int *tmpStateList;
	if (amaListPosition < abpds_info->stack_size * abpds_info->state_size) {
		AMANode *currentNode = ama->list[amaListPosition].next;
		if(ama->list[amaListPosition].nodeFlag&0x01){
			currentNode->state=-1;
			ama->list[amaListPosition].next = currentNode;
				currentNode->next = NULL;
		}
		while (currentNode != NULL) {
			//updateState
			if (currentNode->state != -1) {
				currentNode->state = encode_state_superScript(
						currentNode->state, recursion);
				atomicAdd(&(ama->count),1);
			}
			currentNode = currentNode->next;
		}
	}
}

void initAMA(AMA *ama, Pool *pool) {
	int amaSize = abpds_info->stack_size * abpds_info->state_size;
//	CUDA_CHECK_RETURN(cudaMallocManaged(&ama, sizeof(AMA)));
	CUDA_CHECK_RETURN(cudaMallocManaged(&ama->list, sizeof(AMAList) * amaSize));
//初始化ama.list
	for (int i = 0; i < amaSize; i++) {
		ama->list[i].mutex = 0;
		ama->list[i].nodeFlag = 0;
		//尾指针指向头结点
		ama->list[i].next = NULL;
	}
	ama->count = 0;
//	CUDA_CHECK_RETURN(cudaMallocManaged(&pool, sizeof(Pool)));
	CUDA_CHECK_RETURN(
			cudaMallocManaged (&pool->item, sizeof(AMANode) * AMAPOOLSIZE));
	pool->size = AMAPOOLSIZE;
	pool->tail = 0;
}

void printAMA(AMA *ama) {
	map<int, string>::iterator it_find;
	string from_state;
	string stack;
	string to_state;
	string super_script;
	int state_id;
	int stack_id;
//int count=0;
	cout << "打印结果" << endl;
	for (int i = 0; i < (abpds_info->state_size); i++) {
		for (int j = 0; j < (abpds_info->stack_size); j++) {
			AMANode *tem_node = ama->list[i * abpds_info->stack_size + j].next;
			bool flag = false;
			if (tem_node != NULL) {
				flag = true;
				it_find = rv_state_mp.find(i);
				if (it_find != rv_state_mp.end()) {
					from_state = it_find->second;
					state_id = i;
				}
				it_find = rv_stack_mp.find(j);
				if (it_find != rv_state_mp.end()) {
					stack = it_find->second;
					stack_id = j;
				}
				cout << from_state;
				//printf("(%d)", state_id);
				cout << " " << stack;
				//printf("(%d)", stack_id);
				cout << "-->{ ";
			}
			while (tem_node != NULL) {
				short int superScript = decode_state_superScript(
						tem_node->state);
				it_find = rv_state_mp.find(tem_node->state & STATEMASK);
				if (it_find != rv_state_mp.end()) {
					to_state = it_find->second;
					//count++;
				} else {
					if (tem_node->state == -1) {
						to_state = "Qf";
					}
				}
				cout << to_state;
				if (tem_node->state != -1) {
					printf("[%d]",
							(int) ((tem_node->state & SPUPERSCRIPTMASK)
									>> STATEBIT));
				}
				//cout << "("<<tem_node->state<<")"<<",";
				cout << ",";
				tem_node = tem_node->next;
			}
			if (flag) {
				cout << "}" << endl;
				flag = false;
			}
		}
	}
	cout << "结果输出结束 count: " << ama->count << endl;
}

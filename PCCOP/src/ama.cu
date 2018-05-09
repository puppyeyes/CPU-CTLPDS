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
		}
	}
}

void add_to_TMP(AMA *ama) {
	for (int i = 0; i < abpds_info->finalStateSize; i++) {
		for (int j = 0; j < abpds_info->stack_size; j++) {
			tmp_ama[finalStateArray[i] * abpds_info->stack_size + j].next =
					ama->list[finalStateArray[i] * abpds_info->stack_size + j].head.next;
		}
	}
}

bool insertTransToAMA(Trans t, AMA *ama, Pool *pool) {
	//if (!isTransInAMA(t, ama, abpds_info)) {
	int insertPosition = t.fromState * abpds_info->stack_size + t.stack;
	AMANode *currentNode = ama->list[insertPosition].head.next;
	if (currentNode == NULL) {
		//插入
		int pool_position = pool->tail++;
		if (pool_position > pool->size) {
			printf("pool exceed \n");
		}
		/*		if (t.toState != -1) {
		 ama->count++;
		 }*/
		pool->item[pool_position].state = t.toState;
		pool->item[pool_position].next = NULL;
		ama->list[insertPosition].tail->next = &(pool->item[pool_position]);
		ama->list[insertPosition].tail = &(pool->item[pool_position]);
		return true;
	}
	//head->3  插-1 head->-1->3
	if ((currentNode->state > t.toState) && currentNode != NULL) {
		//插入
		int pool_position = pool->tail++;
		if (pool_position > pool->size) {
			printf("pool exceed \n");
		}
		/*		if (t.toState != -1) {
		 ama->count++;
		 }*/
		pool->item[pool_position].state = t.toState;
		pool->item[pool_position].next = NULL;
		ama->list[insertPosition].head.next = &(pool->item[pool_position]);
		pool->item[pool_position].next = currentNode;
		return true;
	}
	while (currentNode != NULL) {
		if (currentNode->state < t.toState
				&& (currentNode->next == NULL
						|| currentNode->next->state > t.toState)) {
			//插入
			int pool_position = pool->tail++;
			if (pool_position > pool->size) {
				printf("pool exceed \n");
				return false;
			}
			/*			if (t.toState != -1) {
			 ama->count++;
			 }*/
			pool->item[pool_position].state = t.toState;
			pool->item[pool_position].next = NULL;
			AMANode *tmp = currentNode->next;
			currentNode->next = &(pool->item[pool_position]);
			pool->item[pool_position].next = tmp;
			return true;
		} else if (currentNode->state == t.toState) {
			return false;
		}
		currentNode = currentNode->next;
	}
	return false;
}

__device__ bool d_insertTransToAMA(Trans t, AMA *ama, Pool *pool,
		ABPDSInfo *abpds_info) {
	int insertPosition = t.fromState * abpds_info->stack_size + t.stack;
	//bool flag = false;
	bool next = true;

	while (next) {
		int v = atomicCAS(&(ama->list[insertPosition].mutex), 0, 1);
		if (v == 0) {
			//在此放置你的临界区
			AMANode *currentNode = ama->list[insertPosition].head.next;
			if (currentNode == NULL) {
				//插入
				int pool_position = atomicAdd(&pool->tail, 1);
				if (pool_position > pool->size) {
					printf("pool exceed \n");
				}
				/*				if (t.toState != -1) {
				 atomicAdd( &ama->count,1);
				 }*/
				pool->item[pool_position].state = t.toState;
				pool->item[pool_position].next = NULL;

				ama->list[insertPosition].tail->next =
						&(pool->item[pool_position]);
				ama->list[insertPosition].tail = &(pool->item[pool_position]);
				atomicExch(&(ama->list[insertPosition].mutex), 0);
				return true;
			}
			//head->3  插-1 head->-1->3
			if (currentNode != NULL && (currentNode->state > t.toState)) {
				//插入
				int pool_position = atomicAdd(&pool->tail, 1);
				if (pool_position > pool->size) {
					printf("pool exceed \n");
				}
				/*				if (t.toState != -1) {
				 atomicAdd( &ama->count,1);
				 }*/
				pool->item[pool_position].state = t.toState;
				pool->item[pool_position].next = NULL;
				ama->list[insertPosition].head.next =
						&(pool->item[pool_position]);
				pool->item[pool_position].next = currentNode;
				atomicExch(&(ama->list[insertPosition].mutex), 0);
				return true;
			}
			while (currentNode != NULL) {
				if (currentNode->state < t.toState
						&& (currentNode->next == NULL
								|| currentNode->next->state > t.toState)) {
					//插入
					int pool_position = atomicAdd(&pool->tail, 1);
					if (pool_position > pool->size) {
						printf("ama->list[i].headpool exceed \n");
						atomicExch(&(ama->list[insertPosition].mutex), 0);
						return false;
					}
					/*					if (t.toState != -1) {
					 atomicAdd( &ama->count,1);
					 }*/
					pool->item[pool_position].state = t.toState;
					pool->item[pool_position].next = NULL;
					AMANode *tmp = currentNode->next;
					currentNode->next = &(pool->item[pool_position]);
					pool->item[pool_position].next = tmp;
					atomicExch(&(ama->list[insertPosition].mutex), 0);
					return true;
				} else if (currentNode->state == t.toState) {
					atomicExch(&(ama->list[insertPosition].mutex), 0);
					return false;
				}
				currentNode = currentNode->next;
			}
			//临界区结束
			atomicExch(&(ama->list[insertPosition].mutex), 0);
			next = false;
		}  //此处是安全的汇聚点
	}  //此处是安全的汇聚点2
	return false;
}

__device__ bool d_insertStateToAMA(int insertPosition, int state, AMA *ama,
		Pool *pool) {
	AMANode *currentNode = ama->list[insertPosition].head.next;
//	bool flag = false;
	if (currentNode == NULL) {
		//插入
		int pool_position = atomicAdd(&pool->tail, 1);
		if (pool_position > pool->size) {
			printf("pool exceed \n");
		}
		if (state != -1) {
			atomicAdd(&ama->count, 1);
		}
		pool->item[pool_position].state = state;
		pool->item[pool_position].next = NULL;
		ama->list[insertPosition].tail->next = &(pool->item[pool_position]);
		ama->list[insertPosition].tail = &(pool->item[pool_position]);
		return true;
	}
	//head->3  插-1 head->-1->3
	if (currentNode != NULL && (currentNode->state > state)) {
		//插入
		int pool_position = atomicAdd(&pool->tail, 1);
		if (pool_position > pool->size) {
			printf("pool exceed \n");
		}
		if (state != -1) {
			atomicAdd(&ama->count, 1);
		}
		pool->item[pool_position].state = state;
		pool->item[pool_position].next = NULL;
		ama->list[insertPosition].head.next = &(pool->item[pool_position]);
		pool->item[pool_position].next = currentNode;
		return true;
	}
	while (currentNode != NULL) {
		if (currentNode->state < state
				&& (currentNode->next == NULL
						|| currentNode->next->state > state)) {
			//插入
			int pool_position = atomicAdd(&pool->tail, 1);
			if (pool_position > pool->size) {
				printf("pool exceed \n");
				return false;
			}
			if (state != -1) {
				atomicAdd(&ama->count, 1);
			}
			pool->item[pool_position].state = state;
			pool->item[pool_position].next = NULL;
			AMANode *tmp = currentNode->next;
			currentNode->next = &(pool->item[pool_position]);
			pool->item[pool_position].next = tmp;
			return true;
		} else if (currentNode->state == state) {
			//printf("--id %d-- %d  exist\n",insertPosition,currentNode->state);
			return false;
		}
		currentNode = currentNode->next;
	}
	return false;

//	int pool_position = pool->tail++;
//	if (pool_position > pool->size) {
//		printf("pool exceed \n");
//	}
//	pool->item[pool_position].state = state;
//	//尾插
//	ama->list[amaListPosition].tail->next = &(pooupdateAMAl->item[pool_position]);
//	ama->list[amaListPosition].tail = &(pool->item[pool_position]);

}
void deleteAMA(AMA *ama, Pool *pool) {
	ama->count = 0;
	for (int i = 0; i < abpds_info->state_size * abpds_info->stack_size; i++) {
		ama->list[i].head.next = NULL;
		//尾指针指向头结点
		ama->list[i].tail = &(ama->list[i].head);
		ama->list[i].mutex = 0;
		ama->list[i].count = 0;
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
__device__ __host__ bool isTransInAMA(Trans t, AMA *ama,
		ABPDSInfo *abpds_info) {
	int pos = t.fromState * abpds_info->stack_size + t.stack;
	AMANode *currentNode = ama->list[pos].head.next;
	while (currentNode != NULL) {
		if (currentNode->state == t.toState) {
			return true;
		}
		currentNode = currentNode->next;
	}
	return false;
}

__global__ void updateAMA(AMA *ama, int recursion, Pool *pool,
		ABPDSInfo *abpds_info) {
	//需要一个数组存储每个AMAList的所有元素  这个数组空间在哪开？
	int amaListPosition = threadIdx.x + blockIdx.x * blockDim.x;
	//int *tmpStateList;
	if (amaListPosition < abpds_info->stack_size * abpds_info->state_size
			&& (amaListPosition == 7 | 17)) {
		int tmpListPosition = 0;
		int *tmpStateList;
		tmpStateList = (int *) malloc(
				sizeof(int) * ama->list[amaListPosition].count);
//	cudaMallocManaged(&tmpStateList,
//			sizeof(int) * ama->list[amaListPosition].count);
		AMANode *currentNode = ama->list[amaListPosition].head.next;
		while (currentNode != NULL) {
			//updateState
			if (currentNode->state != -1) {
				tmpStateList[tmpListPosition++] = encode_state_superScript(
						currentNode->state, recursion);
			} else {
				tmpStateList[tmpListPosition++] = currentNode->state;
			}
			currentNode = currentNode->next;
		}
		//断链
		ama->list[amaListPosition].head.next = NULL;
		ama->list[amaListPosition].tail = &(ama->list[amaListPosition].head);
		ama->list[amaListPosition].count = 0;
		//重新插入
		for (int i = 0; i < tmpListPosition; i++) {
			int result = d_insertStateToAMA(amaListPosition, tmpStateList[i],
					ama, pool);
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
		ama->list[i].count = 0;
		//尾指针指向头结点
		ama->list[i].tail = &(ama->list[i].head);
	}
	ama->count = 0;
//	CUDA_CHECK_RETURN(cudaMallocManaged(&pool, sizeof(Pool)));
	CUDA_CHECK_RETURN(
			cudaMallocManaged (&pool->item, sizeof(AMANode) * AMAPOOLSIZE));
	pool->size = AMAPOOLSIZE;
	pool->tail = 0;
//finalStateArray 用-1表示
//		for(int i=0;i<abpds_info->finalStateSize; i++){
//			for(int j=0;j<abpds_info->stack_size;j++){
//				Trans t={finalStateArray[i],j,-1};
//				insertTransToAMA(t,ama,pool);
//			}
//		}
//cout<<isTransInAMA(t2,ama);
}

void printAMA(AMA *ama) {
	map<int, string>::iterator it_find;
	string from_state;
	string stack;
	string to_state;
	string super_script;
	int state_id;
	int stack_id;
	cout << "打印结果" << endl;
	for (int i = 0; i < (abpds_info->state_size); i++) {
		for (int j = 0; j < (abpds_info->stack_size); j++) {
			AMANode *tem_node =
					ama->list[i * abpds_info->stack_size + j].head.next;
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
	cout << "结果输出结束" << endl;
}

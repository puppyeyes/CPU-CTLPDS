#include"ama.cuh"
using namespace std;
#define STATEMASK 0x3ff
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
__device__ __host__ void insertTransToAMA(int amaListPosition, int state, AMA *ama, Pool *pool) {
	if (!isTransInAMA(t, ama, abpds_info)) {
		int pool_position = pool->tail++;
		if (pool_position > pool->size) {
			printf("pool exceed \n");
		}
		if (t.toState != -1) {
			ama->count++;
		}
		pool->item[pool_position].state = state;
		//尾插
		ama->list[amaListPosition].tail->next = &(pool->item[pool_position]);
		ama->list[amaListPosition].tail = &(pool->item[pool_position]);
		ama->list[amaListPosition].count++;
	}

}
__device__ void d_insertTransToAMA(int amaListPosition, int state, AMA *ama,
		Pool *pool, ABPDSInfo *abpds_info) {
	bool next = true;
	while (next) {
		int v = atomicCAS(&(ama->list[insertPosition].mutex), 0, 1);
		if (v == 0) {
			//在此放置你的临界区
			if (!isTransInAMA(t, ama, abpds_info)) {
				int pool_position = pool->tail++;
				if (pool_position > pool->size) {
					printf("pool exceed \n");
				}
				if (t.toState != -1) {
					ama->count++;
				}
				pool->item[pool_position].state = state;
				//尾插
				ama->list[amaListPosition].tail->next =
						&(pool->item[pool_position]);
				ama->list[amaListPosition].tail = &(pool->item[pool_position]);
				ama->list[amaListPosition].count++;

			}
			//临界区结束
			atomicExch(&(ama->list[amaListPosition].mutex), 0);
			next = false;
		}  //此处是安全的汇聚点
	}  //此处是安全的汇聚点2
}
void deleteAMA(AMA *ama, Pool *pool) {
	ama->count = 0;
	for (int i = 0; i < AMASIZE; i++) {
		ama->list[i].head.next = NULL;
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
	int *tmpStateList;
	int tmpListPosition = 0;
	cudaMallocManaged(&tmpStateList,
			sizeof(int) * ama->list[amaListPosition].count);
	AMANode *currentNode = ama->list[amaListPosition].head.next;
	while (currentNode != NULL) {
		//updateState
		tmpStateList[tmpListPosition++] = encode_state_superScript(
				currentNode->state, recursion);
		currentNode = currentNode->next;
	}
	//断链
	ama->list[amaListPosition].head.next=NULL;
	ama->list[amaListPosition].tail=ama->list[amaListPosition].head;
	//重新插入
	for (int i = 0; i < tmpListPosition; i++) {
		insertTransToAMA(amaListPosition,tmpStateList[i],ama,pool,abpds_info);
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
ama->count = amaSize;
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
cout << "打印结果" << endl;
for (int i = 0; i < (abpds_info->state_size); i++) {
	for (int j = 0; j < (abpds_info->stack_size); j++) {
		AMANode *tem_node = ama->list[i * abpds_info->stack_size + j].head.next;
		bool flag = false;
		if (tem_node != NULL) {
			flag = true;
			it_find = rv_state_mp.find(i);
			if (it_find != rv_state_mp.end()) {
				from_state = it_find->second;
			}
			it_find = rv_stack_mp.find(j);
			if (it_find != rv_state_mp.end()) {
				stack = it_find->second;
			}
			cout << from_state << " " << stack << "-->{ ";
		}
		while (tem_node != NULL) {
			short int superScript = decode_state_superScript(tem_node->state);
			it_find = rv_state_mp.find(tem_node->state & STATEMASK);
			if (it_find != rv_state_mp.end()) {
				to_state = it_find->second;
			} else {
				if (tem_node->state == -1) {
					to_state = "Qf";
				}
			}
			cout << to_state << ",";
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


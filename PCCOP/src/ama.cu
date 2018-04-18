#include"ama.cuh"
#include <stdio.h>
void insertTransToAMA(Trans t,AMA *ama,Pool *pool){
	int pool_position = pool->tail++;
	if (pool_position > pool->size){
			printf("pool exceed \n");
	}
	if(t.toState!=00000){
		ama->count++;
	}
	pool->item[pool_position].state= t.toState;
	int insertPosition=t.fromState*t.stack;
	//尾插
	ama->list[insertPosition].tail->next=&(pool->item[pool_position]);
	ama->list[insertPosition].tail=&(pool->item[pool_position]);
}
__device__ void d_insertTransToAMA(Trans t,AMA *ama,Pool *pool){
	int pool_position = atomicAdd(&(pool->tail), 1);
	if (pool_position > pool->size){
		printf("pool exceed \n");
	}
	if(t.toState!=00000){
		atomicAdd(&(ama->count), 1);
	}
	pool->item[pool_position].state= t.toState;
	int insertPosition=t.fromState*t.stack;
	bool next = true;
	while (next) {
		int v = atomicCAS(&(ama->list[insertPosition].mutex), 0, 1);
		if (v == 0) {
			//在此放置你的临界区
			//尾插
			ama->list[insertPosition].tail->next=&(pool->item[pool_position]);
			ama->list[insertPosition].tail=&(pool->item[pool_position]);
			//临界区结束
			atomicExch(&(ama->list[insertPosition].mutex), 0);
			next = false;
		}  //此处是安全的汇聚点
	}  //此处是安全的汇聚点2
}
void deleteAMA(AMA *ama,Pool *pool){
	ama->count=0;
	for (int i = 0; i < AMASIZE; i++) {
		ama->list[i].head.next=NULL;
	}
	pool->tail=0;
}
bool isEqual(AMA *ama_1,AMA *ama_2){
	if(ama_1->count==ama_2->count){
		return true;
	}
	return false;
}
__device__ __host__ bool isTransInAMA(Trans t,AMA *ama){

	AMANode *currentNode=ama->list[t.fromState*t.stack].head.next;
	while(currentNode!=NULL){
		if(currentNode->state==t.toState){
			return true;
		}
		currentNode=currentNode->next;
	}
	return false;
}

/*void initAMA(AMA *ama,Pool *pool){
		int amaSize=abpds_info->stack_size*abpds_info->state_size;
		CUDA_CHECK_RETURN(cudaMallocManaged(&ama, sizeof(AMA)));
		CUDA_CHECK_RETURN(cudaMallocManaged (&ama->list, sizeof(AMAList)*amaSize));
		//初始化ama.list
		for (int i = 0; i < amaSize; i++) {
			ama->list[i].mutex = 0;
			//尾指针指向头结点
			ama->list[i].tail=&(ama->list[i].head);
		}
		CUDA_CHECK_RETURN(cudaMallocManaged(&pool, sizeof(Pool)));
		CUDA_CHECK_RETURN(cudaMallocManaged (&pool->item, sizeof(AMANode) * AMAPOOLSIZE));
		pool->size=AMAPOOLSIZE;
		pool->tail=0;
		//finalStateArray 用-1表示
//		for(int i=0;i<abpds_info->finalStateSize; i++){
//			for(int j=0;j<abpds_info->stack_size;j++){
//				Trans t={finalStateArray[i],j,-1};
//				insertTransToAMA(t,ama,pool);
//			}
//		}
		//cout<<isTransInAMA(t2,ama);
}*/

/*

#include "ama.cuh"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
static void CheckCudaErrorAux(const char *, unsigned, const char *,
		cudaError_t);
#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)

  Check the return value of the CUDA runtime API call and exit
  the application if the call has failed.

static void CheckCudaErrorAux(const char *file, unsigned line,
		const char *statement, cudaError_t err) {
	if (err == cudaSuccess)
		return;
	std::cerr << statement << " returned " << cudaGetErrorString(err) << "("
			<< err << ") at " << file << ":" << line << std::endl;
	exit(1);
}


int main(void){
	AMA *ama;
	Pool *pool;
	CUDA_CHECK_RETURN(cudaMallocManaged(&ama, sizeof(AMA)));
	CUDA_CHECK_RETURN(cudaMallocManaged (&ama->list, sizeof(AMAList)*AMASIZE));


	//初始化ama.list
	for (int i = 0; i < AMASIZE; i++) {
		ama->list[i].mutex = 0;
//		CUDA_CHECK_RETURN(cudaMallocManaged (&ama->list[i].tail, sizeof(AMANode)));
		//尾指针指向头结点
		ama->list[i].tail=&(ama->list[i].head);
	}
	CUDA_CHECK_RETURN(cudaMallocManaged(&pool, sizeof(Pool)));
	CUDA_CHECK_RETURN(cudaMallocManaged (&pool->item, sizeof(AMANode) * AMAPOOLSIZE));
	pool->size=AMAPOOLSIZE;
	pool->tail=0;


	Trans t={1,2,1};
	insertTransToAMA(t,ama,pool);
	Trans t2={1,2,2};
	insertTransToAMA(t2,ama,pool);
//	cout<<ama->list[2].head.next->state<<endl;
//	cout<<ama->list[2].head.next->next->state<<endl;

	cout<<isTransInAMA(t2,ama,abpds);
	return 0;
}
*/

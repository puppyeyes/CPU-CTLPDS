#include "utility.cuh"
#include "abpdspre.cuh"
#include "common.cuh"
#include <cooperative_groups.h>

using namespace std;
using namespace cooperative_groups;
#define QUEUEBASESIZE 5
#define DEFAULT_XML_FILE "abpds.xml"
#define ARGSNUM 7
#define THREADPERNUM 32
#define BLOCKSIZE 1

int main() {

	/*	char * file_name = DEFAULT_XML_FILE;
	 if (parse_abpds_xml(file_name) != 0) {
	 printf("Failed to parse abpds\n ");
	 } else {
	 printf("parse abpds compelet\n");
	 }

	 print_parse_result();*/

	AMA ama_1,ama_2;

	initGQueue(QUEUEBASESIZE * abpds_info->stack_size);

	dim3 dimBlock(THREADPERNUM, 1, 1); //一个块中开threadsPerBlock个线程
	dim3 dimGrid(BLOCKSIZE, 1, 1); //一个gird里开blockSize个块

	bool *isReach;
	CUDA_SAFE_CALL(cudaMallocManaged(&isReach, sizeof(bool)));

	void **kernelArgs = NULL;
	kernelArgs = (void**) malloc(ARGSNUM * sizeof(*kernelArgs));

	kernelArgs[0] = malloc(sizeof(isReach));
	memcpy(kernelArgs[0], &isReach, sizeof(isReach));

	kernelArgs[1] = malloc(sizeof(delta));
	memcpy(kernelArgs[1], &isReach, sizeof(delta));

	kernelArgs[2] = malloc(sizeof(ama_1));
	memcpy(kernelArgs[2], &ama_1, sizeof(ama_1));

	kernelArgs[3] = malloc(sizeof(ama_2));
	memcpy(kernelArgs[3], &ama_2, sizeof(ama_2));

	kernelArgs[4] = malloc(sizeof(finalStateArray));
	memcpy(kernelArgs[4], &finalStateArray, sizeof(finalStateArray));

	kernelArgs[5] = malloc(sizeof(gqueue));
	memcpy(kernelArgs[5], &gqueue, sizeof(gqueue));

	kernelArgs[6] = malloc(sizeof(abpds_info));
	memcpy(kernelArgs[6], &abpds_info, sizeof(abpds_info));

	cudaLaunchCooperativeKernel((void*) compute_pre_on_pds, dimGrid, dimBlock,
			kernelArgs, NULL, NULL);
	cudaDeviceSynchronize();

	return 0;
}

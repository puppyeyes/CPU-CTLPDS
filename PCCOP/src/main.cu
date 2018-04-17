#include "utility.cuh"
#include "abpdspre.cuh"
#include "common.cuh"

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

	initGQueue(QUEUEBASESIZE * abpds_info->stack_size);

	dim3 dimBlock(THREADPERNUM, 1, 1); //一个块中开threadsPerBlock个线程
	dim3 dimGrid(BLOCKSIZE, 1, 1); //一个gird里开blockSize个块

	bool *isReach = false;
	cudaMallocManaged(isReach, sizeof(bool));

	void **kernelArgs = NULL;
	kernelArgs = (void**) malloc(ARGSNUM * sizeof(*kernelArgs));
	kernelArgs[0] = malloc(sizeof(bool));
	memcpy(kernelArgs[0], &isReach, sizeof(bool));

	cudaLaunchCooperativeKernel((void*) compute_pre_on_pds, dimGrid, dimBlock,
			kernelArgs, NULL, NULL);
	cudaDeviceSynchronize();

	return 0;
}

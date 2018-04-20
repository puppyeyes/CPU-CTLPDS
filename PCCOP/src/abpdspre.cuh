/*
 * abpdspre.cuh
 *
 *  Created on: Apr 17, 2018
 *      Author: chuancy
 */

#ifndef ABPDSPRE_CUH_
#define ABPDSPRE_CUH_
#include "utility.cuh"
#include "ama.cuh"
#include "abpds.cuh"
__global__ void compute_pre_on_pds(int*finish, Delta delta, AMA *latest_ama,
		short int *recursion, Gqueue *gqueue, ABPDSInfo *abpds_info, Pool *pool);

__global__ void compute_epsilon(Delta delta, AMA *ama, Pool *pool,
		ABPDSInfo *abpds_info, Gqueue *gqueue,short int *recursion);

__host__ __device__ short int decode_state_superScript(int state);
#endif /* ABPDSPRE_CUH_ */

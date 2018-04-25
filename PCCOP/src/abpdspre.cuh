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
__global__ void compute_pre_on_pds(int*finish, Delta delta, AMA *pre_ama,
		Pool *pool_1, AMA *latest_ama, Pool *pool_2, short int *recursion,
		Gqueue *gqueue, ABPDSInfo *abpds_info, TMPAMA tmp_ama);

__global__ void compute_epsilon(Delta delta, AMA *ama, Pool *pool,
		ABPDSInfo *abpds_info, Gqueue *gqueue, short int *recursion);

#endif /* ABPDSPRE_CUH_ */

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
__global__ void compute_pre_on_pds(int*finish, Delta delta, AMA *ama1,
		AMA *ama2, int *finalStateArray, Gqueue *gqueue, ABPDSInfo abpds_info);

#endif /* ABPDSPRE_CUH_ */

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
		AMA *latest_ama, int *finalStateArray, Gqueue *gqueue, ABPDSInfo abpds_info,AMA *tmp_ama,Pool *pool);

#endif /* ABPDSPRE_CUH_ */

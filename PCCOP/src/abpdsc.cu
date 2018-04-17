#include "abpds.cuh"

Delta delta;

ABPDSInfo *abpds_info;

void initABPDSInfo() {
	cudaMallocManaged(&abpds_info, sizeof(int) * 2);
}

void initDelta(int delta_size, Delta delta) {
	cudaMallocManaged(&delta, sizeof(int) * delta_size);
}

int getTransitionPos(TransitionRule *t) {
	int pos = t->to->controlLocation * (abpds_info->stack_size) + t->to->stack1;
	return pos;
}

void addRuleToDelta(TransitionRule *t, Delta delta) {
	//头插法
	int pos=getTransitionPos(t);
	if(delta[pos].next==NULL)
	{
		delta[pos].next=t;
	}
	else{
		t->next=delta[pos].next;
		delta[pos].next=t;
	}
}

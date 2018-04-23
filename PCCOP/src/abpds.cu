#include "abpds.cuh"

Delta delta;

ABPDSInfo *abpds_info;

int *finalStateArray;

void initABPDSInfo() {
	cudaMallocManaged(&abpds_info, sizeof(int) * 2);
}

void initDelta(int delta_size) {
	cudaMallocManaged(&delta, sizeof(int) * delta_size);
}

int getTransitionPos(TransitionRule *t) {
	int pos = t->to[0].controlLocation * (abpds_info->stack_size)
			+ t->to[0].stack1;
	return pos;
}

void addRuleToDelta(TransitionRule *t) {

	//头插法
	int pos = getTransitionPos(t);
	if (delta[pos].next == NULL) {
		delta[pos].next = t;
	} else {
		t->next = delta[pos].next;
		delta[pos].next = t;
	}

	if (t->to_config_size > 1) {
		for (int i = 1; i < t->to_config_size; i++) {
			int pos = t->to[i].controlLocation * (abpds_info->stack_size)
					+ t->to[i].stack1;
			TransitionRule *t_copy;
			cudaMallocManaged(&t_copy,sizeof(t));
			t_copy=t;
			t_copy->next=NULL;
			if (delta[pos].next == NULL) {
				delta[pos].next = t_copy;
			} else {
				t_copy->next = delta[pos].next;
				delta[pos].next = t_copy;
			}
		}
	}

}
__device__ void printRule(TransitionRule *rule) {
	if (rule != NULL) {
		if (rule->to_config_size == 1) {
			printf("<%d,%d>--><%d,%d %d>\n", rule->from.controlLocation,
					rule->from.stack, rule->to[0].controlLocation,
					rule->to[0].stack1, rule->to[0].stack2);
		}
	} else {
		printf("no rule\n");
	}
}


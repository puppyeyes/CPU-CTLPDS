/*
 * abpds.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */

#ifndef ABPDS_H_
#define ABPDS_H_

typedef struct FromConfig{
	int controlLocation;
	int stack;
}FromConfig;

typedef struct ToConfig{
	int controlLocation;
	int stack1;
	int stack2;
}ToConfig;

typedef struct TransitionRule{
	bool tag;
	FromConfig from;
	ToConfig *to;

}TransitionRule;

typedef struct ABPDS{
	int *finalState;
	int finalStateSize;
}ABPDS;




#endif /* ABPDS_H_ */

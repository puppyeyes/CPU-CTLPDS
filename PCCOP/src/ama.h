/*
 * ama.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include "common.h"
#ifndef AMA_H_
#define AMA_H_
typedef struct AMANode{
	int state;
	int *next;
};

typedef struct AMAHead{
	/* from 表示链表的头结点 表现为一个格局的编码 类型long int
	 * tail 指向链表的尾部节点 表现为一个state的编码 类型是int的指针
	 * */
	long int from;
	int *tail;
	AMANode *next;
}*AMA;

/*为链表预先开设的空间*/
typedef struct Pool{
	AMANode *item;
	int size;
	int head;
	int tail;
};

/*保存最近两次的自动机*/
AMA ama_1;
AMA ama_2;

/*判断是否两个自动机是否相等*/
bool isEqual(AMA ama_1,AMA ama_2);

/*插入一个trans到ama*/
void insertTransToAMA(Trans t,AMA ama);

/*删除ama*/
void deleteAMA(AMA ama,Pool pool);

/*判断ama中是否存在trans*/
bool isTransInAMA(Trans t,AMA ama);

/*更新AMA中state上标*/
void updateAMA(AMA ama);

#endif /* AMA_H_ */

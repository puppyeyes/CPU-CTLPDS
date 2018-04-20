/*
 * ama.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include "common.cuh"
#ifndef AMA_H_
#define AMA_H_

#define AMASIZE 1000 //ama.list大小
#define AMAPOOLSIZE 1000 //ama poll大小
struct AMANode{
	int state;
	AMANode *next;
};
typedef struct AMAHead{
	/* from 表示链表的头结点 表现为一个格局的编码 类型long int
	 * tail 指向链表的尾部节点 表现为一个state的编码 类型是int的指针
	 * */
	//long int from;
	//AMANode *tail;//尾指针
	AMANode head; //记录头     初始化时要将尾指针指向头结点的地址
	AMANode *tail;
	int mutex;//互斥量
}AMAList;
struct AMA{
	int count;
	AMAList *list;
};


/*为链表预先开设的空间*/
struct Pool{
	/**
	 * size: pool的大小
	 * head: pool的开始位置
	 * tail: pool的最后一个非空元素的位置
	 */
	AMANode *item;
	int size;
	int head;
	int tail;
};

/*保存最近两次的自动机*/
//AMA *ama_1;
//AMA *ama_2;

/*判断是否两个自动机是否相等*/
bool isEqual(AMA *ama_1,AMA *ama_2);
/*插入一个trans到ama*/
void insertTransToAMA(Trans t,AMA *ama,Pool *pool);
__device__ void d_insertTransToAMA(Trans t,AMA *ama,Pool *pool,ABPDSInfo *abpds_info);
/*删除ama*/
void deleteAMA(AMA *ama,Pool *pool);


/*判断ama中是否存在trans*/
__device__ __host__  bool isTransInAMA(Trans t,AMA *ama,ABPDSInfo *abpds_info);


/*更新AMA中state上标*/
void updateAMA(AMA *ama);

/*初始化AMA*/
void initAMA(AMA *ama,Pool *pool);

void printAMA(AMA *ama);
bool isFinalState(int state);


#endif /* AMA_H_ */

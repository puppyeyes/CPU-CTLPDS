/*
 * ama.h
 *
 *  Created on: Apr 11, 2018
 *      Author: scott
 */
#include "common.cuh"
#ifndef AMA_H_
#define AMA_H_

//#define AMASIZE 1000 //ama.list大小
#define AMAPOOLSIZE 500000 //ama poll大小
struct AMANode{
	int state;
	AMANode *next;
};
typedef struct TMPINDEX{
	AMANode *next;
	long int nodeFlag;
}*TMPAMA;

extern TMPAMA tmp_ama;

typedef struct AMAHead{
	/* from 表示链表的头结点 表现为一个格局的编码 类型long int
	 * tail 指向链表的尾部节点 表现为一个state的编码 类型是int的指针
	 * */
	//long int from;
	//AMANode *tail;//尾指针
	//int count;
	AMANode *next; //下一个
	unsigned long long int nodeFlag;
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
	int tail; //pool当前位置
};

/*保存最近两次的自动机*/
//AMA *ama_1;
//AMA *ama_2;

/*判断是否两个自动机是否相等*/
bool isEqual(AMA *ama_1,AMA *ama_2);
/*插入一个trans到ama
 * 不加锁
 * */
bool insertTransToAMA(Trans t, AMA *ama, Pool *pool);
__device__ bool d_insertTransToAMA(Trans t, AMA *ama, Pool *pool,ABPDSInfo *abpds_info);
/*删除ama*/
void deleteAMA(AMA *ama,Pool *pool);
void initTMP();
void add_to_TMP(AMA *ama);
/*判断ama中是否存在trans*/
__device__ __host__ bool isExist(unsigned long long int nodeFlag,short int state);

/*更新AMA中state上标*/
__global__ void updateAMA(AMA *ama, int recursion, Pool *pool,ABPDSInfo *abpds_info);

/*初始化AMA*/
void initAMA(AMA *ama,Pool *pool);

void printAMA(AMA *ama);
bool isFinalState(int state);


#endif /* AMA_H_ */

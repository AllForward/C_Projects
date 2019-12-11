#pragma once
#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED  

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define m 3 
typedef int Status;

typedef int KeyType;

typedef struct {
	KeyType key;
	char data;
} Record;

typedef struct BTNode {
	int keynum;              //结点当前的关键字个数
	KeyType key[m + 1];        //关键字数组，key[0]未用 
	struct BTNode* parent;   //双亲结点指针
	struct BTNode* ptr[m + 1]; //孩子结点指针数组
	Record* recptr[m + 1];     //句录指针向量，0号单元未用 
}BTNode, *BTree;

typedef struct {
	BTree pt; //指向找到的节点 
	int i;    //1<=i<=m，在节点中的关键字位序 
	int tag;  //1：查找成功；0：查找失败 
} result;     //B树的查找结果类型 

/********B树的查找，参数k是要查找的关键字，用result返回*******/
int Search(BTree p, int k);

void SearchBTree(BTree t, int k, result& result);

/********结合B树的查找以及树的分裂，执行B树的插入操作 ********/
void Insert(BTree& t, int i, int data, BTree ap);

//生成新的根节点
void newRoot(BTree& t, BTree p, int data, BTree ap);

//将出现上溢的节点进行分裂
void split(BTree& q, int s, BTree& ap);

//在B树t中q结点的key[i-1]和key[i]之间插入关键字k 
void InsertBTree(BTree& t, int k, BTree q, int i);

/********************B树的删除********************************/
//实行删除的功能 
void Remove(BTree& p, int i);

//由后继最下层非终端结点的最小关键字代替它
void Successor(BTree& p, int i);

//相邻兄弟节点中没有“多余”的关键字的方法（用于删除结点时调用）
void MergeParent(BTree& p, int i, BTree& t);

//对B树进行调整 
void Restore(BTree& p, int i, BTree& t);

//删除B树上p结点的第i个关键字（删除前应该先调用查找功能）
void DeleteBTree(BTree& p, int i, BTree& t);

/*************B树打印功能(采用递归的先序遍历方法)*************/
void Display(BTree q, int layer);

//将整棵B树销毁
void DestroyBTree(BTree& t);

#endif
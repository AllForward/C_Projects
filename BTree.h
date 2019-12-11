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
	int keynum;              //��㵱ǰ�Ĺؼ��ָ���
	KeyType key[m + 1];        //�ؼ������飬key[0]δ�� 
	struct BTNode* parent;   //˫�׽��ָ��
	struct BTNode* ptr[m + 1]; //���ӽ��ָ������
	Record* recptr[m + 1];     //��¼ָ��������0�ŵ�Ԫδ�� 
}BTNode, *BTree;

typedef struct {
	BTree pt; //ָ���ҵ��Ľڵ� 
	int i;    //1<=i<=m���ڽڵ��еĹؼ���λ�� 
	int tag;  //1�����ҳɹ���0������ʧ�� 
} result;     //B���Ĳ��ҽ������ 

/********B���Ĳ��ң�����k��Ҫ���ҵĹؼ��֣���result����*******/
int Search(BTree p, int k);

void SearchBTree(BTree t, int k, result& result);

/********���B���Ĳ����Լ����ķ��ѣ�ִ��B���Ĳ������ ********/
void Insert(BTree& t, int i, int data, BTree ap);

//�����µĸ��ڵ�
void newRoot(BTree& t, BTree p, int data, BTree ap);

//����������Ľڵ���з���
void split(BTree& q, int s, BTree& ap);

//��B��t��q����key[i-1]��key[i]֮�����ؼ���k 
void InsertBTree(BTree& t, int k, BTree q, int i);

/********************B����ɾ��********************************/
//ʵ��ɾ���Ĺ��� 
void Remove(BTree& p, int i);

//�ɺ�����²���ն˽�����С�ؼ��ִ�����
void Successor(BTree& p, int i);

//�����ֵܽڵ���û�С����ࡱ�Ĺؼ��ֵķ���������ɾ�����ʱ���ã�
void MergeParent(BTree& p, int i, BTree& t);

//��B�����е��� 
void Restore(BTree& p, int i, BTree& t);

//ɾ��B����p���ĵ�i���ؼ��֣�ɾ��ǰӦ���ȵ��ò��ҹ��ܣ�
void DeleteBTree(BTree& p, int i, BTree& t);

/*************B����ӡ����(���õݹ�������������)*************/
void Display(BTree q, int layer);

//������B������
void DestroyBTree(BTree& t);

#endif
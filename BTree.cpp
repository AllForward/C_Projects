#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "BTree.h"


/*************B树的查找，参数k是要查找的关键字，用result返回 *************/
int Search(BTree p, int k) {
	int i = 1;
	while (i <= p->keynum && k > p->key[i]) i++;
	return i;
}

void SearchBTree(BTree t, int k, result& result) {
	int i = 0, found = 0;
	//q用来指向根节点 
	BTree p = t, q = NULL;
	while (p != NULL && found == 0) {
		i = Search(p, k);
		if (i <= p->keynum && k == p->key[i]) found = 1;
		else {
			q = p;
			p = p->ptr[i - 1];
		}
	}
	if (1 == found) {
		result.i = i;
		result.pt = p;
		result.tag = 1;
	}
	else {
		//返回K可以插入的位置以及i 
		result.pt = q;
		result.i = i;
		result.tag = 0;
	}
}

/********结合B树的查找以及树的分裂，执行B树的插入操作 *************/
void Insert(BTree& t, int i, int data, BTree ap) {
	// i为在节点中的关键字位序 
	int j, n = t->keynum;
	for (j = n; j >= i; j--) {
		t->ptr[j + 1] = t->ptr[j];
		t->key[j + 1] = t->key[j];
	}
	t->key[i] = data;
	t->ptr[i] = ap;
	if (ap != NULL) ap->parent = t;
	t->keynum++;
}

//生成新的根节点 
void newRoot(BTree& t, BTree p, int data, BTree ap) {
	t = (BTree)malloc(sizeof(BTNode));
	if (NULL == t) exit(0);
	t->keynum = 1; t->ptr[0] = p; t->ptr[1] = ap; t->key[1] = data;
	if (p != NULL) p->parent = t;
	if (ap != NULL) ap->parent = t;
	t->parent = NULL;
}

//将出现上溢的节点进行分裂
void split(BTree& q, int s, BTree& ap) {
	//将q结点分裂成两半，一半保留在原结点，另一半移入ap所指的新节点
	int i = 0;
	ap = (BTree)malloc(sizeof(BTNode));
	ap->ptr[0] = q->ptr[s];
	//将后半部分的值移到ap中 
	for (i = s + 1; i <= q->keynum; i++) {
		ap->ptr[i - s] = q->ptr[i];
		ap->key[i - s] = q->key[i];
	}
	ap->keynum = q->keynum - s;
	ap->parent = q->parent;
	//修改新节点中的子节点parent域 
	for (i = 0; i <= q->keynum - s; i++) {
		if (ap->ptr[i] != NULL) ap->ptr[i]->parent = ap;
	}
	q->keynum = s - 1;
}

//在B树t中q结点的key[i-1]和key[i]之间插入关键字k 
void InsertBTree(BTree& t, int k, BTree q, int i) {
	int finished = 0, needNewRoot = 0, x = k;
	BTree ap;
	if (NULL == q) {
		//生成一个新的根结点
		newRoot(t, NULL, k, NULL);
	}
	else {
		//先对该树进行查询是否存在该k
		result r;
		SearchBTree(t, x, r);
		if (r.tag == 1) {
			return;
		}
		else {
			ap = NULL;
			while (0 == needNewRoot && 0 == finished) {
				Insert(q, i, x, ap);
				//表示插入完成 
				if (q->keynum < m) finished = 1;
				else {
					//出现插入上溢的情况
					int s = (m + 1) / 2;
					split(q, s, ap);
					x = q->key[s];
					if (q->parent != NULL) {
						q = q->parent;
						//查找溢出的结点需要插入的位置 
						i = Search(q, x);
					}
					//说明为根节点 
					else needNewRoot = 1;
				}
			}
			if (1 == needNewRoot) {
				//生成含信息(q,x,ap)的新的根节点t 
				newRoot(t, q, x, ap);
			}
		}
	}
}

/********************B树的删除**********************/
//实行删除的功能 
void Remove(BTree& p, int i) {
	for (int index = i; index <= p->keynum; ++index) {
		p->key[index] = p->key[index + 1];
		p->ptr[index] = p->ptr[index + 1];
	}
	p->keynum--;
}

//由后继最下层非终端结点的最小关键字代替它
void Successor(BTree& p, int i) {
	BTree last = p->ptr[i];
	while (last->ptr[0] != NULL)
	{
		last = last->ptr[0];
	}
	p->key[i] = last->key[1];
	p = last;
}

//相邻兄弟节点中没有“多余”的关键字的方法（用于删除结点时调用）
void MergeParent(BTree& p, int i, BTree& t) {
	BTree brother, parent;
	int j = 0, sign = 0;

	parent = p->parent;
	//找到p结点的位置 
	for (brother = parent->ptr[j]; brother != p; ) {
		brother = parent->ptr[++j];
	}
	//找其右兄弟,若存在则取其最小值给p 
	if (j < parent->keynum && parent->ptr[j + 1] != NULL) {
		brother = parent->ptr[j + 1];
		sign = 1;
	}
	else if (j > 0 && parent->ptr[j - 1] != NULL) {
		//找其左兄弟 
		brother = parent->ptr[j - 1];
		sign = 2;
	}
	//将该结点中的关键字删除后，与其兄弟和父结点中的分割兄弟两人的关键字进行合并
	int place = 0;
	while (true) {
		//获取分割两兄弟的关键字位置 
		if ((parent->ptr[place] == p && parent->ptr[place + 1] == brother)
			|| (parent->ptr[place] == brother && parent->ptr[place + 1] == p)) {
			place++;
			break;
		}
		place++;
	}
	if (1 == sign) {
		//说明该兄弟在p节点的右边，将父亲节点的值放到该节点的最左边
		//先对该兄弟结点进行位置移动
		for (int index = brother->keynum; index >= 0; --index) {
			brother->key[index + 1] = brother->key[index];
			brother->ptr[index + 1] = brother->ptr[index];
		}
		brother->key[1] = parent->key[place];
		//brother->ptr[0] = p->ptr[0];
		brother->keynum++;
		//将删除了关键字的结点合并到其兄弟最左边中,并将其孩子合并 
		for (int number = p->keynum; number >= 0; number--) {
			if (p->keynum == number) {
				//只可能有一个孩子
				for (int z = 0; z < m; z++) {
					if (p->ptr[z] != NULL) {
						brother->ptr[0] = p->ptr[z];
						brother->ptr[0]->parent = brother;
						break;
					}
				}
			}
			else {
				for (int index = brother->keynum; index >= 0; --index) {
					brother->key[index + 1] = brother->key[index];
					brother->ptr[index + 1] = brother->ptr[index];
				}
				brother->key[1] = p->key[number];
				brother->ptr[0] = p->ptr[number];
				brother->ptr[0]->parent = brother;
				brother->keynum++;
			}
		}
	}
	else {
		//将该关键字放置到左兄弟中 
		brother->key[++brother->keynum] = parent->key[place];
		//将删除了关键字的结点合并到其兄弟中
		if (0 == p->keynum) {
			//最多只可能有一个孩子
			for (int z = 0; z < m; z++) {
				if (p->ptr[z] != NULL) {
					brother->ptr[brother->keynum] = p->ptr[z];
					brother->ptr[brother->keynum]->parent = brother;
					break;
				}
			}
		}
		for (int number = 1; number <= p->keynum; number++) {
			brother->key[++brother->keynum] = p->key[number];
			brother->ptr[brother->keynum] = p->ptr[number];
			brother->ptr[brother->keynum]->parent = brother;
		}
	}
	//将双亲节点的key以及孩子结点移动好
	if (1 == sign) {
		//说明刚刚移到了右子树上
		for (int index = place; index <= parent->keynum; ++index) {
			parent->key[index] = parent->key[index + 1];
			parent->ptr[index - 1] = parent->ptr[index];
		}
		parent->keynum--;
		//Remove(parent, place - 1);
	}
	if (2 == sign) {
		//说明刚刚移到了左子树上
		Remove(parent, place);
	}
	free(p);

	if (parent->keynum < (m - 1) / 2) {
		//Restore(parent, i);
		if (NULL == parent->parent) {
			//说明已经递归到根节点
			if (1 == sign) {
				//说明刚刚合并在右子树上
				t = parent->ptr[place];
				t->parent = NULL;
				free(parent);
			}
			else {
				//说明刚刚合并在左子树上
				t = parent->ptr[place - 1];
				t->parent = NULL;
				free(parent);
			}
		}
		else {
			Restore(parent, parent->keynum + 1, t);
		}

	}
}

//对B树进行调整 
void Restore(BTree& p, int i, BTree& t) {
	//判断该节点的兄弟有没有多余的关键字
	BTree brother, parent;
	parent = p->parent;
	int place = 0, sign = 0, j = 0, last = 0;
	for (int z = 0; z < m; z++) {
		if (p->ptr[z] != NULL) {
			last = 1;
			break;
		}
	}
	//判断是否为整棵树的最后剩余的一个节点
	if (NULL == parent && last == 0) {
		return;
	}
	//找到p结点的位置 
	for (brother = parent->ptr[j]; brother != p; ) {
		brother = parent->ptr[++j];
	}
	place = j;
	//找其右兄弟,若存在则取其最小值给p 
	if (j != parent->keynum && parent->ptr[place + 1] != NULL && parent->ptr[place + 1]->keynum > m / 2) {
		place++;
		brother = parent->ptr[place];
		sign = 1;
	}
	else if (j != 0 && parent->ptr[place - 1] != NULL && parent->ptr[place - 1]->keynum > m / 2) {
		//找其左兄弟
		place--;
		brother = parent->ptr[place];
		sign = 2;
	}
	//说明右兄弟有足够的关键字可以借 
	if (1 == sign) {
		p->key[i] = parent->key[place];
		p->keynum++;
		parent->key[place] = brother->key[1];
		if (brother->ptr[0] != NULL) {
			//将该兄弟的最左边的孩子移到p的孩子最右边
			p->ptr[p->keynum] = brother->ptr[0];
			//修改该孩子的父母指针
			p->ptr[p->keynum]->parent = p;
			//brother->ptr[0] = NULL;
		}
		Remove(brother, 0);
	}
	//说明左兄弟有足够的关键字可以借 
	else if (2 == sign) {
		p->key[i] = parent->key[place + 1];
		p->keynum++;
		//将该兄弟结点中的最大关键字给双亲 
		parent->key[place + 1] = brother->key[brother->keynum];
		if (brother->ptr[brother->keynum] != NULL) {
			//对p结点的孩子进行移位
			for (int index = p->keynum; index >= 0; --index) {
				p->ptr[index + 1] = p->ptr[index];
			}
			//将该兄弟的最右边的孩子移到p的孩子最左边
			p->ptr[0] = brother->ptr[brother->keynum];
			//修改该孩子的父母指针
			p->ptr[0]->parent = p;
			brother->ptr[brother->keynum] = NULL;
		}
		brother->keynum--;
	}
	//兄弟结点关键字数也不足以借用 
	else {
		//调用相邻兄弟节点中没有“多余”的关键字的方法
		MergeParent(p, i, t);
	}

}

//删除B树上p结点的第i个关键字（删除前应该先调用查找功能）
void DeleteBTree(BTree& p, int i, BTree& t) {
	//说明该节点不在最下层非终端节点 
	if (p->ptr[i] != NULL) {
		//执行删除非最下层非终端节点
		//由后继最下层非终端结点的最小关键字代替它
		Successor(p, i);
		//变成删除最下层非终端结点中的最小关键字
		DeleteBTree(p, 1, t);
	}
	//说明该节点在最下层非终端节点 
	else {
		//从p中删除key[i] 
		Remove(p, i);
		//将最下层的孩子指针全部赋值为NULL
		for (int j = 0; j < m; j++) {
			p->ptr[j] = NULL;
		}
		if (p->keynum < (m - 1) / 2) {
			//删除关键字后个数小于(m-1)/2,对树进行调整
			Restore(p, i, t);
		}
	}
	if (t->keynum == 0) t = NULL;
}

/*************B树打印功能(采用递归的先序遍历方法)*******************/
void Display(BTree q, int layer) {
	if (q)
	{
		for (int i = 0; i < layer; i++)
			printf("      ");
		for (int i = 1; i <= q->keynum; i++)
		{
			printf("%d", q->key[i]);
			if (i != q->keynum)
				printf(",");
			else
				printf("\n");
		}
		for (int j = 0; j <= q->keynum; j++)
			Display(q->ptr[j], layer + 1);
	}
}

//将整棵B树销毁
void DestroyBTree(BTree& t) {
	int i;
	BTNode* p = t;
	if (p != NULL) {                                    //B树不为空  
		for (i = 0; i <= p->keynum; i++) {              //递归释放每一个结点 
			DestroyBTree(*&p->ptr[i]);
		}
		free(p);
	}
	t = NULL;
}
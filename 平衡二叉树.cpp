#include <iostream>
#include<vector>
using namespace std;

//定义结点
class AVLnode {
public:
	int val;      //节点的值
	AVLnode* lchild, * rchild, * parent;    //指针，左右孩子，父节点
	int tag;              //平衡因子

	AVLnode(int x) : val(x), tag(0), lchild(nullptr),
		rchild(nullptr), parent(nullptr) {} //构造函数
};

//定义AVL平衡二叉树
class AVLtree {
private:
	AVLnode* root;      //根节点
public:
	AVLtree() :root(nullptr) {}      //构造函数
	~AVLtree()      //析构函数
	{
		clear(root);
	}
	void clear(AVLnode* p)
	{
		if (!p)
			return;
		if (p->lchild)
			clear(p->lchild);
		if (p->rchild)
			clear(p->rchild);
		delete p;
	}

	//获取平衡因子
	int get_tag(AVLnode* root1)
	{
		if (!root1)      //空节点
			return 0;
		int lh = get_hight(root1->lchild);
		int rh = get_hight(root1->rchild);
		return abs(lh - rh);
	}

	//获取树高
	int get_hight(AVLnode* T)
	{
		int lh = 0, rh = 0;
		if (T == nullptr)
			return 0;
		lh = get_hight(T->lchild);
		rh = get_hight(T->rchild);
		return lh > rh ? lh + 1 : rh + 1;
	}

	//更新平衡因子
	void tag_update(AVLnode* p) {
		if (!p)
			return;
		tag_update(p->lchild);
		tag_update(p->rchild);
		p->tag = get_tag(p);
	}

	//仅插入节点，利用二叉排序树的插入方式
	bool BST_insert(AVLnode* p, int x, AVLnode*& k)
	{
		if (p == nullptr)
		{
			// 创建新节点
			AVLnode* temp = new AVLnode(x);
			k = temp;
			// 如果当前插入位置是根节点，则更新根节点
			if (p == root) {
				root = temp;
			}
			return true;
		}
		else if (x == p->val)     // 值已存在，不重复插入
		{
			return false;
		}
		else if (x < p->val)   // 插入到左子树
		{
			bool res = BST_insert(p->lchild, x, k);
			if (res)            // 更新插入结点的父节点
			{
				p->lchild = k;  // 确保子节点指针也更新
				k->parent = p;
			}
			return res;
		}
		else              // 插入到右子树
		{
			bool res = BST_insert(p->rchild, x, k);
			if (res)            // 更新插入结点的父节点
			{
				p->rchild = k;  // 确保子节点指针也更新
				k->parent = p;
			}
			return res;
		}
	}

	//插入元素，并调整平衡二叉树
	void insert(int x)
	{
		AVLnode* k = nullptr;
		bool is_insert = BST_insert(root, x, k);
		if (!is_insert)     //插入失败
		{
			cout << "插入失败，该节点已存在" << endl;
			return;
		}
		tag_update(root);    //更新平衡因子
		AVLnode* newpoint = k;
		while (k->parent && abs(k->parent->tag) < 2)   //向上寻找第一个平衡因子异常的点
		{
			k = k->parent;
		}
		//k是根节点
		if (k == root)
		{
			if (k->tag < 2)    //未出现异常
				return;
			//出现异常的是根节点
			else {
				//插入结点是左孩子
				if (newpoint->parent->lchild && newpoint == newpoint->parent->lchild)
				{
					AVLnode* temp = newpoint;
					while (temp->parent != k)//向上寻找，此时temp为k的孩子
						temp = temp->parent;
					//在k的左子树的左孩子中的插入 LL
					if (k->lchild && k->lchild == temp)
					{
						//旋转操作，一次右单旋
						k->lchild = temp->rchild;
						if (temp->rchild)
							temp->rchild->parent = k;
						temp->rchild = k;
						k->parent = temp;
						temp->parent = nullptr;
						root = temp;
						//更新平衡因子
						tag_update(root);
					}
					//在k的右子树的左孩子中的插入  RL
					else if (k->rchild && k->rchild == temp)
					{
						// 找到 k 的右子树的左孩子 q
						AVLnode* q = temp->lchild;
						// 第一次旋转（右旋）
						temp->rchild = q->rchild;
						if (q->rchild)
							q->rchild->parent = temp;
						q->rchild = temp;
						temp->parent = q;
						// 第二次旋转（左旋）
						k->rchild = q->lchild;
						if (q->lchild)
							q->lchild->parent = k;
						q->lchild = k;
						k->parent = q;
						q->parent = nullptr;
						root = q;   // q 现在成为新的根节点
						// 更新平衡因子
						tag_update(root);
					}
				}
				//插入结点是右孩子
				else if (newpoint->parent->rchild && newpoint == newpoint->parent->rchild)
				{
					AVLnode* temp = newpoint;
					while (temp->parent != k)//向上寻找，此时temp为k的孩子
						temp = temp->parent;
					//在k的左子树的右孩子中的插入 LR
					if (k->lchild && k->lchild == temp)
					{
						AVLnode* q = temp->rchild;
						//第一次左单旋
						temp->rchild = q->lchild;
						if (q->lchild)
							q->lchild->parent = temp;
						q->rchild = temp;
						temp->parent = q;
						//第二次右单旋
						k->lchild = q->rchild;
						if (q->rchild)
							q->rchild->parent = k;
						q->rchild = k;
						k->parent = q;
						q->parent = nullptr;
						root = q;
						//更新平衡因子
						tag_update(root);
					}
					//在k的右子树的右孩子中的插入  RR
					else if (k->rchild && k->rchild == temp)
					{
						//旋转操作，左单旋
						k->rchild = temp->lchild;
						if (temp->lchild)
							temp->lchild->parent = k;
						temp->lchild = k;
						k->parent = temp;
						temp->parent = nullptr;
						root = temp;
						//更新平衡因子
						tag_update(root);
					}
				}
			}
		}
		//k是分支结点
		else if (k != root)
		{
			//插入结点是左孩子
			if (newpoint->parent->lchild && newpoint == newpoint->parent->lchild)
			{
				AVLnode* temp = newpoint;
				while (temp->parent != k)//向上寻找，此时temp为k的孩子
					temp = temp->parent;
				//在k的左子树的左孩子中的插入
				if (k->lchild && temp == k->lchild)
				{
					AVLnode* top = k->parent;   //用top记录最小非平衡子树根节点的父节点
					AVLnode* k_tem = k;          //记录最小非平衡子树根节点
					//旋转操作，一次右单旋
					k->lchild = temp->rchild;
					if (temp->rchild)
						temp->rchild->parent = k;
					temp->rchild = k;
					k->parent = temp;
					temp->parent = top;
					//改变最小非平衡子树根节点的父节点的孩子指针
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					//更新平衡因子
					tag_update(root);
				}
				//在k的右子树的左孩子中的插入
				else if (k->rchild && temp == k->rchild)
				{
					AVLnode* top = k->parent;   //用top记录最小非平衡子树根节点的父节点
					AVLnode* k_tem = k;          //记录最小非平衡子树根节点
					// 找到 k 的右子树的左孩子 q
					AVLnode* q = temp->lchild;
					// 第一次旋转（右旋）
					temp->rchild = q->rchild;
					if (q->rchild)
						q->rchild->parent = temp;
					q->rchild = temp;
					temp->parent = q;
					// 第二次旋转（左旋）
					k->rchild = q->lchild;
					if (q->lchild)
						q->lchild->parent = k;
					q->lchild = k;
					k->parent = q;
					q->parent = top;
					//改变最小非平衡子树根节点的父节点的孩子指针
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					// 更新平衡因子
					tag_update(root);
				}
			}
			//插入结点是右孩子
			else if (newpoint->parent->rchild && newpoint == newpoint->parent->rchild)
			{
				AVLnode* temp = newpoint;
				while (temp->parent != k)//向上寻找，此时temp为k的孩子
					temp = temp->parent;
				//在k的左子树的右孩子中的插入
				if (k->lchild && temp == k->lchild)
				{
					AVLnode* top = k->parent;   //用top记录最小非平衡子树根节点的父节点
					AVLnode* k_tem = k;          //记录最小非平衡子树根节点
					AVLnode* q = temp->rchild;
					//第一次左单旋
					temp->rchild = q->lchild;
					if (q->lchild)
						q->lchild->parent = temp;
					q->rchild = temp;
					temp->parent = q;
					//第二次右单旋
					k->lchild = q->rchild;
					if (q->rchild)
						q->rchild->parent = k;
					q->rchild = k;
					k->parent = q;
					q->parent = top;
					//改变最小非平衡子树根节点的父节点的孩子指针
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					//更新平衡因子
					tag_update(root);
				}
				//在k的右子树的右孩子中的插入
				else if (k->lchild && temp == k->rchild)
				{
					AVLnode* top = k->parent;   //用top记录最小非平衡子树根节点的父节点
					AVLnode* k_tem = k;          //记录最小非平衡子树根节点
					//旋转操作，左单旋
					k->rchild = temp->lchild;
					if (temp->lchild)
						temp->lchild->parent = k;
					temp->lchild = k;
					k->parent = temp;
					temp->parent = top;
					//改变最小非平衡子树根节点的父节点的孩子指针
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					//更新平衡因子
					tag_update(root);
				}
			}
		}

	}


	// 中序遍历
	void inorder_traversal(AVLnode* node) {
		if (node != nullptr) {
			inorder_traversal(node->lchild);
			cout << node->val << " ";
			inorder_traversal(node->rchild);
		}
	}

	// 打印中序遍历结果
	void print_inorder() {
		inorder_traversal(root);
		cout << endl;
	}
};

//测试函数
void test01() {
	AVLtree avl;

	// 基本插入
	vector<int> basic = { 1, 2,3 };
	for (int num : basic) {
		avl.insert(num);
	}
	cout << "基本插入后中序遍历: ";
	avl.print_inorder();
}

int main() {
	test01();
	system("pause");
	return 0;
}


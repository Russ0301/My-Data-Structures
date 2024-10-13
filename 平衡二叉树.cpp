#include <iostream>
#include<vector>
using namespace std;

//������
class AVLnode {
public:
	int val;      //�ڵ��ֵ
	AVLnode* lchild, * rchild, * parent;    //ָ�룬���Һ��ӣ����ڵ�
	int tag;              //ƽ������

	AVLnode(int x) : val(x), tag(0), lchild(nullptr),
		rchild(nullptr), parent(nullptr) {} //���캯��
};

//����AVLƽ�������
class AVLtree {
private:
	AVLnode* root;      //���ڵ�
public:
	AVLtree() :root(nullptr) {}      //���캯��
	~AVLtree()      //��������
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

	//��ȡƽ������
	int get_tag(AVLnode* root1)
	{
		if (!root1)      //�սڵ�
			return 0;
		int lh = get_hight(root1->lchild);
		int rh = get_hight(root1->rchild);
		return abs(lh - rh);
	}

	//��ȡ����
	int get_hight(AVLnode* T)
	{
		int lh = 0, rh = 0;
		if (T == nullptr)
			return 0;
		lh = get_hight(T->lchild);
		rh = get_hight(T->rchild);
		return lh > rh ? lh + 1 : rh + 1;
	}

	//����ƽ������
	void tag_update(AVLnode* p) {
		if (!p)
			return;
		tag_update(p->lchild);
		tag_update(p->rchild);
		p->tag = get_tag(p);
	}

	//������ڵ㣬���ö����������Ĳ��뷽ʽ
	bool BST_insert(AVLnode* p, int x, AVLnode*& k)
	{
		if (p == nullptr)
		{
			// �����½ڵ�
			AVLnode* temp = new AVLnode(x);
			k = temp;
			// �����ǰ����λ���Ǹ��ڵ㣬����¸��ڵ�
			if (p == root) {
				root = temp;
			}
			return true;
		}
		else if (x == p->val)     // ֵ�Ѵ��ڣ����ظ�����
		{
			return false;
		}
		else if (x < p->val)   // ���뵽������
		{
			bool res = BST_insert(p->lchild, x, k);
			if (res)            // ���²�����ĸ��ڵ�
			{
				p->lchild = k;  // ȷ���ӽڵ�ָ��Ҳ����
				k->parent = p;
			}
			return res;
		}
		else              // ���뵽������
		{
			bool res = BST_insert(p->rchild, x, k);
			if (res)            // ���²�����ĸ��ڵ�
			{
				p->rchild = k;  // ȷ���ӽڵ�ָ��Ҳ����
				k->parent = p;
			}
			return res;
		}
	}

	//����Ԫ�أ�������ƽ�������
	void insert(int x)
	{
		AVLnode* k = nullptr;
		bool is_insert = BST_insert(root, x, k);
		if (!is_insert)     //����ʧ��
		{
			cout << "����ʧ�ܣ��ýڵ��Ѵ���" << endl;
			return;
		}
		tag_update(root);    //����ƽ������
		AVLnode* newpoint = k;
		while (k->parent && abs(k->parent->tag) < 2)   //����Ѱ�ҵ�һ��ƽ�������쳣�ĵ�
		{
			k = k->parent;
		}
		//k�Ǹ��ڵ�
		if (k == root)
		{
			if (k->tag < 2)    //δ�����쳣
				return;
			//�����쳣���Ǹ��ڵ�
			else {
				//������������
				if (newpoint->parent->lchild && newpoint == newpoint->parent->lchild)
				{
					AVLnode* temp = newpoint;
					while (temp->parent != k)//����Ѱ�ң���ʱtempΪk�ĺ���
						temp = temp->parent;
					//��k���������������еĲ��� LL
					if (k->lchild && k->lchild == temp)
					{
						//��ת������һ���ҵ���
						k->lchild = temp->rchild;
						if (temp->rchild)
							temp->rchild->parent = k;
						temp->rchild = k;
						k->parent = temp;
						temp->parent = nullptr;
						root = temp;
						//����ƽ������
						tag_update(root);
					}
					//��k���������������еĲ���  RL
					else if (k->rchild && k->rchild == temp)
					{
						// �ҵ� k �������������� q
						AVLnode* q = temp->lchild;
						// ��һ����ת��������
						temp->rchild = q->rchild;
						if (q->rchild)
							q->rchild->parent = temp;
						q->rchild = temp;
						temp->parent = q;
						// �ڶ�����ת��������
						k->rchild = q->lchild;
						if (q->lchild)
							q->lchild->parent = k;
						q->lchild = k;
						k->parent = q;
						q->parent = nullptr;
						root = q;   // q ���ڳ�Ϊ�µĸ��ڵ�
						// ����ƽ������
						tag_update(root);
					}
				}
				//���������Һ���
				else if (newpoint->parent->rchild && newpoint == newpoint->parent->rchild)
				{
					AVLnode* temp = newpoint;
					while (temp->parent != k)//����Ѱ�ң���ʱtempΪk�ĺ���
						temp = temp->parent;
					//��k�����������Һ����еĲ��� LR
					if (k->lchild && k->lchild == temp)
					{
						AVLnode* q = temp->rchild;
						//��һ������
						temp->rchild = q->lchild;
						if (q->lchild)
							q->lchild->parent = temp;
						q->rchild = temp;
						temp->parent = q;
						//�ڶ����ҵ���
						k->lchild = q->rchild;
						if (q->rchild)
							q->rchild->parent = k;
						q->rchild = k;
						k->parent = q;
						q->parent = nullptr;
						root = q;
						//����ƽ������
						tag_update(root);
					}
					//��k�����������Һ����еĲ���  RR
					else if (k->rchild && k->rchild == temp)
					{
						//��ת����������
						k->rchild = temp->lchild;
						if (temp->lchild)
							temp->lchild->parent = k;
						temp->lchild = k;
						k->parent = temp;
						temp->parent = nullptr;
						root = temp;
						//����ƽ������
						tag_update(root);
					}
				}
			}
		}
		//k�Ƿ�֧���
		else if (k != root)
		{
			//������������
			if (newpoint->parent->lchild && newpoint == newpoint->parent->lchild)
			{
				AVLnode* temp = newpoint;
				while (temp->parent != k)//����Ѱ�ң���ʱtempΪk�ĺ���
					temp = temp->parent;
				//��k���������������еĲ���
				if (k->lchild && temp == k->lchild)
				{
					AVLnode* top = k->parent;   //��top��¼��С��ƽ���������ڵ�ĸ��ڵ�
					AVLnode* k_tem = k;          //��¼��С��ƽ���������ڵ�
					//��ת������һ���ҵ���
					k->lchild = temp->rchild;
					if (temp->rchild)
						temp->rchild->parent = k;
					temp->rchild = k;
					k->parent = temp;
					temp->parent = top;
					//�ı���С��ƽ���������ڵ�ĸ��ڵ�ĺ���ָ��
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					//����ƽ������
					tag_update(root);
				}
				//��k���������������еĲ���
				else if (k->rchild && temp == k->rchild)
				{
					AVLnode* top = k->parent;   //��top��¼��С��ƽ���������ڵ�ĸ��ڵ�
					AVLnode* k_tem = k;          //��¼��С��ƽ���������ڵ�
					// �ҵ� k �������������� q
					AVLnode* q = temp->lchild;
					// ��һ����ת��������
					temp->rchild = q->rchild;
					if (q->rchild)
						q->rchild->parent = temp;
					q->rchild = temp;
					temp->parent = q;
					// �ڶ�����ת��������
					k->rchild = q->lchild;
					if (q->lchild)
						q->lchild->parent = k;
					q->lchild = k;
					k->parent = q;
					q->parent = top;
					//�ı���С��ƽ���������ڵ�ĸ��ڵ�ĺ���ָ��
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					// ����ƽ������
					tag_update(root);
				}
			}
			//���������Һ���
			else if (newpoint->parent->rchild && newpoint == newpoint->parent->rchild)
			{
				AVLnode* temp = newpoint;
				while (temp->parent != k)//����Ѱ�ң���ʱtempΪk�ĺ���
					temp = temp->parent;
				//��k�����������Һ����еĲ���
				if (k->lchild && temp == k->lchild)
				{
					AVLnode* top = k->parent;   //��top��¼��С��ƽ���������ڵ�ĸ��ڵ�
					AVLnode* k_tem = k;          //��¼��С��ƽ���������ڵ�
					AVLnode* q = temp->rchild;
					//��һ������
					temp->rchild = q->lchild;
					if (q->lchild)
						q->lchild->parent = temp;
					q->rchild = temp;
					temp->parent = q;
					//�ڶ����ҵ���
					k->lchild = q->rchild;
					if (q->rchild)
						q->rchild->parent = k;
					q->rchild = k;
					k->parent = q;
					q->parent = top;
					//�ı���С��ƽ���������ڵ�ĸ��ڵ�ĺ���ָ��
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					//����ƽ������
					tag_update(root);
				}
				//��k�����������Һ����еĲ���
				else if (k->lchild && temp == k->rchild)
				{
					AVLnode* top = k->parent;   //��top��¼��С��ƽ���������ڵ�ĸ��ڵ�
					AVLnode* k_tem = k;          //��¼��С��ƽ���������ڵ�
					//��ת����������
					k->rchild = temp->lchild;
					if (temp->lchild)
						temp->lchild->parent = k;
					temp->lchild = k;
					k->parent = temp;
					temp->parent = top;
					//�ı���С��ƽ���������ڵ�ĸ��ڵ�ĺ���ָ��
					if (k_tem == top->lchild)
						top->lchild = temp;
					else if (k_tem == top->rchild)
						top->rchild = temp;
					//����ƽ������
					tag_update(root);
				}
			}
		}

	}


	// �������
	void inorder_traversal(AVLnode* node) {
		if (node != nullptr) {
			inorder_traversal(node->lchild);
			cout << node->val << " ";
			inorder_traversal(node->rchild);
		}
	}

	// ��ӡ����������
	void print_inorder() {
		inorder_traversal(root);
		cout << endl;
	}
};

//���Ժ���
void test01() {
	AVLtree avl;

	// ��������
	vector<int> basic = { 1, 2,3 };
	for (int num : basic) {
		avl.insert(num);
	}
	cout << "����������������: ";
	avl.print_inorder();
}

int main() {
	test01();
	system("pause");
	return 0;
}


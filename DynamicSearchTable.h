#pragma once

template <class KEY,class OTHER>
struct SET {
	KEY key;
	OTHER other;
};

template<class KEY,class OTHER>
class dynamicSearchTable
{
public:
	virtual SET<KEY, OTHER>* find(const KEY& x)const = 0;
	virtual void insert(const SET<KEY, OTHER>& x) = 0;
	virtual void remove(const KEY& x) = 0;
	virtual ~dynamicSearchTable() {};
};

template<class KEY,class OTHER>
class BinarySearchTree :public dynamicSearchTable<KEY, OTHER>
{
private:
	struct BinaryNode
	{
		SET<KEY, OTHER> data;
		BinaryNode* left;
		BinaryNode* right;

		BinaryNode(const SET<KEY, OTHER>& thedata, BinaryNode* lt = nullptr, BinaryNode* rt = nullptr)
			:data(thedata), left(lt), right(rt) {}

	};
	BinaryNode* root;
public:
	BinarySearchTree();
	~BinarySearchTree();
	SET<KEY, OTHER>* find(const KEY& x)const;
	void insert(const SET<KEY, OTHER>& x);
	void remove(const KEY& x);
private:
	void insert(const SET<KEY, OTHER>& x, BinaryNode*& t);
	void remove(const KEY& x, BinaryNode*& t);
	SET<KEY, OTHER>* find(const KEY& x, BinaryNode* t)const;
	void makeEmpty(BinaryNode* t);
};

template<class KEY, class OTHER>
inline BinarySearchTree<KEY, OTHER>::BinarySearchTree()
{
	root = nullptr;
}

template<class KEY, class OTHER>
inline BinarySearchTree<KEY, OTHER>::~BinarySearchTree()
{
	makeEmpty(root);
}

template<class KEY, class OTHER>
inline SET<KEY, OTHER>* BinarySearchTree<KEY, OTHER>::find(const KEY& x) const
{
	return find(x, root);
}

template<class KEY, class OTHER>
inline void BinarySearchTree<KEY, OTHER>::insert(const SET<KEY, OTHER>& x)
{
	insert(x, root);
}

template<class KEY, class OTHER>
inline void BinarySearchTree<KEY, OTHER>::remove(const KEY& x)
{
	remove(x, root);
}

template<class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::insert(const SET<KEY, OTHER>& x, BinaryNode*& t)
{
	if (t == nullptr)
		t = new BinaryNode(x, nullptr, nullptr);
	else if (x.key < t->data.key)
		insert(x, t->left);
	else if (x.key > t->data.key)
		insert(x, t->right);
}

template<class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::remove(const KEY& x, BinaryNode*& t)
{
	if (t == nullptr) return;
	if (x > t->data.key) remove(x, t->right);
	else if (x < t->data.key) remove(x, t->left);
	else if (t->left != nullptr && t->right != nullptr)
	{
		BinaryNode* tmp = t->right;
		while (tmp->left != nullptr) tmp = tmp->left;
		t->data = tmp->data;
		remove(t->data.key, t->right);
	}
	else
	{//被删结点只有一个孩子或是叶结点
		BinaryNode* oldNode = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete oldNode;

	}
}

template<class KEY, class OTHER>
SET<KEY, OTHER>* BinarySearchTree<KEY, OTHER>::find(const KEY& x, BinaryNode* t) const
{
	if (t == nullptr || t->data.key == x)
		return (SET<KEY, OTHER>*)t;
	if (x < t->data.key)
		return find(x, t->left);
	else
		return find(x, t->right);
}

template<class KEY, class OTHER>
void BinarySearchTree<KEY, OTHER>::makeEmpty(BinaryNode* t)
{
	if (t == nullptr) return;
	makeEmpty(t->left);
	makeEmpty(t->right);
	delete t;
}

//AVL树类的定义
template<class KEY,class OTHER>
class AvlTree :public dynamicSearchTable<KEY, OTHER>
{
	struct AvlNode
	{
		SET<KEY, OTHER> data;
		AvlNode* left;
		AvlNode* right;
		int height;  //结点的高度
		AvlNode(const SET<KEY, OTHER>& element, AvlNode* lt, AvlNode* rt, int h = 1)
			:data(element), left(lt), right(rt), height(h) {}
	};

	AvlNode* root;
public:
	AvlTree() { root = nullptr; };
	~AvlTree() { makeEmpty(root); }
	SET<KEY, OTHER>* find(const KEY& x)const;
	void insert(const SET<KEY, OTHER>& x);
	void remove(const KEY& x);
private:
	void insert(const SET<KEY, OTHER>& x, AvlNode*& t);
	bool remove(const KEY& x, AvlNode*& t);
	void makeEmpty(AvlNode* t);
	int height(AvlNode* t)const { return t == nullptr ? 0 : t->height; }
	void LL(AvlNode*& t);
	void LR(AvlNode*& t);
	void RL(AvlNode*& t);
	void RR(AvlNode*& t);
	int max(int a, int b) { return (a > b) ? a : b; }
	bool adjust(AvlNode*& t, int subTree);
};

template<class KEY, class OTHER>
SET<KEY, OTHER>* AvlTree<KEY, OTHER>::find(const KEY& x) const
{
	AvlNode* t = root;
	while (t != nullptr && t->data.key != x)
		if (t->data.key > x) t = t->left;
		else t = t->right;

	if (t == nullptr) return nullptr;
	else return (SET<KEY, OTHER>*)t;
}

template<class KEY, class OTHER>
inline void AvlTree<KEY, OTHER>::insert(const SET<KEY, OTHER>& x)
{
	insert(x, root);
}

template<class KEY, class OTHER>
inline void AvlTree<KEY, OTHER>::remove(const KEY& x)
{
	remove(x, root);
}

template<class KEY, class OTHER>
void AvlTree<KEY, OTHER>::insert(const SET<KEY, OTHER>& x, AvlNode*& t)
{
	if (t == nullptr)
		t = new AvlNode(x, nullptr, nullptr);
	else if (x.key < t->data.key)
	{
		insert(x, t->left);
		if (height(t->left) - height(t->right) == 2)
			if (x.key < t->left->data.key) LL(t);
			else LR(t);
	}
	else if (t->data.key < x.key)
	{
		insert(x, t->right);
		if (height(t->right) - height(t->left) == 2)
			if (t->right->data.key < x.key) RR(t);
			else RL(t);
	}
	//重新计算t的高度
	t->height = max(height(t->left), height(t->right)) + 1;
}

template<class KEY, class OTHER>
bool AvlTree<KEY, OTHER>::remove(const KEY& x, AvlNode*& t)
{
	if (t == nullptr) return true;
	if (x == t->data.key)
	{//删除根节点
		if (t->left == nullptr || t->right == nullptr)
		{//被删结点是叶结点或只有一个儿子
			AvlNode* oldNode = t;
			t = (t->left) != nullptr?t->left : t->right;
			delete oldNode;
			return false;
		}
		else
		{//被删结点有两个儿子
			AvlNode* tmp = t->right;
			while (tmp->left != nullptr) tmp = tmp->left;
			t->data = tmp->data;
			if (remove(tmp->data.key, t->right)) return true;    //删除后右子树没有变矮
			return adjust(t, 1);
		}
	}
	if (x < t->data.key)
	{
		if (remove(x, t->left)) return true;   //删除后左子树没有变矮
		return adjust(t, 0);
	}
	else
	{
		if (remove(x, t->right)) return true;  //删除后右子树没有变矮
		return adjust(t, 1);
	}
}

template<class KEY, class OTHER>
void AvlTree<KEY, OTHER>::makeEmpty(AvlNode* t)
{
	if (t == nullptr) return;
	makeEmpty(t->left);
	makeEmpty(t->right);
	delete t;
}

template<class KEY, class OTHER>
void AvlTree<KEY, OTHER>::LL(AvlNode*& t)
{
	AvlNode* t1 = t->left;        //未来的树根
	t->left = t1->right;
	t1->right = t;
	t->height = max(height(t->left), height(t->right)) + 1;
	t1->height = max(height(t1->left), height(t)) + 1;
	t = t1;
}

template<class KEY, class OTHER>
void AvlTree<KEY, OTHER>::LR(AvlNode*& t)
{
	RR(t->left);
	LL(t);
}

template<class KEY, class OTHER>
void AvlTree<KEY, OTHER>::RL(AvlNode*& t)
{
	LL(t->right);
	RR(t);
}

template<class KEY, class OTHER>
void AvlTree<KEY, OTHER>::RR(AvlNode*& t)
{
	AvlNode* t1 = t;
	t->right = t1->left;
	t1->left = t;
	t->height = max(height(t->left), height(t->right)) + 1;
	t1->height = max(height(t1->right), height(t)) + 1;
	t = t1;
}

template<class KEY, class OTHER>
bool AvlTree<KEY, OTHER>::adjust(AvlNode*& t, int subTree)
{
	if (subTree)
	{//在右子树上删除，使右子树变矮
		if (height(t->left) - height(t->right) == 1) return true;       //情况a
		if (height(t->left) == height(t->right)) { --t->height; return false; }      //情况b
		if (height(t->left->right) > height(t->left->left))      //情况d
		{
			LR(t);
			return false;
		}
		LL(t);
		if (height(t->right) == height(t->left)) return false;
		else return true;

	}
	else
	{
		if (height(t->right) - height(t->left) == 1) return true;     //情况a
		if (height(t->right) == height(t->left)) {
			--t->height;
			return false;
		}
		if (height(t->right->left) > height(t->right->right))     //情况d
		{
			RL(t);
			return false;
		}
		RR(t);
		if (height(t->right) == height(t->left)) return false;
		else return true;
	}
}
template<class KEY,class OTHER>
class RedBlackTree :public dynamicSearchTable<KEY, OTHER>
{
	enum colourT
	{
		RED,
		BLACK
	};
	struct RedBlackNode
	{
		SET<KEY, OTHER> data;
		RedBlackNode* left;
		RedBlackNode* right;
		colourT colour;
		RedBlackNode(const SET<KEY, OTHER>& element, RedBlackNode* lt = nullptr,
			RedBlackNode* rt = nullptr, colourT h = RED)
			:data(element), left(lt), right(rt), colour(h) {}
	};
	RedBlackNode* root;
public:
	RedBlackTree(RedBlackNode* t = nullptr) { root = t; }
	~RedBlackTree() { makeEmpty(root); }
	SET<KEY, OTHER>* find(const KEY& x)const;
	void insert(const SET<KEY, OTHER>& x);
	void remove(const KEY& x);
private:
	void makeEmpty(RedBlackNode*& t);
	void insertAdjust(RedBlackNode* gp, RedBlackNode* p, RedBlackNode* t);
	void removeAdjust(RedBlackNode*& p, RedBlackNode*& c, RedBlackNode*& t, KEY del);
	void LL(RedBlackNode* gp);
	void LR(RedBlackNode* gp);
	void RL(RedBlackNode* gp);
	void RR(RedBlackNode* gp);
};

template<class KEY, class OTHER>
SET<KEY, OTHER>* RedBlackTree<KEY, OTHER>::find(const KEY& x) const
{
	RedBlackNode* t = root;
	while (t != nullptr && t->data.key != x)
		if (t->data.key > x) t = t->left;
		else t = t->right;
	if (t == nullptr) return nullptr;
	else return (SET<KEY, OTHER> *)t;
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::insert(const SET<KEY, OTHER>& x)
{
	RedBlackNode* t, * parent, * grandP;
	if (root == nullptr)
	{
		root = new RedBlackNode(x, nullptr, nullptr, BLACK);
		return;
	}
	parent = grandP = t = root;
	while (true)
	{
		if (t)
		{
			if (t->left && t->left->colour == RED && t->right && t->right->colour == RED)
			{
				t->left->colour = t->right->colour = BLACK;
				t->colour = RED;
				insertAdjust(grandP, parent, t);
			}
			grandP = parent;
			parent = t;
			t = (t->data.key>x.key ? t->left : t->right);

		}
		else
		{
			t = new RedBlackNode(x);
			if (x.key < parent->data.key) parent->left = t;
			else parent->right = t;
			insertAdjust(grandP, parent, t);
			root->colour = BLACK;
			return;
		}
	}
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::remove(const KEY& x)
{
	KEY del = x;           //被删结点的关键字
	RedBlackNode* t, * p, * c;      //c为当前结点,p为父结点,t为兄弟结点

	if (root == nullptr) return;
	if (root->data.key == x && root->left == nullptr && root->right == nullptr)
	{
		delete root;
		root = nullptr;
		return;
	}

	p = c = t = root;
	while (true)
	{
		removeAdjust(p, c, t, del);
		if (c->data.key == del && c->left && c->right)
		{
			RedBlackNode* tmp = c->right;
			while (tmp->left) tmp = tmp->left;
			c->data = tmp->data;
			del = tmp->data.key;
			p = c;
			c = c->right;
			t = p->left;
			continue;
		}
		if (c->data.key == del)
		{
			if (p->left == c) p->left = nullptr;
			else p->right = nullptr;
			delete c;
			root->colour = BLACK;
			return;
		}
		p = c;
		c = (del < p->data.key ? p->left : p->right);
		t = (c == p->left ? p->right : p->left);

	}
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::makeEmpty(RedBlackNode*& t)
{
	if (t != nullptr)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = nullptr;
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::insertAdjust(RedBlackNode* gp, RedBlackNode* p, RedBlackNode* t)
{
	if (p->colour) return;       //没有出现连续红结点
	if (p == root)
	{
		p->colour = BLACK;
		return;
	}
	if (gp->left == p)
		if (p->left == t) LL(gp);
		else LR(gp);
	else
		if (p->right == t) RR(gp);
		else RL(gp);
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::removeAdjust(RedBlackNode*& p, RedBlackNode*& c, RedBlackNode*& t, KEY del)
{//c为当前结点,p为父结点, t为c的兄弟结点
	if (c->colour == RED) return;
	if (c==root)
		if (c->left && c->right && c->right->colour == c->left->colour)
		{
			c->colour = RED;
			c->left->colour = c->right->colour = BLACK;
			return;
		}

	if ((c->left && c->left->colour || c->left == nullptr) && (c->right && c->right->colour || c->right == nullptr))
	{//当前结点的左右孩子都是黑色
		if ((t->left && t->left->colour || t->left == nullptr)
			&& (t->right && t->right->colour || t->right == nullptr))
		{//c的兄弟结点t的左右孩子都是黑色
			p->colour = BLACK;
			t->colour = c->colour = RED;
		}
		else
		{
			if (p->left == t)
			{
				if (t->left && t->left->colour == RED)
				{
					t->left->colour = BLACK;
					LL(p);
					p = t;
				}
				else
				{
					LR(p);
					p = p->right;
					p->colour = BLACK;
				}
			}
			else if (t->right && t->right->colour == RED)
			{
				t->right->colour = BLACK;
				RR(p);
				p = t;
			}
			else
			{
				RL(p);
				p = p->left;
				p->colour = BLACK;
			}
			c->colour = RED;
		}
	}
	else
	{
		if (c->data.key == del)
		{
			if (c->left && c->right)
			{
				if (c->right->colour == BLACK)
				{
					LL(c);
					c = c->right;
				}
				return;
			}
			if (c->left)
			{
				LL(c);
				p = c;
				c = c->right;
			}
			else
			{
				RR(c);
				p = c;
				c = c->left;
			}
		}
		else
		{
			p = c;
			c = (del < p->data.key ? p->left : p->right);
			t = (c == p->left ? p->right : p->left);
			if (c->colour == BLACK)
			{
				if (t == p->right) RR(p);
				else LL(p);
				p = t;
				t = (c == p->left ? p->right : p->left);
				removeAdjust(p, c, t, del);
			}
		}
	}
} 

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::LL(RedBlackNode* gp)
{
	RedBlackNode* p = gp->left, * t = p->left;
	RedBlackNode tmp = *gp;
	gp->data = p->data;
	gp->left = t;
	gp->right = p;
	p->data = tmp.data;
	p->left = p->right;
	p->right = tmp.right;
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::LR(RedBlackNode* gp)
{//交换gp和t的值
	RedBlackNode* p = gp->left, * t = p->right;
	RedBlackNode tmp = *gp;
	gp->data = t->data;
	gp->right = t;
	p->right = t->left;
	t->data = tmp.data;
	t->left = t->right;
	t->right = tmp.right;
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::RL(RedBlackNode* gp)
{//交换gp和t的值
	RedBlackNode* p = gp->right, * t = p->left;
	RedBlackNode tmp = *gp;
	gp->data = t->data;
	gp->left = t;
	p->left = t->right;
	t->data = tmp.data;
	t->right = t->left;
	t->left = tmp.left;
}

template<class KEY, class OTHER>
void RedBlackTree<KEY, OTHER>::RR(RedBlackNode* gp)
{//交换gp和p的值
	RedBlackNode* p = gp->right, * t = p->right;
	RedBlackNode tmp = *gp;
	gp->data = p->data;
	gp->right = t;
	gp->left = p;
	p->data = tmp.data;
	p->right = p->left;
	p->left = tmp.left;
}

template<class KEY,class OTHER>
class AATree :public dynamicSearchTable<KEY, OTHER>
{
	struct AAnode
	{
		SET<KEY, OTHER> data;
		AAnode* left;
		AAnode* right;
		int level;         //结点的层次

		AAnode(const SET<KEY, OTHER>& thedata, AAnode* lt, AAnode* rt, int lv = 1)
			:data(thedata), left(lt), right(rt), level(lv) {}
	};
	AAnode* root;

public:
	AATree() { root = nullptr; }
	~AATree() { makeEmpty(root); }
	SET<KEY, OTHER>* find(const KEY& x)const;
	void insert(const SET<KEY, OTHER>& x) { insert(x, root); }
	void remove(const KEY& x) { remove(x, root); }
private:
	void insert(const SET<KEY, OTHER>& x, AAnode*& t);
	void remove(const KEY& x, AAnode*& t);
	void makeEmpty(AAnode* t);
	void LL(AAnode*& t);
	void RR(AAnode*& t);
	int min(int a, int b) { return a < b ? a : b; }
};

template<class KEY, class OTHER>
SET<KEY, OTHER>* AATree<KEY, OTHER>::find(const KEY& x) const
{
	AAnode* t = root;
	if (!t) return nullptr;
	while (t)
	{
		if (t->data.key == x) return (SET<KEY, OTHER>*)t;
		t = (t->data.key > x ? t->left : t->right);
	}
}

template<class KEY, class OTHER>
void AATree<KEY, OTHER>::insert(const SET<KEY, OTHER>& x, AAnode*& t)
{
	if (t == nullptr)
		t = new AAnode(x, nullptr, nullptr);
	else if (x.key < t->data.key)
		insert(x, t->left);
	else if (x.key > t->data.key)
		insert(x, t->right);
	else
		return;

	LL(t);
	RR(t);
}

template<class KEY, class OTHER>
void AATree<KEY, OTHER>::remove(const KEY& x, AAnode*& t)
{
	if (t == nullptr) return;
	if (x < t->data.key) remove(x, t->left);
	else if (t->data.key < x)
		remove(x, t->right);
	else if (t->left != nullptr && t->right != nullptr)
	{
		AAnode* tmp = t->right;
		while (tmp->left != nullptr) tmp = tmp->left;
		t->data = tmp->data;
		remove(t->data.key, t->right);
	}
	else
	{
		AAnode* oldNode = t;
		t = (t->left != nullptr ? t->left : t->right);
		delete oldNode;
		return;
	}
	//修改父节点的层次并调整平衡
	if (t->left == nullptr || t->right == nullptr) t->level = 1;
	else t->level = min(t->left->level, t->right->level) + 1;
	if (t->right != nullptr && t->right->level > t->level)
		t->right->level = t->level;
	LL(t);
	if (t->right != nullptr) LL(t->right);
	if (t->right != nullptr && t->right->right != nullptr) LL(t->right->right);
	RR(t);
	if (t->right != nullptr) RR(t->right);
}

template<class KEY, class OTHER>
void AATree<KEY, OTHER>::makeEmpty(AAnode* t)
{
	if (t != nullptr)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
}

template<class KEY, class OTHER>
void AATree<KEY, OTHER>::LL(AAnode*& t)
{
	if (t->left != nullptr && t->left->level == t->level)
	{
		AAnode* t1 = t->left;
		t->left = t1->right;
		t1->right = t;
		t = t1;
	}
}

template<class KEY, class OTHER>
void AATree<KEY, OTHER>::RR(AAnode*& t)
{
	if (t->right != nullptr && t->right->right != nullptr && t->right->right->level == t->level)
	{
		AAnode* t1 = t->right;
		t->right = t1->left;
		t1->left = t;
		t = t1;
		t->level++;
	}
}

template<class KEY,class OTHER>
class closeHashTable :public dynamicSearchTable<KEY, OTHER>
{
private:
	struct node
	{
		SET<KEY, OTHER> data;
		int state;        //0--empty 1--active 2--deleted
		node() { state = 0; }
	};
	node* aray;
	int size;
	int (*key)(const KEY& x);
	static int defaultKey(const int& x) { return x; }
public:
	closeHashTable(int length = 101, int (*f)(const KEY& x) = defaultKey);
	~closeHashTable() { delete[]aray; }
	SET<KEY, OTHER>* find(const KEY& x)const;
	void insert(const SET<KEY, OTHER>& x);
	void remove(const KEY& x);
};

template<class KEY, class OTHER>
closeHashTable<KEY, OTHER>::closeHashTable(int length, int(*f)(const KEY& x))
{
	size = length;
	key = f;
	aray = new node[size];
}

template<class KEY, class OTHER>
SET<KEY, OTHER>* closeHashTable<KEY, OTHER>::find(const KEY& x) const
{
	int initPos, pos;
	initPos = pos = key(x) % size;
	do
	{
		if (aray[pos].state == 0) return nullptr;
		if (aray[pos].state == 1 && aray[pos].data.key == x)
			return (SET<KEY, OTHER>*)&aray[pos];
		pos = (pos + 1) % size;
	} while (pos!=initPos);
}

template<class KEY, class OTHER>
void closeHashTable<KEY, OTHER>::insert(const SET<KEY, OTHER>& x)
{
	int initPos, pos;

	initPos = pos = key(x.key) % size;
	do
	{
		if (aray[pos].state != 1)
		{
			aray[pos].data = x;
			aray[pos].state = 1;
			return;
		}
		pos = (pos + 1) % size;
	} while (pos!=initPos);
}

template<class KEY, class OTHER>
void closeHashTable<KEY, OTHER>::remove(const KEY& x)
{
	int initPos, pos;
	initPos = pos = key(x) % size;

	do
	{
		if (aray[pos].state == 0) return;
		if (aray[pos].state == 1 && aray[pos].data.key == x)
		{
			aray[pos].state = 2;
			return;
		}
		pos = (pos + 1) % size;
	} while (pos!=initPos);
}

template<class KEY,class OTHER>
class openHashTable :public dynamicSearchTable<KEY, OTHER>
{
private:
	struct node
	{
		SET<KEY, OTHER> data;
		node* next;
		node(const SET<KEY, OTHER>& d, node* n = nullptr) { data = d; next = n; }
		node() { next = nullptr; }
	};

	node** aray;     //指针数组
	int size;
	int (*key)(const KEY& x);
	static int defaultKey(const int& x) { return x; }
public:
	openHashTable(int length = 101, int (*f)(const KEY& x) = defaultKey);
	~openHashTable();
	SET<KEY, OTHER>* find(const KEY& x)const;
	void insert(const SET<KEY, OTHER>& x);
	void remove(const KEY& x);
};

template<class KEY, class OTHER>
openHashTable<KEY, OTHER>::openHashTable(int length, int(*f)(const KEY& x))
{
	size = length;
	key = f;
	aray = new node * [size];
	for (int i = 0; i < size; ++i)
		aray[i] = nullptr;
}

template<class KEY, class OTHER>
openHashTable<KEY, OTHER>::~openHashTable()
{
	node* p, * q;
	for (int i = 0; i < size; ++i)
	{
		p = aray[i];
		while (p != nullptr)
		{
			q = p->next;
			delete p;
			p = q;
		}
	}
	delete[]aray;
}

template<class KEY, class OTHER>
SET<KEY, OTHER>* openHashTable<KEY, OTHER>::find(const KEY& x) const
{
	int pos;
	pos = key(x) % size;

	node* p = aray[pos];
	if (p == nullptr) return nullptr;
	while (p != nullptr && p->data.key != x) p = p->next;
	if (p == nullptr) return nullptr;
	else return (SET<KEY, OTHER>*) p;
}

template<class KEY, class OTHER>
void openHashTable<KEY, OTHER>::insert(const SET<KEY, OTHER>& x)
{
	int pos;
	node* p;
	pos = key(x.key) % size;
	aray[pos] = new node(x, aray[pos]);
}

template<class KEY, class OTHER>
void openHashTable<KEY, OTHER>::remove(const KEY& x)
{
	int pos;
	node* p, * q;
	pos = key(x) % size;

	p = aray[pos];
	//第一个就是要删除的元素
	if (p!=nullptr&&p->data.key == x)
	{
		aray[pos] = p->next;
		delete p;
		return;
	}
	//往后找
	while (p->next != nullptr)
	{
		if (p->next->data.key == x)
		{
			q = p->next;
			p->next = q->next;
			delete q;
			return;
		}
		p = p->next;
	}
}

#include <iostream>
#include <memory>
#include <math.h>

using namespace std;

class node {
public:
	int value;
	shared_ptr<node> right;
	shared_ptr<node> l_child;
	shared_ptr<node> r_child;
	node() {}
	node(int i) { value = i; }
};

class tree {
public:
	shared_ptr<node> root;
	int level;
	tree() { level = 0; }

	//Implement all member functions below
	tree(int n);//constructor for n-level tree
	//and initilize values as shown in the diagram; 0, 1, 2, ...
	//Note that root node is at level 1 and its value will be initialized to 0

	tree(const tree &T);//copy constructor
	~tree();//destructor
	tree(tree &&T); //move constructor

	tree(const initializer_list<int> &V);//The first number in V is tree level;
	//the rest are values from top to bottom and from left to right
	//For example, to create the tree with n=3 in the diagram,
	//tree T1 = {3, 0,1,2,3,4,5,6}; //where the first 3 is tree level, and the rest are values

	void operator= (const tree &R);//L-value operator=
	void operator= (tree &&R); //R-value operator=

	tree ThreeTimes(); //return a tree with all node value being three times
	friend ostream & operator<<(ostream &str, const tree &T);

	int sum(shared_ptr<node> p);//sum of node values in sub-tree rooted at p
	void delete_level(int i); // Delete nodes at level i. Some nodes at level(s) higher
	//than i will also be deleted accordingly. As described in class.  (Also see the
	//example in the main function.)

	shared_ptr<node> find(int i); //find the first node with value i and return
	//its address; if not found, return nullptr;
};

tree::tree(int n) : tree() {
	level = n;
	root = make_shared<node>(0);
	int total = (1 << level) - 2;
	int count = 0;
	shared_ptr<node> cur = root;
	while (++count <= total)
		cur = cur->right = make_shared<node>(count);
	shared_ptr<node> cur2 = cur = cur->right = root;
	while (cur2->right != root) {
		cur->l_child = cur2 = cur2->right;
		cur->r_child = cur2 = cur2->right;
		cur = cur->right;
	}
}

tree::tree(const tree& that) : tree() {
	level = that.level;
	root = make_shared<node>(that.root->value);
	int total = (1 << level) - 2;
	int count = 0;
	shared_ptr<node> cur = root;
	shared_ptr<node> cur2 = that.root;
	while (++count <= total) {
		cur = cur->right = make_shared<node>(cur2->right->value);
		cur2 = cur2->right;
	}
	cur2 = cur = cur->right = root;
	while (cur2->right != root) {
		cur->l_child = cur2 = cur2->right;
		cur->r_child = cur2 = cur2->right;
		cur = cur->right;
	}
}

tree::tree(tree&& that) {
	level = that.level;
	root = that.root;
	that.level = 0;
	that.root.reset();
}

void tree::operator=(const tree& that) {
	this->~tree();
	level = that.level;
	root = make_shared<node>(that.root->value);
	int total = (1 << level) - 2;
	int count = 0;
	shared_ptr<node> cur = root;
	shared_ptr<node> cur2 = that.root;
	while (++count <= total) {
		cur = cur->right = make_shared<node>(cur2->right->value);
		cur2 = cur2->right;
	}
	cur2 = cur = cur->right = root;
	while (cur2->right != root) {
		cur->l_child = cur2 = cur2->right;
		cur->r_child = cur2 = cur2->right;
		cur = cur->right;
	}
}

void tree::operator=(tree&& that) {
	this->~tree();
	level = that.level;
	root = that.root;
	that.level = 0;
	that.root.reset();
}

tree::tree(const initializer_list<int>& V) : tree(*V.begin()) {
	const int* p = V.begin();
	shared_ptr<node> cur = root;
	do cur->value = p[cur->value + 1]; while ((cur = cur->right) != root);
}

tree tree::ThreeTimes() {
	tree ret;
	ret.level = level;
	ret.root = make_shared<node>(root->value * 3);
	int total = (1 << level) - 2;
	int count = 0;
	shared_ptr<node> cur = ret.root;
	shared_ptr<node> cur2 = root;
	while (++count <= total) {
		cur = cur->right = make_shared<node>(cur2->right->value * 3);
		cur2 = cur2->right;
	}
	cur2 = cur = cur->right = ret.root;
	while (cur2->right != ret.root) {
		cur->l_child = cur2 = cur2->right;
		cur->r_child = cur2 = cur2->right;
		cur = cur->right;
	}
	return ret;
}

tree::~tree() {
	if (!root)
		return;
	shared_ptr<node> cur = root;
	while (cur->right != root)
		cur = cur->right;
	cur->right.reset();
}

ostream& operator<<(ostream& os, const tree &obj) {
	shared_ptr<node> cur = obj.root;
	do os << cur->value << ' '; while ((cur = cur->right) != obj.root);
	return os;
}

int tree::sum(shared_ptr<node> p) {
	int result = 0;
	int base = 0;
	while (p) {
		int count = 1 << base;
		shared_ptr<node> cur = p;
		while (--count >= 0) {
			result += cur->value;
			cur = cur->right;
		}
		++base;
		p = p->l_child;
	}
	return result;
}

void tree::delete_level(int i) {
	--level;
	int base = 1 << --i;
	int base2 = 0;
	shared_ptr<node> cur = root;
	shared_ptr<node> cur2 = root;
	while (--i) {
		cur = cur->l_child;
		cur2 = cur2->r_child;
	}
	// Last Level
	if (!cur->l_child->l_child) {
		cur2->right = root;
		while (cur != root) {
			cur->l_child.reset();
			cur->r_child.reset();
			cur = cur->right;
		}
	}
	// Not Last Level
	else {
		cur = cur2 = cur2->right = cur->l_child->l_child;
		while (cur != root) {
			for (int n = 0; n < base; ++n) {
				for (int j = 0; j < (1 << base2 + 1) - 1; ++j)
					cur2 = cur2->right;
				for (int j = 0; j < (1 << base2) - 1; ++j)
					cur = cur->right;
				cur = cur2 = cur->right = cur2->right;
			}
			++base2;
		}
	}
	cur = cur2 = root;
	while (cur2->right != root) {
		cur->l_child = cur2 = cur2->right;
		cur->r_child = cur2 = cur2->right;
		cur = cur->right;
	}
}

shared_ptr<node> tree::find(int i) {
	shared_ptr<node> cur = root;
	while (cur->right != root)
		if (cur->value == i)
			return cur;
		else
			cur = cur->right;
	return cur->value == i ? cur : nullptr;
}

int main() {
	tree T1(3);

	cout << T1 << endl; //will print 0 1 2 3 4 5 6

	tree T2 = { 4, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	cout << T2 << endl; //will print 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24

	tree T3(T2);
	cout << T3 << endl; //will print 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24

	tree T4;
	T4 = T3;
	cout << T4 << endl; //will print 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24

	T4 = T3.ThreeTimes();
	cout << T4 << endl;//will print 30 33 36 39 42 45 48 51 54 57 60 63 66 69 72

	T4.delete_level(3);
	cout << T4 << endl;//will print 30 33 36 51 57 63 69

	cout << T3.sum(T3.find(12)) << endl; //will print 133

	getchar();
	getchar();
	return 0;
}
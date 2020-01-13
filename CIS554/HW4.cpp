//HW4: Due Sunday (Oct. 13) at 11:59PM
//Implement functions of
// class linked_list
// class node
// class bag
// class item
//All member functions listed need to be implemented.
//You will also need to implement <initializer_list>, and the <needed overloaded operators>
//If a class needs <destructor>, then you need to implement destructor, <copy constructor>, <move constructor>, <Lvalue operator=> and <Rvalue operator=> for the class
//You will also need to overload <operator<< for vector, bag, linked_list, ThreeD> using <template>
//See the sample output for formats when implementing operator<< for all classes
#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

class IndexOutofBounds {
public:
	IndexOutofBounds() {
		cout << "!!! Index Out of Bounds !!!" << endl;
	}
};

class EmptyList {
public:
	EmptyList() {
		cout << "!!! Empty List !!!" << endl;
	}
};

template <class T>
class ThreeD {
public:
	T ht;
	T wid;
	T dep;

	ThreeD() { ht = wid = dep = 0; }
	ThreeD(T i) { ht = wid = dep = i; }
	ThreeD(T a, T b, T c) { ht = a; wid = b; dep = c; }

	// copy constructor
	ThreeD(const ThreeD<T>& that) {
		this->ht = that.ht;
		this->wid = that.wid;
		this->dep = that.dep;
	}

	// move constructor
	ThreeD(ThreeD<T>&& that) {
		this->ht = that.ht;
		this->wid = that.wid;
		this->dep = that.dep;
		that = nullptr;
	}

	// initializer list
	ThreeD(const initializer_list<T> initList) {
		if (initList.size() == 3) {
			const T* p = initList.begin();
			this->ht = p[0];
			this->wid = p[1];
			this->dep = p[2];
		}
	}

	// Lvalue =
	ThreeD& operator=(const ThreeD<T>& that) {
		this->ht = that.ht;
		this->wid = that.wid;
		this->dep = that.dep;
		return *this;
	}

	// Rvalue = 
	void operator=(ThreeD<T>&& that) {
		this->ht = that.ht;
		this->wid = that.wid;
		this->dep = that.dep;
	}

	//two objects are equal if their getVol() return the same value.
	T getVol() const { return ht * wid * dep; }

	friend ostream& operator<< (ostream& os, const ThreeD<T>& obj) {
		return os << '(' << ' ' << obj.ht << ',' << ' ' << obj.wid << ',' << ' ' << obj.dep << ' ' << ')';
	}
};

template <class T>
class node {
public:
	T value;
	node<T>* next;
	node<T>* previous;

	node<T>() { next = nullptr; previous = nullptr; }
	node<T>(T v) { value = v; next = nullptr; previous = nullptr; }

	friend ostream& operator<<(ostream& os, const node<T>& obj) {
		return os << obj.value << ' ';
	}
};

template <class T>
class linked_list {
public:
	node<T>* head; // position of first node
	node<T>* tail; // position of last node

	linked_list() { head = tail = nullptr; }

	// destructor
	~linked_list() {
		node<T>* cur = this->head;
		while (cur) {
			node<T>* prev = cur;
			cur = cur->next;
			delete(prev);
		}
		this->head = this->tail = nullptr;
	}

	// copy constructor
	linked_list(const linked_list<T>& that) : linked_list() {
		node<T>* thisCur = nullptr;
		node<T>* prev = nullptr;
		node<T>* cur = that.head;
		while (cur) {
			thisCur = new node<T>(cur->value);
			if (!this->head)
				this->head = thisCur;
			thisCur->previous = prev;
			if (thisCur->previous)
				thisCur->previous->next = thisCur;
			prev = thisCur;
			cur = cur->next;
		}
		this->tail = thisCur;
	}

	// move constructor
	linked_list(linked_list<T>&& that) {
		this->head = that.head;
		this->tail = that.tail;
		that.head = that.tail = nullptr;
	}

	// initializer list
	linked_list(const initializer_list<T> initList) : linked_list() {
		const T* p = initList.begin();
		node<T>* prev = nullptr;
		node<T>* cur = nullptr;
		while (p != initList.end()) {
			cur = new node<T>();
			cur->value = *p;
			if (!this->head)
				this->head = cur;
			cur->previous = prev;
			if (cur->previous)
				cur->previous->next = cur;
			prev = cur;
			++p;
		}
		this->tail = cur;
	}

	// Lvalue =
	linked_list& operator=(const linked_list<T>& that) {
		this->~linked_list();
		node<T>* thisCur = nullptr;
		node<T>* prev = nullptr;
		node<T>* cur = that.head;
		while (cur) {
			thisCur = new node<T>(cur->value);
			if (!this->head)
				this->head = thisCur;
			thisCur->previous = prev;
			if (thisCur->previous)
				thisCur->previous->next = thisCur;
			prev = thisCur;
			cur = cur->next;
		}
		this->tail = thisCur;
		return *this;
	}

	// Rvalue =
	void operator=(linked_list<T>&& that) {
		this->~linked_list();
		this->head = that.head;
		this->tail = that.tail;
		that.head = that.tail = nullptr;
	}

	void push_front(T t) {
		node<T>* newNode = new node<T>(t);
		if (!this->head)
			this->head = this->tail = newNode;
		else {
			this->head->previous = newNode;
			newNode->next = this->head;
			this->head = newNode;
		}
	}

	void push_back(T t) {
		node<T>* newNode = new node<T>(t);
		if (!this->tail)
			this->head = this->tail = newNode;
		else {
			this->tail->next = newNode;
			newNode->previous = this->tail;
			this->tail = newNode;
		}
	}

	friend ostream& operator<<(ostream& os, const linked_list<T>& obj) {
		node<T>* cur = obj.head;
		while (cur) {
			os << *cur;
			cur = cur->next;
		}
		return os;
	}
};

template <class X>
class item {
public:
	X data;
	item<X>* next;
	item<X>* previous;

	item<X>(X d) { data = d; next = nullptr; previous = nullptr; }

	friend ostream& operator<<(ostream& os, const item<X>& obj) {
		return os << obj.data << ' ';
	}
};

template <class X>
class bag {
public:
	item<X>* first; //position of first item
	item<X>* last; //positon of last item
	int num_items;
	int size() { return num_items; }

	bag() { first = nullptr; last = nullptr; num_items = 0; }

	// destrcutor
	~bag() {
		this->clear();
	}

	// copy constructor
	bag(const bag<X>& that) : bag() {
		item<X>* thisCur = nullptr;
		item<X>* prev = nullptr;
		item<X>* cur = that.first;
		while (cur) {
			thisCur = new item<X>(cur->data);
			if (!this->first)
				this->first = thisCur;
			thisCur->previous = prev;
			if (thisCur->previous)
				thisCur->previous->next = thisCur;
			prev = thisCur;
			cur = cur->next;
		}
		this->last = thisCur;
		this->num_items = that.num_items;
	}

	// move constructor
	bag(bag<X>&& that) {
		this->first = that.first;
		this->last = that.last;
		this->num_items = that.num_items;
		that.first = that.last = nullptr;
		that.num_items = 0;
	}

	// initializer list
	bag(const initializer_list<X> initList) : bag() {
		const X* p = initList.begin();
		item<X>* prev = nullptr;
		item<X>* cur = nullptr;
		while (p != initList.end()) {
			cur = new item<X>(*p);
			this->num_items++;
			if (!this->first)
				this->first = cur;
			cur->previous = prev;
			if (cur->previous)
				cur->previous->next = cur;
			prev = cur;
			++p;
		}
		this->last = cur;
	}

	// Lvalue = 
	bag& operator=(const bag<X>& that) {
		this->~bag();
		item<X>* thisCur = nullptr;
		item<X>* prev = nullptr;
		item<X>* cur = that.first;
		while (cur) {
			thisCur = new item<X>(cur->data);
			if (!this->first)
				this->first = thisCur;
			thisCur->previous = prev;
			if (thisCur->previous)
				thisCur->previous->next = thisCur;
			prev = thisCur;
			cur = cur->next;
		}
		this->last = thisCur;
		this->num_items = that.num_items;
		return *this;
	}

	// Rvalue = 
	void operator=(bag<X>&& that) {
		this->~bag();
		this->first = that.first;
		this->last = that.last;
		this->num_items = that.num_items;
		that.first = that.last = nullptr;
		that.num_items = 0;
	}

	// insert an item with data d to the back of the bag
	void push_back(X d) {
		item<X>* newNode = new item<X>(d);
		if (!this->last)
			this->first = this->last = newNode;
		else {
			this->last->next = newNode;
			newNode->previous = this->last;
			this->last = newNode;
		}
		this->num_items++;
	}

	// insert an item with data d to the front of the bag
	void push_front(X d) {
		item<X>* newNode = new item<X>(d);
		if (!this->first)
			this->first = this->last = newNode;
		else {
			this->first->previous = newNode;
			newNode->next = this->first;
			this->first = newNode;
		}
		this->num_items++;
	}

	// delete the last item in the bag
	void pop_back() {
		if (!this->last)
			return;
		this->num_items--;
		if (this->first == this->last) {
			delete(this->first);
			this->first = this->last = nullptr;
			return;
		}
		this->last = this->last->previous;
		delete(this->last->next);
		this->last->next = nullptr;
	}

	// delete the first item in the bag
	void pop_front() {
		if (!this->first)
			return;;
		this->num_items--;
		if (this->first == this->last) {
			delete(this->first);
			this->first = this->last = nullptr;
			return;
		}
		this->first = this->first->next;
		delete(this->first->previous);
		this->first->previous = nullptr;
	}

	// Access bag item with index. 
	X& operator[](int i) noexcept(false) {
		item<X>* cur = this->first;
		while (cur && i--)
			cur = cur->next;
		if (cur)
			return cur->data;
		else
			throw(IndexOutofBounds());			// !!! INDEX OUT OF BOUNDS ERROR !!!
	}

	// it returns the data of the first item in the bag.
	// if (num_items == 0) do nothing;
	// else return first->data;
	X front() noexcept(false) {
		if (this->first)
			return this->first->data;
		else
			throw(EmptyList());					// ERROR
	}

	// it returns the data of the last item in the bag
	/*
	if (num_items == 0) do nothing;
	else return last->data;
	*/
	X back() noexcept(false) {
		if (this->last)
			return this->last->data;
		else
			throw(EmptyList());					// ERROR
	}

	// Delete all items in the bag
	void clear() {
		item<X>* cur = this->first;
		while (cur) {
			item<X>* prev = cur;
			cur = cur->next;
			delete(prev);
		}
		this->first = this->last = nullptr;
		this->num_items = 0;
	}

	// return nullptr if no match for find or if the bag is empty.
	// else, return the position of the matched item
	item<X>* find(X d) {
		item<X>* cur = this->first;
		while (cur) {
			if (cur->data == d)
				break;
			cur = cur->next;
		}
		return cur;
	}

	void erase(int index) {
		item<X>* cur = this->first;
		while (cur && index--)
			cur = cur->next;
		if (cur) {
			if (cur->previous)
				cur->previous->next = cur->next;
			if (cur->next)
				cur->next->previous = cur->previous;
			if (this->first == cur)
				this->first = cur->next;
			if (this->last == cur)
				this->last = cur->previous;
			delete(cur);
			cur = nullptr;
			this->num_items--;
		}
	}

	void erase(item<X>* p) {
		item<X>* cur = this->first;
		while (cur && !(cur->data == p->data))
			cur = cur->next;
		if (cur) {
			if (cur->previous)
				cur->previous->next = cur->next;
			if (cur->next)
				cur->next->previous = cur->previous;
			if (this->first == cur)
				this->first = cur->next;
			if (this->last == cur)
				this->last = cur->previous;
			delete(cur);
			cur = nullptr;
			this->num_items--;
		}
	}

	// insert data d to the position before p and return the position of the inserted item
	item<X>* insert(item<X>* p, X d) {
		item<X>* newNode = new item<X>(d);
		item<X>* cur = this->first;
		while (cur && !(cur->data == p->data))
			cur = cur->next;
		if (cur) {
			if (cur->previous)
				cur->previous->next = newNode;
			newNode->previous = cur->previous;
			cur->previous = newNode;
			newNode->next = cur;
			if (this->first == cur)
				this->first = newNode;
		}
		else {
			if (this->last)
				this->last->next = newNode;
			newNode->previous = this->last;
			this->last = newNode;
			if (!this->first)
				this->first = this->last;
		}
		this->num_items++;
		return newNode;
	}

	friend ostream& operator<<(ostream& os, const bag<X>& obj) {
		item<X>* cur = obj.first;
		while (cur) {
			os << *cur;
			cur = cur->next;
		}
		return os;
	}
};

template<class T1, class T2>
bool operator==(const ThreeD<T1>& lhs, const ThreeD<T2>& rhs) {
	return lhs.getVol() == rhs.getVol();
}

template<class T1, class T2>
bool operator==(const linked_list<T1>& lhs, const linked_list<T2>& rhs) {
	node<T1>* p1 = lhs.head;
	node<T2>* p2 = rhs.head;
	while (p1 && p2) {
		if (!(p1->value == p2->value))
			return false;
		p1 = p1->next;
		p2 = p2->next;
	}
	return !p1 && !p2;
}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& obj) {
	os << '[';
	for (auto iter = obj.begin();;) {
		os << *iter;
		if (++iter != obj.end())
			os << ',' << ' ';
		else
			break;
	}
	return os << ']';
}

template<class T>
ostream& operator<<(ostream& os, const list<T>& obj) {
	os << '[';
	for (auto iter = obj.begin();;) {
		os << *iter;
		if (++iter != obj.end())
			os << ',' << ' ';
		else
			break;
	}
	return os << ']';
}

int main() {
	bag<double> bag_d;
	bag_d.push_back(5.5);
	bag_d.push_back(6.6);
	bag_d.push_front(4.4);
	bag_d.push_front(3.3);
	bag_d.pop_front();
	bag_d.pop_back();
	cout << bag_d << endl;
	bag<int> bag_i;
	bag_i.push_back(5);
	bag_i.push_back(6);
	bag_i.push_front(4);
	bag_i.push_front(3);
	bag_i.pop_front();
	bag_i.pop_back();
	cout << bag_i << endl;

	ThreeD<int> td3(3), td4(4), td5(5), td6(6), td7(100, 200, 300);
	bag<ThreeD<int>> bag_3D;
	bag_3D.push_back(td5);
	bag_3D.push_back(td6);
	bag_3D.push_front(td4);
	bag_3D.push_front(td3);
	bag_3D.pop_front();
	bag_3D.pop_back();
	cout << bag_3D << endl;;
	cout << bag_3D.front() << bag_3D.back();
	cout << bag_3D[0] << " " << bag_3D[1] << endl;
	bag_3D[1] = td7;
	cout << bag_3D[0] << " " << bag_3D[1] << endl;
	bag_3D.clear();
	// bag_3D was cleared, so "cout << bag_3D << endl;" will only print an empty line
	cout << bag_3D << endl;
	cout << bag_3D.size() << endl;
	linked_list<string> ls_1;
	ls_1.push_front("David");
	ls_1.push_front("John");
	ls_1.push_front("Pat");
	ls_1.push_front("Ben");
	ls_1.push_front("Jeff");
	cout << ls_1 << endl;

	linked_list<string>ls_2;
	ls_2.push_front("Wendy");
	ls_2.push_front("Mary");
	ls_2.push_front("Nancy");
	ls_2.push_front("Jennifer");
	cout << ls_2 << endl;

	bag<linked_list<string>> bag_string;
	bag_string.push_back(ls_1);
	bag_string.push_back(ls_2);

	cout << bag_string << endl;
	cout << bag_string[1] << endl;

	ThreeD<double> t10(3.2, 7.4, 8.9), t11(5.6, 7.7, 2.987), t12(4.6, 7.5, 3.1416), t13(55.6, 66.8, 333.45);
	linked_list<ThreeD<double>> LTD1;
	LTD1.push_front(t10);
	LTD1.push_front(t11);
	linked_list<ThreeD<double>> LTD2;
	LTD2.push_front(t13);
	LTD2.push_front(t12);
	LTD2.push_front(t10);
	LTD2.push_front(t11);

	bag<linked_list<ThreeD<double>>> BLTD;
	BLTD.push_back(LTD1);
	BLTD.push_back(LTD2);
	cout << BLTD << endl;

	item<linked_list<ThreeD<double>>>* p2;
	p2 = BLTD.find(LTD1);
	BLTD.erase(p2);
	cout << BLTD << endl;
	BLTD.push_back(LTD1);
	cout << BLTD << endl;
	BLTD.erase(0);
	cout << BLTD << endl;

	vector<ThreeD<int>> V1 = { { 1,2,3 },{ 4,5,6 },{ 7,8,9 } };
	cout << V1 << endl;

	vector<bag<ThreeD<int>>> V2 = { { { 1,2,3 },{ 4,5,6 },{ 7,8,9 } },{ { 20,30,40 },{ 11,22, 33 } } };
	cout << V2 << endl;

	list<bag<linked_list<int>>> V3 = { { { 1, 2, 3 },{ 4, 5 } },{ { 6,7 },{ 8, 9, 10 } } };
	cout << V3 << endl;

	bag<int> B10 = { 1,2,3,4,5 };
	bag<int> B11 = B10, B12;
	B12 = B10;
	cout << B10 << endl;
	B10.first->data = 1000;
	cout << B10 << endl;
	cout << B11 << endl;
	cout << B12 << endl;

	bag<list<linked_list<ThreeD<int>>>> B13 = { { { { 1,2,3 },{ 4,5,6 } },{ { 7,8,9 },{ 10,11,12 },{ 13,14,15 } } },{ { { 16,17,18 },{ 19,20,21 },{ 22,23,24 } },{ { 25,26,27 },{ 28,29,30 } },{ { 31,32,33 },{ 34,35,36 },{ 37,38,39 },{ 40,41,42 } } } };
	cout << B13 << endl;

	bag<bag<bag<ThreeD<int>>>> B14 = { { { { 1,2,3 },{ 4,5,6 } },{ { 7,8,9 },{ 10,11,12 },{ 13,14,15 } } },{ { { 16,17,18 },{ 19,20,21 },{ 22,23,24 } },{ { 25,26,27 },{ 28,29,30 } },{ { 31,32,33 },{ 34,35,36 },{ 37,38,39 },{ 40,41,42 } } } };
	cout << B14 << endl;

	bag<linked_list<int>>* p10 = new bag<linked_list<int>>({ { 1,2,3,4 },{ 5,6,7 } });
	cout << *p10 << endl;
	delete p10;
	bag<list<linked_list<ThreeD<int>>>> B15 = { { { { 1,2,3 },{ 4,5,6 } },{ { 7,8,9 },{ 10,11,12 },{ 13,14,15 } } },{ { { 16,17,18 },{ 19,20,21 },{ 22,23,24 } },{ { 25,26,27 },{ 28,29,30 } },{ { 31,32,33 },{ 34,35,36 },{ 37,38,39 },{ 40,41,42 } } } };
	cout << B15 << endl;
	B15.erase(1);
	cout << B15 << endl;
	bag<list<linked_list<ThreeD<int>>>> B16 = { { { { 1,2,3 },{ 4,5,6 } },{ { 7,8,9 },{ 10,11,12 },{ 13,14,15 } } },{ { { 16,17,18 },{ 19,20,21 },{ 22,23,24 } },{ { 25,26,27 },{ 28,29,30 } },{ { 31,32,33 },{ 34,35,36 },{ 37,38,39 },{ 40,41,42 } } } };
	list<linked_list<ThreeD<int>>> V4 = { { { 6,1,1 },{ 2,5,12 } },{ { 9,8,7 },{ 11,10,12 },{ 26,7,15 } } };
	list<linked_list<ThreeD<int>>> V5 = { { { 6,1,1 },{ 2,5,12 } },{ { 9,8,7 },{ 11,10,12 },{ 26,7,5 } },{ { 6,1,1 },{ 2,5,12 } } };
	B16.insert(B16.first, V5);
	cout << B16.front().size() << endl;
	item<list<linked_list<ThreeD<int>>>>* p20 = B16.find(V4);
	if (p20 != nullptr)
		cout << (p20->data).size() << endl;
	getchar();
	getchar();
	return 0;
}

//The following is a screenshot of a sample output
/*
4.4 5.5
4 5
( 4, 4, 4 ) ( 5, 5, 5 )
( 4, 4, 4 )( 5, 5, 5 )( 4, 4, 4 ) ( 5, 5, 5 )
( 4, 4, 4 ) ( 100, 200, 300 )
0
Jeff Ben Pat John David
Jennifer Nancy Mary Wendy
Jeff Ben Pat John David  Jennifer Nancy Mary Wendy
Jennifer Nancy Mary Wendy
( 5.6, 7.7, 2.987 ) ( 3.2, 7.4, 8.9 )  ( 5.6, 7.7, 2.987 ) ( 3.2, 7.4, 8.9 ) ( 4.6, 7.5, 3.1416 ) ( 55.6, 66.8, 333.45 )
( 5.6, 7.7, 2.987 ) ( 3.2, 7.4, 8.9 ) ( 4.6, 7.5, 3.1416 ) ( 55.6, 66.8, 333.45 )
( 5.6, 7.7, 2.987 ) ( 3.2, 7.4, 8.9 ) ( 4.6, 7.5, 3.1416 ) ( 55.6, 66.8, 333.45 )  ( 5.6, 7.7, 2.987 ) ( 3.2, 7.4, 8.9 )
( 5.6, 7.7, 2.987 ) ( 3.2, 7.4, 8.9 )
[( 1, 2, 3 ), ( 4, 5, 6 ), ( 7, 8, 9 )]
[( 1, 2, 3 ) ( 4, 5, 6 ) ( 7, 8, 9 ) , ( 20, 30, 40 ) ( 11, 22, 33 ) ]
[1 2 3  4 5  , 6 7  8 9 10  ]
1 2 3 4 5
1000 2 3 4 5
1 2 3 4 5
1 2 3 4 5
[( 1, 2, 3 ) ( 4, 5, 6 ) , ( 7, 8, 9 ) ( 10, 11, 12 ) ( 13, 14, 15 ) ] [( 16, 17, 18 ) ( 19, 20, 21 ) ( 22, 23, 24 ) , ( 25, 26, 27 ) ( 28, 29, 30 ) , ( 31, 32, 33 ) ( 34, 35, 36 ) ( 37, 38, 39 ) ( 40, 41, 42 ) ]
( 1, 2, 3 ) ( 4, 5, 6 )  ( 7, 8, 9 ) ( 10, 11, 12 ) ( 13, 14, 15 )   ( 16, 17, 18 ) ( 19, 20, 21 ) ( 22, 23, 24 )  ( 25, 26, 27 ) ( 28, 29, 30 )  ( 31, 32, 33 ) ( 34, 35, 36 ) ( 37, 38, 39 ) ( 40, 41, 42 )
1 2 3 4  5 6 7
[( 1, 2, 3 ) ( 4, 5, 6 ) , ( 7, 8, 9 ) ( 10, 11, 12 ) ( 13, 14, 15 ) ] [( 16, 17, 18 ) ( 19, 20, 21 ) ( 22, 23, 24 ) , ( 25, 26, 27 ) ( 28, 29, 30 ) , ( 31, 32, 33 ) ( 34, 35, 36 ) ( 37, 38, 39 ) ( 40, 41, 42 ) ]
[( 1, 2, 3 ) ( 4, 5, 6 ) , ( 7, 8, 9 ) ( 10, 11, 12 ) ( 13, 14, 15 ) ]
3
2
*/
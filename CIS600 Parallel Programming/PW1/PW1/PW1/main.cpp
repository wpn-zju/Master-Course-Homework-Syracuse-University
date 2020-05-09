//CIS600/CSE691  HW1
//Due: 11:59PM, Friday(1/31)

/*
Implement the two member functions: merge_sort and merge, as defined below for a sequential merge sort.
Note that the merge will be called by merge_sort.

In implementing both functions, you are only allowed to modify "next" and "previous" of nodes, but not "values" of nodes.
You are not allowed to use any external structures such as array, linked list, etc.
You are not allowed to create any new node.
You are not allowed to create any new function.


After completing the above sequential version,  create a parallel version, by using two additional threads to speed up the merge sort.
You have to use the two functions you have implemented above.  You are not allowed to create new functions. Extra work will be needed in main function.

In your threaded implementation, you are allowed to introduce an extra node and a global pointer to the node.

It is alright if your implementation does not require the extra node or global pointer to node.

*/

#include <iostream>
#include <thread>

using namespace std;

class node {
public:
	int value;
	node* next;
	node* previous;
	node(int i) { value = i; next = previous = nullptr; }
	node() { next = previous = nullptr; }
};

class doubly_linked_list {
public:
	int num_nodes;
	node* head;
	node* tail;
	doubly_linked_list() { num_nodes = 0; head = tail = nullptr; }
	void make_random_list(int m, int n);
	void print_forward();
	void print_backward();

	//Recursively merge sort i numbers starting at node pointed by p
	void merge_sort(node* p, int i);//in-place recursive merge sort

	//Merge i1 numbers starting at node pointed by p1 with i2 numbers
	//starting at node pointed by p2
	void merge(node* p1, int i1, node* p2, int i2);

	//~doubly_linked_list() {
	//	while (head) {
	//		node* tmp = head->next;
	//		delete(head);
	//		head = tmp;
	//	}

	//	tail = nullptr;
	//	num_nodes = 0;
	//}
};

void doubly_linked_list::merge_sort(node* p, int i) {
	// Return if only 1 node
	if (i <= 1)
		return;

	// Divide into two parts
	int half = i >> 1;
	node* mid = p;
	for (int n = 0; n < half; ++n)
		mid = mid->next;

	// The previous node of the first node
	node* prev = p->previous;

	// The next node of the last node
	node* next = p;
	for (int n = 0; n < i; ++n)
		next = next->next;

	// Merge the first part
	merge_sort(p, half);

	// Find the first node after mergesort
	while (p->previous != prev)
		p = p->previous;

	// Merge the second part
	merge_sort(mid, i - half);

	// Find the last node after mergesort
	while (mid->next != next)
		mid = mid->next;

	// Find the middle node after mergesort
	for (int n = 0; n < i - half - 1; ++n)
		mid = mid->previous;

	// Merge two parts
	merge(p, half, mid, i - half);

	// Reset head and tail
	if (i == num_nodes) {
		node* tmp = p;
		while (tmp->previous)
			tmp = tmp->previous;
		head = tmp;
		while (tmp->next)
			tmp = tmp->next;
		tail = tmp;
	}
}

void doubly_linked_list::merge(node* p1, int i1, node* p2, int i2) {
	int index1 = 0, index2 = 0;

	node* p3 = p1;
	node* p4 = p2;
	for (int i = 0; i < i1 - 1; ++i)
		p3 = p3->next;
	for (int i = 0; i < i2 - 1; ++i)
		p4 = p4->next;

	if (p1->value > p2->value) {
		node* tmp = p2->previous;
		p2->previous = p1->previous;
		p1->previous = tmp;
	}

	if (p3->value > p4->value) {
		node* tmp = p3->next;
		p3->next = p4->next;
		p4->next = tmp;
	}

	node* ptr = nullptr;

	if (p1->value <= p2->value) {
		ptr = p1;
		p1 = p1->next;
		++index1;
	}
	else {
		ptr = p2;
		p2 = p2->next;
		++index2;
	}

	while (index1 != i1 || index2 != i2) {
		if (index2 == i2 || index1 != i1 && p1->value <= p2->value) {
			ptr->next = p1;
			p1->previous = ptr;
			p1 = p1->next;
			++index1;
		}
		else {
			ptr->next = p2;
			p2->previous = ptr;
			p2 = p2->next;
			++index2;
		}
		ptr = ptr->next;
	}
}

void doubly_linked_list::make_random_list(int m, int n) {
	for (int i = 0; i < m; i++) {
		node * p1 = new node(rand() % n);
		p1->previous = tail;
		if (tail != nullptr) tail->next = p1;
		tail = p1;
		if (head == nullptr) head = p1;
		num_nodes++;
	}
}

void doubly_linked_list::print_forward() {
	cout << endl;
	node * p1 = head;
	while (p1 != nullptr) {
		cout << p1->value << " ";
		p1 = p1->next;
	}
}

void doubly_linked_list::print_backward() {
	cout << endl;
	node * p1 = tail;
	while (p1 != nullptr) {
		cout << p1->value << " ";
		p1 = p1->previous;
	}
}

int main() {
	/*
	Implement the merge_sort and merge_functions defined above to complete a sequential version of
	merge sort.
	*/

	doubly_linked_list d1, d2;
	d1.make_random_list(30, 20);
	d1.print_forward();
	d1.print_backward();

	d1.merge_sort(d1.head, d1.num_nodes);
	d1.print_forward();
	d1.print_backward();


	d2.make_random_list(50, 40);
	d2.print_forward();
	d2.print_backward();

	/*
	Create two additional threads to speed up the merge sort.
	You have to still use the same merge_sort and merge functions implemented above.
	You will need to do some extra work within main funciton.
	*/

	// Sequential Version
	// d2.merge_sort(d2.head, d2.num_nodes);

	// Parallel Version
	if (d2.num_nodes) {
		node* mid = d2.head;
		int half = d2.num_nodes >> 1;
		for (int i = 0; i < half; ++i)
			mid = mid->next;

		thread th1(&doubly_linked_list::merge_sort, ref(d2), d2.head, half);
		thread th2(&doubly_linked_list::merge_sort, ref(d2), mid, d2.num_nodes - half);
		th1.join();
		th2.join();

		while (d2.head->previous)
			d2.head = d2.head->previous;

		while (d2.tail->next)
			d2.tail = d2.tail->next;

		mid = d2.tail;
		for (int i = 0; i < d2.num_nodes - half - 1; ++i)
			mid = mid->previous;

		d2.merge(d2.head, half, mid, d2.num_nodes - half);

		while (d2.head->previous)
			d2.head = d2.head->previous;

		while (d2.tail->next)
			d2.tail = d2.tail->next;
	}

	d2.print_forward();
	d2.print_backward();
	return 0;
}

//#include <ctime>
//
//int main() {
//	clock_t begin, end;
//	begin = clock();
//
//	/*
//	Implement the merge_sort and merge_functions defined above to complete a sequential version of
//	merge sort.
//	*/
//
//	doubly_linked_list d2;
//	d2.make_random_list(1000000, 1000000);
//	//d2.print_forward();
//	//d2.print_backward();
//
//	/*
//	Create two additional threads to speed up the merge sort.
//	You have to still use the same merge_sort and merge functions implemented above.
//	You will need to do some extra work within main funciton.
//	*/
//
//#ifdef SEQ
//	d2.merge_sort(d2.head, d2.num_nodes);
//#endif
//
//#ifndef SEQ
//	node* mid = d2.head;
//	int half = d2.num_nodes >> 1;
//	for (int i = 0; i < half; ++i)
//		mid = mid->next;
//
//	thread th1(&doubly_linked_list::merge_sort, ref(d2), d2.head, half);
//	thread th2(&doubly_linked_list::merge_sort, ref(d2), mid, d2.num_nodes - half);
//	th1.join();
//	th2.join();
//
//	while (d2.head->previous)
//		d2.head = d2.head->previous;
//
//	while (d2.tail->next)
//		d2.tail = d2.tail->next;
//
//	mid = d2.tail;
//	for (int i = 0; i < d2.num_nodes - half - 1; ++i)
//		mid = mid->previous;
//
//	d2.merge(d2.head, half, mid, d2.num_nodes - half);
//
//	while (d2.head->previous)
//		d2.head = d2.head->previous;
//
//	while (d2.tail->next)
//		d2.tail = d2.tail->next;
//#endif // !SEQ
//
//	// d2.print_forward();
//	// d2.print_backward();
//
//	end = clock();
//	cout << end - begin << endl;
//
//	return 0;
//}
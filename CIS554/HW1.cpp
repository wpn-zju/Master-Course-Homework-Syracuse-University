//HW1 by Peinan Weng
//SU Net ID: peweng SUID: 914854061
//CIS554 HW1
//Due: 11:59PM, Friday ( September 13)
//Do not modify main funaiton.
//Do not introduce new functions
//In-place implementation is required.

#include <iostream>
using namespace std;

class Node {
public:
	int value;
	Node* next;
	Node* previous;
	Node(int i) { value = i; next = previous = nullptr; }
	Node() { next = previous = nullptr; }
};

class DoublyLinkedList {
public:
	Node* head;
	Node* tail;
	DoublyLinkedList() { head = tail = nullptr; }
	void makeRandomList(int m, int n);
	void printForward();
	void printBackward();

	//implement the following member functions:

	void sort();
	//sort all values based on frequency in ascending order.
	//In case of ties, the smaller values will appear earlier
	//Example:  for list with values  7 6 12 4 33 12 6 6 7
	//sorted results: 4 33 7 7 12 12 6 6 6


	//If your algorithm is inefficient, you might lose points.
	//You will not modify L.
};

void DoublyLinkedList::sort() {
	if (!head)
		return;

	// First - Sort by value.
	Node *ptr1 = nullptr, *ptr2 = nullptr, *ptr3 = nullptr, *ptr4 = nullptr;

	// Loop Control Variables
	bool control = false;

	// Bubble Loop
	do {
		control = false;

		ptr1 = head;
		ptr2 = head->next;

		// One Loop
		while (ptr2) {
			if (ptr1->value > ptr2->value) {
				ptr2->previous = ptr1->previous;
				if (ptr1->previous)
					ptr1->previous->next = ptr2; 
				ptr1->previous = ptr2;
				ptr1->next = ptr2->next;
				if (ptr2->next)
					ptr2->next->previous = ptr1;
				ptr2->next = ptr1;
				if (head == ptr1)
					head = ptr2;
				if (tail == ptr2)
					tail = ptr1;

				ptr2 = ptr1->next;
				control = true;
			}
			else {
				ptr1 = ptr1->next;
				ptr2 = ptr2->next;
			}
		}
	} while (control);

	// Print the sorted linked list
	// printForward();
	// printBackward();

	// Second - Sort by frequency.
	// Pointers
	// We set 2 intervals with nodes of equal values to compare.
	// ptr1 : Start of the first interval.
	// ptr2 : End of the first interval.
	// ptr3 : Start of the second interval.
	// ptr4 : End of the second interval.
	// Actually we only need 3 pointers as ptr2->next == ptr3, use 4 pointers to make it more comprehensible.
	ptr1 = ptr2 = nullptr;

	control = false;

	// Bubble Loop
	do {
		control = false;

		// Temp Variables Reset
		// Pointer Reset
		ptr1 = ptr2 = head;
		ptr3 = ptr4 = head->next;
		// l1 and l2 denote the length of those 2 intervals.
		int l1 = 1, l2 = 1;
		// v1 and v2 denote the value of those 2 intervals.
		int v1 = 0, v2 = 0;
		// 2 boolean variable to control the loop.
		bool ptr1Set = false, ptr3Set = false;

		// One Loop
		while (ptr4) {
			// Order : Set first interval -> Set second interval -> compare and swap / move on
			// Check if first interval has been set.
			if (!ptr1Set) {
				if (ptr2->next && ptr2->next->value == ptr2->value) {
					ptr2 = ptr2->next;
					l1++;
					v1 = ptr2->value;
				}
				else {
					ptr3 = ptr4 = ptr2->next;
					ptr1Set = true;
				}
			}
			// Check if second interval has been set.
			else if (!ptr3Set) {
				if (ptr4->next && ptr4->next->value == ptr4->value) {
					ptr4 = ptr4->next;
					l2++;
					v2 = ptr4->value;
				}
				else {
					v2 = ptr4->value;
					ptr3Set = true;
				}
			}
			// Both intervals were set, do compare now.
			else {
				if (l1 > l2 || l1 == l2 && v1 > v2) {
					// Swap 2 intervals.
					ptr3->previous = ptr1->previous;
					if (ptr1->previous)
						ptr1->previous->next = ptr3;
					ptr1->previous = ptr4;
					ptr2->next = ptr4->next;
					if (ptr4->next)
						ptr4->next->previous = ptr2;
					ptr4->next = ptr1;
					if (ptr1 == head)
						head = ptr3;
					if (ptr4 == tail)
						tail = ptr2;

					// Update control variables
					ptr3 = ptr4 = ptr2->next;
					ptr3Set = false;
					l2 = 1;
					v2 = 0;
					control = true;
				}
				else{
					// Keep the linked list and move on.
					ptr1 = ptr3;
					ptr2 = ptr4;

					// Update control variables
					ptr3 = ptr4 = ptr2->next;
					ptr3Set = false;
					l1 = l2;
					l2 = 1;
					v1 = v2;
					v2 = 0;
				}
			}
		}
	} while (control);	
}

void DoublyLinkedList::makeRandomList(int m, int n) {
	for (int i = 0; i < m; i++) {
		Node* p1 = new Node(rand() % n);
		p1->previous = tail;
		if (tail != nullptr) tail->next = p1;
		tail = p1;
		if (head == nullptr) head = p1;
	}
}

void DoublyLinkedList::printForward() {
	cout << endl;
	Node* p1 = head;
	while (p1 != nullptr) {
		cout << p1->value << " ";
		p1 = p1->next;
	}
}

void DoublyLinkedList::printBackward() {
	cout << endl;
	Node* p1 = tail;
	while (p1 != nullptr) {
		cout << p1->value << " ";
		p1 = p1->previous;
	}
}

int main() {
	DoublyLinkedList d1;
	d1.makeRandomList(20, 50);
	d1.printForward();
	d1.printBackward();

	d1.sort();
	d1.printForward();
	d1.printBackward();

	getchar();
	getchar();
	return 0;
}
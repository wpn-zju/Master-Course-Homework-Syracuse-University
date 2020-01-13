//CIS554 HW2 Second 
//Due: 11:59PM Tuesday ( October 22 )
/*
-   Implement the functions reverse of the DoublyLinkedList class which is sorted in frequency.
The description of the reverse function is given in the class.
-   In the implementation reverse, you are not allowed to modify values of nodes.
You only can modify pointers of nodes.
-   Again, you are not allowed to create new nodes in your implementation.  No external structures (such as arrays,
linked list, map, etc.) are allowed.  You can use some temporary pointes or variables for your
implementation.
-   In implementaing reverse, you are not allowed to do sorting from scratch.

When you submit your code, do not modify main function.

Do not introduce new functions
In-place implementation is required.  (This means all operations should directly performed on the list.)
*/
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
	DoublyLinkedList(const std::initializer_list<int>& V);
	void printForward();
	void printBackward();
	void reverse();
	/*
	Given an already sorted DoublyLinkedList in frequency (i.e., the result of HW1), make
	the numbers with the same frequency be sorted in decending order.
	For example, 4 9 33 7 7 12 12 6 6 6 will be re-arranged into 33 9 4 12 12 7 7 6 6 6
	*/
};

DoublyLinkedList::DoublyLinkedList(const std::initializer_list<int>& V) {
	head = tail = nullptr;
	for (auto& value : V) {
		Node* p1 = new Node(value);
		p1->previous = tail;
		if (tail != nullptr)
			tail->next = p1;
		tail = p1;
		if (head == nullptr)
			head = p1;
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

void DoublyLinkedList::reverse() {
	if (!head)
		return;

	// First - Sort by value.
	Node *ptr1 = nullptr, *ptr2 = nullptr, *ptr3 = nullptr, *ptr4 = nullptr;

	// Loop Control Variables
	bool control = false;

	// Pointers
	// We set 2 intervals with nodes of equal values to compare.
	// ptr1 : Start of the first interval.
	// ptr2 : End of the first interval.
	// ptr3 : Start of the second interval.
	// ptr4 : End of the second interval.
	// Actually we only need 3 pointers as ptr2->next == ptr3, use 4 pointers to make it more comprehensible.
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
		int v1 = head->value, v2 = INT_MAX;
		// 2 boolean variable to control the loop.
		bool ptr1Set = false, ptr3Set = false;

		// One Loop
		while (ptr4) {
			// Order : Set first interval -> Set second interval -> compare and swap / move on
			// Check if first interval has been set.
			if (!ptr1Set) {
				if (ptr2->next && ptr2->next->value == ptr2->value) {
					ptr2 = ptr2->next;
					++l1;
				}
				else {
					ptr1Set = true;
					ptr3 = ptr4 = ptr2->next;
				}
			}
			// Check if second interval has been set.
			else if (!ptr3Set) {
				v2 = ptr4->value;
				if (ptr4->next && ptr4->next->value == ptr4->value) {
					ptr4 = ptr4->next;
					++l2;
				}
				else
					ptr3Set = true;
			}
			// Both intervals were set, do compare now.
			else {
				if (l1 == l2 && v1 < v2) {
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
					control = true;
				}
				else {
					// Keep the linked list and move on.
					ptr1 = ptr3;
					ptr2 = ptr4;

					// Update control variables
					l1 = l2;
					v1 = v2;
				}
				l2 = 1;
				v2 = INT_MAX;
				ptr3Set = false;
				ptr3 = ptr4 = ptr2->next;
			}
		}
	} while (control);
}

int main() {
	DoublyLinkedList d{ 1,9,15,2,2,8,8,0,0,0,0,3,3,3,3,3,19,19,19,19,19,11,11,11,11,11,4,4,4,4,4,4,4 };
	d.reverse();

	d.printForward();
	d.printBackward();

	DoublyLinkedList d1{ 1,9,9,9 };
	d1.reverse();

	d1.printForward();
	d1.printBackward();

	DoublyLinkedList d2{ 0,0,3,3,3,4,4,4 };
	d2.reverse();

	d2.printForward();
	d2.printBackward();

	DoublyLinkedList d3{ 8,8,9,9,13,13 };
	d3.reverse();

	d3.printForward();
	d3.printBackward();

	DoublyLinkedList d4{ };
	d4.reverse();

	d4.printForward();
	d4.printBackward();

	DoublyLinkedList d5{ 13,13,13,13,13 };
	d5.reverse();

	d5.printForward();
	d5.printBackward();

	getchar();
	getchar();
	return 0;
}
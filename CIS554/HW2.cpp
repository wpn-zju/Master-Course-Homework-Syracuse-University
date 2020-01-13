//CIS554 HW2
//Due: 11:59PM, Monday ( September 23)
/*
-   Implement two member functions, merge and remove, of the DoublyLinkedList class of HW1.
The descriptions of the two member functions are given below.
-   In the implementation of merge and remove, you are not allowed to modify values of nodes.
You only can modify pointers of nodes.
-   Again, you are not allowed to create new nodes in your implementation.  No external structures (such as arrays,
linked list, map, etc.) are allowed.  You can use some temporary pointes or variables for your
implementation.
-   In implementaing merge and remove, you are not allowed to call sort() in HW1


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
	void makeRandomList(int m, int n);
	void printForward();
	void printBackward();

	void sort();//sort all values based on frequency in ascending order.
	//In case of ties, the smaller values will appear earlier
	//Example:  for list with values  7 6 12 4 33 12 6 6 7
	//sorted results: 4 33 7 7 12 12 6 6 6

	void merge(DoublyLinkedList& L);
	/*
	Given an already sorted DoublyLinkedList (i.e., the result of HW1) , take
	another unsorted list L as function argument, and merge L into the current
	list to form a single sorted DoublyLinkedList.  The result of merge is a sorted list.
	You are not allowed to invoke sort() in HW1 in your implementation of merge.
	*/

	void remove(int m, int n);
	/*
	Given an already sorted DoublyLinkedList (i.e., the result of the HW1),
	remove n times of value m from the DoublyLinkedList.
	If n is more than the total number of m's in the DoublyLinkedList,
	then remove all m's from the list.
	If m does not exist in the list, then do nothing.
	The result of remove will be a sorted list.
	You are not allowed to invoke sort() in HW1 in your implementation of remove.
	*/
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
				// Sept.15 Modified
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
				else {
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

void DoublyLinkedList::merge(DoublyLinkedList& head) {
	if (!head.head)
		return;

	// First - Group/Sort by value.
	Node *ptr1 = nullptr, *ptr2 = nullptr, *ptr3 = nullptr, *ptr4 = nullptr;

	// Loop Control Variables
	bool control = false;

	// Bubble Loop
	do {
		control = false;

		ptr1 = head.head;
		ptr2 = head.head->next;

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
				if (head.head == ptr1)
					head.head = ptr2;
				if (head.tail == ptr2)
					head.tail = ptr1;

				ptr2 = ptr1->next;
				control = true;
			}
			else {
				ptr1 = ptr1->next;
				ptr2 = ptr2->next;
			}
		}
	} while (control);

	// Now LinkedList B is sorted and grouped.
	// Merge Step 1 : Find the position in linked list A to insert intervals in linked list B.
	// Merge Step 2 : Move the new extended interval to its right position.
	// Merge Here
	while (head.head) {
		ptr1 = ptr2 = this->head;
		ptr3 = ptr4 = head.head;

		int l1 = 1, l2 = 1;
		int v1 = 0;
		if (ptr1)
			v1 = ptr1->value;
		int v2 = ptr3->value;

		// Set Interval 1
		while (ptr4->next && ptr4->next->value == ptr4->value) {
			ptr4 = ptr4->next;
			l2++;
		}

		while (true) {
			// If not found, insert front of the head.
			if (!ptr2) {
				head.head = ptr4->next;

				// Insert
				if (!this->head) {
					this->head = ptr3;
					this->head->previous = nullptr;
					this->tail = ptr4;
					this->tail->next = nullptr;
				}
				else {
					this->head->previous = ptr4;
					ptr4->next = this->head;
					this->head = ptr3;
					this->head->previous = nullptr;
				}
				break;
			}
			// Set Interval 2
			while (ptr2->next && ptr2->next->value == ptr2->value) {
				ptr2 = ptr2->next;
				l1++;
			}
			if (v1 == v2) {
				head.head = ptr4->next;

				// Insert intervals.
				if (ptr1->previous)
					ptr1->previous->next = ptr3;
				else
					this->head = ptr3;
				ptr3->previous = ptr1->previous;
				ptr4->next = ptr1;
				if (ptr1)
					ptr1->previous = ptr4;
				break;
			}
			else {
				// Move on
				ptr1 = ptr2 = ptr2->next;
				l1 = 1;
				if (ptr1)
					v1 = ptr1->value;
			}
		}

		// Extend Interval
		while (ptr4->next && ptr4->next->value == v2) {
			ptr4 = ptr4->next;
			l2++;
		}

		// Move Interval
		ptr1 = ptr2 = ptr4->next;
		l1 = 1;
		if (ptr1)
			v1 = ptr1->value;
		while (ptr2) {
			// Set Interval 2
			while (ptr2->next && ptr2->next->value == ptr2->value) {
				ptr2 = ptr2->next;
				l1++;
			}
			if (l2 > l1 || l2 == l1 && v2 > v1) {
				// Swap 2 intervals.
				ptr1->previous = ptr3->previous;
				if (ptr3->previous)
					ptr3->previous->next = ptr1;
				ptr3->previous = ptr2;
				ptr4->next = ptr2->next;
				if (ptr2->next)
					ptr2->next->previous = ptr4;
				ptr2->next = ptr3;
				if (ptr3 == this->head)
					this->head = ptr1;
				if (ptr2 == this->tail)
					this->tail = ptr4;

				ptr1 = ptr2 = ptr4->next;

				l1 = 1;
				if (ptr2)
					v1 = ptr2->value;
			}
			else
				break;
		}
	}

	head.head = this->head;
	head.tail = this->tail;
}

void DoublyLinkedList::remove(int m, int n) {
	// Remove elements
	Node* ptr = head;
	while (ptr && ptr->value != m)
		ptr = ptr->next;
	while (ptr && ptr->value == m && n--) {
		if (ptr->previous)
			ptr->previous->next = ptr->next;
		else
			head = ptr->next;
		if (ptr->next)
			ptr->next->previous = ptr->previous;
		else
			tail = ptr->previous;
		Node* deletePtr = ptr;
		ptr = ptr->next;
		delete(deletePtr);
	}

	// If all elements with value m are deleted, then there is no need for re-sorting, just return.
	if (!ptr || !ptr->previous || ptr->value != m)
		return;

	Node* ptr1 = ptr, *ptr2 = ptr, *ptr3 = ptr->previous, *ptr4 = ptr->previous;
	int l1 = 1, l2 = 1;
	int v1 = m, v2 = ptr3->value;

	// Set Interval 1
	while (ptr2->next && ptr2->next->value == m) {
		ptr2 = ptr2->next;
		l1++;
	}

	while (ptr3) {
		// Set Interval 2
		while (ptr3->previous && ptr3->previous->value == ptr3->value) {
			ptr3 = ptr3->previous;
			l2++;
		}
		if (l2 > l1 || l2 == l1 && v2 > v1) {
			// Swap 2 intervals.
			ptr1->previous = ptr3->previous;
			if (ptr3->previous)
				ptr3->previous->next = ptr1;
			ptr3->previous = ptr2;
			ptr4->next = ptr2->next;
			if (ptr2->next)
				ptr2->next->previous = ptr4;
			ptr2->next = ptr3;
			if (ptr3 == head)
				head = ptr1;
			if (ptr2 == tail)
				tail = ptr4;

			ptr3 = ptr4 = ptr1->previous;
			if (!ptr3)
				return;

			l2 = 1;
			v2 = ptr3->value;
		}
		else
			break;
	}
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
	DoublyLinkedList d1, d2;
	d1.makeRandomList(50, 20);
	d1.printForward();
	d1.printBackward();

	d1.sort();
	d1.printForward();
	d1.printBackward();

	d2.makeRandomList(50, 20);
	d2.printForward();			// Modified "d1.printForward();" => "d2.printForward();"
	d2.printBackward();			// Modified "d1.printBackward();" => "d2.printBackward();"

	d1.merge(d2);
	d1.printForward();
	d1.printBackward();

	d1.remove(13, 3);
	d1.printForward();
	d1.printBackward();
	getchar();
	getchar();
	return 0;
}
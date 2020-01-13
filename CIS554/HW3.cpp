//HW3
//Due: Thursday (10/3) at 11:59PM

#include <iostream>
#include <list>
#include <map>
#include <string>
using namespace std;

class course {
public:
	string name;
	int section;
	int credits;
	course() {}
	course(string n, int s, int c) { name = n; section = s; credits = c; }

	//Add additional needed member functions and implement them.
	//You also need to implement some needed functions for overloading operator<< .
	friend ostream& operator<< (ostream& os, const course& obj);
};

//Some additional functions for overloading operator<<
ostream& operator<< (ostream& os, const course& obj) {
	return os << obj.name << " " << obj.section << " " << obj.credits << " ";
}

ostream& operator<< (ostream& os, list<course*>& obj) {
	for (list<course*>::iterator iter = obj.begin(); iter != obj.end(); ++iter)
		os << **iter;
	return os << endl;
}

ostream& operator<< (ostream& os, map<int, list<course*>*>& obj) {
	for (map<int, list<course*>*>::iterator iter = obj.begin(); iter != obj.end(); ++iter) {
		os << "semester = " << iter->first << endl;
		os << *iter->second;
	}
	return os;
}

ostream& operator<< (ostream& os, map<int, map<int, list<course*>*>>& obj) {
	for (map<int, map<int, list<course*>*>>::iterator iter = obj.begin(); iter != obj.end(); ++iter) {
		os << "student id = " << iter->first << endl;
		os << iter->second;
	}
	return os;
}

void add_student(map<int, map<int, list<course*>*>> &DB, int id);
void remove_student(map<int, map<int, list<course*>*>> &DB, int id);
void add_course(map<int, map<int, list<course*>*>> &DB, int semester, int id, course c); //20171 Spring semester of 2017; 20172: Fall semester of 2017										   
void drop_course(map<int, map<int, list<course*>*>> &DB, int semester, int id, course c);
void print_student_semester_courses(map<int, map<int, list<course*>*>> &DB, int semester, int id);
void print_student_all_courses(map<int, map<int, list<course*>*>> &DB, int id);
void print_DB(map<int, map<int, list<course*>*>> &DB);

//Implement the following functions
void add_student(map<int, map<int, list<course*>*>> &DB, int id) {
	DB[id];
}

void remove_student(map<int, map<int, list<course*>*>> &DB, int id) {
	if (!DB.count(id))
		return;
	for (map<int, list<course*>*>::iterator iter1 = DB[id].begin(); iter1 != DB[id].end(); ++iter1) {
		for (list<course*>::iterator iter2 = iter1->second->begin(); iter2 != iter1->second->end(); ++iter2)
			delete(*iter2);
		delete(iter1->second);
	}
	DB.erase(id);
}

void add_course(map<int, map<int, list<course*>*>> &DB, int semester, int id, course c) {
	if (!DB.count(id))
		return;
	if (!DB[id][semester])
		DB[id][semester] = new list<course*>();
	for (list<course*>::iterator iter = DB[id][semester]->begin(); iter != DB[id][semester]->end(); ++iter) {
		if ((*iter)->name >= c.name) {
			if ((*iter)->name != c.name)
				DB[id][semester]->insert(iter, new course(c.name, c.section, c.credits));
			return;
		}
	}
	DB[id][semester]->insert(DB[id][semester]->end(), new course(c.name, c.section, c.credits));
}

void drop_course(map<int, map<int, list<course*>*>> &DB, int semester, int id, course c) {
	for (list<course*>::iterator iter = DB[id][semester]->begin(); iter != DB[id][semester]->end(); ++iter)
		if ((*iter)->name == c.name) {
			delete(*iter);
			iter = DB[id][semester]->erase(iter);
		}
}

void print_student_semester_courses(map<int, map<int, list<course*>*>> &DB, int semester, int id) {
	cout << "student id = " << id << endl;
	cout << "semester = " << semester << endl;
	cout << *DB[id][semester];
	cout << endl;
}

void print_student_all_courses(map<int, map<int, list<course*>*>> &DB, int id) {
	cout << "student id = " << id << endl;
	cout << DB[id];
	cout << endl;
}

void print_DB(map<int, map<int, list<course*>*>> &DB) {
	cout << DB;
	cout << endl;
}

//For the print funcitons, you need to add more fucntions to overload operator<<.
//for example print_DB is simply cout << DB;
//Courses in a semeste are sorted alphabetically.

int main() {
	//Do not change code for main function
	map<int, map<int, list<course*>*>> DB;
	add_student(DB, 11111);
	course C1("CIS554", 1, 3), C2("CSE674", 1, 3), C3("MAT296", 8, 4), C4("WRT205", 5, 3);

	add_course(DB, 20171, 11111, C1);
	add_course(DB, 20171, 11111, C4);
	add_course(DB, 20171, 11111, C3);
	add_course(DB, 20171, 11111, C2);
	print_student_semester_courses(DB, 20171, 11111);

	drop_course(DB, 20171, 11111, C1);
	print_student_semester_courses(DB, 20171, 11111);

	add_course(DB, 20172, 11111, C2);
	add_course(DB, 20172, 11111, C4);
	add_course(DB, 20172, 11111, C3);
	add_course(DB, 20172, 11111, C1);
	print_student_all_courses(DB, 11111);

	add_student(DB, 11112);
	add_course(DB, 20171, 11112, C2);
	add_course(DB, 20171, 11112, C4);
	add_course(DB, 20171, 11112, C3);
	add_course(DB, 20171, 11112, C1);
	print_student_semester_courses(DB, 20171, 11112);

	add_course(DB, 20172, 11112, C2);
	add_course(DB, 20172, 11112, C4);
	add_course(DB, 20172, 11112, C3);
	add_course(DB, 20172, 11112, C1);
	print_student_semester_courses(DB, 20172, 11112);
	print_student_all_courses(DB, 11112);
	print_DB(DB);
	remove_student(DB, 11111);
	print_DB(DB);
	getchar();
	getchar();
	return 0;
}

/*
//Your output needs to keep the identical format
//Sample Screenshot
student id = 11111
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11111
semester = 20171
CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11111
semester = 20171
CSE674 1 3  MAT296 8 4  WRT205 5 3    
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3  

student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3   

student id = 11112
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3  
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3  

student id = 11111
semester = 20171
CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3  
student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3

student id = 11112
semester = 20171
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3
semester = 20172
CIS554 1 3  CSE674 1 3  MAT296 8 4  WRT205 5 3 
*/
#pragma once
#pragma once
#pragma warning(disable : 4996)=
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Node_Linkedlist {
public:
	int vcode;
	string vehiclenumber;
	time_t date_code;
	Node_Linkedlist* next;

	Node_Linkedlist() {
		vcode = NULL;
		vehiclenumber = "";
		date_code = NULL;
		next = NULL;
	}

	Node_Linkedlist(int c, time_t date, string vnum) {
		vcode = c;
		vehiclenumber = vnum;
		date_code = date;
		next = NULL;
	}
};

class LinkedList_TreeNode {
public:
	Node_Linkedlist* head;
	Node_Linkedlist* tail;
	int size;

	LinkedList_TreeNode() {
		head = NULL;
		tail = NULL;
		size = 0;
	}

	void insert(Node_Linkedlist* node) {
		if (head == NULL) {
			head = node;
			tail = node;
		}
		else {
			tail->next = node;
			tail = node;
		}
		size++;
	}
};

class TreeNode {
public:
	int index;
	TreeNode* parent;
	TreeNode* left;
	TreeNode* right;
	TreeNode* next;
	Node_Linkedlist* dateNode;
	LinkedList_TreeNode datesOccurances;//Eventhough this attribute is not called in constructors, when the treenode is created, this list will be called automatically with the default constructor of the linkedlist

	TreeNode() {
		index = NULL;
		parent = NULL;
		left = NULL;
		right = NULL;
		next = NULL;
		dateNode = new Node_Linkedlist();
	}

	TreeNode(int i, time_t d, string vnum) {
		index = i;
		parent = NULL;
		left = NULL;
		right = NULL;
		next = NULL;
		dateNode = new Node_Linkedlist(i, d, vnum);
	}
};

class LinkedList_Array {
public:
	TreeNode* head;
	TreeNode* tail;

	LinkedList_Array() {
		head = NULL;
		tail = NULL;
	}

	void insert(TreeNode* node) {
		if (head == NULL) {
			head = node;
			tail = node;
		}
		else {
			tail->next = node;
			tail = node;
		}
	}
};

class Node_ViolationArray {
public:
	int code;
	int fine;
	string violation;
	LinkedList_Array crossConnectList;

	Node_ViolationArray() {
		code = NULL;
		fine = NULL;
		violation = "";
	}

	Node_ViolationArray(int c, int f, string v) {
		code = c;
		fine = f;
		violation = v;
	}
};

class ViolationArray {
public:
	static const int size = 19;
	Node_ViolationArray arrayViolations[size];

	ViolationArray() {}

	void insert(int i, int j, string k) {
		Node_ViolationArray node(i, j, k);
		arrayViolations[i] = node;
	}

	void printViolations() {
		cout << "Code\t" << "Violation" << endl;
		cout << "----\t" << "---------" << endl;
		for (int k = 0; k < size; k++) {
			cout << k << "\t" << (arrayViolations[k].violation) << endl;
		}
	}

	void analyzeViolations(int code) {
		vector<Node_Linkedlist*> row{};
		TreeNode* current = arrayViolations[code].crossConnectList.head;
		if (current == NULL) cout << "***The Traffic Law Violation of '" << arrayViolations[code].violation << "' has not been committed by any vehicle upto now***" << endl << endl;
		else {
			while (current != NULL) {
				Node_Linkedlist* toggle = current->datesOccurances.head;
				do {
					row.push_back(toggle);
					toggle = toggle->next;
				} while (toggle);
				current = current->next;
			}
			for (int m = 0; m < row.size() - 1; m++) {
				for (int n = m + 1; n < row.size(); n++) {
					if (row[m]->date_code < row[n]->date_code) {
						Node_Linkedlist* temp = row[m];
						row[m] = row[n];
						row[n] = temp;
					}
				}
			}
			cout << "Vehicle Number\t" << "Date" << endl;
			cout << "--------------\t" << "----" << endl;
			for (int i = 0; i < row.size(); i++) {
				char* datetime = ctime(&(row[i]->date_code));
				cout << row[i]->vehiclenumber << "\t\t" << datetime;
			}
			cout << endl << "There are " << row.size() << " incidents of committing the Traffic Law Violation of '" << arrayViolations[code].violation << "' upto now." << endl;
		}
	}

};

class Queue {
public:
	int front;
	int rear;
	int capacity;
	int size;
	TreeNode* data;

	Queue() {
		front = 0;
		rear = 0;
		capacity = 0;
		size = 0;
		data = NULL;
	}

	Queue(int x) {
		front = 0;
		rear = 0;
		capacity = x;
		size = 0;
		data = new TreeNode[capacity];
	}

	void enQueue(TreeNode* n) {
		data[rear] = *n;
		rear = (rear + 1) % capacity;
		size++;
	}

	TreeNode* deQueue() {
		TreeNode* temp = &data[front];
		front = (front + 1) % capacity;
		size--;
		return temp;
	}
};

class Tree {
public:
	TreeNode* root;
	int size;

	Tree() {
		root = NULL;
		size = 0;
	}

	int insert(int code, time_t date, ViolationArray* arr, string vnum) {
		int fineValue = arr->arrayViolations[code].fine;
		TreeNode* node = new TreeNode(code, date, vnum);
		TreeNode* current = root;
		TreeNode* previous = NULL;
		bool flag = false;
		while (current != NULL) {
			previous = current;
			if (code > current->index) {
				current = current->right;
			}
			else if (code < current->index) {
				current = current->left;
			}
			else {
				flag = true;
				break;
			}
		}
		if (!flag) {
			size++;
			if (previous == NULL) {
				root = node;
			}
			else if (code > previous->index) {
				node->parent = previous;
				previous->right = node;
			}
			else {
				node->parent = previous;
				previous->left = node;
			}
			arr->arrayViolations[code].crossConnectList.insert(node);
			(node->datesOccurances).insert(node->dateNode);
		}
		else {
			(current->datesOccurances).insert(node->dateNode);
			fineValue += ((current->datesOccurances.size - 1) * arr->arrayViolations[code].fine * 5) / 100;
		}
		return fineValue;
	}

	void printTree(ViolationArray* arr) {
		TreeNode* current = root;
		if (!root) {
			cout << "***The vehicle has not been fined for any violation***" << endl;
		}
		else {
			vector<Node_Linkedlist*> line;
			Queue tempQueue((size + 1) / 2);//the result will be casted to an int and inserted to the function
			tempQueue.enQueue(current);
			while (tempQueue.size != 0) {
				Node_Linkedlist* toggle = current->datesOccurances.head;
				do {
					line.push_back(toggle);
					toggle = toggle->next;
				} while (toggle);
				tempQueue.deQueue();
				if (current->left) tempQueue.enQueue(current->left);
				if (current->right) tempQueue.enQueue(current->right);
				current = &tempQueue.data[tempQueue.front];
			}
			for (int m = 0; m < line.size() - 1; m++) {
				for (int n = m + 1; n < line.size(); n++) {
					if (line[m]->date_code < line[n]->date_code) {
						Node_Linkedlist* temp = line[m];
						line[m] = line[n];
						line[n] = temp;
					}
				}
			}
			cout << "Code\t" << "Date\t\t\t\t" << "Violation" << endl;
			cout << "----\t" << "----\t\t\t\t" << "---------" << endl;
			for (int i = 0; i < line.size(); i++) {
				char* datetime = ctime(&(line[i]->date_code));
				string format = datetime;
				format.erase(std::remove(format.begin(), format.end(), '\n'), format.end());//removes the automatic newline char from the datetime. returns a string of length about 24 characters
				cout << line[i]->vcode << "\t" << format << "\t" << arr->arrayViolations[line[i]->vcode].violation << endl;
			}
		}
	}
};

class Node_VehicleHashTable {
public:
	string vehicleNumber;
	Node_VehicleHashTable* next;
	Tree treeforViolations;

	Node_VehicleHashTable() {
		vehicleNumber = "";
		next = NULL;
	}

	Node_VehicleHashTable(string v) {
		vehicleNumber = v;
		next = NULL;
	}
};


class VehicleHashTable {
public:
	static const int size = 100;
	Node_VehicleHashTable vehicles[size];

	VehicleHashTable() {}

	int getHash(string vnum) {//assuming that the samplesapce is limited to 100
		int total{};
		for (int i = 0; i < vnum.length(); i++) {
			total += int(vnum[i]);//int() will give the ASCII value of the vnum[i]
		}
		return total % size;
	}

	bool iskeyExist(int key, string vnum) {
		Node_VehicleHashTable* current = &vehicles[key];
		while (current->vehicleNumber != "") {
			if (current->vehicleNumber != vnum) {
				current = current->next;
			}
			return true;
		}
		return false;
	}

	void insertVehicle(string vnum) {
		int key = getHash(vnum);
		Node_VehicleHashTable* node = new Node_VehicleHashTable(vnum);
		if (!iskeyExist(key, vnum)) {
			vehicles[key] = *node;
		}
		else {
			Node_VehicleHashTable* current = &vehicles[key];
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = node;
		}
	}

	int insertViolationToVehicle(string vnum, int code, int date, ViolationArray* arr) {
		int key = getHash(vnum);
		Node_VehicleHashTable* current = &vehicles[key];
		while (current->vehicleNumber != vnum) {
			current = current->next;
		}
		int fine = current->treeforViolations.insert(code, date, arr, vnum);
		return fine;
	}

	void getViolations(string vnum, ViolationArray* a) {
		int key = getHash(vnum);
		Node_VehicleHashTable* current = &vehicles[key];
		while (current->vehicleNumber != vnum) {
			current = current->next;
		}
		current->treeforViolations.printTree(a);
	}
};


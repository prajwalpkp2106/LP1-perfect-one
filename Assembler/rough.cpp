#include <iostream>
#include <ctime>
using namespace std;

class Node {
    public:
	string start;
	string end;
	string min;
	string max;
	bool visited;
	Node *next;

	Node() {
		next = NULL;
		min = "0.5hr";
		max = "1hr";
		visited = 0;
		next = NULL;
	}
	Node(string start, string end) {
		this->start = start;
		this->end = end;
//		this->min = min;
//		this->max = max;
//		this->visited = visited;
		this->next = next;
	}
};

class Appointment {
	Node *head;
	public:
	Appointment() {
		head = NULL;
	}
	void create_App(string start, string end) {
		Node *Newnode = new Node(start, end);

		if(head == NULL) {
			head = Newnode;
			return ;
		}
		Newnode->next = this->head;
		this->head = Newnode;
	}

};


int main() {
	Appointment a1;
	string start;
	string end;
	for(int i = 0; i < 4; i++) {
		cout << "Enter start time: ";
		cin >> start;
		cout << "Enter end time: ";
		cin >> end;
		a1.create_App(start, end);
	}

}
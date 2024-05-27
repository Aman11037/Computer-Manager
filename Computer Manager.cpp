#include <iostream>
#include <iomanip>
#include <algorithm>
#include <conio.h>
#include <string>
using namespace std;
class SoftwareManager {
	struct node {
		string name;
		int PID;
		int memUsage;
		string status;
		node* right;
		node* left;
		int height;
	}*root;


public:

	SoftwareManager() {
		root = NULL;
	}

	node* createSoftware(string name, int PID, int memUsage, string status) {
		node* temp = new node();
		temp->name = name;
		temp->PID = PID;
		temp->memUsage = memUsage;
		temp->status = status;
		temp->left = temp->right = NULL;
		temp->height = 1;
		return temp;
	}

	int getHeight(node* temp) {
		if (temp == NULL) {
			return 0;
		}
		return temp->height;
	}

	node* getRoot() {
		return root;
	}

	void setStatus(node* temp, string status) {
		temp->status = status;
	}

	int getBalanceFac(node* temp) {
		if (temp == NULL) {
			return 0;
		}
		return getHeight(temp->left) - getHeight(temp->right);
	}

	node* leftRotate(node* temp) {
		node* y = temp->right;
		node* z = y->left;

		y->left = temp;
		temp->right = z;

		temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
		y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

		return y;
	}

	node* rightRotate(node* temp) {
		node* y = temp->left;
		node* z = y->right;

		y->right = temp;
		temp->left = z;

		temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
		y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

		return y;
	}

	void installSoftware(string name, int PID, int memUsage, string status) {
		root = installSoftware(root, name, PID, memUsage, status);
	}

	void displayInOrder(node* current) {
		if (current == NULL) {
			return;
		}
		displayInOrder(current->left);
		cout << current->name << "\n" << endl;
		displayInOrder(current->right);
	}

	void displayPostOrder(node* current) {
		if (current == NULL) {
			return;
		}
		displayPostOrder(current->right);
		cout << current->name << "\n" << endl;
		displayPostOrder(current->left);
	}

	node* uninstallSoftware(node* temp, string name) {
		if (temp == NULL) {
			cout << "Software not found.\n" << endl;
			return temp;
		}
		if (temp->status == "Running") {
			cout << "Cannot uninstall while program is running.\n" << endl;
			return NULL;
		}
		if (name < temp->name) {
			temp->left = uninstallSoftware(temp->left, name);
		}
		else if (name > temp->name) {
			temp->right = uninstallSoftware(temp->right, name);
		}
		else {
			if (temp->left == NULL) {
				node* rightChild = temp->right;
				delete temp;
				cout << name << " has been successfully deleted.\n" << endl;
				return rightChild;
			}
			else if (temp->right == NULL) {
				node* leftChild = temp->left;
				delete temp;
				cout << name << " has been successfully deleted.\n" << endl;
				return leftChild;
			}

			node* successor = getMinValueNode(temp->right);
			temp->name = successor->name;
			temp->PID = successor->PID;
			temp->memUsage = successor->memUsage;
			temp->status = successor->status;
			temp->right = uninstallSoftware(temp->right, successor->name);
		}

		temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
		int balanceFac = getBalanceFac(temp);

		if (balanceFac > 1 && getBalanceFac(temp->left) >= 0) {
			return rightRotate(temp);
		}

		if (balanceFac > 1 && getBalanceFac(temp->left) < 0) {
			temp->left = leftRotate(temp->left);
			return rightRotate(temp);
		}

		if (balanceFac < -1 && getBalanceFac(temp->right) <= 0) {
			return leftRotate(temp);
		}

		if (balanceFac < -1 && getBalanceFac(temp->right) > 0) {
			temp->right = rightRotate(temp->right);
			return leftRotate(temp);
		}

		return temp;
	}

	node* getMinValueNode(node* temp) {
		node* current = temp;
		while (current->left != NULL) {
			current = current->left;
		}
		return current;
	}

	node* searchSoftware(string name) {
		if (root == NULL) {
			cout << "No installed software.\n" << endl;
			return NULL;
		}
		node* current = root;
		while (current != NULL) {
			if (name == current->name) {
				return current;
			}
			else if (name > current->name) {
				current = current->right;
			}
			else {
				current = current->left;
			}
		}
		cout << name << " is not installed.\n" << endl;
		return NULL;
	}

private:
	node* installSoftware(node* temp, string name, int PID, int memUsage, string status) {
		if (temp == NULL) {
			cout << name << " has been installed successfully.\n" << endl;
			return createSoftware(name, PID, memUsage, status);
		}
		if (PID == temp->PID) {
			cout << "Another software with Process ID " << PID << " is already installed. PID cannot be duplicate.\n" << endl;
			return temp;
		}
		if (name < temp->name) {
			temp->left = installSoftware(temp->left, name, PID, memUsage, status);
		}
		else if (name > temp->name) {
			temp->right = installSoftware(temp->right, name, PID, memUsage, status);
		}
		else {
			cout << name << " is already installed.\n" << endl;
			return temp;
		}

		temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;

		int balanceFac = getBalanceFac(temp);

		if (balanceFac > 1 && name < temp->left->name) {
			return rightRotate(temp);
		}
		if (balanceFac < -1 && name > temp->right->name) {
			return leftRotate(temp);
		}
		if (balanceFac > 1 && name > temp->left->name) {
			temp->left = leftRotate(temp->left);
			return rightRotate(temp);
		}
		if (balanceFac < -1 && name < temp->right->name) {
			temp->right = rightRotate(temp->right);
			return leftRotate(temp);
		}
		return temp;
	}
};

class TaskManager {
	struct node {
		string name;
		int PID;
		int memUsage;
		string status;
		node* next;
	}*first;

	SoftwareManager* s;
	int totalMemory = 16000;
	int usedMemory;

public:
	TaskManager(SoftwareManager& sManager) {
		first = NULL;
		s = &sManager;
	}

	void runProgram(string name) {
		if (s->searchSoftware(name) == NULL) {
			return;
		}
		node* temp = new node;
		temp->name = s->searchSoftware(name)->name;
		temp->PID = s->searchSoftware(name)->PID;
		temp->memUsage = s->searchSoftware(name)->memUsage;
		if (usedMemory + temp->memUsage > totalMemory) {
			cout << "Insufficient Memory. Cannot Run More Programs. Stop Unused Programs to Free Memory.\n" << endl;
			return;
		}

		node* current = first;
		while (current != NULL) {
			if (current->name == name) {
				cout << name << " is already running.\n" << endl;
				return;
			}
			current = current->next;
		}

		temp->status = "Running";
		s->setStatus(s->searchSoftware(name), "Running");
		temp->next = first;
		usedMemory += temp->memUsage;
		first = temp;
		cout << temp->name << " is now running.\n" << endl;
	}



	void stopProgram(string name) {
		if (first == NULL) {
			cout << "No programs running.\n" << endl;
			return;
		}
		node* current = first;
		if (name == first->name) {
			first = current->next;
			usedMemory -= current->memUsage;
			s->setStatus(s->searchSoftware(name), "Suspended");
			delete current;
			cout << name << " has been suspended." << endl;
			return;
		}
		node* currentPrev = NULL;
		while (current != NULL) {
			if (current->name == name) {
				if (!current->next) {
					currentPrev->next = NULL;
					delete current;
					return;
				}
				currentPrev->next = current->next;
				usedMemory -= current->memUsage;
				delete current;
				cout << name << " has been suspended." << endl;
				return;
			}
			currentPrev = current;
			current = current->next;
		}
		cout << name << " is not installed or already suspended.\n" << endl;
	}

	void showRunningPrograms(node* temp) {
		if (first == NULL) {
			cout << "No programs running.\n" << endl;
			return;
		}
		if (temp == NULL) {
			return;
		}
		cout << left << setw(25) << temp->name << left << setw(17) << temp->status << left << setw(17) << temp->PID << left << setw(17) << temp->memUsage << "\n" << endl;
		showRunningPrograms(temp->next);
	}

	void showRunningProgramsInReverse(node* temp) {
		if (first == NULL) {
			cout << "No programs running.\n" << endl;
			return;
		}
		if (temp == NULL) {
			return;
		}
		showRunningProgramsInReverse(temp->next);
		cout << left << setw(25) << temp->name << left << setw(17) << temp->status << left << setw(17) << temp->PID << left << setw(17) << temp->memUsage << "\n" << endl;
	}

	node* getfirst() {
		return first;
	}
	void sortByName() {
		if (first == NULL || first->next == NULL) {
			return;
		}
		bool swapped;
		node* current;
		node* lastChecked = NULL;

		do {
			swapped = false;
			current = first;
			while (current->next != lastChecked) {
				if (current->name > current->next->name) {
					swapNodes(current, current->next);
					swapped = true;
				}
				current = current->next;
			}
			lastChecked = current;
		} while (swapped);
	}

	void sortByPID() {
		if (first == NULL || first->next == NULL) {
			return;
		}
		bool swapped;
		node* current;
		node* lastChecked = NULL;

		do {
			swapped = false;
			current = first;
			while (current->next != lastChecked) {
				if (current->PID > current->next->PID) {
					swapNodes(current, current->next);
					swapped = true;
				}
				current = current->next;
			}
			lastChecked = current;
		} while (swapped);
	}

	void sortByMemUsage() {
		if (first == NULL || first->next == NULL) {
			return;
		}
		bool swapped;
		node* current;
		node* lastChecked = NULL;

		do {
			swapped = false;
			current = first;
			while (current->next != lastChecked) {
				if (current->memUsage > current->next->memUsage) {
					swapNodes(current, current->next);
					swapped = true;
				}
				current = current->next;
			}
			lastChecked = current;
		} while (swapped);
	}

	void searchByName(string name) {
		node* current = first;
		while (current != NULL) {
			if (current->name == name) {
				cout << "Task Manager\n" << endl;
				cout << left << setw(25) << "Name" << left << setw(17) << "Status" << left << setw(17) << "PID" << left << setw(15) << "Memory Usage(MB)" << "\n" << endl;
				cout << left << setw(25) << current->name << left << setw(17) << current->status << left << setw(17) << current->PID << left << setw(17) << current->memUsage << "\n" << endl;
				return;
			}
			current = current->next;
		}
		cout << "No running program found against searched name." << endl;
		return;
	}

	void searchByPID(int PID) {
		node* current = first;
		while (current != NULL) {
			if (current->PID == PID) {
				cout << left << setw(25) << "Name" << left << setw(17) << "Status" << left << setw(17) << "PID" << left << setw(15) << "Memory Usage(MB)" << "\n" << endl;
				cout << left << setw(25) << current->name << left << setw(17) << current->status << left << setw(17) << current->PID << left << setw(17) << current->memUsage << "\n" << endl;
				return;
			}
			current = current->next;
		}
		cout << "No running program found against searched PID." << endl;
		return;
	}

private:
	void swapNodes(node* left, node* right) {
		string tempName = left->name;
		int tempPID = left->PID;
		int tempMemUsage = left->memUsage;
		string tempStatus = left->status;
		left->name = right->name;
		left->PID = right->PID;
		left->memUsage = right->memUsage;
		left->status = right->status;
		right->name = tempName;
		right->PID = tempPID;
		right->memUsage = tempMemUsage;
		right->status = tempStatus;
	}
};

int choiceExceptionHandling() {
	while (true) {
		try {
			cout << "Enter choice: ";
			string input;
			cin >> input;
			int value = stoi(input);
			return value;
		}
		catch (const invalid_argument& e) {
			cout << "Invalid input. Please enter an integer." << endl;
			cin.ignore();
		}
		catch (const out_of_range& e) {
			cout << "Input out of range. Please enter a smaller integer." << endl;
		}
	}
}

int processIDExceptionHandling() {
	while (true) {
		try {
			cout << "Enter Process ID: ";
			string input;
			cin >> input;
			int value = stoi(input);
			return value;
		}
		catch (const invalid_argument& e) {
			cout << "Invalid input. Please enter an integer." << endl;
			cin.ignore();
		}
		catch (const out_of_range& e) {
			cout << "Input out of range. Please enter a smaller integer." << endl;
		}
	}
}

int memUsageExceptionHandling() {
	while (true) {
		try {
			cout << "Enter Memory Usage in MB: ";
			string input;
			cin >> input;
			int value = stoi(input);
			return value;
		}
		catch (const invalid_argument&) {
			cout << "Invalid input. Please enter an integer." << endl;
			cin.ignore();
		}
		catch (const out_of_range&) {
			cout << "Input out of range. Please enter a smaller integer." << endl;
		}
	}
}


int main()
{
	SoftwareManager softwareManager;
	TaskManager taskManager(softwareManager);
	string name;
	int PID;
	int memUsage;
	string status;
	bool reverse = false;
	while (true) {
		cout << "PC Manager\n" << endl;
		cout << "1. Software Manager\n2. Task Manager\n0. Exit.\n" << endl;
		int choice;
		choice = choiceExceptionHandling();
		cin.ignore();
		cout << endl;
		if (choice == 0) {
			break;
		}
		switch (choice) {
		case 1:
			while (true) {
				cout << "Software Manager\n" << endl;
				cout << "Installed Software\n" << endl;
				if (reverse == true) {
					softwareManager.displayInOrder(softwareManager.getRoot());
				}
				else {
					softwareManager.displayPostOrder(softwareManager.getRoot());
				}
				cout << "1. Install Software\n2. Uninstall Software\n3. Search for Installed Software.\n4. Change Sorting Order\n0. Back\n" << endl;

				choice = choiceExceptionHandling();
				cin.ignore();
				cout << endl;
				if (choice == 0) {
					break;
				}
				switch (choice) {
				case 1:
					cout << "Enter Name: ";
					getline(cin, name);
					cout << endl;
					if (name.empty()) {
						cout << "Software name cannot be blank.\n" << endl;
						break;
					}
					PID = processIDExceptionHandling();
					cout << endl;
					memUsage = memUsageExceptionHandling();
					cin.ignore();
					cout << endl;
					if (memUsage > 0 && PID > 0) {
						status = "Suspended";
						softwareManager.installSoftware(name, PID, memUsage, status);
					}
					else {
						cout << "PID and Memory Usage must be greater than zero.\n" << endl;
					}
					break;
				case 2:
					cout << "Enter Name of software to uninstall: ";
					getline(cin, name);
					cout << endl;
					softwareManager.uninstallSoftware(softwareManager.getRoot(), name);
					break;
				case 3:
					cout << "Enter Name of software to search: ";
					getline(cin, name);
					cout << endl;
					if (softwareManager.searchSoftware(name) != NULL) {
						cout << name << " is installed.\n" << endl;
					}
					break;
				case 4:
					cout << "1. Keep Current Order\n2. Reverse Current Order\n" << endl;
					choice = choiceExceptionHandling();
					cin.ignore();
					cout << endl;
					cout << "Installed Software\n" << endl;
					if (choice == 1) {
						reverse = false;
					}
					else if (choice == 2) {
						reverse = true;
					}
					else {
						cout << "Invalid Choice.\n" << endl;
					}
					break;
				case 5:
					cout << "Installed Software\n" << endl;

					break;
				default:
					cout << "Invalid Choice\n" << endl;
				}
			}
			break;
		case 2:
			reverse = false;
			while (true) {
				cout << "Task Manager\n" << endl;
				cout << left << setw(25) << "Name" << left << setw(17) << "Status" << left << setw(17) << "PID" << left << setw(15) << "Memory Usage(MB)" << "\n" << endl;
				if (reverse == false) {
					taskManager.showRunningPrograms(taskManager.getfirst());
				}
				else {
					taskManager.showRunningProgramsInReverse(taskManager.getfirst());
				}
				cout << "1. Run Program\n2. Stop Program\n3. Search Running Programs\n4. Sort Running Programs by Name\n5. Sort Running Programs by Process ID\n6. Sort Running Programs by Memory Usage\n0. Back\n" << endl;
				choice = choiceExceptionHandling();
				cin.ignore();
				cout << endl;
				if (choice == 0) {
					break;
				}
				switch (choice) {
				case 1:
					cout << "Enter Program Name to Run: ";
					getline(cin, name);
					cout << endl;
					taskManager.runProgram(name);
					break;
				case 2:
					cout << "Enter Program Name to Terminate: ";
					getline(cin, name);
					cout << endl;
					taskManager.stopProgram(name);
					break;
				case 3:
					cout << "1. Search by Name\n2. Search by PID\n" << endl;
					choice = choiceExceptionHandling();
					cin.ignore();
					cout << endl;
					if (choice == 1) {
						cout << "Enter Program Name to Search: ";
						getline(cin, name);
						cout << endl;
						taskManager.searchByName(name);
						_getch();
					}
					else if (choice == 2) {
						cout << "Enter Process ID to Search: ";
						cin >> PID;
						cin.ignore();
						cout << endl;
						taskManager.searchByPID(PID);
						_getch();
					}
					else {
						cout << "Invalid Choice. Redirecting to Menu.\n" << endl;
					}
					break;
				case 4:
					taskManager.sortByName();
					cout << "1. Ascending Order\n2. Descending Order" << endl;
					choice = choiceExceptionHandling();
					cin.ignore();
					cout << endl;
					if (choice == 1) {
						reverse = false;
					}
					else if (choice == 2) {
						reverse = true;
					}
					else {
						cout << "Invalid Choice. Redirecting to Menu.\n" << endl;
					}
					break;
				case 5:
					taskManager.sortByPID();
					cout << "1. Ascending Order\n2. Descending Order" << endl;
					choice = choiceExceptionHandling();
					cin.ignore();
					cout << endl;
					if (choice == 1) {
						reverse = false;
					}
					else if (choice == 2) {
						reverse = true;
					}
					else {
						cout << "Invalid Choice. Redirecting to Menu.\n" << endl;
					}
					break;
				case 6:
					taskManager.sortByMemUsage();
					cout << "1. Ascending Order\n2. Descending Order" << endl;
					choice = choiceExceptionHandling();
					cin.ignore();
					cout << endl;
					if (choice == 1) {
						reverse = false;
					}
					else if (choice == 2) {
						reverse = true;
					}
					else {
						cout << "Invalid Choice. Redirecting to Menu.\n" << endl;
					}
					break;
				default:
					cout << "Invalid Choice.\n" << endl;
				}
			}
		case 0:
			break;
		default:
			cout << "Invalid Choice.\n" << endl;
		}
	}
	return 0;
}


// Midterm Project - Employee Management System
// Author: Ahmad Alfaleh
// Date: 07/11/2025

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

// Employee class
class Employee {
private:
    int id;
    string name;
    string role;
    float salary;

public:
    Employee() : id(0), name(""), role(""), salary(0.0f) {}
    Employee(int id, string name, string role, float salary)
        : id(id), name(name), role(role), salary(salary) {}

    int getId() const { return id; }

    void display() const {
        cout << "ID: " << id << ", Name: " << name << ", Role: " << role
             << ", Salary: $" << salary << endl;
    }

    string serialize() const {
        return to_string(id) + "," + name + "," + role + "," + to_string(salary);
    }

    static Employee deserialize(const string& line) {
        int id; float salary;
        string name, role;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        id = stoi(line.substr(0, pos1));
        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        role = line.substr(pos2 + 1, pos3 - pos2 - 1);
        salary = stof(line.substr(pos3 + 1));
        return Employee(id, name, role, salary);
    }
};

// Company class
class Company {
private:
    Employee* employees;
    int size;
    int capacity;

public:
    Company(int capacity) : size(0), capacity(capacity) {
        employees = new Employee[capacity];
    }

    ~Company() {
        delete[] employees;
    }

    void addEmployee(const Employee& emp) {
        if (size < capacity) {
            employees[size++] = emp;
        } else {
            cout << "Employee list is full.\n";
        }
    }

    void listEmployees() const {
        cout << "\n--- Employee List ---\n";
        for (int i = 0; i < size; i++) {
            employees[i].display();
        }
    }

    void sortEmployees() {
        sort(employees, employees + size, [](Employee a, Employee b) {
            return a.getId() < b.getId();
        });
    }

    int binarySearch(int id) const {
        int low = 0, high = size - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (employees[mid].getId() == id) return mid;
            else if (employees[mid].getId() < id) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    void findEmployeeById(int id) {
        sortEmployees();
        int index = binarySearch(id);
        if (index != -1) {
            cout << "Employee found:\n";
            employees[index].display();
        } else {
            cout << "Employee not found.\n";
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (int i = 0; i < size; i++) {
            file << employees[i].serialize() << endl;
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            addEmployee(Employee::deserialize(line));
        }
        file.close();
    }
};

int main() {
    Company company(100);
    company.loadFromFile("employees.txt");

    int choice;
    do {
        cout << "\n--- Employee Management System ---\n";
        cout << "1. Add Employee\n2. List Employees\n3. Find by ID\n4. Save to File\n5. Show Data Sizes\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id; string name, role; float salary;
            cout << "Enter ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Name: "; getline(cin, name);
            cout << "Enter Role: "; getline(cin, role);
            cout << "Enter Salary: "; cin >> salary;
            company.addEmployee(Employee(id, name, role, salary));
        } else if (choice == 2) {
            company.listEmployees();
        } else if (choice == 3) {
            int searchId;
            cout << "Enter ID to search: ";
            cin >> searchId;
            company.findEmployeeById(searchId);
        } else if (choice == 4) {
            company.saveToFile("employees.txt");
            cout << "Saved to employees.txt\n";
        } else if (choice == 5) {
            cout << "Size of int: " << sizeof(int) << " bytes\n";
            cout << "Size of float: " << sizeof(float) << " bytes\n";
            cout << "Size of string: " << sizeof(string) << " bytes\n";
        }

    } while (choice != 6);

    return 0;
}

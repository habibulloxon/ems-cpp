#include <iostream>

#include <fstream>

#include <vector>

#include <string>

#include <sstream>

#include <algorithm>

using namespace std;

struct Employee {
    int id;
    string name;
    string department;
    int salary;
};

class EmployeeManagementSystem {
private: vector < Employee > employees;

public: int getOption() {
    int option;
    cout << endl << "Enter your option:";
    cin >> option;
    return option;
}

      void addNewRecord() {
          Employee emp;
          cout << "Enter employee ID: ";
          cin >> emp.id;

          cout << "Enter employee name: ";
          cin.ignore();
          getline(cin, emp.name);
          
          do {
              cout << "Enter employee department: ";
              getline(cin, emp.department);

              if (emp.department.empty()) {
                  cout << "Department cannot be empty. Please enter a department." << endl;
              }
          } while (emp.department.empty());

          double salary;
          do {
              cout << "Enter employee salary (must be greater than 0): ";
              cin >> salary;

              if (salary <= 0) {
                  cout << "Invalid salary. Salary must be a positive value." << endl;
              }
          } while (salary <= 0);

          emp.salary = salary;

          employees.push_back(emp);
          cout << "Employee record added successfully!" << endl;
          saveDataToFile();
      }



      void searchRecordById() {
          int id;
          cout << "Enter employee ID to search: ";
          cin >> id;
          bool found = false;
          for (const Employee& emp : employees) {
              if (emp.id == id) {
                  cout << "Employee details:" << endl;
                  cout << "ID: " << emp.id << endl;
                  cout << "Name: " << emp.name << endl;
                  found = true;
                  break;
              }
          }
          if (!found) {
              cout << "Employee with ID " << id << " not found." << endl;
          }
      }

      void displayEmployees() {
          int choice;
          cout << "Display: " << endl;
          cout << "[1] All records" << endl;
          cout << "[2] Department records" << endl;
          cout << "[3] Sort by salary (descending)" << endl;
          cout << "[4] Sort by salary (ascending)" << endl;
          cout << "Enter your choice: ";
          cin >> choice;

          if (choice == 1) {
              displayAllEmployees();
          }
          else if (choice == 2) {
              string deptToSearch;
              cout << "Enter the department to display employees (or leave empty for all departments): ";
              cin >> deptToSearch;
              displayEmployeesByDepartment(deptToSearch);
          }
          else if (choice == 3) {
              displayEmployeesBySalary(true);
          }
          else if (choice == 4) {
              displayEmployeesBySalary(false);
          }
          else {
              cout << "Invalid choice. Please try again." << endl;
          }
      }


      void displayEmployeesBySalary(bool descending) {
          sort(employees.begin(), employees.end(),
              [descending](const Employee& emp1, const Employee& emp2) {
                  return descending ? (emp1.salary > emp2.salary) : (emp1.salary < emp2.salary);
              });

          cout << "Employees sorted by salary (" << (descending ? "descending" : "ascending") << "):" << endl;
          for (const Employee& emp : employees) {
              cout << "  - ID: " << emp.id << ", Name: " << emp.name << ", Department: " << emp.department
                  << ", Salary: $" << emp.salary << endl;
          }
      }

      void displayAllEmployees() {
          if (employees.empty()) {
              cout << "No employee records found." << endl;
              return;
          }
          cout << "List of all employees:" << endl;
          for (const Employee& emp : employees) {
              cout << "ID: " << emp.id << endl;
              cout << "Name: " << emp.name << endl;
              cout << "Department: " << emp.department << endl;
              cout << "Salary: " << emp.salary << endl;
              cout << endl;
          }
      }

      void displayEmployeesByDepartment(const string& department) {
          bool found = false;
          for (const Employee& emp : employees) {
              if (emp.department == department) {
                  if (!found) {
                      cout << "Employees in department " << department << ":" << endl;
                      found = true;
                  }
                  cout << "  - ID: " << emp.id << ", Name: " << emp.name << ", Salary: $" << emp.salary << endl;
              }
          }
          if (!found) {
              cout << "No employees found in department " << department << "." << endl;
          }
      }

      void loadDataFromFile() {
          ifstream file("ems-data.csv");
          if (!file.is_open()) {
              cout << "Error: Could not open file 'ems-data.csv'." << endl;
              return;
          }

          string line;
          getline(file, line);

          while (getline(file, line)) {
              stringstream ss(line);
              string idStr, name, department;
              double salary;

              if (getline(ss, idStr, ',') && getline(ss, name, ',') && getline(ss, department, ',')) {
                  if (ss >> salary) {
                      Employee emp{ stoi(idStr), name, department, salary };
                      employees.push_back(emp);
                  }
                  else {
                      cerr << "Error: Invalid salary format in line: " << line << endl;
                  }
              }
              else {
                  cerr << "Error: Invalid data format in line: " << line << endl;
              }
          }

          file.close();
          cout << "Employee data loaded from file successfully!" << endl;
      }


      void saveDataToFile() {
          ofstream file("ems-data.csv");
          if (!file.is_open()) {
              cout << "Error: Could not open file 'ems-data.csv' for writing." << endl;
              return;
          }

          file << "ID,Name,Department,Salary\n";

          for (const Employee& emp : employees) {
              file << emp.id << "," << emp.name << "," << emp.department << "," << emp.salary << endl;
          }

          file.close();
          cout << "Employee data saved to file successfully!" << endl;
      }

      void updateRecord() {
          int id;
          cout << "Enter employee ID to update: ";
          cin >> id;

          bool found = false;
          for (int i = 0; i < employees.size(); ++i) {
              if (employees[i].id == id) {
                  found = true;
                  cout << "Enter new details for employee:" << endl;
                  cout << "New name (leave blank if unchanged): ";
                  string newName;
                  cin >> newName;
                  if (!newName.empty()) {
                      employees[i].name = newName;
                  }
                  cout << "Employee record updated successfully!" << endl;
                  break;
              }
          }

          if (!found) {
              cout << "Employee with ID " << id << " not found." << endl;
          }
      }

      void deleteRecord() {
          int choice;
          cout << "Delete record by:" << endl;
          cout << "[1] Employee ID" << endl;
          cout << "[2] All records" << endl;
          cout << "Enter your choice: ";
          cin >> choice;

          switch (choice) {
          case 1:
              deleteRecordById();
              break;
          case 2:
              deleteAllRecords();
              break;
          default:
              cout << "Invalid choice. Please try again." << endl;
          }
      }

      void deleteRecordById() {
          int id;
          cout << "Enter employee ID to delete: ";
          cin >> id;

          int index = -1;
          for (int i = 0; i < employees.size(); ++i) {
              if (employees[i].id == id) {
                  index = i;
                  break;
              }
          }

          if (index != -1) {
              employees.erase(employees.begin() + index);
              cout << "Employee record deleted successfully!" << endl;
          }
          else {
              cout << "Employee with ID " << id << " not found." << endl;
          }
      }

      void deleteAllRecords() {
          if (employees.empty()) {
              cout << "No employee records to delete." << endl;
              return;
          }

          cout << "Are you sure you want to delete all records (y/n)? ";
          char choice;
          cin >> choice;
          if (tolower(choice) == 'y') {
              employees.clear();
              cout << "All employee records deleted successfully!" << endl;
          }
          else {
              cout << "Deletion cancelled." << endl;
          }
      }
};

int main() {
    EmployeeManagementSystem empSys;

    empSys.loadDataFromFile();

    bool toRun = true;
    int option;

    do {
        cout << endl << "******* Employee Management System *******" << endl;
        cout << "[1] => Add a new record" << endl;
        cout << "[2] => Search record from employee ID" << endl;
        cout << "[3] => Display employees" << endl;
        cout << "[4] => Update record of an employee" << endl;
        cout << "[5] => Delete records / delete record by ID" << endl;
        cout << "[6] => Exit from the program" << endl;

        option = empSys.getOption();

        switch (option) {
        case 1:
            empSys.addNewRecord();
            break;
        case 2:
            empSys.searchRecordById();
            break;
        case 3:
            empSys.displayEmployees();
            break;
        case 4:
            empSys.updateRecord();
            break;
        case 5:
            empSys.deleteRecord();
            break;
        case 6:
            toRun = false;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    } while (toRun);

    empSys.saveDataToFile();

    return 0;
}
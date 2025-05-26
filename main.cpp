#include <iostream>
#include <vector>
#include "Student.hpp"
#include "ExcelUtils.hpp"
#include "MenuUtils.hpp"

using namespace std;

int main() {
    system("clear");
    string filename = "studentdata.xlsx";
    vector<Student> students = readExcelToVector(filename);

    vector<string> mainMenu = {
        "Add New Records",
        "Delete the Record",
        "Update the record",
        "Show all records",
        "Exit"
    };

    int option;
    do {
        printMenu(mainMenu);
        cout << ">> Choose your option: ";
        cin >> option;

        switch (option) {
            case 1: {
                cout << "Adding new student 📖!!" << endl;
                cout << "Enter Student Name: ";
                string name;
                cin.ignore();
                getline(cin, name);

                int age;
                cout << "Enter student age: ";
                cin >> age;

                students.emplace_back(name, age);
                writeExcel(filename, students);
                break;
            }
            case 2: {
                int index;
                cout << "Choose index of student to delete: ";
                cin >> index;

                if (index < 1 || index > students.size()) {
                    cout << "Invalid index ❌" << endl;
                    break;
                }

                students.erase(students.begin() + index - 1);
                writeExcel(filename, students);
                cout << "Successfully deleted the data 🥡" << endl;
                break;
            }
            case 3:
                // Optional: implement record editing logic
                break;
            case 4:
                displayTable(students);
                break;
            case 5:
                break;
        }

    } while (option != 5);

    return 0;
}

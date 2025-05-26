#include "MenuUtils.hpp"
#include <tabulate/table.hpp>
#include <iostream>

using namespace std;
using namespace tabulate;

void displayTable(const std::vector<Student> &students) {
    Table table;
    table.add_row({"Name", "Age"});
    for (auto student : students) {
        table.add_row({student.getName(), to_string(student.getAge())});
    }
    table[0].format().font_style({FontStyle::bold});
    cout << table << endl;
}

void printMenu(const std::vector<std::string> &items) {
    Table table;
    table.add_row({"No", "Menu"});
    for (size_t i = 0; i < items.size(); i++) {
        table.add_row({to_string(i + 1), items[i]});
    }
    table[0].format().font_style({FontStyle::bold});
    cout << table << endl;
}

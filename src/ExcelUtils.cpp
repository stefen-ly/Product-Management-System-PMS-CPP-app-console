#include "ExcelUtils.hpp"
#include <iostream>
#include <xlnt/xlnt.hpp>

using namespace std;

void writeExcel(const std::string &filename, std::vector<Student> &students) {
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Sheet1");

    ws.cell("A1").value("Name");
    ws.cell("B1").value("Age");

    int row = 2;
    for (auto &student : students) {
        ws.cell("A" + to_string(row)).value(student.getName());
        ws.cell("B" + to_string(row)).value(student.getAge());
        row++;
    }

    wb.save(filename);
    cout << "Successfully saved student records ☁️!" << endl;
}

std::vector<Student> readExcelToVector(const std::string &filename) {
    std::vector<Student> students;
    xlnt::workbook wb;

    try {
        wb.load(filename);
    } catch (...) {
        cout << "⚠️ Couldn't open file, returning empty student list." << endl;
        return students;
    }

    auto ws = wb.active_sheet();
    for (auto row : ws.rows(false)) {
        if (row[0].to_string() == "Name") continue;

        string name = row[0].to_string();
        int age = stoi(row[1].to_string());
        students.emplace_back(name, age);
    }
    return students;
}

void readExcel(const std::string &filename) {
    xlnt::workbook wb;
    wb.load(filename);
    auto ws = wb.active_sheet();

    for (auto row : ws.rows(false)) {
        for (auto cell : row) {
            cout << cell.to_string() << " ";
        }
        cout << endl;
    }
}

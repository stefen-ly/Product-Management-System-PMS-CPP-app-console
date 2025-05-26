#pragma once
#include <string>
#include <vector>
#include "Student.hpp"

void writeExcel(const std::string &filename, std::vector<Student> &students);
std::vector<Student> readExcelToVector(const std::string &filename);
void readExcel(const std::string &filename);

#pragma once

#include <string>
#include <vector>

// using namespace std;
std::vector<std::string> parseCsvLine(const std::string& line);
std::string getCurrentDate();
std::string inputExpireDate();
bool isValidDateFormat(const std::string& date);




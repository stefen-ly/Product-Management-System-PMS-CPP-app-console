#pragma once

#include <string>
#include <vector>

// using namespace std;
std::vector<std::string> parseCsvLine(const std::string& line);
std::string getCurrentDate();
std::string inputExpireDate();
bool isValidDateFormat(const std::string& date);
std::string inputNonEmptyString(const std::string& prompt);
double inputNonNegativeDouble(const std::string& prompt);
int inputNonNegativeInt(const std::string& prompt);
double inputSellPrice(double basePrice);
int inputPositiveInt(const std::string& prompt);
std::string maskPassword(const std::string& password) ;
std::string inputOptionalString(const std::string& prompt, int minLength = 3);
std::string toLower(const std::string& str);
void printLogo();
std::string inputWithCancel(const std::string& promptMessage);
std::string inputWithCancel(const std::string& promptMessage, const std::string& currentValue);
std::string inputWithCancel(const std::string& prompt);
std::string maskPassword(const std::string& password);

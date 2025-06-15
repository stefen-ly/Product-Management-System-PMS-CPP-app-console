#include "utils.hpp"
#include <ctime>
#include <algorithm>
#include <regex>
#include <iostream>  
#include <sstream>
#include <fstream>
#define NOMINMAX
#include <windows.h>
#include <limits>

using namespace std;

vector<string> parseCsvLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;
    
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    
    if (!field.empty()) {
        fields.push_back(field);
    }
    
    // -------------- Remove quotes from fields-------------------
    for (auto& f : fields) {
        if (!f.empty() && f.front() == '"' && f.back() == '"') {
            f = f.substr(1, f.length() - 2);
        }
        if (f.empty()) f = "";
    }
    
    return fields;
}

string getCurrentDate() {
    time_t now = time(nullptr);
    tm ltm;
    localtime_s(&ltm, &now);

    string year = to_string(1900 + ltm.tm_year);
    string month = (ltm.tm_mon < 9 ? "0" : "") + to_string(1 + ltm.tm_mon);
    string day = (ltm.tm_mday < 10 ? "0" : "") + to_string(ltm.tm_mday);
    
    return day + " " + month + " " + year;
}

bool isValidDateFormat(const string& date) {
    std::regex datePattern(R"(^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-\d{4}$)");
    return std::regex_match(date, datePattern);
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDayMonth(int day, int month, int year) {
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    if (month == 2 && isLeapYear(year))
        return day >= 1 && day <= 29;

    return day >= 1 && day <= daysInMonth[month - 1];
}

bool isDateAfterToday(int day, int month, int year) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    int currYear = now->tm_year + 1900;
    int currMonth = now->tm_mon + 1;
    int currDay = now->tm_mday;

    if (year > currYear) return true;
    if (year == currYear && month > currMonth) return true;
    if (year == currYear && month == currMonth && day > currDay) return true;

    return false;
}

string inputExpireDate() {
    string expiration;
    while (true) {
        cout << " - Enter expiration date (DD-MM-YYYY): ";
        getline(cin, expiration);

        if (expiration.empty()) {
            return ""; 
        }

        if (!isValidDateFormat(expiration)) {
            cout << "‼️  Invalid date format. Please enter date in DD-MM-YYYY format.\n";
            continue;
        }

        int day = stoi(expiration.substr(0, 2));
        int month = stoi(expiration.substr(3, 2));
        int year = stoi(expiration.substr(6, 4));

        if (!isValidDayMonth(day, month, year)) {
            cout << "‼️  Invalid day/month combination. Please enter a valid date.\n";
            continue;
        }

        if (!isDateAfterToday(day, month, year)) {
            cout << "‼️  Expiration date must be after today's date.\n";
            continue;
        }

        break;
    }

    return expiration;
}

string inputNonEmptyString(const string& prompt) {
    string input;
    do {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) {
            cout << " ‼️ Input cannot be empty! Please try again.\n";
        } else if (input.length() < 3) {
            cout << " ‼️ Input must be at least 3 characters long! Please try again.\n";
        }
    } while (input.empty() || input.length() < 3);
    return input;
}

string inputOptionalString(const string& prompt, int minLength) {
    string input;
    cout << prompt;
    getline(cin, input);
    if (!input.empty() && input.length() < minLength) {
        while (true) {
            cout << " ‼️ Input must be at least " << minLength << " characters long or press Enter to skip: ";
            getline(cin, input);
            if (input.empty() || input.length() >= minLength) break;
        }
    }
    return input;
}

double inputNonNegativeDouble(const string& prompt) {
    string input;
    double value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << " ‼️ Price cannot be empty! Please enter a value.\n";
            continue;
        }

        stringstream ss(input);
        if ((ss >> value) && !(ss >> input)) {
            if (value <= 0) {
                cout << " ‼️ Price must be greater than 0.\n";
            } else {
                return value;
            }
        } else {
            cout << " ‼️ Invalid input. Enter a valid positive number.\n";
        }
    }
}

double inputSellPrice(double basePrice) {
    string input;
    double value;

    while (true) {
        cout << " - Enter sell price (retail): ";
        getline(cin, input);

        if (input.empty()) {
            cout << " ‼️ Sell price cannot be empty!\n";
            continue;
        }

        stringstream ss(input);
        if ((ss >> value) && !(ss >> input)) {
            if (value <= 0) {
                cout << " ‼️ Sell price must be greater than 0.\n";
            } else if (value <= basePrice) {
                cout << " ‼️ Sell price must be greater than base price (" << basePrice << ").\n";
            } else {
                return value;
            }
        } else {
            cout << " ‼️ Invalid input. Please enter a valid number.\n";
        }
    }
}

int inputNonNegativeInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << " ‼️ Invalid input. Enter a non-negative integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int inputPositiveInt(const string& prompt) {
    string input;
    int value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << " ‼️ Quantity cannot be empty!\n";
            continue;
        }

        stringstream ss(input);
        if ((ss >> value) && !(ss >> input)) {
            if (value <= 0) {
                cout << " ‼️ Quantity must be greater than 0.\n";
            } else {
                return value;
            }
        } else {
            cout << " ‼️ Invalid input. Please enter a valid positive integer.\n";
        }
    }
}

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void printLogo() {
      HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
      fstream file("logo.txt");  
      string line;
          
      while (getline(file, line)) {
          SetConsoleTextAttribute(consoleColor, 4);
          cout << right << "\t\t" << line << endl;  
          Sleep(120);
      }
}

string inputWithCancel(const string& promptMessage, const string& currentValue) {
    cout << promptMessage << " (current: " << currentValue << ", or press Enter to cancel): ";
    string input;
    getline(cin, input);
    return input;
}

string inputWithCancel(const string& promptMessage) {
    cout << promptMessage;
    string input;
    getline(cin, input);
    return input; 
}

string maskPassword(const string& password) {
    return string(password.length(), '*'); 
}

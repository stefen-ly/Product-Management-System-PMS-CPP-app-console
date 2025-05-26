#pragma once
#include <string>

class Student {
private:
    std::string name;
    int age;

public:
    Student(std::string name, int age);
    std::string getName() const;
    int getAge() const;
    void setName(std::string name);
    void setAge(int age);
};

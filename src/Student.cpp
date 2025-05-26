#include "Student.hpp"

Student::Student(std::string name, int age) : name(name), age(age) {}

std::string Student::getName() const {
    return name;
}

int Student::getAge() const {
    return age;
}

void Student::setName(std::string name) {
    this->name = name;
}

void Student::setAge(int age) {
    this->age = age;
}

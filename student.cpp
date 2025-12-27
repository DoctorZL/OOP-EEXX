#include "student.h"

Student::Student(int studentId, int labs)
    : id(studentId), finishedTasks(labs), totalLabs(labs) {}

Student Student::operator+(int labsToAdd) const {
    Student result = *this;
    result.finishedTasks += labsToAdd;
    if (result.finishedTasks > result.totalLabs) {
        result.finishedTasks = result.totalLabs;
    }
    return result;
}

Student& Student::operator+=(int labsToAdd) {
    finishedTasks += labsToAdd;
    if (finishedTasks > totalLabs) {
        finishedTasks = totalLabs;
    }
    return *this;
}

bool Student::operator==(const Student& other) const {
    return id == other.id;
}

QString Student::toString() const {
    return QString("Студент %1 (лабы: %2/%3)")
        .arg(id)
        .arg(finishedTasks)
        .arg(totalLabs);
}

void Student::setTotalLabs(int newTotal) {
    totalLabs = newTotal;
    if (finishedTasks > totalLabs) {
        finishedTasks = totalLabs;
    }
}

void Student::setFinishedTasks(int labs) {
    finishedTasks = labs;
    if (finishedTasks > totalLabs) {
        finishedTasks = totalLabs;
    }
}

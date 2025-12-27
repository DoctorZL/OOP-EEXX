#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student {
private:
    int id;
    int finishedTasks;
    int totalLabs;

public:
    Student(int studentId = 0, int labs = 1);

    Student operator+(int labsToAdd) const;
    Student& operator+=(int labsToAdd);
    bool operator==(const Student& other) const;

    QString toString() const;

    int getFinishedTasks() const { return finishedTasks; }
    int getTotalLabs() const { return totalLabs; }
    int getId() const { return id; }

    void setTotalLabs(int newTotal);
    void setFinishedTasks(int labs);
};

#endif // STUDENT_H

#ifndef EXAMSYSTEM_H
#define EXAMSYSTEM_H

#include "teacher.h"
#include "student.h"
#include <queue>
#include <vector>
#include <memory>
#include <random>
#include <QObject>

class ExamSystem : public QObject {
    Q_OBJECT

signals:
    void logMessage(const QString& message);
    void statsUpdated(int inQueue, int passed);
    void studentProcessed(const QString& info);

public:
    ExamSystem(int totalLabs = 3, QObject* parent = nullptr);

    void addTeacher(std::unique_ptr<Teacher> teacher);
    void addStudent(int initialLabs);
    void addRandomStudent();
    void clearStudents();

    void runOneStep();
    void runSteps(int steps);
    void reset();

    ExamSystem& operator+=(int initialLabs);

    QString getStats() const;
    QString getQueueInfo() const;
    QString getTeachersInfo() const;

    int getQueueSize() const { return studentQueue.size(); }
    int getPassedCount() const { return passedStudents; }
    int getIteration() const { return iteration; }

private:
    std::queue<Student> studentQueue;
    std::vector<std::unique_ptr<Teacher>> teachers;
    int m;
    int studentCounter = 0;
    int iteration = 0;
    int passedStudents = 0;

    std::mt19937 randomGen;
};

#endif

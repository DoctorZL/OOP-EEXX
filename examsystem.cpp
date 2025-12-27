#include "examsystem.h"
#include <random>
#include <QString>

ExamSystem::ExamSystem(int totalLabs, QObject* parent)
    : QObject(parent), m(totalLabs) {
    std::random_device rd;
    randomGen = std::mt19937(rd());
}

void ExamSystem::addTeacher(std::unique_ptr<Teacher> teacher) {
    teachers.push_back(std::move(teacher));
}

void ExamSystem::addStudent(int initialLabs) {
    if (initialLabs < 1) initialLabs = 1;
    if (initialLabs > m) initialLabs = m;

    studentCounter++;
    Student newStudent(studentCounter, initialLabs);
    newStudent.setTotalLabs(m);

    studentQueue.push(newStudent);
    emit logMessage(QString("Добавлен студент %1 с %2 лабами (нужно: %3)")
                        .arg(studentCounter)
                        .arg(initialLabs)
                        .arg(m));
    emit statsUpdated(studentQueue.size(), passedStudents);
}

void ExamSystem::addRandomStudent() {
    std::uniform_int_distribution<> labsDist(1, m);
    addStudent(labsDist(randomGen));
}

void ExamSystem::clearStudents() {
    std::queue<Student> empty;
    std::swap(studentQueue, empty);
    studentCounter = 0;
    passedStudents = 0;
    iteration = 0;
    emit statsUpdated(0, 0);
}

void ExamSystem::runOneStep() {
    if (teachers.empty()) {
        emit logMessage("Нет преподавателей!!");
        return;
    }

    if (studentQueue.empty()) {
        emit logMessage("Нет студентов в очереди!!");
        return;
    }

    iteration++;
    emit logMessage(QString("\n== Итерация %1 ==").arg(iteration));

    Student currentStudent = studentQueue.front();
    studentQueue.pop();

    std::uniform_int_distribution<> teacherDist(0, static_cast<int>(teachers.size()) - 1);
    Teacher* teacher = teachers[teacherDist(randomGen)].get();

    QString stepInfo = QString("%1 идет к %2")
                           .arg(currentStudent.toString())
                           .arg(teacher->getName());
    emit studentProcessed(stepInfo);

    int newLabs = 0;
    bool labsAccepted = teacher->checkLabs(
        currentStudent.getFinishedTasks(),
        m,
        newLabs
        );

    if (!labsAccepted) {
        if (currentStudent.getFinishedTasks() < m) {
            currentStudent += newLabs;
            if (currentStudent.getFinishedTasks() > m) {
                currentStudent.setFinishedTasks(m);
            }
        }
        studentQueue.push(currentStudent);
        emit logMessage(QString("Лабы не приняты, добавлена %1 лаба")
                            .arg(newLabs));
        emit logMessage(QString("Теперь у студента: %1/%2 лаб")
                            .arg(currentStudent.getFinishedTasks())
                            .arg(m));
    } else {
        emit logMessage("Лабы приняты");

        bool theoryPassed = teacher->checkTheory();
        if (!theoryPassed) {
            studentQueue.push(currentStudent);
            emit logMessage("Теория не сдана");
            emit logMessage("Студент возвращается в очередь");
        } else {
            passedStudents++;
            emit logMessage("ЗАЧЕТ СДАН, УРА !!!");
            emit logMessage(QString("Всего сдали: %1").arg(passedStudents));
        }
    }

    emit statsUpdated(studentQueue.size(), passedStudents);
    emit logMessage(QString("В очереди: %1 студент(ов)")
                        .arg(studentQueue.size()));
}

void ExamSystem::runSteps(int steps) {
    for (int i = 0; i < steps && !studentQueue.empty(); i++) {
        runOneStep();
    }
}

void ExamSystem::reset() {
    clearStudents();
    teachers.clear();
    emit logMessage("Система сбршена");
}

ExamSystem& ExamSystem::operator+=(int initialLabs) {
    addStudent(initialLabs);
    return *this;
}

QString ExamSystem::getStats() const {
    return QString("Статистика:\n"
                   "Всего лаб: %1\n"
                   "Преподавателей: %2\n"
                   "Студентов в очереди: %3\n"
                   "Сдали зачет: %4\n"
                   "Итераций проведено: %5")
        .arg(m)
        .arg(teachers.size())
        .arg(studentQueue.size())
        .arg(passedStudents)
        .arg(iteration);
}

QString ExamSystem::getQueueInfo() const {
    if (studentQueue.empty()) {
        return "Очередь пуста";
    }

    QString result = "Очередь студентов:\n";
    std::queue<Student> tempQueue = studentQueue;
    int count = 1;
    while (!tempQueue.empty()) {
        result += QString("%1. %2\n")
        .arg(count++)
            .arg(tempQueue.front().toString());
        tempQueue.pop();
    }
    return result;
}

QString ExamSystem::getTeachersInfo() const {
    if (teachers.empty()) {
        return "Нет преподавателей";
    }

    QString result = "Преподаватели:\n";
    for (size_t i = 0; i < teachers.size(); ++i) {
        result += QString("%1. %2\n")
        .arg(i + 1)
            .arg(teachers[i]->getDescription());
    }
    return result;
}

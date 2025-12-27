#include "teacher.h"
#include <QString>
#include <random>

Teacher::Teacher(const QString& teacherName, double probability)
    : name(teacherName), theoryPassProbability(probability) {}

bool Teacher::checkTheory() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < theoryPassProbability;
}

bool Teacher::operator==(const Teacher& other) const {
    return name == other.name;
}

EvilTeacher::EvilTeacher(const QString& name)
    : Teacher(name, 0.5) {}

bool EvilTeacher::checkLabs(int finishedTasks, int totalTasks, int& newLabs) {
    newLabs = 1;
    return finishedTasks >= totalTasks;
}

QString EvilTeacher::getDescription() const {
    return QString("%1\n Тип: Злой\n Принимает только при всех сданных лабах\n Всегда добавляет +1 лабу\n Шанс сдачи теории: 50%")
        .arg(name);
}

KindTeacher::KindTeacher(const QString& name)
    : Teacher(name, 0.75) {}

bool KindTeacher::checkLabs(int finishedTasks, int totalTasks, int& newLabs) {
    studentCounter++;

    if (studentCounter % 2 == 0) {
        newLabs = 0;
        return true;
    }

    newLabs = 1;
    return finishedTasks >= totalTasks;
}

QString KindTeacher::getDescription() const {
    return QString("%1\n Тип: Добрый\n Гарантированно принимает каждую вторую работу\n Обычно добавляет +1 лабу\n Шанс сдачи теории: 75%")
        .arg(name);
}

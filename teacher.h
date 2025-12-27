#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <random>
#include <QString>

class Teacher {
protected:
    QString name;
    double theoryPassProbability;

public:
    Teacher(const QString& teacherName, double probability);
    virtual ~Teacher() = default;

    virtual bool checkLabs(int finishedTasks, int totalTasks, int& newLabs) = 0;
    bool checkTheory();

    QString getName() const { return name; }
    virtual QString getType() const = 0;
    virtual QString getDescription() const = 0;

    bool operator==(const Teacher& other) const;
};

class EvilTeacher : public Teacher {
public:
    EvilTeacher(const QString& name = "Злой преподаватель");
    bool checkLabs(int finishedTasks, int totalTasks, int& newLabs) override;
    QString getType() const override { return "Злой"; }
    QString getDescription() const override;
};

class KindTeacher : public Teacher {
private:
    mutable int studentCounter = 0;

public:
    KindTeacher(const QString& name = "Добрый преподаватель");
    bool checkLabs(int finishedTasks, int totalTasks, int& newLabs) override;
    QString getType() const override { return "Добрый"; }
    QString getDescription() const override;
};

#endif // TEACHER_H

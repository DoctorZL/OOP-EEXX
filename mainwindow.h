#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "examsystem.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTeacherClicked();
    void onAddStudentClicked();
    void onAddRandomStudentClicked();
    void onStepClicked();
    void onRun10StepsClicked();
    void onAutoRunClicked();
    void onResetClicked();
    void onUpdateStats(int inQueue, int passed);
    void onLogMessage(const QString& message);
    void onStudentProcessed(const QString& info);

private:
    void setupUI();
    void updateUI();

    ExamSystem* examSystem;
    QTimer* autoTimer;
    bool isAutoRunning = false;

    QTextEdit* logTextEdit;
    QTextEdit* infoTextEdit;
    QLabel* statsLabel;
    QLabel* queueLabel;

    QSpinBox* totalLabsSpin;
    QSpinBox* studentLabsSpin;
    QComboBox* teacherTypeCombo;
    QLineEdit* teacherNameEdit;

    QPushButton* addTeacherButton;
    QPushButton* addStudentButton;
    QPushButton* addRandomStudentButton;
    QPushButton* stepButton;
    QPushButton* run10StepsButton;
    QPushButton* autoRunButton;
    QPushButton* resetButton;

    QGroupBox* controlGroup;
    QGroupBox* teachersGroup;
    QGroupBox* studentsGroup;
};

#endif

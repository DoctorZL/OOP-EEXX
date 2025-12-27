#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    examSystem = new ExamSystem(3, this);
    autoTimer = new QTimer(this);
    autoTimer->setInterval(500);

    connect(autoTimer, &QTimer::timeout, examSystem, &ExamSystem::runOneStep);
    connect(examSystem, &ExamSystem::statsUpdated, this, &MainWindow::onUpdateStats);
    connect(examSystem, &ExamSystem::logMessage, this, &MainWindow::onLogMessage);
    connect(examSystem, &ExamSystem::studentProcessed, this, &MainWindow::onStudentProcessed);

    setupUI();
    updateUI();

    setWindowTitle("Система приема зачета");
    resize(1000, 700);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);


    QVBoxLayout* leftLayout = new QVBoxLayout();

    QGroupBox* infoGroup = new QGroupBox("Информация", this);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);

    statsLabel = new QLabel("Статистика", this);
    statsLabel->setWordWrap(true);
    infoLayout->addWidget(statsLabel);

    queueLabel = new QLabel("Очередь", this);
    queueLabel->setWordWrap(true);
    infoLayout->addWidget(queueLabel);

    infoTextEdit = new QTextEdit(this);
    infoTextEdit->setReadOnly(true);
    infoLayout->addWidget(infoTextEdit);

    leftLayout->addWidget(infoGroup);


    QVBoxLayout* rightLayout = new QVBoxLayout();


    controlGroup = new QGroupBox("Управление", this);
    QGridLayout* controlLayout = new QGridLayout(controlGroup);

    controlLayout->addWidget(new QLabel("Всего лаб:", this), 0, 0);
    totalLabsSpin = new QSpinBox(this);
    totalLabsSpin->setRange(1, 10);
    totalLabsSpin->setValue(3);
    controlLayout->addWidget(totalLabsSpin, 0, 1);

    stepButton = new QPushButton("1 шаг", this);
    controlLayout->addWidget(stepButton, 0, 2);

    run10StepsButton = new QPushButton("10 шагов", this);
    controlLayout->addWidget(run10StepsButton, 0, 3);

    autoRunButton = new QPushButton("Авто", this);
    controlLayout->addWidget(autoRunButton, 1, 2);

    resetButton = new QPushButton("Сброс", this);
    controlLayout->addWidget(resetButton, 1, 3);

    rightLayout->addWidget(controlGroup);


    teachersGroup = new QGroupBox("Преподаватели", this);
    QGridLayout* teachersLayout = new QGridLayout(teachersGroup);

    teachersLayout->addWidget(new QLabel("Тип:", this), 0, 0);
    teacherTypeCombo = new QComboBox(this);
    teacherTypeCombo->addItem("Злой");
    teacherTypeCombo->addItem("Добрый");
    teachersLayout->addWidget(teacherTypeCombo, 0, 1);

    teachersLayout->addWidget(new QLabel("Имя:", this), 1, 0);
    teacherNameEdit = new QLineEdit(this);
    teacherNameEdit->setPlaceholderText("Имя преподавателя");
    teachersLayout->addWidget(teacherNameEdit, 1, 1);

    addTeacherButton = new QPushButton("Добавить", this);
    teachersLayout->addWidget(addTeacherButton, 1, 2);

    rightLayout->addWidget(teachersGroup);

    studentsGroup = new QGroupBox("Студенты", this);
    QGridLayout* studentsLayout = new QGridLayout(studentsGroup);

    studentsLayout->addWidget(new QLabel("Лабы:", this), 0, 0);
    studentLabsSpin = new QSpinBox(this);
    studentLabsSpin->setRange(1, 10);
    studentLabsSpin->setValue(1);
    studentsLayout->addWidget(studentLabsSpin, 0, 1);

    addStudentButton = new QPushButton("Добавить", this);
    studentsLayout->addWidget(addStudentButton, 0, 2);

    addRandomStudentButton = new QPushButton("Случайный", this);
    studentsLayout->addWidget(addRandomStudentButton, 1, 2);

    rightLayout->addWidget(studentsGroup);

    QGroupBox* logGroup = new QGroupBox("Логи ", this);
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);

    logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    logLayout->addWidget(logTextEdit);

    rightLayout->addWidget(logGroup, 1);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    connect(addTeacherButton, &QPushButton::clicked, this, &MainWindow::onAddTeacherClicked);
    connect(addStudentButton, &QPushButton::clicked, this, &MainWindow::onAddStudentClicked);
    connect(addRandomStudentButton, &QPushButton::clicked, this, &MainWindow::onAddRandomStudentClicked);
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::onStepClicked);
    connect(run10StepsButton, &QPushButton::clicked, this, &MainWindow::onRun10StepsClicked);
    connect(autoRunButton, &QPushButton::clicked, this, &MainWindow::onAutoRunClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(totalLabsSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        delete examSystem;
        examSystem = new ExamSystem(value, this);
        connect(autoTimer, &QTimer::timeout, examSystem, &ExamSystem::runOneStep);
        connect(examSystem, &ExamSystem::statsUpdated, this, &MainWindow::onUpdateStats);
        connect(examSystem, &ExamSystem::logMessage, this, &MainWindow::onLogMessage);
        connect(examSystem, &ExamSystem::studentProcessed, this, &MainWindow::onStudentProcessed);
        updateUI();
    });
}

void MainWindow::updateUI() {
    statsLabel->setText(examSystem->getStats());
    queueLabel->setText("В очереди: " + QString::number(examSystem->getQueueSize()) +
                        ", Сдали: " + QString::number(examSystem->getPassedCount()));
    infoTextEdit->setText(examSystem->getTeachersInfo() + "\n\n" + examSystem->getQueueInfo());

    studentLabsSpin->setMaximum(totalLabsSpin->value());
}

void MainWindow::onAddTeacherClicked() {
    QString name = teacherNameEdit->text().trimmed();
    if (name.isEmpty()) {
        name = teacherTypeCombo->currentText() + " преподаватель";
    }

    QString teacherType = teacherTypeCombo->currentText();

    if (teacherType == "Злой") {
        examSystem->addTeacher(std::make_unique<EvilTeacher>(name));
        logTextEdit->append("Добавлен ЗЛОЙ преподаватель: " + name);
    } else {
        examSystem->addTeacher(std::make_unique<KindTeacher>(name));
        logTextEdit->append("Добавлен ДОБРыЙ преподаватель: " + name);
    }

    teacherNameEdit->clear();
    updateUI();
}

void MainWindow::onAddStudentClicked() {
    int labs = studentLabsSpin->value();
    examSystem->addStudent(labs);
    updateUI();
}

void MainWindow::onAddRandomStudentClicked() {
    examSystem->addRandomStudent();
    updateUI();
}

void MainWindow::onStepClicked() {
    examSystem->runOneStep();
    updateUI();
}

void MainWindow::onRun10StepsClicked() {
    examSystem->runSteps(10);
    updateUI();
}

void MainWindow::onAutoRunClicked() {
    if (!isAutoRunning) {
        autoTimer->start();
        autoRunButton->setText("Стоп");
        isAutoRunning = true;
    } else {
        autoTimer->stop();
        autoRunButton->setText("Авто");
        isAutoRunning = false;
    }
    updateUI();
}

void MainWindow::onResetClicked() {
    examSystem->reset();
    logTextEdit->clear();
    updateUI();
}

void MainWindow::onUpdateStats(int inQueue, int passed) {
    Q_UNUSED(inQueue);
    Q_UNUSED(passed);
    updateUI();
}

void MainWindow::onLogMessage(const QString& message) {
    logTextEdit->append(message);
}

void MainWindow::onStudentProcessed(const QString& info) {
    logTextEdit->append(info);
}

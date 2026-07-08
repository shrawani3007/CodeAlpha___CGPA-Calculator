#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CGPACalculator.h"
#include "Validator.h"

#include <QDate>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QGraphicsDropShadowEffect>

// ==========================================
// CourseDialog Implementation
// ==========================================

CourseDialog::CourseDialog(QWidget* parent, const Course* existingCourse)
    : QDialog(parent) {
    setWindowTitle(existingCourse ? "Edit Course" : "Add Course");
    setMinimumWidth(320);

    auto* layout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout();

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("e.g. Advanced Calculus");
    formLayout->addRow("Course Name:", nameEdit);

    gradeSpinBox = new QDoubleSpinBox(this);
    gradeSpinBox->setRange(0.0, 10.0);
    gradeSpinBox->setDecimals(2);
    gradeSpinBox->setSingleStep(0.1);
    gradeSpinBox->setValue(8.0);
    formLayout->addRow("Grade Point (0-10):", gradeSpinBox);

    creditSpinBox = new QDoubleSpinBox(this);
    creditSpinBox->setRange(0.1, 10.0);
    creditSpinBox->setDecimals(1);
    creditSpinBox->setSingleStep(0.5);
    creditSpinBox->setValue(3.0);
    formLayout->addRow("Credit Hours (>0):", creditSpinBox);

    layout->addLayout(formLayout);

    if (existingCourse) {
        nameEdit->setText(QString::fromStdString(existingCourse->getName()));
        gradeSpinBox->setValue(existingCourse->getGradePoint());
        creditSpinBox->setValue(existingCourse->getCreditHour());
    }

    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    // Apply simple styling to the dialog
    setStyleSheet(parent ? parent->styleSheet() : "");
}

std::string CourseDialog::getCourseName() const {
    return nameEdit->text().trimmed().toStdString();
}

float CourseDialog::getGradePoint() const {
    return static_cast<float>(gradeSpinBox->value());
}

float CourseDialog::getCreditHour() const {
    return static_cast<float>(creditSpinBox->value());
}

// ==========================================
// MainWindow Implementation
// ==========================================

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Dynamic date header
    ui->dateLabel->setText(QDate::currentDate().toString("dd MMM yyyy"));

    setupConnections();
    applyTheme();

    // Add shadow effects to panel frames for visual depth
    auto* detailsShadow = new QGraphicsDropShadowEffect(this);
    detailsShadow->setBlurRadius(15);
    detailsShadow->setOffset(0, 4);
    detailsShadow->setColor(QColor(0, 0, 0, 30));
    ui->detailsFrame->setGraphicsEffect(detailsShadow);

    auto* resultsShadow = new QGraphicsDropShadowEffect(this);
    resultsShadow->setBlurRadius(15);
    resultsShadow->setOffset(0, 4);
    resultsShadow->setColor(QColor(0, 0, 0, 30));
    ui->resultsFrame->setGraphicsEffect(resultsShadow);

    // Initialize with 1 semester
    ui->semesterCountSpinBox->setValue(1);
    onSemesterCountChanged(1);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->semesterCountSpinBox, &QSpinBox::valueChanged, this, &MainWindow::onSemesterCountChanged);
    connect(ui->calculateBtn, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(ui->resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveReportClicked);
    connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadReportClicked);
    connect(ui->printBtn, &QPushButton::clicked, this, &MainWindow::onPrintClicked);
    connect(ui->exportPdfBtn, &QPushButton::clicked, this, &MainWindow::onExportPdfClicked);
    connect(ui->themeToggleBtn, &QPushButton::clicked, this, &MainWindow::onThemeToggleClicked);
    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::onSemesterCountChanged(int count) {
    updateSemesterTabs(count);
}

void MainWindow::updateSemesterTabs(int count) {
    // Save any existing course data before rebuilding tabs
    m_courses = getCoursesFromUI();

    // Clean up existing tab configurations
    ui->semesterTabWidget->clear();
    m_semesterTables.clear();

    for (int i = 1; i <= count; ++i) {
        auto* tabWidget = new QWidget();
        auto* tabLayout = new QVBoxLayout(tabWidget);

        // Course list table
        auto* table = new QTableWidget(0, 4, tabWidget);
        table->setHorizontalHeaderLabels({"Course Name", "Grade Point", "Credit Hours", "Grade Points (Auto)"});
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        
        // Control buttons bar for this active semester tab
        auto* btnLayout = new QHBoxLayout();
        auto* addBtn = new QPushButton("➕ Add Course", tabWidget);
        auto* editBtn = new QPushButton("✏️ Edit Course", tabWidget);
        auto* deleteBtn = new QPushButton("❌ Delete Course", tabWidget);
        
        btnLayout->addWidget(addBtn);
        btnLayout->addWidget(editBtn);
        btnLayout->addWidget(deleteBtn);
        btnLayout->addStretch();

        // GPA Label for the semester tab
        auto* semesterGpaLabel = new QLabel("Semester GPA: 0.00", tabWidget);
        semesterGpaLabel->setObjectName(QString("semGpaLabel_%1").arg(i));
        semesterGpaLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        btnLayout->addWidget(semesterGpaLabel);

        tabLayout->addWidget(table);
        tabLayout->addLayout(btnLayout);

        ui->semesterTabWidget->addTab(tabWidget, QString("Semester %1").arg(i));
        m_semesterTables.push_back(table);

        // Connect button slots
        connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddCourseClicked);
        connect(editBtn, &QPushButton::clicked, this, &MainWindow::onEditCourseClicked);
        connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteCourseClicked);
    }

    // Restore course details into the newly drawn tabs
    populateUIFromCourses();
}

void MainWindow::onAddCourseClicked() {
    CourseDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        std::string name = dialog.getCourseName();
        float grade = dialog.getGradePoint();
        float credits = dialog.getCreditHour();

        if (!Validator::validateCourseName(name)) {
            showErrorMessage("Validation Error", "Course Name is required.");
            return;
        }

        int currentSemester = ui->semesterTabWidget->currentIndex() + 1;
        Course newCourse(name, grade, credits, currentSemester);

        m_courses.push_back(newCourse);
        populateUIFromCourses();
        onCalculateClicked(); // auto calculate on change
    }
}

void MainWindow::onEditCourseClicked() {
    int activeTabIdx = ui->semesterTabWidget->currentIndex();
    if (activeTabIdx < 0 || activeTabIdx >= static_cast<int>(m_semesterTables.size())) return;

    auto* table = m_semesterTables[activeTabIdx];
    int selectedRow = table->currentRow();
    if (selectedRow < 0) {
        showErrorMessage("Selection Error", "Please select a course to edit.");
        return;
    }

    // Find the course in our data vector
    int currentSemester = activeTabIdx + 1;
    int matchIdx = -1;
    int tableRowCounter = 0;
    for (size_t i = 0; i < m_courses.size(); ++i) {
        if (m_courses[i].getSemester() == currentSemester) {
            if (tableRowCounter == selectedRow) {
                matchIdx = static_cast<int>(i);
                break;
            }
            tableRowCounter++;
        }
    }

    if (matchIdx != -1) {
        CourseDialog dialog(this, &m_courses[matchIdx]);
        if (dialog.exec() == QDialog::Accepted) {
            std::string name = dialog.getCourseName();
            float grade = dialog.getGradePoint();
            float credits = dialog.getCreditHour();

            if (!Validator::validateCourseName(name)) {
                showErrorMessage("Validation Error", "Course Name is required.");
                return;
            }

            m_courses[matchIdx].setName(name);
            m_courses[matchIdx].setGradePoint(grade);
            m_courses[matchIdx].setCreditHour(credits);

            populateUIFromCourses();
            onCalculateClicked();
        }
    }
}

void MainWindow::onDeleteCourseClicked() {
    int activeTabIdx = ui->semesterTabWidget->currentIndex();
    if (activeTabIdx < 0 || activeTabIdx >= static_cast<int>(m_semesterTables.size())) return;

    auto* table = m_semesterTables[activeTabIdx];
    int selectedRow = table->currentRow();
    if (selectedRow < 0) {
        showErrorMessage("Selection Error", "Please select a course to delete.");
        return;
    }

    int currentSemester = activeTabIdx + 1;
    int tableRowCounter = 0;
    for (auto it = m_courses.begin(); it != m_courses.end(); ++it) {
        if (it->getSemester() == currentSemester) {
            if (tableRowCounter == selectedRow) {
                m_courses.erase(it);
                break;
            }
            tableRowCounter++;
        }
    }

    populateUIFromCourses();
    onCalculateClicked();
}

std::vector<Course> MainWindow::getCoursesFromUI() const {
    // Keep courses but make sure we don't drop courses from active UI tables
    std::vector<Course> currentCourses;
    
    // Scan all active semester tables to collect currently visible courses
    for (size_t semIdx = 0; semIdx < m_semesterTables.size(); ++semIdx) {
        auto* table = m_semesterTables[semIdx];
        int semNum = static_cast<int>(semIdx) + 1;
        for (int row = 0; row < table->rowCount(); ++row) {
            QString name = table->item(row, 0)->text();
            float grade = table->item(row, 1)->text().toFloat();
            float credits = table->item(row, 2)->text().toFloat();
            currentCourses.emplace_back(name.toStdString(), grade, credits, semNum);
        }
    }
    return currentCourses;
}

void MainWindow::populateUIFromCourses() {
    // Clear and redraw all tables
    for (auto* table : m_semesterTables) {
        table->setRowCount(0);
    }

    for (const auto& course : m_courses) {
        int semIdx = course.getSemester() - 1;
        if (semIdx >= 0 && semIdx < static_cast<int>(m_semesterTables.size())) {
            auto* table = m_semesterTables[semIdx];
            int row = table->rowCount();
            table->insertRow(row);

            table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(course.getName())));
            table->setItem(row, 1, new QTableWidgetItem(QString::number(course.getGradePoint(), 'f', 2)));
            table->setItem(row, 2, new QTableWidgetItem(QString::number(course.getCreditHour(), 'f', 1)));
            table->setItem(row, 3, new QTableWidgetItem(QString::number(course.getGradePoints(), 'f', 2)));
        }
    }
}

void MainWindow::onCalculateClicked() {
    // Synchronize vector data from tables
    m_courses = getCoursesFromUI();

    float totalCredits = CGPACalculator::calculateCredits(m_courses);
    float totalPoints = CGPACalculator::calculateGradePoints(m_courses);
    float cgpa = CGPACalculator::calculateCGPA(m_courses);
    std::string performance = CGPACalculator::calculatePerformance(cgpa);

    // Update main result labels
    ui->totalCreditsLabel->setText(QString("Total Credits: %1").arg(QString::number(totalCredits, 'f', 1)));
    ui->totalPointsLabel->setText(QString("Total Grade Points: %1").arg(QString::number(totalPoints, 'f', 2)));
    ui->cgpaLabel->setText(QString("CGPA: %1").arg(QString::number(cgpa, 'f', 2)));
    ui->performanceLabel->setText(QString("Performance: %1").arg(QString::fromStdString(performance)));

    // Update individual semester GPA labels
    for (size_t i = 0; i < m_semesterTables.size(); ++i) {
        int semNum = static_cast<int>(i) + 1;
        float semGpa = CGPACalculator::calculateGPA(m_courses, semNum);
        
        auto* label = ui->semesterTabWidget->widget(static_cast<int>(i))->findChild<QLabel*>(QString("semGpaLabel_%1").arg(semNum));
        if (label) {
            label->setText(QString("Semester GPA: %1").arg(QString::number(semGpa, 'f', 2)));
        }
    }
}

void MainWindow::onResetClicked() {
    if (QMessageBox::question(this, "Reset Confirmation", "Are you sure you want to clear all inputs and courses?") == QMessageBox::Yes) {
        ui->nameEdit->clear();
        ui->deptEdit->clear();
        ui->rollEdit->clear();
        ui->semesterCountSpinBox->setValue(1);
        m_courses.clear();
        onSemesterCountChanged(1);
        onCalculateClicked();
    }
}

void MainWindow::onSaveReportClicked() {
    m_courses = getCoursesFromUI();

    QString fileName = QFileDialog::getSaveFileName(this, "Save Report", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonObject rootObj;
    rootObj["studentName"] = ui->nameEdit->text();
    rootObj["department"] = ui->deptEdit->text();
    rootObj["rollNumber"] = ui->rollEdit->text();
    rootObj["semestersCount"] = ui->semesterCountSpinBox->value();

    QJsonArray coursesArray;
    for (const auto& course : m_courses) {
        coursesArray.append(course.toJson());
    }
    rootObj["courses"] = coursesArray;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        showErrorMessage("File Error", "Could not open file for writing.");
        return;
    }

    QJsonDocument doc(rootObj);
    file.write(doc.toJson());
    file.close();

    showSuccessMessage("Success", "Report saved successfully.");
}

void MainWindow::onLoadReportClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Report", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        showErrorMessage("File Error", "Could not open file for reading.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        showErrorMessage("Parse Error", "Invalid report format.");
        return;
    }

    QJsonObject rootObj = doc.object();
    ui->nameEdit->setText(rootObj["studentName"].toString());
    ui->deptEdit->setText(rootObj["department"].toString());
    ui->rollEdit->setText(rootObj["rollNumber"].toString());
    
    int semCount = rootObj["semestersCount"].toInt(1);
    ui->semesterCountSpinBox->setValue(semCount);
    
    m_courses.clear();
    QJsonArray coursesArray = rootObj["courses"].toArray();
    for (const auto& ref : coursesArray) {
        m_courses.push_back(Course::fromJson(ref.toObject()));
    }

    // Force redraw layout and tabs
    onSemesterCountChanged(semCount);
    onCalculateClicked();
    showSuccessMessage("Success", "Report loaded successfully.");
}

void MainWindow::generateReportHtml(QString& html) const {
    float totalCredits = CGPACalculator::calculateCredits(m_courses);
    float totalPoints = CGPACalculator::calculateGradePoints(m_courses);
    float cgpa = CGPACalculator::calculateCGPA(m_courses);
    std::string performance = CGPACalculator::calculatePerformance(cgpa);

    html = "<html><head><style>"
           "body { font-family: 'Segoe UI', Arial, sans-serif; color: #333; margin: 30px; }"
           "h1 { color: #2c3e50; border-bottom: 2px solid #34495e; padding-bottom: 10px; margin-bottom: 20px; }"
           ".student-info { margin-bottom: 25px; padding: 15px; background-color: #f8f9fa; border-left: 5px solid #3498db; border-radius: 4px; }"
           ".student-info table { width: 100%; border-collapse: collapse; }"
           ".student-info td { padding: 5px 10px; }"
           "table.courses-table { width: 100%; border-collapse: collapse; margin-top: 15px; }"
           "table.courses-table th, table.courses-table td { border: 1px solid #bdc3c7; padding: 10px; text-align: left; }"
           "table.courses-table th { background-color: #ecf0f1; color: #2c3e50; font-weight: bold; }"
           "table.courses-table tr:nth-child(even) { background-color: #f9f9f9; }"
           ".summary-panel { margin-top: 30px; padding: 15px; background-color: #2c3e50; color: white; border-radius: 6px; }"
           ".summary-table { width: 100%; border-collapse: collapse; }"
           ".summary-table td { padding: 8px 15px; font-size: 16px; }"
           ".summary-table td.highlight { font-size: 24px; font-weight: bold; color: #f1c40f; }"
           "</style></head><body>";

    html += "<h1>🎓 Academic CGPA Performance Report</h1>";

    // Student Info
    html += "<div class='student-info'><table>";
    html += QString("<tr><td><strong>Student Name:</strong></td><td>%1</td><td><strong>Department:</strong></td><td>%2</td></tr>")
                .arg(ui->nameEdit->text().isEmpty() ? "N/A" : ui->nameEdit->text())
                .arg(ui->deptEdit->text().isEmpty() ? "N/A" : ui->deptEdit->text());
    html += QString("<tr><td><strong>Roll Number:</strong></td><td>%1</td><td><strong>Date:</strong></td><td>%2</td></tr>")
                .arg(ui->rollEdit->text().isEmpty() ? "N/A" : ui->rollEdit->text())
                .arg(QDate::currentDate().toString("dd MMMM yyyy"));
    html += "</table></div>";

    // Semesters and course lists
    int numSemesters = ui->semesterCountSpinBox->value();
    for (int sem = 1; sem <= numSemesters; ++sem) {
        float semGpa = CGPACalculator::calculateGPA(m_courses, sem);
        html += QString("<h3>Semester %1 (GPA: %2)</h3>").arg(sem).arg(QString::number(semGpa, 'f', 2));

        html += "<table class='courses-table'>";
        html += "<thead><tr><th>Course Name</th><th>Grade Point</th><th>Credit Hours</th><th>Grade Points</th></tr></thead><tbody>";

        int coursesCount = 0;
        for (const auto& course : m_courses) {
            if (course.getSemester() == sem) {
                html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td></tr>")
                            .arg(QString::fromStdString(course.getName()))
                            .arg(QString::number(course.getGradePoint(), 'f', 2))
                            .arg(QString::number(course.getCreditHour(), 'f', 1))
                            .arg(QString::number(course.getGradePoints(), 'f', 2));
                coursesCount++;
            }
        }
        if (coursesCount == 0) {
            html += "<tr><td colspan='4' style='text-align: center; color: #7f8c8d;'>No courses recorded for this semester.</td></tr>";
        }

        html += "</tbody></table>";
    }

    // Results Summary
    html += "<div class='summary-panel'><table class='summary-table'><tr>";
    html += QString("<td>Total Credits Earned: <strong>%1</strong></td>").arg(QString::number(totalCredits, 'f', 1));
    html += QString("<td>Total Grade Points: <strong>%1</strong></td>").arg(QString::number(totalPoints, 'f', 2));
    html += QString("<td class='highlight'>CGPA: %1</td>").arg(QString::number(cgpa, 'f', 2));
    html += QString("<td>Performance: <strong>%1</strong></td>").arg(QString::fromStdString(performance));
    html += "</tr></table></div>";

    html += "</body></html>";
}

void MainWindow::onPrintClicked() {
    m_courses = getCoursesFromUI();

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle("Print Report");
    if (dialog.exec() != QDialog::Accepted) return;

    QString html;
    generateReportHtml(html);

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);
}

void MainWindow::onExportPdfClicked() {
    m_courses = getCoursesFromUI();

    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fileName);

    QString html;
    generateReportHtml(html);

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    showSuccessMessage("Success", "PDF report exported successfully.");
}

void MainWindow::onThemeToggleClicked() {
    m_isDarkMode = !m_isDarkMode;
    ui->themeToggleBtn->setText(m_isDarkMode ? "Toggle Light Mode" : "Toggle Dark Mode");
    applyTheme();
}

void MainWindow::applyTheme() {
    QString qss;
    if (m_isDarkMode) {
        // Premium Dark Theme styling
        qss = R"(
            QMainWindow, QDialog {
                background-color: #121214;
            }
            QWidget {
                color: #e2e2e7;
                font-family: "Segoe UI", Arial, Helvetica, sans-serif;
                font-size: 13px;
            }
            QFrame#headerFrame {
                background-color: #1e1e24;
                border: 1px solid #2d2d34;
                border-radius: 8px;
            }
            QLabel#titleLabel {
                color: #5e9ff2;
            }
            QLabel#subtitleLabel {
                color: #a0a0ab;
            }
            QFrame#detailsFrame, QFrame#resultsFrame {
                background-color: #1a1a20;
                border: 1px solid #2a2a30;
                border-radius: 8px;
            }
            QLineEdit {
                background-color: #25252b;
                border: 1px solid #3e3e4a;
                border-radius: 4px;
                padding: 6px 10px;
                color: #ffffff;
            }
            QLineEdit:focus {
                border: 1px solid #5e9ff2;
            }
            QSpinBox, QDoubleSpinBox {
                background-color: #25252b;
                border: 1px solid #3e3e4a;
                border-radius: 4px;
                padding: 5px;
                color: #ffffff;
            }
            QTabWidget::pane {
                border: 1px solid #2d2d34;
                background-color: #1a1a20;
                border-radius: 8px;
            }
            QTabBar::tab {
                background-color: #25252b;
                border: 1px solid #2d2d34;
                padding: 8px 16px;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
                color: #a0a0ab;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background-color: #1a1a20;
                border-bottom-color: transparent;
                color: #ffffff;
                font-weight: bold;
            }
            QTableWidget {
                background-color: #1c1c22;
                gridline-color: #2d2d34;
                border: 1px solid #2d2d34;
                border-radius: 4px;
            }
            QHeaderView::section {
                background-color: #27272f;
                padding: 6px;
                border: 1px solid #2d2d34;
                color: #e2e2e7;
                font-weight: bold;
            }
            QTableWidget::item:selected {
                background-color: #35495e;
                color: #ffffff;
            }
            QPushButton {
                background-color: #2c2c35;
                border: 1px solid #3e3e4a;
                border-radius: 4px;
                padding: 8px 16px;
                font-weight: bold;
                color: #ffffff;
            }
            QPushButton:hover {
                background-color: #3e3e4a;
            }
            QPushButton#calculateBtn {
                background-color: #27ae60;
                border-color: #219653;
            }
            QPushButton#calculateBtn:hover {
                background-color: #219653;
            }
            QPushButton#exportPdfBtn {
                background-color: #2980b9;
                border-color: #2471a3;
            }
            QPushButton#exportPdfBtn:hover {
                background-color: #2471a3;
            }
            QLabel#cgpaLabel {
                color: #f1c40f;
            }
        )";
    } else {
        // Premium Light Theme styling
        qss = R"(
            QMainWindow, QDialog {
                background-color: #f4f6f8;
            }
            QWidget {
                color: #2c3e50;
                font-family: "Segoe UI", Arial, Helvetica, sans-serif;
                font-size: 13px;
            }
            QFrame#headerFrame {
                background-color: #ffffff;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
            }
            QLabel#titleLabel {
                color: #3498db;
            }
            QLabel#subtitleLabel {
                color: #7f8c8d;
            }
            QFrame#detailsFrame, QFrame#resultsFrame {
                background-color: #ffffff;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
            }
            QLineEdit {
                background-color: #fdfdfd;
                border: 1px solid #cbd5e1;
                border-radius: 4px;
                padding: 6px 10px;
                color: #333333;
            }
            QLineEdit:focus {
                border: 1px solid #3498db;
            }
            QSpinBox, QDoubleSpinBox {
                background-color: #fdfdfd;
                border: 1px solid #cbd5e1;
                border-radius: 4px;
                padding: 5px;
                color: #333333;
            }
            QTabWidget::pane {
                border: 1px solid #e2e8f0;
                background-color: #ffffff;
                border-radius: 8px;
            }
            QTabBar::tab {
                background-color: #f1f5f9;
                border: 1px solid #e2e8f0;
                padding: 8px 16px;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
                color: #64748b;
                margin-right: 2px;
            }
            QTabBar::tab:selected {
                background-color: #ffffff;
                border-bottom-color: transparent;
                color: #1e293b;
                font-weight: bold;
            }
            QTableWidget {
                background-color: #ffffff;
                gridline-color: #f1f5f9;
                border: 1px solid #cbd5e1;
                border-radius: 4px;
            }
            QHeaderView::section {
                background-color: #f8fafc;
                padding: 6px;
                border: 1px solid #e2e8f0;
                color: #475569;
                font-weight: bold;
            }
            QTableWidget::item:selected {
                background-color: #e0f2fe;
                color: #0369a1;
            }
            QPushButton {
                background-color: #ffffff;
                border: 1px solid #cbd5e1;
                border-radius: 4px;
                padding: 8px 16px;
                font-weight: bold;
                color: #475569;
            }
            QPushButton:hover {
                background-color: #f1f5f9;
            }
            QPushButton#calculateBtn {
                background-color: #2ecc71;
                color: white;
                border-color: #27ae60;
            }
            QPushButton#calculateBtn:hover {
                background-color: #27ae60;
            }
            QPushButton#exportPdfBtn {
                background-color: #3498db;
                color: white;
                border-color: #2980b9;
            }
            QPushButton#exportPdfBtn:hover {
                background-color: #2980b9;
            }
            QLabel#cgpaLabel {
                color: #d35400;
            }
        )";
    }
    setStyleSheet(qss);
}

void MainWindow::showErrorMessage(const QString& title, const QString& message) {
    QMessageBox::critical(this, title, message);
}

void MainWindow::showSuccessMessage(const QString& title, const QString& message) {
    QMessageBox::information(this, title, message);
}

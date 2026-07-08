#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <vector>
#include <memory>
#include "Course.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class CourseDialog
 * @brief A dialog to input or edit Course details with validation.
 */
class CourseDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* nameEdit;
    QDoubleSpinBox* gradeSpinBox;
    QDoubleSpinBox* creditSpinBox;

public:
    explicit CourseDialog(QWidget* parent = nullptr, const Course* existingCourse = nullptr);

    [[nodiscard]] std::string getCourseName() const;
    [[nodiscard]] float getGradePoint() const;
    [[nodiscard]] float getCreditHour() const;
};

/**
 * @class MainWindow
 * @brief The main window controller of the CGPA Calculator application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSemesterCountChanged(int count);
    void onAddCourseClicked();
    void onEditCourseClicked();
    void onDeleteCourseClicked();
    void onCalculateClicked();
    void onResetClicked();
    void onSaveReportClicked();
    void onLoadReportClicked();
    void onPrintClicked();
    void onExportPdfClicked();
    void onThemeToggleClicked();

private:
    Ui::MainWindow* ui;
    bool m_isDarkMode{false};

    // Pointers to dynamically created tables for each semester tab
    std::vector<QTableWidget*> m_semesterTables;

    // Course data structure mapping: course lists
    std::vector<Course> m_courses;

    // UI Helper Methods
    void setupConnections();
    void updateSemesterTabs(int count);
    void refreshTable(int semesterIdx);
    void applyTheme();
    std::vector<Course> getCoursesFromUI() const;
    void populateUIFromCourses();
    void showErrorMessage(const QString& title, const QString& message);
    void showSuccessMessage(const QString& title, const QString& message);
    
    // PDF/Print helpers
    void generateReportHtml(QString& html) const;
};

#endif // MAINWINDOW_H

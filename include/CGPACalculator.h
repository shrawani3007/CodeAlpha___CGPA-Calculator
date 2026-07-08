#ifndef CGPACALCULATOR_H
#define CGPACALCULATOR_H

#include <vector>
#include <string>
#include "Course.h"

/**
 * @class CGPACalculator
 * @brief Logic engine for computing GPA and CGPA values.
 * No UI logic is present in this class.
 */
class CGPACalculator {
public:
    // Computes total grade points (sum of gradePoint * creditHour) for a list of courses
    static float calculateGradePoints(const std::vector<Course>& courses);

    // Computes total credits for a list of courses
    static float calculateCredits(const std::vector<Course>& courses);

    // Calculates the GPA for a specific semester
    static float calculateGPA(const std::vector<Course>& courses, int semester);

    // Calculates the overall CGPA across all semesters
    static float calculateCGPA(const std::vector<Course>& courses);

    // Translates the CGPA to a qualitative performance level
    static std::string calculatePerformance(float cgpa);
};

#endif // CGPACALCULATOR_H

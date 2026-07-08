#include "CGPACalculator.h"
#include <numeric>
#include <algorithm>

float CGPACalculator::calculateGradePoints(const std::vector<Course>& courses) {
    return std::accumulate(courses.begin(), courses.end(), 0.0f,
        [](float sum, const Course& c) { return sum + c.getGradePoints(); });
}

float CGPACalculator::calculateCredits(const std::vector<Course>& courses) {
    return std::accumulate(courses.begin(), courses.end(), 0.0f,
        [](float sum, const Course& c) { return sum + c.getCreditHour(); });
}

float CGPACalculator::calculateGPA(const std::vector<Course>& courses, int semester) {
    float totalPoints = 0.0f;
    float totalCredits = 0.0f;
    for (const auto& c : courses) {
        if (c.getSemester() == semester) {
            totalPoints += c.getGradePoints();
            totalCredits += c.getCreditHour();
        }
    }
    if (totalCredits <= 0.0f) {
        return 0.0f;
    }
    return totalPoints / totalCredits;
}

float CGPACalculator::calculateCGPA(const std::vector<Course>& courses) {
    float totalPoints = calculateGradePoints(courses);
    float totalCredits = calculateCredits(courses);
    if (totalCredits <= 0.0f) {
        return 0.0f;
    }
    return totalPoints / totalCredits;
}

std::string CGPACalculator::calculatePerformance(float cgpa) {
    if (cgpa >= 9.0f) {
        return "Outstanding";
    } else if (cgpa >= 8.0f) {
        return "Excellent";
    } else if (cgpa >= 7.0f) {
        return "Very Good";
    } else if (cgpa >= 5.0f) {
        return "Good";
    } else {
        return "Needs Improvement";
    }
}

#include "Validator.h"
#include <algorithm>
#include <cctype>

bool Validator::validateCourseName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    // Check if it's not just spaces
    return std::any_of(name.begin(), name.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });
}

bool Validator::validateGradePoint(float grade) {
    return grade >= 0.0f && grade <= 10.0f;
}

bool Validator::validateCreditHours(float credits) {
    return credits > 0.0f;
}

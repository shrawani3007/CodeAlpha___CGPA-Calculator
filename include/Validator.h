#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

/**
 * @class Validator
 * @brief Utility class to validate course attributes and student fields.
 */
class Validator {
public:
    // Checks if the course name is non-empty and contains non-whitespace characters
    static bool validateCourseName(const std::string& name);

    // Checks if the grade point is between 0.0 and 10.0 inclusive
    static bool validateGradePoint(float grade);

    // Checks if credit hours is greater than zero
    static bool validateCreditHours(float credits);
};

#endif // VALIDATOR_H

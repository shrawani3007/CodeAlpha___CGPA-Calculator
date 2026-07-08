#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <QJsonObject>

/**
 * @class Course
 * @brief Represents a single course with its name, grade point, credit hours, and semester.
 */
class Course {
private:
    std::string name;
    float gradePoint;
    float creditHour;
    int semester;

public:
    // Constructors
    Course();
    Course(std::string name, float gradePoint, float creditHour, int semester);

    // Getters
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] float getGradePoint() const;
    [[nodiscard]] float getCreditHour() const;
    [[nodiscard]] int getSemester() const;

    // Setters
    void setName(const std::string& newName);
    void setGradePoint(float newGradePoint);
    void setCreditHour(float newCreditHour);
    void setSemester(int newSemester);

    // Calculated Value
    [[nodiscard]] float getGradePoints() const;

    // JSON Serialization
    [[nodiscard]] QJsonObject toJson() const;
    static Course fromJson(const QJsonObject& json);
};

#endif // COURSE_H

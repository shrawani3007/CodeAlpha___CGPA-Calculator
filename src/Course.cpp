#include "Course.h"

Course::Course() : name(""), gradePoint(0.0f), creditHour(0.0f), semester(1) {}

Course::Course(std::string name, float gradePoint, float creditHour, int semester)
    : name(std::move(name)), gradePoint(gradePoint), creditHour(creditHour), semester(semester) {}

std::string Course::getName() const {
    return name;
}

float Course::getGradePoint() const {
    return gradePoint;
}

float Course::getCreditHour() const {
    return creditHour;
}

int Course::getSemester() const {
    return semester;
}

void Course::setName(const std::string& newName) {
    name = newName;
}

void Course::setGradePoint(float newGradePoint) {
    gradePoint = newGradePoint;
}

void Course::setCreditHour(float newCreditHour) {
    creditHour = newCreditHour;
}

void Course::setSemester(int newSemester) {
    semester = newSemester;
}

float Course::getGradePoints() const {
    return gradePoint * creditHour;
}

QJsonObject Course::toJson() const {
    QJsonObject json;
    json["name"] = QString::fromStdString(name);
    json["gradePoint"] = static_cast<double>(gradePoint);
    json["creditHour"] = static_cast<double>(creditHour);
    json["semester"] = semester;
    return json;
}

Course Course::fromJson(const QJsonObject& json) {
    return Course(
        json["name"].toString().toStdString(),
        static_cast<float>(json["gradePoint"].toDouble()),
        static_cast<float>(json["creditHour"].toDouble()),
        json["semester"].toInt(1)
    );
}

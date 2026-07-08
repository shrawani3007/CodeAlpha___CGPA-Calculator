# 🎓 CGPA Calculator

A production-quality desktop application built using **Modern C++20**, **Qt 6 Widgets**, and **CMake**. This project implements an MVC-inspired architecture and professional design guidelines, suitable for academic reporting and portfolio presentation.

---

## 🌟 Key Features

- **Multi-Semester Management**: Dynamically configure the number of semesters (1 to 8). Switch semesters instantly using tabs, managing individual course lists for each term.
- **Dynamic Calculation**: Real-time evaluation of Semester GPA, total credits, total grade points, and Cumulative CGPA.
- **Academic Performance Mapping**: Automatically maps scores to descriptive academic levels (*Outstanding*, *Excellent*, *Very Good*, *Good*, *Needs Improvement*).
- **Theme Customization**: Beautiful custom styling with a polished **Light Mode** and **Dark Mode** toggle.
- **Validation Engine**: Prevents corrupt or invalid inputs (e.g. restricts grades to `0-10`, requires non-empty course names, and enforces positive credit hours).
- **Report Save & Load**: Local reports are saved in structured **JSON** files and can be reloaded at any time.
- **PDF Exporting & Printing**: Instantly exports print-ready, professional HTML-styled PDF reports containing student summaries and course tables.

---

## 📂 Project Structure

```text
CGPA-Calculator/
├── CMakeLists.txt
├── resources.qrc
├── assets/
│   └── icon.png
├── include/
│   ├── Course.h
│   ├── CGPACalculator.h
│   ├── Validator.h
│   └── MainWindow.h
├── src/
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── Course.cpp
│   ├── CGPACalculator.cpp
│   └── Validator.cpp
├── ui/
│   └── MainWindow.ui
└── README.md
```

---

## 💻 Tech Stack

- **Language**: Modern C++20
- **Framework**: Qt 6.5+ Widgets (and Qt PrintSupport)
- **Build System**: CMake 3.16+
- **Compiler**: GCC / Clang / MSVC

---

## 🚀 How to Build & Run

### Prerequisites
1. **C++20 Compiler** (e.g., GCC 13+ or MSVC 2022)
2. **CMake** (3.16 or higher)
3. **Qt 6 SDK** (specifically `qtbase` and `qttools` modules)

### Compilation Steps

1. **Configure the Project**:
   Point CMake to your Qt installation using the `-DCMAKE_PREFIX_PATH` flag:
   ```bash
   cmake -G "Ninja" -DCMAKE_PREFIX_PATH="<path-to-qt>/mingw_64" -B build -S .
   ```

2. **Build the Application**:
   ```bash
   cmake --build build --config Release
   ```

3. **Running**:
   Ensure the Qt DLLs are in your system PATH or environment:
   ```powershell
   $env:PATH += ";<path-to-qt>/mingw_64/bin"
   .\build\CGPACalculator.exe
   ```

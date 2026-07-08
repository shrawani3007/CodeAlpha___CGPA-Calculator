# 🎓 CGPA Calculator

A production-quality desktop application built in **Modern C++20**, **Qt 6 Widgets**, and **CMake**. Designed with a clean architecture (MVC-inspired) and professional styling, this calculator supports dynamic multi-semester management, data serialization (JSON), PDF report generation, and Light/Dark themes.

---

## 🌟 Key Features

*   **Dynamic Multi-Semester Layout**: Configure the number of semesters (1 to 8) dynamically. The application builds individual workspace tabs containing distinct course entry tables for each term.
*   **Mathematical Calculations**: Computes **Semester GPA** and **Cumulative CGPA** in real-time.
*   **Qualitative Academic Performance**: Automatically determines performance status (*Outstanding*, *Excellent*, *Very Good*, *Good*, *Needs Improvement*).
*   **Input Validation & Protection**: Prevents crashes or corrupt data (restricts grades to `0-10`, requires non-empty course names, and enforces positive credit hours).
*   **JSON Save/Load**: Save your semester workspaces into clean, portable JSON files and reload them later.
*   **PDF Performance Report**: Generates and prints custom HTML-styled academic transcripts containing student information, term tables, and overall metrics.
*   **Polished Themes**: Toggle instantly between a modern **Light Mode** and an eye-friendly **Dark Mode** (complete with soft drop shadows and rounded card panels).

---

## 📂 Project Structure

```text
CGPA-Calculator/
├── CMakeLists.txt
├── resources.qrc
├── assets/
│   └── icon.png           # Professional custom application icon
├── include/
│   ├── Course.h           # Data Model
│   ├── CGPACalculator.h   # Business Logic Engine
│   ├── Validator.h        # Input Validator
│   └── MainWindow.h       # UI Controller
├── src/
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── Course.cpp
│   ├── CGPACalculator.cpp
│   └── Validator.cpp
├── ui/
│   └── MainWindow.ui      # UI Layout definitions
└── README.md
```

---

## 🛠️ Tech Stack & Dependencies

*   **Language**: C++20
*   **Framework**: Qt 6.5+ (QtCore, QtGui, QtWidgets, QtPrintSupport)
*   **Build System**: CMake 3.16+
*   **Compilers**: GCC 11+ / Clang 13+ / MSVC 2022

---

## 🚀 Easy Setup & Compilation

### Step 1: Install Prerequisites

#### 🪟 Windows Setup (MinGW)
1. **Compiler & CMake**: Install [MinGW-w64 (winlibs)](https://winlibs.com/) or another compiler suite, and ensure `g++`, `cmake`, and `ninja` are in your system `PATH`.
2. **Qt 6 SDK**: You can install Qt 6 easily using `pip` and the `aqtinstall` helper:
   ```bash
   pip install aqtinstall
   python -m aqt install-qt windows desktop 6.5.3 win64_mingw --archives qtbase qttools
   ```
   *This installs the minimal components needed to compile widgets (approx. 200MB).*

#### 🐧 Linux Setup (Ubuntu/Debian)
Install compiler tools and Qt6 development libraries via `apt`:
```bash
sudo apt update
sudo apt install build-essential cmake ninja-build qt6-base-dev qt6-tools-dev qt6-translations-l10n
```

#### 🍎 macOS Setup
Install via Homebrew:
```bash
brew install cmake ninja qt@6
```

---

### Step 2: Configure & Build

1.  **Clone the Repository**:
    ```bash
    git clone https://github.com/shrawani3007/CodeAlpha___CGPA-Calculator.git
    cd CodeAlpha___CGPA-Calculator
    ```

2.  **Configure with CMake**:
    Point CMake to your Qt6 installation path:
    ```bash
    # For Windows (replace with your custom Qt output directory path):
    cmake -G "Ninja" -DCMAKE_PREFIX_PATH="qt/6.5.3/mingw_64" -B build -S .

    # For macOS (Homebrew default path):
    cmake -G "Ninja" -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6" -B build -S .

    # For Linux (usually auto-detected):
    cmake -G "Ninja" -B build -S .
    ```

3.  **Compile the Binary**:
    ```bash
    cmake --build build --config Release
    ```

---

### Step 3: Run the Application

#### 🪟 Windows (Stand-alone Package)
We have packaged the build directory so you can run it directly. To make the executable portable and run it stand-alone:
```bash
# Deploys dependencies (DLLs and platform plugins) next to the executable
qt/6.5.3/mingw_64/bin/windeployqt.exe build/CGPACalculator.exe
```
Now, simply double-click **`build/CGPACalculator.exe`** from your File Explorer to run it!

#### 🐧 Linux / 🍎 macOS
Launch directly from the terminal:
```bash
./build/CGPACalculator
```

---

## 📈 Formula Details & Grading Scale

The calculations inside [CGPACalculator.cpp](file:///s:/Project/kadu/src/CGPACalculator.cpp) run using standard weightage rules:

*   **Semester GPA**:
    $$\text{GPA} = \frac{\sum (\text{Grade Point} \times \text{Credit Hours})}{\sum \text{Credit Hours}} \quad \text{for active semester courses}$$
*   **Cumulative CGPA**:
    $$\text{CGPA} = \frac{\sum (\text{Grade Point} \times \text{Credit Hours})}{\sum \text{Credit Hours}} \quad \text{across all semester courses}$$

### Performance Standards:
*   $\ge 9.0$: **Outstanding**
*   $\ge 8.0$: **Excellent**
*   $\ge 7.0$: **Very Good**
*   $\ge 5.0$: **Good**
*   $< 5.0$: **Needs Improvement**

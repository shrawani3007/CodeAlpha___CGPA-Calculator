cmake_minimum_required(VERSION 3.16)
project(CGPACalculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure Qt 6 Auto compilers
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)


# Find Qt6 components
find_package(Qt6 COMPONENTS Widgets Core Gui PrintSupport REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Course.cpp
    src/CGPACalculator.cpp
    src/Validator.cpp
    resources.qrc
)

# UI files
set(UIS
    ui/MainWindow.ui
)

# Headers (primarily for IDE visibility)
set(HEADERS
    include/MainWindow.h
    include/Course.h
    include/CGPACalculator.h
    include/Validator.h
)

# Target executable definition
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${UIS}
    ${HEADERS}
)

# Link Qt 6 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
)

# Cross-platform deployment configurations (e.g. disable console on Windows Release builds)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

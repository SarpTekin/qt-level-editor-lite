cmake_minimum_required(VERSION 3.16)
project(QtLevelEditorLite VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 (or Qt5 if you prefer - adjust accordingly)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC (Meta-Object Compiler) for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source files
set(SOURCES
    main.cpp
    MainWindow.cpp
    Canvas.cpp
    Entity.cpp
)

set(HEADERS
    MainWindow.h
    Canvas.h
    Entity.h
)

# Create executable
add_executable(QtLevelEditorLite ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(QtLevelEditorLite
    Qt6::Core
    Qt6::Widgets
)
cmake_minimum_required(VERSION 3.16)
project(QtLevelEditorLite VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC (Meta-Object Compiler) for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Include directories - headers are in include/
include_directories(include)

# Source files (all in src/)
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Canvas.cpp
    src/Entity.cpp
    src/InspectorPanel.cpp
    src/AddEntityCommand.cpp
    src/DeleteEntityCommand.cpp
    src/MoveEntityCommand.cpp
)

# Header files (all in include/)
set(HEADERS
    include/MainWindow.h
    include/Canvas.h
    include/Entity.h
    include/InspectorPanel.h
    include/AddEntityCommand.h
    include/DeleteEntityCommand.h
    include/MoveEntityCommand.h
)

# Create executable
add_executable(QtLevelEditorLite ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(QtLevelEditorLite
    Qt6::Core
    Qt6::Widgets
)
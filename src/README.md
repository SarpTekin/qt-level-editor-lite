# Qt Level Editor Lite

A clean, lightweight 2D level editor built with C++17 and Qt6 Widgets. This desktop application provides a simple yet powerful interface for creating and editing 2D scenes with entities, featuring real-time property editing, grid-based alignment, and a complete undo/redo system.

## Features

### Core Editing
- **Entity Creation**: Click on the canvas to place new entities
- **Entity Selection**: Click entities or select from the object list panel
- **Entity Movement**: Drag-and-drop to reposition entities
- **Entity Deletion**: Delete key to remove selected entities
- **Entity Duplication**: Ctrl+D to duplicate selected entities with offset positioning

### Inspector Panel
- **Real-time Property Editing**: Edit entity properties (name, color, size) with bidirectional synchronization
- **Color Picker**: Visual color selection dialog
- **Live Updates**: Changes in the inspector immediately reflect on the canvas

### Grid System
- **Grid Overlay**: Toggleable grid visualization for alignment reference
- **Snap-to-Grid**: Optional snap-to-grid functionality for precise placement
- **Configurable Grid Size**: Default 20px grid with configurable size

### Undo/Redo System
- **Full Undo/Redo Support**: All operations (add, delete, move) are undoable
- **Command Pattern**: Implemented using Qt's QUndoStack framework
- **Keyboard Shortcuts**: Ctrl+Z for undo, Ctrl+Y for redo

### Scene Persistence
- **JSON Format**: Save and load scenes in human-readable JSON format
- **File Operations**: Save/Load dialogs via File menu
- **Keyboard Shortcuts**: Ctrl+S to save, Ctrl+O to load

### Object List Panel
- **Entity Overview**: Docked panel showing all entities in the scene
- **Bidirectional Selection**: Selecting from list or canvas syncs selection state

## Screenshots

*Add screenshots here if available*

## Technology Stack

- **Language**: C++17
- **Framework**: Qt6 (Widgets, Core, JSON, Undo)
- **Build System**: CMake 3.16+
- **Architecture Patterns**:
  - Object-Oriented Programming
  - Command Pattern (for undo/redo)
  - Custom Widgets
  - Signal/Slot patterns

## Building from Source

### Prerequisites

- CMake 3.16 or higher
- Qt6 (Core and Widgets modules)
- C++17 compatible compiler (GCC, Clang, or MSVC)

### macOS (using Homebrew)

# Install CMake
brew install cmake

# Install Qt6
brew install qt6### Build Instructions

# Clone the repository
git clone https://github.com/SarpTekin/qt-level-editor-lite.git
cd qt-level-editor-lite

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run the application
./QtLevelEditorLite### Linux

# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake qt6-base-dev qt6-base-dev-tools build-essential

# Follow build instructions above### Windows
ash
# Install Qt6 via Qt Installer
# Ensure CMake is installed and in PATH

# Configure with Qt6 path (adjust path as needed)
cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2019_64

# Build
cmake --build . --config Release

# Run
.\Release\QtLevelEditorLite.exe## Usage

### Basic Workflow

1. **Create Entities**: Click anywhere on the canvas to create a new entity
2. **Select Entity**: Click on an entity or select from the Objects panel
3. **Edit Properties**: Use the Inspector panel to modify name, color, and size
4. **Move Entities**: Click and drag entities to reposition them
5. **Duplicate**: Select an entity and press Ctrl+D to create a copy
6. **Delete**: Select an entity and press Delete or Backspace
7. **Save Scene**: File → Save Scene (Ctrl+S) to save your work
8. **Load Scene**: File → Load Scene (Ctrl+O) to ope
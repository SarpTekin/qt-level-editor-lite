# 🚀 Qt Level Editor Lite

A clean, lightweight **2D level editor** built with **C++17** and **Qt6 Widgets**.  
This desktop application provides a simple yet powerful interface for creating and editing 2D scenes, featuring real-time property editing, grid-based alignment, undo/redo, and scene persistence.

---

## ✨ Features

### 🧱 Core Editing
- **Entity Creation:** Click on the canvas to place new entities  
- **Selection System:** Click entities or use the Objects panel  
- **Movement:** Drag-and-drop repositioning  
- **Deletion:** Delete key removes selected entity  
- **Duplication:** `Ctrl + D` duplicates entities with offset  

### 🔎 Inspector Panel
- Real-time editing of **name, color, width, height**  
- Qt-native **Color Picker**  
- Immediate canvas synchronization  

### 📏 Grid System
- Toggleable grid overlay  
- Optional **Snap-to-Grid**  
- Adjustable grid size (default 20px)  

### ↩️ Undo / Redo
- Full history tracking for all actions  
- Built using **Qt’s QUndoStack**  
- `Ctrl + Z` / `Ctrl + Y` shortcuts  

### 📁 Scene Persistence
- Save/Load scenes using **JSON**  
- File dialogs integrated  
- `Ctrl + S` / `Ctrl + O` shortcuts  

### 📋 Object List Panel
- Displays all entities in the scene  
- **Bidirectional selection** between list and canvas  

---

## 🛠 Tech Stack

- **Language:** C++17  
- **Framework:** Qt6 (Widgets, Core, JSON, Undo Framework)  
- **Build System:** CMake 3.16+  
- **Architecture Patterns:**  
  - Object-Oriented Programming  
  - **Command Pattern** (Undo/Redo)  
  - Qt Signals/Slots  
  - Custom Widgets  

---

## 🔧 Building from Source

### 📦 Prerequisites
- Qt6 (Widgets & Core)  
- CMake 3.16+  
- C

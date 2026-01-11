# 🗡️ Text RPG (Console-Based)

Text RPG is a console-based role-playing game written in **C++**, featuring character creation, turn-based combat, and a persistent save/load system.

This project focuses on core game logic, state management, and file I/O, making it a solid demonstration of C++ fundamentals and system-level programming on Windows.

---

## ✨ Features

### 🎮 Gameplay
- Character creation (name, stats, progression)
- Turn-based combat system
- Simple enemy encounters
- Text-based exploration and menu navigation

### 💾 Save & Load System
- Manual save and load support
- Persistent character data stored on disk
- Multiple save files supported
- Binary file I/O using the Windows API

### 🧠 Core Systems
- Player stats and leveling
- Combat calculations
- Input validation and error handling
- Game state persistence between sessions

---

## 🚀 How to Run

### Option 1: Compile with g++ (Recommended)

```bash
g++ -std=gnu++17 SimpleRPGfinal123.cpp -o TextRPG.exe
TextRPG.exe
```
### Option 2: Run Prebuilt Executable

1. Navigate to the `output/` folder  
2. Run `TextRPG.exe`

⚠️ **Note:** On Windows, you may see a security warning for unsigned executables. This is expected for locally built binaries.

---

## 🧱 Project Structure

```text
TextRPG/
├── SimpleRPGfinal123.cpp
├── save/
│   └── savefile/
│       └── *.sav
├── output/
│   └── TextRPG.exe
└── README.md
```
## 🧠 How It Works

### Character System
- Player stats are stored in a structured data model
- Stats directly affect combat outcomes and progression
- Character data persists across sessions via save files

### Combat System
- Turn-based combat loop
- Player and enemy actions resolved sequentially
- Damage calculated using player stats and enemy attributes
- Game ends when the player’s HP reaches zero

### Save / Load Mechanism
- Save files are stored in the `save/savefile/` directory
- Each save file contains:
  - Character name
  - Player stats
  - Progress data
- Data is written to and read from disk using binary file operations
- Required directories are created automatically if missing

---

## 🛠️ Technical Details
- **Language:** C++
- **Standard:** C++17
- **Platform:** Windows
- **File I/O:** Windows API (CreateFile, ReadFile, WriteFile)
- **Build Tool:** g++ (MinGW)

---

## 📌 Learning Objectives
- C++ control flow and data structures
- Binary file handling
- Game state persistence
- Console-based UI design
- Defensive programming and input validation

---

## 🔮 Future Improvements
- Cross-platform file I/O support
- Inventory and item system
- Expanded enemy variety and basic AI
- Configurable difficulty levels
- Refactor to modern C++ (std::filesystem, RAII, smart pointers)

---

## 📜 License

This project is released under the **MIT License**.
Feel free to use, modify, or extend it for learning purposes.

# Thiranex Mini Game Project - Snake Game (C++)

A console-based, arcade-style **Snake Game** developed in C++ for the **Thiranex Mini Game Project**. This project satisfies all core criteria by demonstrating essential programming concepts such as **loops**, **arrays**, and **conditional logic**, along with dynamic board rendering, win/loss detection, and replay functionality.

---

## 🎯 Project Features & Criteria Alignment

| Requirement | Implementation Details |
| :--- | :--- |
| **Loops** | • Game loop (`while (!gameOver)`)<br>• Board rendering nested loops (`for (int y...)` & `for (int x...)`)<br>• Tail propagation array shift loop (`for (int i...)`)<br>• Outer replay loop (`do ... while (playAgain)`) |
| **Arrays** | • 1D fixed-size coordinate arrays: `tailX[MAX_TAIL]` and `tailY[MAX_TAIL]` tracking the historical positions of the snake's body segments. |
| **Conditional Logic** | • Boundary collision checks (`headX < 0 \|\| headX >= BOARD_WIDTH ...`)<br>• Self-collision checks against tail coordinates<br>• Fruit eating and score calculation<br>• Win condition (maximum tail length) vs Loss condition<br>• Direction reversal prevention (e.g., cannot turn immediately backward) |
| **Dynamic Board** | • Renders board dynamically every frame using `SetConsoleCursorPosition` to prevent console screen flickering. |
| **Win/Loss & Replay** | • Game Over screen displaying final score and session high score with a `Y/N` replay prompt. |

---

## 🕹️ Controls & Gameplay

- **W** or **Up Arrow**: Move Up
- **S** or **Down Arrow**: Move Down
- **A** or **Left Arrow**: Move Left
- **D** or **Right Arrow**: Move Right
- **X** or **Q**: Quit current game
- **Objective**: Collect the food (`*`) to grow the snake (`O` is the head, `o` is the body) and increase your score. Avoid hitting the borders (`#`) or running into your own body!

---

## 🛠️ How to Compile and Run

### Prerequisites
- A C++ compiler (such as **MinGW GCC / g++** or MSVC).

### Step 1: Open Terminal / PowerShell
Navigate to the project directory:
```powershell
cd "c:\Users\kirti\OneDrive\Desktop\Thiranex Project"
```

### Step 2: Compile
Compile the C++ source file using `g++`:
```powershell
g++ -O2 snake_game.cpp -o snake_game.exe
```

### Step 3: Run the Game
Execute the compiled binary:
```powershell
.\snake_game.exe
```

---

## 📋 Code Structure Overview

- **`initConsole()`**: Configures console properties, hiding the blinking cursor to provide a smooth arcade presentation.
- **`resetGame()`**: Initializes coordinates, clears the tail coordinate arrays, and places the snake at the center of the board.
- **`spawnFruit()`**: Randomly places food (`*`) in unoccupied grid cells using a loop that checks both the head and tail arrays.
- **`drawBoard()`**: Dynamically updates the console grid, printing borders, snake head, tail, food, and current/high score HUD without screen flickering.
- **`processInput()`**: Captures non-blocking input with `_kbhit()` and `_getch()`.
- **`updateLogic()`**: Propagates tail coordinates through the array, updates head position, tests for boundary/body collisions, and updates scores.
- **`main()`**: Manages the overall game loop and replay cycle (`do ... while`).

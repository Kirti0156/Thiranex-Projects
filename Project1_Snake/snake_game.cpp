/**
 * ============================================================================
 * Project: Mini Game Project - Snake Game
 * Language: C++ (Console-Based)
 * Description: Interactive arcade-style Snake game demonstrating core
 *              programming fundamentals:
 *              1. Loops (Nested for-loops, while game-loops, array shifting)
 *              2. Arrays (1D arrays tracking snake body coordinate history)
 *              3. Conditional Statements (Collision detection, direction logic,
 *                 win/loss evaluation, replay controls)
 * ============================================================================
 */

#include <iostream>
#include <conio.h>    // For non-blocking console input: _kbhit(), _getch()
#include <windows.h>  // For Sleep(), SetConsoleCursorPosition(), console controls
#include <cstdlib>    // For rand(), srand()
#include <ctime>      // For time()

using namespace std;

// ============================================================================
// CONSTANTS & CONFIGURATION
// ============================================================================
const int BOARD_WIDTH  = 28;    // Width of the playable arena
const int BOARD_HEIGHT = 18;    // Height of the playable arena
const int MAX_TAIL     = 300;   // Maximum possible snake length
const int GAME_SPEED_MS = 90;   // Game loop delay in milliseconds (lower = faster)

// Directions for snake movement
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// ============================================================================
// GAME STATE VARIABLES
// ============================================================================
bool gameOver;
bool gameWon;
Direction dir;

// Snake Head position (X, Y coordinates)
int headX, headY;

// Fruit position (X, Y coordinates)
int fruitX, fruitY;

// ----------------------------------------------------------------------------
// ARRAY USAGE REQUIREMENT:
// Fixed-size 1D arrays storing (X, Y) coordinate history of each tail segment.
// tailX[0], tailY[0] is the segment directly following the head.
// ----------------------------------------------------------------------------
int tailX[MAX_TAIL];
int tailY[MAX_TAIL];
int tailLength;

// Score Tracking
int currentScore = 0;
int highScore = 0;

// Windows Console Handle for smooth, flicker-free rendering
HANDLE consoleHandle;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================
void initConsole();
void resetGame();
void spawnFruit();
void drawBoard();
void processInput();
void updateLogic();
void showGameOverScreen();

// ============================================================================
// CONSOLE SETUP & FLICKER CONTROL
// ============================================================================

/**
 * Prepares the console window: hides the blinking cursor and stores handle.
 */
void initConsole() {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Hide console text cursor to give clean arcade look
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

/**
 * Resets console cursor to top-left (0,0).
 * This eliminates the violent screen flickering caused by system("cls").
 */
void setCursorToHome() {
    COORD origin = {0, 0};
    SetConsoleCursorPosition(consoleHandle, origin);
}

// ============================================================================
// GAME INITIALIZATION
// ============================================================================

/**
 * Spawns fruit at random coordinates within the arena, ensuring it
 * does not land on the snake's head or body segments.
 */
void spawnFruit() {
    bool onSnake;
    do {
        onSnake = false;
        fruitX = rand() % BOARD_WIDTH;
        fruitY = rand() % BOARD_HEIGHT;

        // Check if fruit overlaps with head
        if (fruitX == headX && fruitY == headY) {
            onSnake = true;
            continue;
        }

        // [LOOP + ARRAY]: Check if fruit overlaps with any tail segment
        for (int i = 0; i < tailLength; i++) {
            if (tailX[i] == fruitX && tailY[i] == fruitY) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
}

/**
 * Initializes/resets game state for a new round.
 */
void resetGame() {
    gameOver = false;
    gameWon = false;
    dir = STOP;

    // Center snake head in the board
    headX = BOARD_WIDTH / 2;
    headY = BOARD_HEIGHT / 2;

    // Reset tail
    tailLength = 0;
    // [LOOP + ARRAY]: Clear tail coordinate arrays
    for (int i = 0; i < MAX_TAIL; i++) {
        tailX[i] = 0;
        tailY[i] = 0;
    }

    currentScore = 0;

    // Seed random number generator
    srand(static_cast<unsigned int>(time(NULL)));

    spawnFruit();
}

// ============================================================================
// DYNAMIC BOARD RENDERING
// ============================================================================

/**
 * [LOOPS & CONDITIONALS]: Renders the dynamic game board, snake, fruit, and HUD.
 */
void drawBoard() {
    setCursorToHome();

    cout << "========================================" << endl;
    cout << "        MINI GAME: SNAKE ARCADE         " << endl;
    cout << "========================================" << endl;
    cout << " Score: " << currentScore << "  |  High Score: " << highScore << endl;
    cout << " Controls: W/A/S/D or Arrows | X: Exit" << endl;
    cout << "----------------------------------------" << endl;

    // [LOOP]: Render Top Border
    for (int i = 0; i < BOARD_WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";

    // [NESTED LOOPS]: Render rows (Y) and columns (X) of the playing grid
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            // [CONDITIONAL]: Left boundary wall
            if (x == 0) {
                cout << "#";
            }

            // [CONDITIONAL]: Snake Head
            if (x == headX && y == headY) {
                cout << "O";
            }
            // [CONDITIONAL]: Fruit
            else if (x == fruitX && y == fruitY) {
                cout << "*";
            }
            // [CONDITIONAL + LOOP + ARRAY]: Check if any tail segment occupies this cell
            else {
                bool isTail = false;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == x && tailY[k] == y) {
                        cout << "o";
                        isTail = true;
                        break;
                    }
                }
                if (!isTail) {
                    cout << " "; // Empty cell inside arena
                }
            }

            // [CONDITIONAL]: Right boundary wall
            if (x == BOARD_WIDTH - 1) {
                cout << "#";
            }
        }
        cout << "\n";
    }

    // [LOOP]: Render Bottom Border
    for (int i = 0; i < BOARD_WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";
    cout << "----------------------------------------" << endl;

    if (dir == STOP) {
        cout << " >> Press W/A/S/D or Arrow Key to Start! <<" << endl;
    } else {
        cout << " Playing... Watch out for walls & tail!   " << endl;
    }
}

// ============================================================================
// INPUT HANDLING
// ============================================================================

/**
 * Non-blocking keyboard input processing using _kbhit() and _getch().
 * Supports W, A, S, D and Arrow keys. Prevents instant 180-degree self-reversals.
 */
void processInput() {
    // [CONDITIONAL]: Check if a key was pressed without blocking the game loop
    if (_kbhit()) {
        int key = _getch();

        // Check for Arrow Key prefix (Windows sends 0 or 224 first)
        if (key == 0 || key == 224) {
            key = _getch(); // Read second code
            switch (key) {
                case 72: // Arrow UP
                    if (dir != DOWN) dir = UP;
                    break;
                case 80: // Arrow DOWN
                    if (dir != UP) dir = DOWN;
                    break;
                case 75: // Arrow LEFT
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 77: // Arrow RIGHT
                    if (dir != LEFT) dir = RIGHT;
                    break;
            }
        } else {
            // Standard letter keys
            switch (key) {
                case 'a':
                case 'A':
                    // [CONDITIONAL]: Prevent turning directly into opposite direction
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd':
                case 'D':
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w':
                case 'W':
                    if (dir != DOWN) dir = UP;
                    break;
                case 's':
                case 'S':
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x':
                case 'X':
                case 'q':
                case 'Q':
                    gameOver = true;
                    break;
            }
        }
    }
}

// ============================================================================
// GAME LOGIC & COLLISION DETECTION
// ============================================================================

/**
 * [LOOPS, ARRAYS & CONDITIONALS]:
 * Updates tail position history, moves head, checks collisions and score.
 */
void updateLogic() {
    // Don't update positions if game hasn't started moving yet
    if (dir == STOP) {
        return;
    }

    // ------------------------------------------------------------------------
    // [LOOP + ARRAY]: TAIL PROPAGATION LOGIC
    // Each tail segment takes the coordinate of the segment preceding it.
    // The first tail segment (index 0) takes the previous head position.
    // ------------------------------------------------------------------------
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = headX;
    tailY[0] = headY;

    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // [CONDITIONAL]: Move Snake Head according to current direction
    switch (dir) {
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
        default:
            break;
    }

    // ------------------------------------------------------------------------
    // [CONDITIONAL]: WALL COLLISION (Loss condition)
    // ------------------------------------------------------------------------
    if (headX < 0 || headX >= BOARD_WIDTH || headY < 0 || headY >= BOARD_HEIGHT) {
        gameOver = true;
        return;
    }

    // ------------------------------------------------------------------------
    // [LOOP + ARRAY + CONDITIONAL]: SELF-COLLISION (Loss condition)
    // Check if head collides with any part of the tail
    // ------------------------------------------------------------------------
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == headX && tailY[i] == headY) {
            gameOver = true;
            return;
        }
    }

    // ------------------------------------------------------------------------
    // [CONDITIONAL]: FRUIT COLLECTION & WIN DETECTION
    // ------------------------------------------------------------------------
    if (headX == fruitX && headY == fruitY) {
        currentScore += 10;

        // Update session high score
        if (currentScore > highScore) {
            highScore = currentScore;
        }

        // Increase tail length (limited by array bounds)
        if (tailLength < MAX_TAIL - 1) {
            tailLength++;
        } else {
            // [CONDITIONAL]: Win condition if snake reached max size
            gameWon = true;
            gameOver = true;
            return;
        }

        // Spawn new fruit in open area
        spawnFruit();
    }
}

// ============================================================================
// GAME OVER & REPLAY PROMPT
// ============================================================================

/**
 * Displays game result, final score, and prompts for replay.
 */
void showGameOverScreen() {
    cout << "\n========================================" << endl;
    if (gameWon) {
        cout << "       *** CONGRATULATIONS! ***         " << endl;
        cout << "   YOU WON! MAX SNAKE SIZE REACHED!     " << endl;
    } else {
        cout << "             GAME OVER!                 " << endl;
        cout << "       Better luck next time!           " << endl;
    }
    cout << "========================================" << endl;
    cout << " Final Score : " << currentScore << endl;
    cout << " High Score  : " << highScore << endl;
    cout << "----------------------------------------" << endl;
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    // Configure console properties
    initConsole();
    system("cls");

    bool playAgain = false;

    // [LOOP]: Outer Replay Loop allowing multiple games per session
    do {
        resetGame();

        // Clear console cleanly at start of each match
        system("cls");

        // [LOOP]: Core Game Loop
        while (!gameOver) {
            drawBoard();
            processInput();
            updateLogic();
            Sleep(GAME_SPEED_MS); // Control game speed
        }

        // Game ended: show outcome
        showGameOverScreen();

        // Prompt for replay
        cout << " Do you want to play again? (Y/N): ";
        char choice;
        cin >> choice;

        // [CONDITIONAL]: Evaluate replay choice
        if (choice == 'y' || choice == 'Y') {
            playAgain = true;
        } else {
            playAgain = false;
        }

    } while (playAgain);

    cout << "\nThank you for playing Snake Game! Goodbye.\n" << endl;
    return 0;
}

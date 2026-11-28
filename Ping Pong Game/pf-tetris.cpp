// ============================================================================
//  CS1002 Programming Fundamentals - Semester Project: TETRIS (SFML 3)
//  TEMPLATE
//
//  HOW THIS FILE IS DIVIDED
//    - Everything marked  [DONE]  is SFML code (window, keys, drawing).
//      You do NOT need to understand it. Just use it.
//    - Everything marked  TODO (YOUR LOGIC)  is game logic that your team must
//      write yourselves (the project rules say the logic must be your own).
//
//  The program compiles as it is, but the game will not work until you fill
//  in the TODOs. Suggested order is given in the comment above main().
// ============================================================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <optional>  // needed by SFML 3 for events

using namespace std;

// ============================================================================
//  1. CONSTANTS  (game values from Section 4 of the PDF)
// ============================================================================
const int   WINDOW_W = 520;
const int   WINDOW_H = 640;

const float CELL = 30.f;      // one cell is 30 x 30 pixels
const float BOARD_X = 20.f;      // top-left corner of the board (pixels)
const float BOARD_Y = 20.f;
const int   ROWS = 20;
const int   COLS = 10;

const int START_ROW = 0;         // a new piece starts here (top-left of 4x4 box)
const int START_COL = 3;

const int BASE_FALL_DELAY = 30;  // frames per row on level 1
const int SPEED_UP = 5;   // frames faster per level
const int MIN_FALL_DELAY = 5;   // never faster than this
const int LINES_PER_LEVEL = 10;

// points for clearing 0 / 1 / 2 / 3 / 4 rows at once
const int POINTS[5] = { 0, 100, 300, 500, 800 };

// game states (Feature 7)
const int MENU = 0;
const int PLAYING = 1;
const int GAME_OVER = 2;

// menu options (in this order)
const int OPT_NEW = 0;
const int OPT_CONTINUE = 1;
const int OPT_EXIT = 2;

// keys: handleInput() stores the key pressed in the variable pressedKey
const int KEY_NONE = 0;
const int KEY_LEFT = 1;
const int KEY_RIGHT = 2;
const int KEY_DOWN = 3;
const int KEY_UP = 4;   // rotate in game, move up in menu
const int KEY_DROP = 5;   // Space
const int KEY_ENTER = 6;
const int KEY_ESCAPE = 7;
const int KEY_HOLD = 8;   // C (bonus)

// colours: index 0 = empty cell, 1..7 = piece number + 1
sf::Color colors[8] = {
    sf::Color(25, 25, 45),   // 0: empty cell
    sf::Color::Cyan,         // 1: I
    sf::Color::Yellow,       // 2: O
    sf::Color(170, 90, 255), // 3: T (purple)
    sf::Color::Green,        // 4: S
    sf::Color::Red,          // 5: Z
    sf::Color::Blue,         // 6: J
    sf::Color(255, 150, 0)   // 7: L (orange)
};

// ============================================================================
//  2. GLOBAL VARIABLES
// ============================================================================
char pieces[7][4][4];      // 7 pieces, each in a 4x4 box (loaded from pieces.txt)
int  board[ROWS][COLS];    // 0 = empty, 1..7 = colour of the landed piece

int  currentPiece;         // number of the falling piece (0..6)
int  nextPiece;            // number of the next piece (0..6)
char shape[4][4];          // current shape of the falling piece (changes on rotate)
int  pieceRow, pieceCol;   // position of the falling piece's 4x4 box on the board

int  score = 0;
int  lines = 0;
int  level = 1;
int  fallCounter = 0;      // counts frames since the piece last moved down

int  gameState = MENU;
int  menuChoice = 0;       // which menu option is highlighted
int  pressedKey = KEY_NONE;

bool piecesLoaded = false; // false -> show an error message (Feature 2)
bool showNoSaveMsg = false;// true -> menu shows "No saved game"
bool wantExit = false;     // true -> main loop closes the window

// ---- bonus (optional) ----
int  holdPiece = -1;       // -1 = nothing held yet
bool holdUsed = false;    // hold is allowed only once per piece

sf::Font font;             // font used to write text

// ============================================================================
//  3. FUNCTION DECLARATIONS
// ============================================================================
// ---- [DONE] SFML functions ----
void handleInput(sf::RenderWindow& window);
void drawGame(sf::RenderWindow& window);
void drawMenu(sf::RenderWindow& window);
void drawPlaying(sf::RenderWindow& window);
void drawGameOver(sf::RenderWindow& window);
void drawBoard(sf::RenderWindow& window);
void drawFallingPiece(sf::RenderWindow& window);
void drawShadow(sf::RenderWindow& window);
void drawSidePanel(sf::RenderWindow& window);
void drawShapeAt(sf::RenderWindow& window, const char s[4][4], float x, float y, int colorIndex, bool outlineOnly);
void drawText(sf::RenderWindow& window, const string& str, float x, float y, int size, sf::Color color);

// ---- TODO (YOUR LOGIC) functions ----
bool loadPieces();
void resetGame();
void startNewGame();
bool canPlace(const char s[4][4], int row, int col);
void copyPieceToShape(int pieceNumber);
bool spawnNewPiece();
void moveLeft();
void moveRight();
bool moveDown();
void rotatePiece();
void hardDrop();
void lockPiece();
int  clearLines();
void updateScoreAndLevel(int rowsCleared);
int  getFallDelay();
void saveGame();
bool loadGame();
void clearSaveFile();
void holdCurrentPiece();      // bonus
int  getShadowRow();          // bonus

void updateGame();
void updateMenu();
void updatePlaying();
void updateGameOver();

// ============================================================================
//  4. TODO (YOUR LOGIC) - FILE LOADING AND SETUP  (Feature 2)
// ============================================================================

// Reads pieces.txt into the 3D array pieces[7][4][4].
// Returns true if the file was read, false if the file is missing.



// Copies pieces[pieceNumber] into the array shape[4][4].

// Empties the board and sets score = 0, lines = 0, level = 1.
void resetGame()
{
    for (int r = 0;r < ROWS;r++)
    {
        for (int c = 0;c < COLS;c++)
        {
            board[r][c] = 0;
        }
    }
    score = 0;
    lines = 0;
    level = 1;
    fallCounter = 0;
    holdPiece = -1;
    holdUsed = false;
}
// Starts a fresh game: reset everything, choose pieces, go to PLAYING.
void startNewGame()
{
    // TODO (YOUR LOGIC):
    //   1. Call resetGame().
    //   2. Choose nextPiece randomly with   rand() % 7
    //   3. Call spawnNewPiece() (it moves nextPiece into the falling piece
    //      and chooses a new random nextPiece).
    //   4. Set gameState = PLAYING;
}

// ============================================================================
//  5. TODO (YOUR LOGIC) - MOVING AND ROTATING  (Feature 3)
// ============================================================================

// THE most important function. Returns true if the 4x4 shape s fits on the
// board when its top-left corner is at (row, col).
bool canPlace(const char s[4][4], int row, int col)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            // Only check filled cells
            if (s[r][c] == '#')
            {
                // Calculate actual board position
                int boardRow = row + r;
                int boardCol = col + c;

                // Check boundaries
                if (boardCol < 0 || boardCol >= COLS ||
                    boardRow >= ROWS)
                {
                    return false;
                }

                // Check collision (only if row is valid)
                if (boardRow >= 0 &&
                    board[boardRow][boardCol] != 0)
                {
                    return false;
                }
            }
        }
    }

    // All filled cells passed the checks
    return true;
}
// Reads pieces.txt into the 3D array pieces[7][4][4].
// Returns true if the file was read, false if the file is missing.

// Copies pieces[pieceNumber] into the array shape[4][4].
void copyPieceToShape(int pieceNumber)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            shape[r][c] = pieces[pieceNumber][r][c];
        }
    }
}

void moveLeft()
{
    if (canPlace(shape, pieceRow, pieceCol - 1))
    {
        pieceCol--;
    }
}

void moveRight()
{
    if (canPlace(shape, pieceRow, pieceCol + 1))
    {
        pieceCol++;
    }
}

// Moves the piece down by one row if it fits. Returns true if it moved,
// false if it could not move (then the piece must lock).
bool moveDown()
{
    if (canPlace(shape, pieceRow + 1, pieceCol))
    {
        pieceRow++;
        return true;
    }
    return false;
}

// Rotates the piece clockwise.
// Rotates the piece clockwise.
void rotatePiece()
{
    char rotated[4][4];

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            rotated[r][c] = shape[3 - c][r];
        }
    }

    if (canPlace(rotated, pieceRow, pieceCol))
    {
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                shape[r][c] = rotated[r][c];
            }
        }
    }
}

// ============================================================================
//  6. TODO (YOUR LOGIC) - FALLING, LOCKING, NEW PIECE  (Feature 4 and 6)
// ============================================================================

// Space key: move the piece down as far as it can go, then lock it.
void hardDrop()
{
    // TODO (YOUR LOGIC): keep calling moveDown() while it returns true,
    //                    then the piece is locked (see updatePlaying).
}

// Copies the blocks of the falling piece into board[][].
void lockPiece()
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (shape[row][col] == '#')
            {
                int boardRow = pieceRow + row;
                int boardCol = pieceCol + col;

                // Add 1 because the board uses 0 for an empty square.
                board[boardRow][boardCol] = currentPiece + 1;
            }
        }
    }
}
// Checks the board for full rows and removes them.
// Returns how many rows were removed.
int clearLines()
{
    int cleared = 0;
    int row = ROWS - 1;

    while (row >= 0)
    {
        bool full = true;

        for (int col = 0; col < COLS; col++)
        {
            if (board[row][col] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int r = row; r > 0; r--)
            {
                for (int col = 0; col < COLS; col++)
                {
                    board[r][col] = board[r - 1][col];
                }
            }

            for (int col = 0; col < COLS; col++)
            {
                board[0][col] = 0;
            }

            cleared++;
            
        }
        else
        {
            row--;
        }
    }

    return cleared;
}
// Adds points, adds lines, and updates the level.
void updateScoreAndLevel(int rowsCleared)
{
    score += POINTS[rowsCleared];
    lines += rowsCleared;
    level = 1 + lines / LINES_PER_LEVEL;
}
// How many frames the piece waits before falling one row.
int getFallDelay()
{
    // TODO (YOUR LOGIC):
    //   level 1 -> 30, level 2 -> 25, level 3 -> 20 ... never below 5.
    //   Use BASE_FALL_DELAY, SPEED_UP, MIN_FALL_DELAY and level.

    return BASE_FALL_DELAY; // TODO: replace with your formula
}

// Takes nextPiece as the new falling piece and chooses a new nextPiece.
// Returns true if the new piece fits at (START_ROW, START_COL).
// Returns false if it does not fit -> game over.
bool spawnNewPiece()
{
    // TODO (YOUR LOGIC):
    //   1. currentPiece = nextPiece;
    //   2. copyPieceToShape(currentPiece);
    //   3. pieceRow = START_ROW;  pieceCol = START_COL;
    //   4. nextPiece = rand() % 7;
    //   5. fallCounter = 0;  (bonus) holdUsed = false;
    //   6. return canPlace(shape, pieceRow, pieceCol);

    return true; // TODO: replace with your result
}

// ============================================================================
//  7. TODO (YOUR LOGIC) - SAVE AND CONTINUE  (Feature 8)
// ============================================================================
// Format of savegame.txt:
//   line 1      : score lines level
//   line 2      : next piece (0 to 6)
//   lines 3-22  : the 20 rows of the board, 10 numbers each, separated by spaces
// The falling piece is NOT saved.

void saveGame()
{
    ofstream fout("savegame.txt");
    
        fout << score << " " << lines << " " << level << endl;
        fout << nextPiece << endl;
        for (int r = 0;r < ROWS;r++)
        {
            for (int c = 0;c < COLS;c++)
            {
                fout << board[r][c] << " ";
            }
            fout << endl;
        }
    fout.close();
}

// Returns true if a game was loaded, false if there is no saved game.
bool loadGame()
{
    // TODO (YOUR LOGIC):
    //   1. Open "savegame.txt" with an ifstream.
    //   2. If it cannot be opened, or it is empty (you cannot read the
    //      score), return false.
    //   3. Read score, lines, level, nextPiece, then the board (nested loops).
    //   4. Close the file.
    //   5. fallCounter = 0; (bonus) holdPiece = -1; holdUsed = false;
    //   6. Call spawnNewPiece() so that the saved next piece starts falling
    //      from the top and a new next piece is chosen.
    //   7. Return true.

    return false; // TODO: replace with your result
}

// Makes savegame.txt empty (a finished game cannot be continued).
void clearSaveFile()
{
    ofstream fout("savegame.txt");
    fout.close();
}

// ============================================================================
//  8. TODO (YOUR LOGIC) - BONUS: SHADOW AND HOLD  (+5)
// ============================================================================

// Returns the row where the falling piece would land.
int getShadowRow()
{
    int row = pieceRow;

    while (canPlace(shape, row + 1, pieceCol))
    {
        row++;
    }

    return row;
}

// Key C: save the falling piece in the hold box, or swap it with the held one.
void holdCurrentPiece()
{
    // TODO (BONUS):
    //   - If holdUsed is true, do nothing.
    //   - If holdPiece == -1 (nothing held): holdPiece = currentPiece, then
    //     spawn the next piece (spawnNewPiece()).
    //   - Else: swap currentPiece and holdPiece, call copyPieceToShape,
    //     reset pieceRow/pieceCol to the start position.
    //   - holdUsed = true;
}

// ============================================================================
//  9. TODO (YOUR LOGIC) - UPDATE FUNCTIONS (the "update" job of the game loop)
//     pressedKey holds the key pressed in this frame (or KEY_NONE).
// ============================================================================

// Called once per frame. Each state does only its own work.
void updateGame()
{
    switch (gameState)
    {
    case MENU:
        updateMenu();
        break;
    case PLAYING:
        updatePlaying();
        break;
    case GAME_OVER:
        updateGameOver();
        break;
    }
    pressedKey = KEY_NONE; // the key has been used, forget it
}

void updateMenu()
{
    // TODO (YOUR LOGIC):
    //   - KEY_UP   : menuChoice goes up   (do not go above option 0)
    //   - KEY_DOWN : menuChoice goes down (do not go below option 2)
    //     (and set showNoSaveMsg = false when the player moves)
    //   - KEY_ENTER: use switch (menuChoice):
    //         OPT_NEW      -> startNewGame();
    //         OPT_CONTINUE -> if (loadGame()) gameState = PLAYING;
    //                         else showNoSaveMsg = true;   (stay in the menu)
    //         OPT_EXIT     -> wantExit = true;
}

void updatePlaying()
{
    // TODO (YOUR LOGIC):
    //   STEP 1 - react to the key. Use switch (pressedKey):
    //         KEY_LEFT   -> moveLeft();
    //         KEY_RIGHT  -> moveRight();
    //         KEY_DOWN   -> moveDown();
    //         KEY_UP     -> rotatePiece();
    //         KEY_DROP   -> hardDrop();  then lock the piece (STEP 3)
    //         KEY_HOLD   -> holdCurrentPiece();   (bonus)
    //         KEY_ESCAPE -> saveGame(); gameState = MENU; return;
    //
    //   STEP 2 - gravity:
    //         fallCounter++;
    //         if fallCounter >= getFallDelay():
    //             fallCounter = 0;
    //             if moveDown() fails -> the piece must lock (STEP 3)
    //
    //   STEP 3 - when a piece locks (write this as one block, or better,
    //            as your own small function):
    //         lockPiece();
    //         int cleared = clearLines();
    //         updateScoreAndLevel(cleared);
    //         if (!spawnNewPiece()) {
    //             clearSaveFile();
    //             gameState = GAME_OVER;
    //         }
}

void updateGameOver()
{
    if (pressedKey == KEY_ENTER)
    {
        gameState = MENU;
        menuChoice = 0;        // menu starts on "New Game"
        showNoSaveMsg = false;
    }

}

// ============================================================================
//  10. [DONE] INPUT  (SFML)  - reads the window events and stores the key
// ============================================================================
void handleInput(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close(); // X button ends the program
        }
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A)
                pressedKey = KEY_LEFT;
            else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D)
                pressedKey = KEY_RIGHT;
            else if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S)
                pressedKey = KEY_DOWN;
            else if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W)
                pressedKey = KEY_UP;
            else if (key->code == sf::Keyboard::Key::Space)
                pressedKey = KEY_DROP;
            else if (key->code == sf::Keyboard::Key::Enter)
                pressedKey = KEY_ENTER;
            else if (key->code == sf::Keyboard::Key::Escape)
                pressedKey = KEY_ESCAPE;
            else if (key->code == sf::Keyboard::Key::C)
                pressedKey = KEY_HOLD;
        }
    }
}

// ============================================================================
//  11. [DONE] DRAWING  (SFML)
// ============================================================================

// Writes a text on the screen.
void drawText(sf::RenderWindow& window, const string& str, float x, float y, int size, sf::Color color)
{
    sf::Text text(font, str, size);
    text.setFillColor(color);
    text.setPosition({ x, y });
    window.draw(text);
}

// Draws a 4x4 shape with its top-left corner at pixel (x, y).
// outlineOnly = true draws only an outline (used for the shadow).
void drawShapeAt(sf::RenderWindow& window, const char s[4][4], float x, float y, int colorIndex, bool outlineOnly)
{
    sf::RectangleShape block({ CELL - 2, CELL - 2 });
    if (outlineOnly)
    {
        block.setFillColor(sf::Color::Transparent);
        block.setOutlineColor(colors[colorIndex]);
        block.setOutlineThickness(2.f);
    }
    else
    {
        block.setFillColor(colors[colorIndex]);
    }

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (s[r][c] == '#')
            {
                block.setPosition({ x + c * CELL + 1, y + r * CELL + 1 });
                window.draw(block);
            }
        }
    }
}

// Draws the board border, every cell of board[][], so empty cells and landed
// blocks are drawn the same way using colors[board[r][c]].
void drawBoard(sf::RenderWindow& window)
{
    // border around the board
    sf::RectangleShape border({ COLS * CELL, ROWS * CELL });
    border.setPosition({ BOARD_X, BOARD_Y });
    border.setFillColor(sf::Color(15, 15, 30));
    border.setOutlineColor(sf::Color(130, 130, 200));
    border.setOutlineThickness(3.f);
    window.draw(border);

    sf::RectangleShape block({ CELL - 2, CELL - 2 });
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            block.setFillColor(colors[board[r][c]]);
            block.setPosition({ BOARD_X + c * CELL + 1, BOARD_Y + r * CELL + 1 });
            window.draw(block);
        }
    }
}

// Draws the falling piece at (pieceRow, pieceCol).
void drawFallingPiece(sf::RenderWindow& window)
{
    drawShapeAt(window, shape, BOARD_X + pieceCol * CELL, BOARD_Y + pieceRow * CELL,
        currentPiece + 1, false);
}

// Bonus: draws the outline of where the piece will land.
void drawShadow(sf::RenderWindow& window)
{
    int shadowRow = getShadowRow();
    drawShapeAt(window, shape, BOARD_X + pieceCol * CELL, BOARD_Y + shadowRow * CELL,
        currentPiece + 1, true);
}

// Side panel: score, lines, level, next piece (and hold box for the bonus).
void drawSidePanel(sf::RenderWindow& window)
{
    float x = 345.f;
    sf::Color label(150, 150, 200);

    drawText(window, "SCORE", x, 20, 18, label);
    drawText(window, to_string(score), x, 42, 26, sf::Color::White);

    drawText(window, "LINES", x, 95, 18, label);
    drawText(window, to_string(lines), x, 117, 26, sf::Color::White);

    drawText(window, "LEVEL", x, 170, 18, label);
    drawText(window, to_string(level), x, 192, 26, sf::Color::Green);

    // next piece box
    drawText(window, "NEXT", x, 245, 18, label);
    sf::RectangleShape box({ 4 * CELL + 10, 4 * CELL + 10 });
    box.setPosition({ x - 5, 270 });
    box.setFillColor(sf::Color(25, 25, 45));
    box.setOutlineColor(sf::Color(130, 130, 200));
    box.setOutlineThickness(2.f);
    window.draw(box);
    drawShapeAt(window, pieces[nextPiece], x, 275, nextPiece + 1, false);

    // hold box (bonus)
    drawText(window, "HOLD (C)", x, 425, 18, label);
    sf::RectangleShape holdBox({ 4 * CELL + 10, 4 * CELL + 10 });
    holdBox.setPosition({ x - 5, 450 });
    holdBox.setFillColor(sf::Color(25, 25, 45));
    holdBox.setOutlineColor(sf::Color(130, 130, 200));
    holdBox.setOutlineThickness(2.f);
    window.draw(holdBox);
    if (holdPiece != -1)
        drawShapeAt(window, pieces[holdPiece], x, 455, holdPiece + 1, false);
}

void drawPlaying(sf::RenderWindow& window)
{
    drawBoard(window);
    drawShadow(window);        // bonus: delete this line if you skip the bonus
    drawFallingPiece(window);
    drawSidePanel(window);
}

void drawMenu(sf::RenderWindow& window)
{
    drawText(window, "TETRIS", 170, 80, 64, sf::Color::Cyan);

    string options[3] = { "New Game", "Continue", "Exit" };
    for (int i = 0; i < 3; i++)
    {
        sf::Color c = (i == menuChoice) ? sf::Color::Yellow : sf::Color::White;
        string text = (i == menuChoice) ? "> " + options[i] : "  " + options[i];
        drawText(window, text, 190, 230.f + i * 60, 32, c);
    }

    if (showNoSaveMsg)
        drawText(window, "No saved game", 185, 440, 26, sf::Color::Red);

    drawText(window, "Up/Down: move   Enter: select", 120, 580, 18, sf::Color(150, 150, 200));
}

void drawGameOver(sf::RenderWindow& window)
{
    drawText(window, "GAME OVER", 110, 200, 56, sf::Color::Red);
    drawText(window, "Final score: " + to_string(score), 150, 300, 30, sf::Color::White);
    drawText(window, "Press Enter", 190, 380, 26, sf::Color::Yellow);
}

// Called once per frame: clears the window, draws the current state, shows it.
void drawGame(sf::RenderWindow& window)
{
    window.clear(sf::Color(15, 15, 30));

    if (!piecesLoaded)
    {
        // Feature 2: error message instead of crashing
        drawText(window, "ERROR: pieces.txt not found", 60, 250, 28, sf::Color::Red);
        drawText(window, "(or loadPieces() is not written yet)", 60, 300, 20, sf::Color::White);
    }
    else
    {
        switch (gameState)
        {
        case MENU:
            drawMenu(window);
            break;
        case PLAYING:
            drawPlaying(window);
            break;
        case GAME_OVER:
            drawGameOver(window);
            break;
        }
    }

    window.display();
}

// ============================================================================
//  12. MAIN  (the game loop: input -> update -> draw)
//
//  SUGGESTED ORDER TO WRITE YOUR LOGIC (test after every step):
//    1. loadPieces()                  -> the error message disappears
//    2. resetGame(), copyPieceToShape(), canPlace()
//    3. spawnNewPiece(), startNewGame() -> the piece appears
//    4. moveLeft/Right/Down(), rotatePiece()
//    5. gravity + lockPiece() inside updatePlaying()
//    6. hardDrop()
//    7. clearLines(), updateScoreAndLevel(), getFallDelay()
//    8. updateMenu(), updateGameOver(), game over check
//    9. saveGame(), loadGame(), clearSaveFile()
//   10. bonus: getShadowRow(), holdCurrentPiece()
// ============================================================================
int main()
{
    srand((unsigned)time(0)); // random numbers different on every run

    // [DONE] window: 520 x 640 titled "Tetris", 60 frames per second
    sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Tetris");
    window.setFramerateLimit(60);

    // [DONE] font. Windows already has Arial. If it fails, put any .ttf file
    // next to the program and rename it to arial.ttf.
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        if (!font.openFromFile("arial.ttf"))
        {
            cout << "Could not load a font." << endl;
            return 1;
        }
    }

    piecesLoaded = loadPieces();

    // make the board empty at the start (so the menu has something safe)
    resetGame();
    nextPiece = rand() % 7;

    // THE GAME LOOP
    while (window.isOpen())
    {
        handleInput(window);   // 1. read keys
        updateGame();          // 2. update the game
        drawGame(window);      // 3. draw

        if (wantExit)          // "Exit" chosen in the menu
            window.close();
    }

    return 0;
}

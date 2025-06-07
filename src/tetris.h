#include <vector>
#include <array>

#pragma once

// All 7 tetromino pieces and their rotations
const Tetromino I_PIECE = Tetromino({
    Rectangle(4, 4, {
        Cell::empty,     Cell::empty,     Cell::lightBlue,     Cell::empty,
        Cell::empty,     Cell::empty,     Cell::lightBlue,     Cell::empty,
        Cell::empty,     Cell::empty,     Cell::lightBlue,     Cell::empty,
        Cell::empty,     Cell::empty,     Cell::lightBlue,     Cell::empty,
    }),
    Rectangle(4, 4, {
        Cell::empty,     Cell::empty,     Cell::empty,     Cell::empty,
        Cell::empty,     Cell::empty,     Cell::empty,     Cell::empty,
        Cell::lightBlue, Cell::lightBlue, Cell::lightBlue, Cell::lightBlue,
        Cell::empty,     Cell::empty,     Cell::empty,     Cell::empty,
    })
});
const Tetromino L_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty,  Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::orange
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::empty,
        Cell::orange, Cell::orange, Cell::orange,
        Cell::orange, Cell::empty, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::orange, Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::orange,
        Cell::orange, Cell::orange, Cell::orange,
        Cell::empty,  Cell::empty,  Cell::empty
    })
});
const Tetromino J_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty, Cell::blue, Cell::empty,
        Cell::empty, Cell::blue, Cell::empty,
        Cell::blue,  Cell::blue, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::blue,  Cell::empty, Cell::empty,
        Cell::blue,  Cell::blue,  Cell::blue,
        Cell::empty, Cell::empty, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::blue, Cell::blue,
        Cell::empty, Cell::blue, Cell::empty,
        Cell::empty, Cell::blue, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::empty,
        Cell::blue,  Cell::blue,  Cell::blue,
        Cell::empty, Cell::empty, Cell::blue
    })
});
const Tetromino O_PIECE = Tetromino({
    Rectangle(2, 2, {
        Cell::yellow, Cell::yellow,
        Cell::yellow, Cell::yellow
    })
});
const Tetromino S_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::empty,
        Cell::empty, Cell::green, Cell::green,
        Cell::green, Cell::green, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::green, Cell::empty, Cell::empty,
        Cell::green, Cell::green, Cell::empty,
        Cell::empty, Cell::green, Cell::empty
    })
});
const Tetromino Z_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::empty,
        Cell::red,   Cell::red,   Cell::empty,
        Cell::empty, Cell::red,   Cell::red
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::red,
        Cell::empty, Cell::red,   Cell::red,
        Cell::empty, Cell::red,   Cell::empty
    })
});
const Tetromino T_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::purple, Cell::purple, Cell::purple,
        Cell::empty,  Cell::empty,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::empty,  Cell::purple, Cell::purple,
        Cell::empty,  Cell::purple, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::empty,
        Cell::purple, Cell::purple, Cell::purple,
        Cell::empty,  Cell::purple, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::purple, Cell::purple, Cell::empty,
        Cell::empty,  Cell::purple, Cell::empty
    })
});

const std::array<Tetromino*, 7> TETROMINOS = {&I_PIECE, &L_PIECE, &J_PIECE, &O_PIECE, &S_PIECE, &Z_PIECE, &T_PIECE};

// Represents a single square or cell in the playfield
enum class Cell {
    empty,
    border,
    lightBlue,
    blue,
    orange,
    yellow,
    green,
    red,
    purple
};

// Represents a point in 2D space
class Coordinate {
    public:
        Coordinate(int x, int y);
        int x, y;
};

// Represents 2D plane of cells of size length * height
class Rectangle {
    public:
        Rectangle(int width, int height, std::vector<Cell> cells);
        Rectangle(int width, int height);
        
        const int width;
        const int height;
        
        Cell getCell(int x, int y) const;
    
    private:
        std::vector<Cell> cells;
};

// Represents a tetromino. Each rotation is stored as an individual square
class Tetromino {
    public:
        Tetromino(std::vector<Rectangle> rotations);
        const std::vector<Rectangle> rotations;
};

/*
 * Represents the currently active Tetromino of a game
 * Intended to allow for a frontend to highlight active tetromino
 * Is updated by Tetris game, so frontend does not need to request a new one upon a change
 */
class ActiveTetromino {
    public:
        const Tetromino* getTetromino();
        const Rectangle* getCurrentRotation();
        // Represents position of bottom left cell
        Coordinate getOffset();

        friend class Tetris;
    
    private:
        ActiveTetromino();
        void rotateClockwise();
        void rotateAnticlockwise();

        const Tetromino* tetromino;
        int currentRotationIndex;
        Coordinate offset;
};

class ClearedRows {
    public:
        ClearedRows(std::vector<int> rows);
        std::vector<int> rows;
};

/*
 * Is a backend for a game of tetris
 * Stores game state, difficulty, score, etc
 * 
 * Despite keeping track of difficulty, this object does not do any kind of time keeping / game loop management
 * That must be handled by the frontend, as well as increases in difficulty
 * 
 * The playfield is a rectangle consisting of x * y cells
 * X value of 0 represents the leftmost column
 * Y value of 0 represents the bottom row
 * 
 * At any time, there is no more than 1 active tetromino
 * The movement methods will shift said tetromino 1 unit in the given direction
 * The exception to this being .moveToBottom which will move the tetromino to the lowest possible position
 * The rotate methods will rotate the tetromino 90 degrees in the given direction
 * If a move is successful, the method will return true
 * If a move is otherwise prevented by a collision, either with an occupied cell or playfield border, the method returns false
 * If a rotation causes a collision, the tetromino will be moved no more than width-1 in an any direction in order to allow the rotation
 * If this is not possible, the rotation will not occur and the method will return false
 * 
 * As timing is not handled by the backend, any non-player downwards movement must be performed by the frontend
 * 
 * The active tetromino will not be placed until .place() is called
 * The .place() method returns a struct containing a vector with a list of rows that will be cleared
 * If no rows are cleared with this placement, the next active tetromino is selected and the game continues
 * However, if 1 or more rows are cleared, the game state will be frozen until .clearRows() is called
 * This is to allow the frontend to highlight the rows before they are cleared out
 * Until .clearRows() is called, activeTetromino, score, and the playfield will not be updated, and any other method calls will throw an error
 * 
 * Score for rows cleared is equal to difficulty * 2 ^ number of rows cleared
 * Unless 1 row is cleared, then it is equal to difficulty * 1
 * 
 * Tetrominos are placed in a queue, which is intended to be displayed to the player
 * Like activeTetromino, this queue is updated by the game and does not have to be renewed
 * New tetrominos are added at random with no regard for balance (as of now)
 * 
 * A player must also be able to save a tetromino for later
 * If there is no currently saved tetromino, the tetromino is simply saved and the next tetromino in the queue becomes active
 * However, if there is a saved tetromino, it will be swapped with the active tetromino
 * A player may not "resave" a tetromino. Once it has been swapped out, it stays
 * Attempting to do so will throw an error
 */
class Tetris {
    public:
        Tetris(int width, int height, int initialDifficulty = 0, int queuedTetrominoSize = 3);
        Tetris(): Tetris(10, 16, 0, 3) {}

        int getScore();
        int getDifficulty();
        
        bool hasSavedTetromino();
        const Tetromino* getSavedTetromino();
        ActiveTetromino* getActiveTetromino();
        std::vector<const Tetromino*>* getQueuedTetrominos();

        Rectangle* getPlayField();

        // Defaults to incrementing by 1, but can increase or decrease level by arbitrary amount
        void updateDifficulty(int increment = 1);

        // Swaps currently active Tetromino with saved Tetromino
        bool saveTetromino();

        // Game moves. Returns true if successful
        bool moveLeft();
        bool moveRight();
        bool moveDown();
        bool moveToBottom();
        bool rotateClockwise();
        bool rotateAnticlockwise();
        ClearedRows place();
        void clearRows();
    
    private:
        const int initialDifficulty;
        
        int difficulty;
        int score;
        Rectangle playField;
        ActiveTetromino activeTetromino;
        std::vector<const Tetromino*> queuedTetrominos;
        const Tetromino* savedTetromino;
        bool savedTetrominoThisTurn;

        bool offsetActiveTetromino(int x, int y);
        std::vector<Coordinate> testForCollisions();
        const Tetromino* getRandomTetromino();
        void nextTurn(const Tetromino* nextTetromino);
};
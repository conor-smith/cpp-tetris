#include <vector>
#include <exception>
#include <string>

#pragma once

enum Cell {
    rectBorder = 0,
    empty = 1,
    cyan = 2,
    blue = 3,
    orange = 4,
    yellow = 5,
    green = 6,
    purple = 7,
    red = 8
};

class Rectangle {
    public:

    Rectangle(int width, int height);
    Rectangle(int width, int height, std::vector<Cell> contents);

    friend class TetrisState;
    
    Cell getCell(int x, int y) const;
    
    const int width;
    const int height;
    
    private:

    void setCell(int x, int y, Cell newValue);

    std::vector<Cell> contents;
};

class Coordinate {
    public:
    
    int getX();
    int getY();

    friend class TetrisState;

    private:
    
    int x;
    int y;
};

class Tetromino {
    public:
    Tetromino(const std::vector<Rectangle> rotations);

    const std::vector<Rectangle> rotations;
};

class ActiveTetromino {
    public:

    Coordinate getLocation();
    Coordinate getGhostLocation();
    const Rectangle* getTetrominoRotation();

    friend class TetrisState;
    
    private:
    
    Coordinate location;
    Coordinate ghostLocation;

    const Tetromino* tetromino;
    int rotation;
};

class TetrisState {
    public:
    
    TetrisState();
    Rectangle& getPlayField();
    ActiveTetromino& getActiveTetromino();
    std::vector<const Tetromino*>& getQueue();
    const Tetromino* getSavedPiece();

    int getScore();
    int getLevel();
    bool getGameOver();

    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotateClockwise();
    bool rotateAnticlockwise();

    bool saveActiveTetromino();
    
    // There must be a step between placing the tetromino and clearing the rows
    // This is to give the renderer time to play an animation
    // Once the first method is called, no moves may be made until the rows are cleared
    // In the step between placing and clearing, the ActiveTetromino tetromino will be added to the playfield
    // The activeTetromino object itself will return NULL when asked for the current tetromino
    std::vector<int>& placeActiveTetrominoAndGetRowsToClear();
    bool clearRowsAndContinue();

    private:
    
    Rectangle playField;
    ActiveTetromino activeTetromino;
    std::vector<const Tetromino*> queue;
    const Tetromino* savedPiece;

    int score;
    int level;
    bool gameOver;

    bool isAwaitingClearRows;
    std::vector<int> rowsToClear;

    const Tetromino* getRandomTetromino();
    void resetActiveTetromino(const Tetromino* nextTetromino);
    bool updateActiveTetromino(ActiveTetromino newAT);
    bool updateRotatedActiveTetromino(ActiveTetromino newAt);
    void calculateGhostTetromino();
};

class TetrisException : public std::exception {
    public:

    TetrisException(std::string message);
    virtual ~TetrisException();

    virtual const char* what() const noexcept;

    private:

    const std::string message;
};
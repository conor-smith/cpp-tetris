#include <vector>

#pragma once

enum class Cell {
    empty,
    border,
    cyan,
    blue,
    orange,
    yellow,
    green,
    purple,
    red
};

class Rectangle {
    public:

    Rectangle(int width, int height);
    Rectangle(int width, int height, std::vector<Cell> contents);
    
    Cell getCell(int x, int y);
    
    private:

    void setCell(int x, int y, Cell newValue);

    const int width;
    const int height;

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
    const Rectangle* getTetromino();

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
    Rectangle* getPlayField();
    ActiveTetromino* getActiveTetromino();
    std::vector<const Tetromino*>* getQueue();
    const Tetromino* getSavedPiece();

    int getScore();
    int getLevel();

    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool moveToBottom();
    bool rotateClockwise();
    bool rotateAnticlockwise();

    bool saveActiveTetromino();
    
    std::vector<int>* beginPlaceAndCheckRowsToClear();
    bool placeAndClearRows();

    private:
    
    Rectangle playField;
    ActiveTetromino activeTetromino;
    std::vector<const Tetromino*> queue;
    const Tetromino* savedPiece;

    int score;
    int level;

    bool isAwaitingClearRows;
    std::vector<int> rowsToClear;
};
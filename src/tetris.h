#include <span>

#pragma once

enum class Cell {
    empty,
    lightBlue,
    blue,
    orange,
    yellow,
    green,
    red,
    purple
};

class Rectangle {
    public:
        int getWidth();
        int getHeight();
        Cell getCell(int x, int y);
};

class Square : public Rectangle {
};

class Tetromino {
    public:
        std::span<Square> getRotations();
    
    private:
        Tetromino(std::span<Square> rotations);
};

class ActiveTetromino {
    public:
        Tetromino& getTetromino();
        int getRotationIndex();
        int getXCoordinate();
        int getYCoordinate();
};

class Tetris {
    public:
        Tetris(int width, int height, int initialDifficulty = 0, int nextTetrominoListSize = 3);
        Tetris(): Tetris(10, 16, 0, 3) {}
        ~Tetris();

        int getScore();
        int getDifficulty();

        bool hasSavedPiece();
        Tetromino& getSavedPiece();
        ActiveTetromino getActiveTetromino();

        Rectangle getPlayField();

        // Defaults to incrementing by 1, but can increase or decrease level by arbitrary amount
        void updateDifficulty(int increment = 1);

        // Game moves. Returns true if successful
        bool moveLeft();
        bool moveRight();
        bool moveDown();
        bool moveToBottom();
        bool rotateClockwise();
        bool rotateAnticlockwise();
        bool place();
};

int main() {
    Tetromino thing(std::span<Square>());
}
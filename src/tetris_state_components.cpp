#include <vector>

#include "tetris_state.h"

// Moved definitions of everything not TetrisState to this file to reduce clutter

// Rectangle definition
Rectangle::Rectangle(int width, int height) : 
width(width),
height(height) {
    int totalNumberOfCells = width * height;

    contents.reserve(totalNumberOfCells);

    for(int i = 0;i < totalNumberOfCells;i++) {
        contents.push_back(Cell::empty);
    }
}

Rectangle::Rectangle(int width, int height, std::vector<Cell> contents) :
width(width),
height(height),
contents(contents) {}

Cell Rectangle::getCell(int x, int y) const {
    // TODO - Ensure doesn't go out of bounds
    return contents.at(y * width + x);
}

void Rectangle::setCell(int x, int y, Cell cell) {
    // TODO - Ensure doesn't go out of bounds
    contents.at(y * width + x) = cell;
}

// Coordinate definition

int Coordinate::getX() {
    return x;
}

int Coordinate::getY() {
    return y;
}

// Tetromino definition
Tetromino::Tetromino(const std::vector<Rectangle> rotations) : rotations(rotations) {}

Coordinate ActiveTetromino::getLocation() {
    return location;
}

Coordinate ActiveTetromino::getGhostLocation() {
    return ghostLocation;
}

const Rectangle* ActiveTetromino::getTetrominoRotation() {
    return &tetromino->rotations.at(rotation);
}
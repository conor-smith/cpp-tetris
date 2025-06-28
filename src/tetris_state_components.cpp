#include <vector>
#include <string>
#include <format>

#include "tetris_state.h"

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
    if(x >= width || x < 0 || y >= height || y < 0) {
        return Cell::rectBorder;
    } else {
        return contents.at(y * width + x);
    }
}

void Rectangle::setCell(int x, int y, Cell cell) {
    if(x >= width || x < 0 || y >= height || y < 0) {
        throw TetrisException(
            std::format("Coordinates {} and {} are out of bounds for rectangle of dimensions {} and {}",
            x, y, width, height));
    } else {
        contents.at(y * width + x) = cell;
    }
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

// ActiveTetromino definition
// Internal state intended to be managed entirely by TetrisState owner
Coordinate ActiveTetromino::getLocation() {
    return location;
}

Coordinate ActiveTetromino::getGhostLocation() {
    return ghostLocation;
}

const Rectangle* ActiveTetromino::getTetrominoRotation() {
    return &tetromino->rotations.at(rotation);
}

// TetrisException implementation

TetrisException::TetrisException(std::string message) : message(message) {}
TetrisException::~TetrisException() {}

const char* TetrisException::what() const noexcept {
    return message.data();
}
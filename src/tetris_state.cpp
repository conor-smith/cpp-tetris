#include <vector>

#include "tetris_state.h"

using namespace std;

// Constant tetrominos
const Tetromino I_PIECE = Tetromino({
    Rectangle(4, 4, {
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty,
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty,
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty,
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty
    }),
    Rectangle(4, 4, {
        Cell::empty, Cell::empty, Cell::empty, Cell::empty,
        Cell::empty, Cell::empty, Cell::empty, Cell::empty,
        Cell::cyan,  Cell::cyan,  Cell::cyan,  Cell::cyan,
        Cell::empty, Cell::empty, Cell::empty, Cell::empty,
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
        Cell::orange, Cell::empty,  Cell::empty
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
        Cell::empty, Cell::blue,  Cell::empty,
        Cell::empty, Cell::blue,  Cell::empty,
        Cell::blue,  Cell::blue,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::blue,  Cell::empty, Cell::empty,
        Cell::blue,  Cell::blue,  Cell::blue,
        Cell::empty, Cell::empty, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::blue,  Cell::blue,
        Cell::empty, Cell::blue,  Cell::empty,
        Cell::empty, Cell::blue,  Cell::empty
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
        Cell::empty, Cell::red,   Cell::empty,
        Cell::red,   Cell::red,   Cell::empty,
        Cell::red,   Cell::empty, Cell::empty
    })
});
const Tetromino T_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::empty,
        Cell::purple, Cell::purple, Cell::purple,
        Cell::empty,  Cell::purple, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::purple, Cell::purple, Cell::empty,
        Cell::empty,  Cell::purple, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::purple, Cell::purple, Cell::purple,
        Cell::empty,  Cell::empty,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::empty,  Cell::purple, Cell::purple,
        Cell::empty,  Cell::purple, Cell::empty
    })
});

const vector<const Tetromino*> TETROMINOS = {
    &I_PIECE,
    &L_PIECE,
    &J_PIECE,
    &O_PIECE,
    &S_PIECE,
    &Z_PIECE,
    &T_PIECE
};

// Rectangle implementation
Rectangle::Rectangle(int width, int height) : 
width(width),
height(height) {
    int totalNumberOfCells = width * height;

    contents.reserve(totalNumberOfCells);

    for(int i = 0;i < totalNumberOfCells;i++) {
        contents.push_back(Cell::empty);
    }
}

Rectangle::Rectangle(int width, int height, vector<Cell> contents) :
width(width),
height(height),
contents(contents) {}

Cell Rectangle::getCell(int x, int y) {
    // TODO - Ensure doesn't go out of bounds
    return contents.at(y * width + x);
}

void Rectangle::setCell(int x, int y, Cell cell) {
    // TODO - Ensure doesn't go out of bounds
    contents.at(y * width + x) = cell;
}

// Tetromino implementation
Tetromino::Tetromino(const vector<Rectangle> rotations) : rotations(rotations) {}

Coordinate ActiveTetromino::getLocation() {
    return location;
}

Coordinate ActiveTetromino::getGhostLocation() {
    return ghostLocation;
}

const Rectangle* ActiveTetromino::getTetrominoRotation() {
    return &tetromino->rotations.at(rotation);
}

// TetrisState implementation
TetrisState::TetrisState() :
playField(Rectangle(10, 20)),
savedPiece(NULL),
score(0),
level(1),
isAwaitingClearRows(false) {
    for(int row = 0;row < 8;row++) {
        for(int column = 0;column < 10;column++) {
            playField.setCell(column, row, static_cast<Cell>(row + 1));
        }
    }
}

Rectangle& TetrisState::getPlayField() {
    return playField;
}

ActiveTetromino& TetrisState::getActiveTetromino() {
    return activeTetromino;
}

vector<const Tetromino*>& TetrisState::getQueue() {
    return queue;
}

const Tetromino* TetrisState::getSavedPiece() {
    return savedPiece;
}

int TetrisState::getScore() {
    return score;
}

int TetrisState::getLevel() {
    return level;
}

bool TetrisState::moveLeft() {
    // TODO
    return true;
}

bool TetrisState::moveRight() {
    // TODO
    return true;
}

bool TetrisState::moveDown() {
    // TODO
    return true;
}

bool TetrisState::moveToBottom() {
    // TODO
    return true;
}

bool TetrisState::rotateClockwise() {
    // TODO
    return true;
}

bool TetrisState::rotateAnticlockwise() {
    // TODO
    return true;
}

bool TetrisState::saveActiveTetromino() {
    // TODO
    return true;
}

vector<int>& TetrisState::beginPlaceAndCheckRowsToClear() {
    // TODO
    return rowsToClear;
}

bool TetrisState::placeAndClearRows() {
    // TODO
    return false;
}
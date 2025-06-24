#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>

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

const array<const Tetromino*, 7> TETROMINOS = {
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

Cell Rectangle::getCell(int x, int y) const {
    // TODO - Ensure doesn't go out of bounds
    return contents.at(y * width + x);
}

void Rectangle::setCell(int x, int y, Cell cell) {
    // TODO - Ensure doesn't go out of bounds
    contents.at(y * width + x) = cell;
}

// Coordinate implementation

int Coordinate::getX() {
    return x;
}

int Coordinate::getY() {
    return y;
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
playField(Rectangle(10, 22)),
savedPiece(NULL),
score(0),
level(1),
isAwaitingClearRows(false) {
    for(int row = playField.height - 1;row > playField.height - 7;row--) {
        for(int column = 0;column < 10;column++) {
            playField.setCell(column, row, static_cast<Cell>(playField.height - row + 1));
        }
    }

    srand(time(0));

    queue.reserve(3);
    for(int i = 0;i < 3;i++) {
        queue.push_back(getRandomTetromino());
    }

    resetActiveTetromino(getRandomTetromino());
    calculateGhostTetromino();
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
    activeTetromino.location.x--;
    return true;
}

bool TetrisState::moveRight() {
    // TODO
    activeTetromino.location.x++;
    return true;
}

bool TetrisState::moveDown() {
    // TODO
    activeTetromino.location.y++;
    return true;
}

bool TetrisState::moveToBottom() {
    // TODO
    return true;
}

bool TetrisState::rotateClockwise() {
    // TODO
    activeTetromino.rotation = activeTetromino.rotation >= activeTetromino.tetromino->rotations.size() - 1 ?
        0 : activeTetromino.rotation + 1; 
    return true;
}

bool TetrisState::rotateAnticlockwise() {
    // TODO
    activeTetromino.rotation = activeTetromino.rotation <= 0 ?
        activeTetromino.tetromino->rotations.size() - 1 : activeTetromino.rotation - 1;
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

const Tetromino* TetrisState::getRandomTetromino() {
    return TETROMINOS[rand() % 7];
}

void TetrisState::resetActiveTetromino(const Tetromino* nextTetromino) {
    activeTetromino.tetromino = nextTetromino;
    activeTetromino.rotation = 0;
    
    // TODO: Determine if collision occurs
    int offset = nextTetromino->rotations[0].width == 2 ? 1 : 0;

    activeTetromino.location.x = 3 + offset;
    activeTetromino.location.y = 0 + offset * 2;
}

void TetrisState::calculateGhostTetromino() {
    activeTetromino.ghostLocation.x = 0;
    activeTetromino.ghostLocation.y = 0;
}
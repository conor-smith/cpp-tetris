#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>

#include "tetris_state.h"
#include "tetrominos.h"

using namespace std;

// Definitions of other declared classes in tetris_state.h were moved to tetris_state_components.cpp

// TetrisState definition
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
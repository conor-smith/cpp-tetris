#include <cstdlib>
#include <ctime>

#include "tetris.h"

Tetris::Tetris(int width, int height, int initialDifficulty=0, int tetrominoQueueSize):
    width(width),
    height(height),
    initialDifficulty(initialDifficulty),
    playField(Rectangle(width, height)),
    difficulty(initialDifficulty),
    savedTetromino(NULL),
    score(0) {
    
    queuedTetrominos.reserve(tetrominoQueueSize);
    
    srand(time(0));

    for(int i = 0;i < tetrominoQueueSize;i++) {
        queuedTetrominos[i] = TETROMINOS[rand() % TETROMINOS.size()];
    }
}

int Tetris::getScore() {
    return score;
}

int Tetris::getDifficulty() {
    return difficulty;
}

bool Tetris::hasSavedTetromino() {
    return savedTetromino != NULL;
}

Tetromino* Tetris::getSavedTetromino() {
    return savedTetromino;
}

ActiveTetromino* Tetris::getActiveTetromino() {
    return &activeTetromino;
}

std::vector<const Tetromino*>* Tetris::getQueuedTetrominos() {
    return &queuedTetrominos;
}

Rectangle* Tetris::getPlayField() {
    return &playField;
}

void Tetris::updateDifficulty(int increment = 1) {
    difficulty += increment;
}

bool Tetris::moveLeft() {
    return false;
}

bool Tetris::moveRight() {
    return false;
}

bool Tetris::moveDown(int amount = 1) {
    return false;
}

bool Tetris::moveToBottom() {
    return false;
}

bool Tetris::rotateClockwise() {
    return false;
}

bool Tetris::rotateAnticlockwise() {
    return false;
}

ClearedRows Tetris::place() {
    return;
}

void Tetris::clearRows() {
    return;
}

std::vector<Coordinate> testForCollisions() {
    return;
}
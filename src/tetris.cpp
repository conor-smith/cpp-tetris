#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>

#include "tetris.h"

/*
 * Rectangle implementation
 */
Rectangle::Rectangle(int width, int height, std::vector<Cell> cells) : 
    width(width), height(height), cells(cells) {}

Rectangle::Rectangle(int width, int height) : width(width), height(height) {
    int totalNoOfCells = width * height;
    
    cells.reserve(totalNoOfCells);

    for(int i = 0;i < totalNoOfCells;i++) {
        cells.at(i) = Cell::empty;
    }
}

Cell Rectangle::getCell(int x, int y) const {
    if(x >= width || x < 0 || y >= height || y < 0) {
        return Cell::border;
    } else {
        return cells.at(width * y + x);
    }
}

/*
 * Tetromino implementation
 */

Tetromino::Tetromino(std::vector<Rectangle> rotations) : rotations(rotations) {}

/*
 * ActiveTetromino implementation
 */

const Tetromino* ActiveTetromino::getTetromino() {
    return tetromino;
}

const Rectangle* ActiveTetromino::getCurrentRotation() {
    return &tetromino->rotations.at(currentRotationIndex);
}

Coordinate ActiveTetromino::getOffset() {
    return offset;
}

ActiveTetromino::ActiveTetromino() : tetromino(NULL), currentRotationIndex(0), offset(Coordinate(0, 0)) {}

// TODO: Document
void ActiveTetromino::rotateClockwise() {
    currentRotationIndex = currentRotationIndex >= tetromino->rotations.size() - 1 ?
        0 : currentRotationIndex + 1;
}

void ActiveTetromino::rotateAnticlockwise() {
    currentRotationIndex = currentRotationIndex <= 0 ?
        tetromino->rotations.size() - 1 : currentRotationIndex - 1;
}

/*
 * ClearedRows implementation
 */

ClearedRows::ClearedRows(std::vector<int> rows) : rows(rows) {}

/*
 * Coordinate implemtation
 */

Coordinate::Coordinate(int x, int y) : x(x), y(y) {}

/*
 * Tetris implementation
 */

Tetris::Tetris(int width, int height, int initialDifficulty=0, int tetrominoQueueSize) :
    initialDifficulty(initialDifficulty), playField(Rectangle(width, height)) {
        
    difficulty = initialDifficulty;
    score = 0;
    savedTetromino = NULL;
    savedTetrominoThisTurn = false;
    
    srand(time(0));
    queuedTetrominos.reserve(tetrominoQueueSize);    

    for(int i = 0;i < tetrominoQueueSize;i++) {
        queuedTetrominos.at(i) = getRandomTetromino();
    }

    nextTurn(getRandomTetromino());
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

const Tetromino* Tetris::getSavedTetromino() {
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

bool Tetris::saveTetromino() {
    if(!savedTetrominoThisTurn) {
        const Tetromino* nextTetromino = savedTetromino;
        savedTetromino = activeTetromino.tetromino;
        savedTetrominoThisTurn = true;
        nextTurn(nextTetromino);

        return true;
    } else {
        return false;
    }
}

bool Tetris::moveLeft() {
    return offsetActiveTetromino(-1, 0);
}

bool Tetris::moveRight() {
    return offsetActiveTetromino(1, 0);
}

bool Tetris::moveDown() {
    return offsetActiveTetromino(0, -1);
}

bool Tetris::moveToBottom() {
    const Rectangle* currentRotation = activeTetromino.getCurrentRotation();

    int movesUntilCollision = INT_MAX;
    Coordinate activeTetCoordinate = Coordinate(0, 0);

    // For each column in activeTetromino, check for collision
    // First, starting from bottom of each tetromino column, go up until we find a non empty cell
    for(;activeTetCoordinate.x < currentRotation->width && movesUntilCollision > 0;activeTetCoordinate.x++) {
        for(;activeTetCoordinate.y < currentRotation->height;activeTetCoordinate.y++) {
            
            if(activeTetromino.getCurrentRotation()->getCell(activeTetCoordinate.x, activeTetCoordinate.y) != Cell::empty) {
                
                // Go down playfield column until we encounter a non empty cell
                Coordinate pfCollisionCoordinate = Coordinate(
                    activeTetCoordinate.x + activeTetromino.offset.x,
                    activeTetCoordinate.y + activeTetromino.offset.y);
                
                for(;pfCollisionCoordinate.y >= 0;pfCollisionCoordinate.y--) {
                    if(playField.getCell(pfCollisionCoordinate.x, pfCollisionCoordinate.y - 1) != Cell::empty) {
                        break;
                    }
                }

                int columnMovesUntilCollision = activeTetCoordinate.y + activeTetromino.offset.y - pfCollisionCoordinate.y;
                if(columnMovesUntilCollision < movesUntilCollision) {
                    movesUntilCollision = columnMovesUntilCollision;
                }

                break;
            }
        }
    }

    if(movesUntilCollision > 0) {
        activeTetromino.offset.y -= movesUntilCollision;
        return true;
    } else {
        return false;
    }
}

bool Tetris::rotateClockwise() {
    activeTetromino.rotateClockwise();

    
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

bool Tetris::offsetActiveTetromino(int x, int y) {
    return false;
}

std::vector<Coordinate> testForCollisions() {
    return;
}

const Tetromino* Tetris::getRandomTetromino() {
    return NULL;
}

void Tetris::nextTurn(const Tetromino* nextTetromino) {
    return;
}

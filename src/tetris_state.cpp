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
isAwaitingClearRows(false),
gameOver(false) {

    // Set random seed
    srand(time(0));

    // Set up queue
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
    ActiveTetromino newAt = activeTetromino;
    newAt.location.x--;

    return updateActiveTetromino(newAt);
}

bool TetrisState::moveRight() {
    ActiveTetromino newAt = activeTetromino;
    newAt.location.x++;

    return updateActiveTetromino(newAt);
}

bool TetrisState::moveDown() {
    ActiveTetromino newAt = activeTetromino;
    newAt.location.y++;

    return updateActiveTetromino(newAt);
}

bool TetrisState::rotateClockwise() {
    ActiveTetromino newAt = activeTetromino;
    
    newAt.rotation = newAt.rotation >= newAt.tetromino->rotations.size() - 1 ?
        0 : newAt.rotation + 1;
    
    return updateRotatedActiveTetromino(newAt);
}

bool TetrisState::rotateAnticlockwise() {
    ActiveTetromino newAt = activeTetromino;

    newAt.rotation = newAt.rotation <= 0 ?
        newAt.tetromino->rotations.size() - 1 : newAt.rotation - 1;
        
    return updateRotatedActiveTetromino(newAt);
}

bool TetrisState::saveActiveTetromino() {
    // TODO
    return true;
}

vector<int>& TetrisState::placeActiveTetrominoAndGetRowsToClear() {
    // TODO
    return rowsToClear;
}

bool TetrisState::clearRowsAndContinue() {
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

bool TetrisState::updateActiveTetromino(ActiveTetromino newAt) {
    if(isAwaitingClearRows || gameOver) {
        throw TetrisException("Moves are not currently allowed");
    }

    // Checks for collisions
    // A collision only occurs if a non empty cell in the active tetromino overlaps with a non empty cell in the playfield
    bool noCollision = true;
    const Rectangle* rotationAt = newAt.getTetrominoRotation();
    Coordinate locationAt = newAt.getLocation();
    for(int xAt = 0;noCollision && xAt < rotationAt->width;xAt++) {
        for(int yAt = 0;noCollision && yAt < rotationAt->height;yAt++) {
            noCollision = 
                rotationAt->getCell(xAt, yAt) == Cell::empty ||
                playField.getCell(locationAt.x + xAt, locationAt.y + yAt) == Cell::empty;
        }
    }

    if(noCollision) {
        activeTetromino = newAt;
        calculateGhostTetromino();
    }

    return noCollision;
}

/*
 * When the active tetromino is rotated and encounters a collision, it may be offset to a nearby "safe" location
 * The maximum offset on both x and y values is equal to tetromino width - 1
 *      Eg. The I_PIECE has a width of 4. Therefore, its x and y values can be offset by a maximum of 3
 *      This can be applied to both x and y in one offset
 *      Meaning if an I_PIECE is rotated at {5, 5}, and the nearest safe location is {8, 2}, it will be moved there
 * The tetromino will preferentially be moved upwards its original column, followed by upwards (or at the same y value)
 * in the surrounding columns
 * If nothing is present, above the Tetromino, spaces below it will also be checked in the same order
 */
bool TetrisState::updateRotatedActiveTetromino(ActiveTetromino newAt) {
    bool noCollision = false;

    // Check current location
    noCollision = updateActiveTetromino(newAt);

    if(noCollision) {
        return noCollision;
    }

    // Check for spaces above
    Coordinate originalLocation = newAt.location;
    int maxOffset = newAt.getTetrominoRotation()->width - 1;

    for(int yOffset = 1;!noCollision && yOffset <= maxOffset;yOffset++) {
        newAt.location.y = originalLocation.y - yOffset;
        noCollision = updateActiveTetromino(newAt);
    }

    if(noCollision) {
        return noCollision;
    }

    // Alternate checking above in columns to the side
    for(int xOffset = 1;!noCollision && xOffset <= maxOffset;xOffset++) {
        for(int yOffset = 0;!noCollision && yOffset <= maxOffset;yOffset++) {
            newAt.location.y = originalLocation.y - yOffset;

            // Check to the left
            newAt.location.x = originalLocation.x - xOffset;
            noCollision = updateActiveTetromino(newAt);

            if(noCollision) {
                break;
            }

            // Check to the right
            newAt.location.x = originalLocation.x + xOffset;
            noCollision = updateActiveTetromino(newAt);
        }
    }

    if(noCollision) {
        return noCollision;
    }

    // Check for spaces below
    for(int yOffset = 1;!noCollision && yOffset <= maxOffset;yOffset++) {
        newAt.location.y = originalLocation.y + yOffset;
        noCollision = updateActiveTetromino(newAt);
    }

    if(noCollision) {
        return noCollision;
    }

    // Alternate checking below in columns to the side
    for(int xOffset = 1;!noCollision && xOffset <= maxOffset;xOffset++) {
        for(int yOffset = 1;!noCollision && yOffset <= maxOffset;yOffset++) {
            newAt.location.y = originalLocation.y + yOffset;

            // Check to the left
            newAt.location.x = originalLocation.x - xOffset;
            noCollision = updateActiveTetromino(newAt);

            if(noCollision) {
                break;
            }

            // Check to the right
            newAt.location.x = originalLocation.x + xOffset;
            noCollision = updateActiveTetromino(newAt);
        }
    }

    return noCollision;
}

// The ghost tetromino is displayed directly under the activeTetromino at the y value it would go to, should it be placed
void TetrisState::calculateGhostTetromino() {
    const Rectangle* currentRotation = activeTetromino.getTetrominoRotation();
    
    // Stores the maximum distance the activeTetromino can move upwards (in y values, downwards in game)
    // before colliding with non empty cell in playfield (border of playfield counts as non empty cell)
    int maxOffset = playField.height;

    for(int xAt = 0;xAt < currentRotation->width && maxOffset > 0;xAt++) {
        // Stores y value of highest (by y value, lowest in actual game) non-empty cell in activeTetromino column xAt
        int topCellAt = -1;

        // As higher y values are lower on the playfield, we start scanning the tetromino at top Y value
        for(int yAt = currentRotation->height - 1;yAt >= 0;yAt--) {
            if(currentRotation->getCell(xAt, yAt) != Cell::empty) {
                topCellAt = yAt;
                break;
            }
        }

        // If a non empty cell exists in this column, find the closest non empty cell above in the playfield
        if(topCellAt != -1) {
            // Stores playField X / Column value
            int xPf = activeTetromino.location.x + xAt;
            // Stores y value of topCellAt in playField coordinates
            int topCellAtInPf = activeTetromino.location.y + topCellAt;

            // Will keep running until it either finds a non empty cell, or the offset becomes greater than maxOffset
            for(int yPf = topCellAtInPf + 1;;yPf++) {
                // 1 is subtracted as if a non empty cell is directly above topCellAt, the activeTetromino cannot move
                int columnOffset = yPf - topCellAtInPf - 1;
                
                // If offset is larger than existing maxOffset, we can stop here
                if(columnOffset >= maxOffset) {
                    break;
                } else if(playField.getCell(xPf, yPf) != Cell::empty) {
                    maxOffset = columnOffset;
                    break;
                }
            }
        }
    }

    activeTetromino.ghostLocation.x = activeTetromino.location.x;
    activeTetromino.ghostLocation.y = activeTetromino.location.y + maxOffset;
}
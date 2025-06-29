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
gameOver(false),
savedTetrominoThisTurn(false) {

    // Set random seed
    srand(time(0));

    // Set up queue
    queue.reserve(3);
    for(int i = 0;i < 3;i++) {
        queue.push_back(getRandomTetromino());
    }

    resetActiveTetromino(getRandomTetromino());
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
    if(savedTetrominoThisTurn) {
        return false;
    }

    if(savedPiece == NULL) {
        savedPiece = activeTetromino.tetromino;
        resetActiveTetromino(popOffQueue());
    } else {
        const Tetromino* swap = savedPiece;
        savedPiece = activeTetromino.tetromino;
        resetActiveTetromino(swap);
    }

    savedTetrominoThisTurn = true;
    return true;
}

vector<int>& TetrisState::placeActiveTetrominoAndGetRowsToClear() {
    isAwaitingClearRows = true;

    const Rectangle* currentRotation = activeTetromino.getTetrominoRotation();

    // Iterate through rows of activeTetromino
    for(int atY = 0;atY < currentRotation->height;atY++) {
        int pfY = activeTetromino.placementLocation.y + atY;

        // For some tetrominos, the lowest row in the square is below the lowest filled in cell
        // If such a peice is placed on the bottom layer, the rotation square may reach below
        // When this happens, we can break out of this loop
        if(pfY >= playField.height) {
            break;
        }

        for(int atX = 0;atX < currentRotation->width;atX++) {
            int pfX = activeTetromino.placementLocation.x + atX;

            // Place activeTetromino cells into actual playfield
            Cell tetCell = currentRotation->getCell(atX, atY);
            if(tetCell != Cell::empty) {
                playField.setCell(pfX, pfY, tetCell);
            }
        }

        // After all the activeTetromino cells have been placed in this row
        // We can check if the all cells in this row are full
        // If so, we add this row to the rowsToClear vector
        bool rowIsFull = true;
        for(int pfX = 0;pfX < playField.width && rowIsFull;pfX++) {
            rowIsFull = playField.getCell(pfX, pfY) != Cell::empty;
        }

        if(rowIsFull) {
            rowsToClear.push_back(pfY);
        }
    }

    return rowsToClear;
}

// Not terribly efficient, but I'm a bit tired
// TODO: Document properly
bool TetrisState::clearRowsAndContinue() {
    isAwaitingClearRows = false;
    if(rowsToClear.size() > 0) {
        int offset = 1;

        int firstRowToClear = rowsToClear.back();
        rowsToClear.pop_back();
        
        int nextRowToClear;
        if(rowsToClear.empty()) {
            nextRowToClear = -10;
        } else {
            nextRowToClear = rowsToClear.back();
            rowsToClear.pop_back();
        }

        for(int pfY = firstRowToClear - 1;pfY + offset >= 0;pfY--) {
            // If current row is to be cleared, increment offset and move on to next row
            // Otherwise, move all cells from current row into row + offset
            if(pfY == nextRowToClear) {
                if(rowsToClear.empty()) {
                    nextRowToClear = -10;
                } else {
                    nextRowToClear = rowsToClear.back();
                    rowsToClear.pop_back();
                }

                offset++;
            } else {
                for(int pfX = 0;pfX < playField.width;pfX++) {
                    Cell currentCell = playField.getCell(pfX, pfY);

                    if(currentCell == Cell::outOfBounds) {
                        currentCell = Cell::empty;
                    }

                    playField.setCell(pfX, pfY + offset, currentCell);
                }
            }
        }
    }

    return resetActiveTetromino(popOffQueue());
}

const Tetromino* TetrisState::getRandomTetromino() {
    return TETROMINOS[rand() % 7];
}

// I would have used a queue rather than a vector, but the C++ queue object can't be arbitrarily accessed
// So I have to do this manually
const Tetromino* TetrisState::popOffQueue() {
    const Tetromino* toReturn = queue.front();

    for(int i = 0;i < queue.size() - 1;i++) {
        queue.at(i) = queue.at(i + 1);
    }

    queue.at(queue.size() - 1) = getRandomTetromino();

    return toReturn;
}

bool TetrisState::resetActiveTetromino(const Tetromino* nextTetromino) {
    ActiveTetromino newAt = ActiveTetromino();

    newAt.tetromino = nextTetromino;
    newAt.rotation = 0;

    const Rectangle* currentRotation = newAt.getTetrominoRotation();

    // Place new activeTetromino at the top of the visible field in the centre (y = 2)
    // If it cannot be placed there, attempt y = 1 and y = 0
    // If it cannot be placed there, game over
    newAt.location.x = currentRotation->width <= 3 ? 4 : 3;
    
    for(int y = 2;y >= 0;y++) {
        newAt.location.y = y;
        if(updateActiveTetromino(newAt)) {
            return true;
        }
    }

    gameOver = true;
    return false;
}

// If new activeTetromino object results in no collisions
// Replace current activeTetromino with new ActiveTetromino and return true
bool TetrisState::updateActiveTetromino(ActiveTetromino newAt) {
    if(isAwaitingClearRows || gameOver) {
        throw TetrisException("Moves are not currently allowed");
    }

    // Checks for collisions
    // A collision only occurs if a non empty cell in the active tetromino overlaps with a non empty cell in the playfield
    bool noCollision = true;
    const Rectangle* rotationAt = newAt.getTetrominoRotation();
    Coordinate locationAt = newAt.getLocation();
    for(int xAt = 0;xAt < rotationAt->width;xAt++) {
        for(int yAt = 0;noCollision && yAt < rotationAt->height;yAt++) {
            noCollision = 
                rotationAt->getCell(xAt, yAt) == Cell::empty ||
                playField.getCell(locationAt.x + xAt, locationAt.y + yAt) == Cell::empty;

            if(!noCollision) {
                break;
            }
        }
    }

    if(noCollision) {
        activeTetromino = newAt;
        calculatePlacementTetromino();
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
    // Check current location
    if(updateActiveTetromino(newAt)) {
        return true;
    }

    // Check for spaces above
    Coordinate originalLocation = newAt.location;
    int maxOffset = newAt.getTetrominoRotation()->width - 1;

    for(int yOffset = 1;yOffset <= maxOffset;yOffset++) {
        newAt.location.y = originalLocation.y - yOffset;
        if(updateActiveTetromino(newAt)) {
            return true;
        }
    }

    // Alternate checking above in columns to the side
    for(int xOffset = 1;xOffset <= maxOffset;xOffset++) {
        for(int yOffset = 0;yOffset <= maxOffset;yOffset++) {
            newAt.location.y = originalLocation.y - yOffset;

            // Check to the left
            newAt.location.x = originalLocation.x - xOffset;
            if(updateActiveTetromino(newAt)) {
                return true;
            }

            // Check to the right
            newAt.location.x = originalLocation.x + xOffset;
            if(updateActiveTetromino(newAt)) {
                return true;
            }
        }
    }

    // Check for spaces below
    for(int yOffset = 1;yOffset <= maxOffset;yOffset++) {
        newAt.location.y = originalLocation.y + yOffset;
        if(updateActiveTetromino(newAt)) {
            return true;
        }
    }

    // Alternate checking below in columns to the side
    for(int xOffset = 1;xOffset <= maxOffset;xOffset++) {
        for(int yOffset = 1;yOffset <= maxOffset;yOffset++) {
            newAt.location.y = originalLocation.y + yOffset;

            // Check to the left
            newAt.location.x = originalLocation.x - xOffset;
            if(updateActiveTetromino(newAt)) {
                return true;
            }

            // Check to the right
            newAt.location.x = originalLocation.x + xOffset;
            if(updateActiveTetromino(newAt)) {
                return true;
            }
        }
    }

    // Will only reach here if no alternate placement could be found
    return false;
}

// The placement tetromino is displayed directly under the activeTetromino at the y value it would go to, should it be placed
// It can be used by both the UI for display and the gameState for logic
void TetrisState::calculatePlacementTetromino() {
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

    activeTetromino.placementLocation.x = activeTetromino.location.x;
    activeTetromino.placementLocation.y = activeTetromino.location.y + maxOffset;
}
#include "tetris.h"

Tetris::Tetris(int width, int height, int initialDifficulty=0, int nextTetrominoSize):
    width(width),
    height(height),
    initialDifficulty(initialDifficulty),
    playField(Rectangle(width, height)),
    difficulty(initialDifficulty) {
}
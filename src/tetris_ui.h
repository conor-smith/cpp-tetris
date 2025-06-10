#include <vector>

#include "tetris_state.h"

#pragma once

enum class Input {
    leftKeyDown,
    leftKeyUp,
    rightKeyDown,
    rightKeyUp,
    downKeyDown,
    downKeyUp,
    bottomKeyDown,
    bottomKeyUp,
    clockwiseKeyDown,
    clockwiseKeyUp,
    anticlockwiseKeyDown,
    anticlockwiseKeyUp,
    placeKeyDown,
    placeKeyUp,
    saveKeyDown,
    saveKeyUp,
    pauseKeyDown,
    pauseKeyUp
};

class TetrisUi {
    public:
        TetrisUi(TetrisState* gameState);
        ~TetrisUi();
        void render();

    private:
        TetrisState* gameState;
};
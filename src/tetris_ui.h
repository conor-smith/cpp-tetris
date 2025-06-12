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
        virtual ~TetrisUi() = default;
        virtual void render() {}
        virtual bool isAnimating() {
            return false;
        }

    protected:
        TetrisUi(TetrisState* gameState) : gameState(gameState) {}
        TetrisState* gameState;
};

TetrisUi* createTetrisUi(TetrisState* gameState);
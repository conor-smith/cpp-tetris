#include <vector>

#include "tetris_state.h"

#pragma once

enum class Input {
    noInput,
    left,
    right,
    down,
    clockwise,
    anticlockwise,
    place,
    save,
    pause
};

class TetrisUi {
    public:

    virtual ~TetrisUi() = default;
    
    virtual void render() {}
    
    virtual void setToggleAcceptInputs(bool acceptInputs) {}
    virtual Input getInput() {
        return Input::noInput;
    }

    virtual void animateClearingRows(std::vector<int> rowsToClear) {}
    virtual bool isAnimating() {
        return false;
    }

    protected:
    
    TetrisUi(TetrisState& gameState) : gameState(gameState) {}
    TetrisState& gameState;
};

TetrisUi* createTetrisUi(TetrisState& gameState);
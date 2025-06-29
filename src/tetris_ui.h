#include <vector>
#include <exception>
#include <string>

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
    pause,
    quit
};

class TetrisUi {
    public:

    virtual ~TetrisUi() = default;
    
    virtual void render() {}
    
    virtual void setToggleAcceptInputs(bool acceptInputs) {}
    virtual Input getInput() {
        return Input::noInput;
    }

    virtual void pause() {}
    virtual void unpause() {}

    virtual void animateClearingRows(std::vector<int> rowsToClear) {}
    virtual bool isAnimating() {
        return false;
    }

    protected:
    
    TetrisUi(TetrisState& gameState) : gameState(gameState) {}
    TetrisState& gameState;
};

class UiException : public std::exception {
    public:

    UiException(std::string message) : message(message) {}
    virtual ~UiException() {}

    virtual const char* what() const noexcept {
        return message.data();
    }

    private:

    const std::string message;
};

TetrisUi* createTetrisUi(TetrisState& gameState);
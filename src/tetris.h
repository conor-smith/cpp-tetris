#include "tetris_state.h"
#include "tetris_ui.h"

#pragma once

class Tetris {
    public:
        Tetris();
        ~Tetris();
        
        int startGame();


    private:
        TetrisState gameState;
        TetrisUi* ui;
};
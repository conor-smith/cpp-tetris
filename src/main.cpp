#include <iostream>
#include <csignal>

#include "tetris.h"

// Placed up here to ensure destructor is called before outputting to std::cerr
int playTetris() {
    Tetris tetris;

    return tetris.startGame();
}

int main() {
    try {
        return playTetris();
    } catch(TetrisException e) {
        std::cerr << "Encountered exception: " << e.what() << std::endl;

        return 1;
    }
}
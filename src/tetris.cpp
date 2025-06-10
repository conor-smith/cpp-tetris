#include <iostream>
#include <thread>
#include <chrono>

#include "tetris.h"

using namespace std::chrono_literals;

Tetris::Tetris() : gameState(TetrisState()), ui(TetrisUi(&gameState)) {}

int Tetris::startGame() {
    ui.render();

    std::this_thread::sleep_for(10000ms);

    return 0;
}
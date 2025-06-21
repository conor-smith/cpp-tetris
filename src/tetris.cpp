#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "tetris.h"
#include "tetris_ui.h"
#include "tetris_state.h"

using namespace std;
using namespace chrono_literals;

// Frame rate of approx 60s
const auto TICK = 16.67ms;

Tetris::Tetris() : gameState(TetrisState()), ui(createTetrisUi(&gameState)) {}

Tetris::~Tetris() {
    delete ui;
}

int Tetris::startGame() {
    // Game loop
    while(true) {
        ui->render();

        this_thread::sleep_for(TICK);
    }
}

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

Tetris::Tetris() :
gameState(TetrisState()),
ui(createTetrisUi(gameState)) {}

Tetris::~Tetris() {
    delete ui;
}

int Tetris::startGame() {
    // Game loop
    bool gameIsRunning = true;

    while(gameIsRunning) {
        
        switch (ui->getInput()) {
            case Input::left:
                gameState.moveLeft();
                break;

            case Input::right:
                gameState.moveRight();
                break;

            case Input::down:
                gameState.moveDown();
                break;
            
            case Input::clockwise:
                gameState.rotateClockwise();
                break;

            case Input::anticlockwise:
                gameState.rotateAnticlockwise();
                break;

            case Input::place:
                //TODO
                break;

            case Input::save:
                gameState.saveActiveTetromino();
                break;

            case Input::pause:
                gameIsRunning = false;
                break;
        }

        ui->render();

        this_thread::sleep_for(TICK);
    }

    return 0;
}

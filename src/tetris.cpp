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
ui(createTetrisUi(gameState)),
paused(false) {}

Tetris::~Tetris() {
    delete ui;
}

int Tetris::startGame() {
    // Game loop
    bool gameIsRunning = true;

    while(gameIsRunning) {
        Input input = ui->getInput();
        
        if(!paused) {
            switch (input) {
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
                    gameState.placeActiveTetrominoAndGetRowsToClear();
                    gameState.clearRowsAndContinue();
                    break;

                case Input::save:
                    gameState.saveActiveTetromino();
                    break;

                case Input::pause:
                    paused = true;
                    ui->pause();
                    break;
                
                case Input::quit:
                    gameIsRunning = false;
                    break;
            }
        } else {
            switch(input) {
                case Input::pause:
                    paused = false;
                    ui->unpause();
                    break;
                case Input::quit:
                    gameIsRunning = false;
                    break;
            }
        }

        ui->render();

        this_thread::sleep_for(TICK);
    }

    return 0;
}

#include <ncurses.h>

#include "tetris_ui.h"
#include "tetris_state.h"

TetrisUi::TetrisUi(TetrisState* gameState) : gameState(gameState) {
    initscr();
}

TetrisUi::~TetrisUi() {
    endwin();
}

void TetrisUi::render() {
    printw("Hello world!");
    refresh();
}
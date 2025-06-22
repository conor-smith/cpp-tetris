#include <iostream>
#include <csignal>

#include "tetris.h"

Tetris* tetris = NULL;

void handler(int) {
    delete tetris;
}

int main() {
    signal(SIGINT, handler);

    tetris = new Tetris();

    return tetris->startGame();
}
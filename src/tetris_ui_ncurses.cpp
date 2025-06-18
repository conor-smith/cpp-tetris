#include <ncurses.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <map>
#include <array>

#include "tetris_ui.h"
#include "tetris_state.h"

const std::map<char, Input> inputMap = {
    {'a', Input::left},
    {'s', Input::down},
    {'d', Input::right},
    {'q', Input::anticlockwise},
    {'e', Input::clockwise},
    {' ', Input::place},
    {'w', Input::save},
    {27, Input::pause} // Escape key
};

/*
 * UI built to use ncurses and run in command line
 * As rows are larger than columns, every grid co-ordinate consists of 1 row and 2 columns
 * Will throw an exception if terminal isn't large enough to render entire game 
 */
class NCursesUi : public TetrisUi {
    public:

    NCursesUi(TetrisState* gameState);
    ~NCursesUi();
    
    friend void readFromTerminal(NCursesUi* ui);

    bool isAnimating() override;
    void render() override;

    private:

    WINDOW* playFieldW;
    WINDOW* savedPieceW;
    WINDOW* scoreW;
    WINDOW* queueW;

    bool animating = false;

    std::atomic_bool acceptInput;
    std::atomic<char> inputBuffer;

    std::thread inputThread;    
};

TetrisUi* createTetrisUi(TetrisState* gameState) {
    return new NCursesUi(gameState);
}

void readFromTerminal(NCursesUi* ui) {
    while(true) {
        auto input = getch();

        // if(acceptInput && inputMap.contains(input)) {
        //     inputBuffer = inputMap.at(input);
        // }
        ui->inputBuffer.store(input);
    }
}

NCursesUi::NCursesUi(TetrisState* gameState) : TetrisUi(gameState) {
    initscr();
    cbreak();
    noecho();

    // Create windows
    savedPieceW = newwin(7, 12, 1, 2);
    wborder(savedPieceW, '|', '|', '-', '-', '+', '+', '+', '+');

    scoreW = newwin(14, 12, 9, 2);
    wborder(scoreW, '|', '|', '-', '-', '+', '+', '+', '+');

    playFieldW = newwin(22, 22, 1, 16);
    wborder(playFieldW, '|', '|', '-', '-', '+', '+', '+', '+');

    queueW = newwin(17, 12, 1, 40);
    wborder(queueW, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(savedPieceW);
    wrefresh(scoreW);
    wrefresh(playFieldW);
    wrefresh(queueW);

    inputThread = std::thread(readFromTerminal, this);
}

NCursesUi::~NCursesUi() {
    // Unsure if necessary, but doesn't hurt
    delwin(savedPieceW);
    delwin(scoreW);
    delwin(playFieldW);
    delwin(queueW);

    endwin();
}

void NCursesUi::render() {
    mvwaddch(playFieldW, 1, 1, inputBuffer.load());
    wrefresh(playFieldW);
}

bool NCursesUi::isAnimating() {
    return false;
}
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

    Input getInput() override;

    bool isAnimating() override;
    void render() override;

    private:

    WINDOW* playFieldW;
    WINDOW* savedPieceW;
    WINDOW* scoreW;
    WINDOW* queueW;
    
    bool animating = false;
    
    std::atomic_bool acceptInput;
    std::atomic<Input> inputBuffer;
    
    std::thread* inputThread;
    std::atomic_bool endInputThread = false;

    void renderRectangle(WINDOW* window, int x, int y, Rectangle* rectangle);
};

TetrisUi* createTetrisUi(TetrisState* gameState) {
    return new NCursesUi(gameState);
}

void readFromTerminal(NCursesUi* ui) {
    while(!ui->endInputThread) {
        char input = halfdelay(10);

        if(ui->acceptInput && inputMap.contains(input)) {
            ui->inputBuffer.store(inputMap.at(input));
        }
    }
}

NCursesUi::NCursesUi(TetrisState* gameState) : TetrisUi(gameState) {
    initscr();

    start_color();
    init_color(8, 255, 165, 0); // Orange
    init_color(9, 148, 0, 211); // Purple

    init_pair(Cell::empty, COLOR_WHITE, COLOR_BLACK);
    init_pair(Cell::cyan, COLOR_CYAN, COLOR_BLACK);
    init_pair(Cell::blue, COLOR_BLUE, COLOR_BLACK);
    init_pair(Cell::orange, 8, COLOR_BLACK);
    init_pair(Cell::yellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Cell::green, COLOR_GREEN, COLOR_BLACK);
    init_pair(Cell::purple, 9, COLOR_BLACK);
    init_pair(Cell::red, COLOR_RED, COLOR_BLACK);

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

    inputThread = new std::thread(readFromTerminal, this);
}

NCursesUi::~NCursesUi() {
    // Unsure if necessary, but doesn't hurt
    delwin(savedPieceW);
    delwin(scoreW);
    delwin(playFieldW);
    delwin(queueW);

    endwin();

    endInputThread = true;
    inputThread->join();
    delete inputThread;
}

Input NCursesUi::getInput() {
    return inputBuffer.exchange(Input::noInput);
}

void NCursesUi::render() {
    wrefresh(playFieldW);
    wrefresh(savedPieceW);
    wrefresh(scoreW);
    wrefresh(queueW);
}

bool NCursesUi::isAnimating() {
    return false;
}

void NCursesUi::renderRectangle(WINDOW* window, int x, int y, Rectangle* rectangle) {
    wattron(window, COLOR_PAIR(Cell::empty));
    Cell currentAttr = Cell::empty;

    for(int ry = 0;ry < rectangle->height;ry++) {
        wmove(window, x, getmaxy(window) - 1 - ry);
        
        for(int rx = 0;rx < rectangle->width;rx++) {
            Cell currentCell = rectangle->getCell(rx, ry);

            if(currentCell != currentAttr) {
                wattron(window, COLOR_PAIR(currentCell));
            }

            
        }
    }
}
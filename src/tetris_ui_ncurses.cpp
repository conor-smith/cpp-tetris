#include <ncurses.h>
#include <iostream>

#include "tetris_ui.h"
#include "tetris_state.h"

/*
 * UI built to use ncurses and run in command line
 * As rows are larger than columns, every grid co-ordinate consists of 1 row and 2 columns
 * Will throw an exception if terminal isn't large enough to render entire game 
 */
class NCursesUi : public TetrisUi {
    private:
        WINDOW* playFieldW;
        WINDOW* savedPieceW;
        WINDOW* scoreW;
        WINDOW* queueW;

        bool animating;

    public:
        NCursesUi(TetrisState* gameState) : TetrisUi(gameState) {
            initscr();
        }

        void render() override {
            // Create windows
            savedPieceW = newwin(7, 12, 1, 2);
            wborder(savedPieceW, '|', '|', '-', '-', '+', '+', '+', '+');

            scoreW = newwin(14, 12, 9, 2);
            wborder(scoreW, '|', '|', '-', '-', '+', '+', '+', '+');
            // renderBox(3, 1, 14, 7);

            playFieldW = newwin(22, 22, 1, 16);
            wborder(playFieldW, '|', '|', '-', '-', '+', '+', '+', '+');

            queueW = newwin(17, 12, 1, 40);
            wborder(queueW, '|', '|', '-', '-', '+', '+', '+', '+');
            // renderBox(3, 9, 14, 22);
            // renderBox(17, 1, 38, 22);
            // renderBox(41, 1, 52, 17);
            refresh();
            wrefresh(savedPieceW);
            wrefresh(scoreW);
            wrefresh(playFieldW);
            wrefresh(queueW);
        }

        ~NCursesUi() {
            endwin();
        }

        bool isAnimating() override {
            return false;
        }
};

TetrisUi* createTetrisUi(TetrisState* gameState) {
    return new NCursesUi(gameState);
}
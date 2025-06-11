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
    public:
        NCursesUi(TetrisState* gameState) : TetrisUi(gameState) {
            initscr();
        }

        void render() override {
            renderBox(3, 1, 14, 7);
            renderBox(3, 9, 14, 22);
            renderBox(17, 1, 38, 22);
            renderBox(41, 1, 52, 17);
            refresh();
        }

        ~NCursesUi() {
            endwin();
        }
    
    private:
        void renderBox(int x1, int y1, int x2, int y2) {
            // Print top line
            mvaddch(y1, x1, '+');

            for(int i = 0;i < x2 - x1 - 1;i++) {
                addch('-');
            }
            addch('+');

            // Print bottom line
            mvaddch(y2, x1, '+');
            for(int i = 0;i < x2 - x1 - 1;i++) {
                addch('-');
            }
            addch('+');

            // Print left line
            for(int i = y1 + 1;i < y2;i++) {
                mvaddch(i, x1, '|');
            }

            // print right line
            for(int i = y1 + 1;i < y2;i++) {
                mvaddch(i, x2, '|');
            }
        }
};

TetrisUi* createTetrisUi(TetrisState* gameState) {
    return new NCursesUi(gameState);
}
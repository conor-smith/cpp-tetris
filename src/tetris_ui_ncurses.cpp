#include <ncurses.h>
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <string>

#include "tetris_ui.h"
#include "tetris_state.h"

const std::map<char, Input> inputMap = {
    {'a', Input::left},
    {'s', Input::down},
    {'d', Input::right},
    {'w', Input::anticlockwise},
    {'e', Input::clockwise},
    {' ', Input::place},
    {'f', Input::save},
    {'q', Input::pause}
};

/*
 * UI built to use ncurses and run in command line
 * As rows are larger than columns, every grid co-ordinate consists of 1 row and 2 columns
 * Will throw an exception if terminal isn't large enough to render entire game 
 */
class NCursesUi : public TetrisUi {
    public:

    NCursesUi(TetrisState& gameState);
    ~NCursesUi();

    Input getInput() override;

    bool isAnimating() override;
    void render() override;

    private:

    WINDOW* playFieldW;
    WINDOW* savedPieceW;
    WINDOW* scoreW;
    WINDOW* queueW;
    
    bool animating = false;
    bool acceptingInput = true;

    // Caching these values allows us to ensure we only render objects that are updated
    // The playfield, however, will always be rendered in full
    int cachedScore = -1;
    int cachedLevel = -1;
    const Tetromino* cachedSavedPiece = NULL;
    std::vector<const Tetromino*> cachedQueue;

    void refreshPlayFieldW();
    void refreshSavedPieceW();
    void refreshScoreW();
    void refreshQueueW();
    
    void renderActiveTetromino();

    void renderCenteredTetromino(WINDOW* window, int x, int y, const Rectangle* rectangle);
    void renderRectangle(WINDOW* window, int x, int y, const Rectangle* rectangle);
};

TetrisUi* createTetrisUi(TetrisState& gameState) {
    return new NCursesUi(gameState);
}

NCursesUi::NCursesUi(TetrisState& gameState) : TetrisUi(gameState) {
    cachedQueue.reserve(gameState.getQueue().size());

    for(int i = 0;i < gameState.getQueue().size();i++) {
        cachedQueue.push_back(NULL);
    }

    initscr();
    noecho();
    cbreak();

    start_color();
    init_color(8, 255, 127, 0); // Orange
    init_color(9, 255, 0, 255); // Purple

    init_pair(Cell::empty, COLOR_WHITE, COLOR_BLACK);
    init_pair(Cell::cyan, COLOR_CYAN, COLOR_BLACK);
    init_pair(Cell::blue, COLOR_BLUE, COLOR_BLACK);
    init_pair(Cell::orange, 8, COLOR_BLACK);
    init_pair(Cell::yellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Cell::green, COLOR_GREEN, COLOR_BLACK);
    init_pair(Cell::purple, 9, COLOR_BLACK);
    init_pair(Cell::red, COLOR_RED, COLOR_BLACK);

    /*
     * As terminal "cells" are twice as tall as they are wide, all x values are multiples of 
     * All windows have at least 1 row and 2 columns spacing
     * All windows have a 1 cell wide border on all 4 sides
     * For display in saved piece window and queue window, each tetromino needs at least 4 rows and 8 (2 * 4) columns
     * Spacing of 1 cell should also be provided on each side of tetrominoes
     * This means that saved piece window and queue windows must at least have the following dimensions
     *      Width: 12 columns (2 for borders, 2 for spacing, 8 for tetromino)
     *      Height: 8 (2 for borders, 2 for spacing, 4 for tetromino)
     * 
     * The display will be built around the actual playfield
     * Although the playfield has 22 rows, only 20 will be displayed, with the top 2 rows hidden
     * Playfield is also 10 blocks wide
     * This means dimensions of 22 width, 22 height (including borders)
     *
     * Also worth remembering for the following equations
     * Window startX + windowWidth actually equals window final column + 1
     */
    int savedPieceWStartX = 2;
    int savedPieceWStartY = 1;
    int savedPieceWWidth = 12;
    int savedPieceWHeight = 8;

    int playFieldWStartX = savedPieceWStartX + savedPieceWWidth + 2;
    int playFieldWStartY = savedPieceWStartY;
    int playFieldWWidth = 22;
    int playFieldWHeight = 22;

    int scoreWStartX = savedPieceWStartX;
    int scoreWStartY = savedPieceWStartY + savedPieceWHeight + 1;
    int scoreWWidth = savedPieceWWidth;
    int scoreWHeight = playFieldWStartY + playFieldWHeight - 1 - scoreWStartX + 1;

    int queueWStartX = playFieldWStartX + playFieldWWidth + 2;
    int queueWStartY = playFieldWStartY;
    int queueWidth = savedPieceWWidth;
    int queueHeight = playFieldWHeight;

    // Create windows
    savedPieceW = newwin(savedPieceWHeight, savedPieceWWidth, savedPieceWStartY, savedPieceWStartX);
    wborder(savedPieceW, '|', '|', '-', '-', '+', '+', '+', '+');

    scoreW = newwin(scoreWHeight, scoreWWidth, scoreWStartY, scoreWStartX);
    wborder(scoreW, '|', '|', '-', '-', '+', '+', '+', '+');

    playFieldW = newwin(playFieldWHeight, playFieldWWidth, playFieldWStartY, playFieldWStartX);
    wborder(playFieldW, '|', '|', '-', '-', '+', '+', '+', '+');

    queueW = newwin(17, 12, 1, 40);
    wborder(queueW, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(savedPieceW);
    wrefresh(scoreW);
    wrefresh(playFieldW);
    wrefresh(queueW);

    nodelay(playFieldW, true);
}

NCursesUi::~NCursesUi() {
    // Unsure if necessary, but doesn't hurt
    delwin(savedPieceW);
    delwin(scoreW);
    delwin(playFieldW);
    delwin(queueW);

    endwin();
}

Input NCursesUi::getInput() {
    move(0, 0);
    char input = wgetch(playFieldW);
    if(acceptingInput && inputMap.contains(input)) {
        return inputMap.at(input);
    } else {
        return Input::noInput;
    }
}

void NCursesUi::render() {
    refreshPlayFieldW();
    refreshSavedPieceW();
    refreshScoreW();
    refreshQueueW();
}

bool NCursesUi::isAnimating() {
    return false;
}

void NCursesUi::refreshPlayFieldW() {
    renderRectangle(playFieldW, 1, -1, &gameState.getPlayField());

    renderActiveTetromino();

    wrefresh(playFieldW);
}

void NCursesUi::refreshSavedPieceW() {
    if(gameState.getSavedPiece() != NULL && gameState.getSavedPiece() != cachedSavedPiece) {
        cachedSavedPiece = gameState.getSavedPiece();

        renderCenteredTetromino(savedPieceW, 2, 2, &gameState.getSavedPiece()->rotations[0]);

        wrefresh(savedPieceW);
    }
}

void NCursesUi::refreshScoreW() {
    if(cachedLevel != gameState.getLevel() || cachedScore != gameState.getScore()) {
        mvwaddstr(scoreW, 2, 1, "Level: ");
        waddstr(scoreW, std::to_string(gameState.getLevel()).data());
        
        mvwaddstr(scoreW, 4, 1, "Score: ");
        waddstr(scoreW, std::to_string(gameState.getScore()).data());

        wrefresh(scoreW);
    }
}

void NCursesUi::refreshQueueW() {
    bool queueUpdated = false;
    for(int i = 0;i < gameState.getQueue().size();i++) {
        if(gameState.getQueue().at(i) != cachedQueue.at(i)) {
            queueUpdated = true;

            cachedQueue.at(i) = gameState.getQueue().at(i);
        }
    }

    if(queueUpdated) {
        for(int i = 0;i < cachedQueue.size();i++) {
            renderCenteredTetromino(queueW, 2, 2 + 5 * i, &cachedQueue.at(i)->rotations[0]);
        }

        wrefresh(queueW);
    }
}

void NCursesUi::renderActiveTetromino() {
    Cell currentAttr = Cell::empty;
    Coordinate tetLocation = gameState.getActiveTetromino().getLocation();
    const Rectangle* tet = gameState.getActiveTetromino().getTetrominoRotation();

    for(int tetY = 0;tetY < tet->height;tetY++) {
        for(int tetX = 0;tetX < tet->width;tetX++) {
            if(tet->getCell(tetX, tetY) != Cell::empty) {
                
                if(currentAttr == Cell::empty) {
                    currentAttr = tet->getCell(tetX, tetY);
                    wattron(playFieldW, COLOR_PAIR(currentAttr));
                }

                int winY = -1 + tetLocation.getY() + tetY;
                int winX = (tetLocation.getX() + tetX) * 2 + 1;

                if(winY < 1) {
                    break;
                }

                mvwaddstr(playFieldW, winY, winX, "[]");
            }
        }
    }
}

void NCursesUi::renderCenteredTetromino(WINDOW* window, int x, int y, const Rectangle* rectangle) {
    // Used for saved and queued tetrominos
    // First draws an empty border of spaces around the centre to erase any existing shapes
    int offset = 0;

    switch (rectangle->width) {
    case 2:
        mvwaddstr(window, y + 3, x, "        ");
        mvwaddstr(window, y + 2, x + 6, "  ");
        mvwaddstr(window, y + 1, x + 6, "  ");
        offset = 2;
    
    case 3:
        mvwaddstr(window, y, x, "        ");
        mvwaddstr(window, y + 1, x, "  ");
        mvwaddstr(window, y + 2, x, "  ");

        offset = offset == 0 ? 1 : offset;
    
    case 4:
        renderRectangle(window, x + offset * 2, y + offset, rectangle);
    }
}

void NCursesUi::renderRectangle(WINDOW* window, int x, int y, const Rectangle* rectangle) {
    wattron(window, COLOR_PAIR(Cell::empty));
    Cell currentAttr = Cell::empty;

    for(int ry = 0;ry < rectangle->height;ry++) {

        // This lets us account for the two cut off rows in the playfield
        int ncursesY = y + ry;
        if(ncursesY <= 0) {
            continue;
        }

        wmove(window, ncursesY, x);
        
        for(int rx = 0;rx < rectangle->width;rx++) {
            Cell currentCell = rectangle->getCell(rx, ry);

            if(currentCell != currentAttr) {
                wattron(window, COLOR_PAIR(currentCell));
                currentAttr = currentCell;
            }

            if(currentCell == Cell::empty) {
                waddstr(window, "  ");
            } else {
                waddstr(window, "[]");
            }
        }
    }
}
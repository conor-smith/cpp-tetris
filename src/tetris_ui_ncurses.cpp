#include <ncurses.h>
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <string>

#include "tetris_ui.h"
#include "tetris_state.h"

#define BORDER(window) wborder(window, '|', '|', '-', '-', '+', '+', '+', '+')

const std::map<char, Input> inputMap = {
    {KEY_LEFT, Input::left},
    {KEY_DOWN, Input::down},
    {KEY_RIGHT, Input::right},
    {'a', Input::anticlockwise},
    {'d', Input::clockwise},
    {' ', Input::place},
    {'s', Input::save},
    {'w', Input::pause},
    {'q', Input::quit}
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

    void pause() override;
    void unpause() override;

    bool isAnimating() override;
    void render() override;

    private:

    WINDOW* playFieldW;
    WINDOW* savedPieceW;
    WINDOW* scoreW;
    WINDOW* levelW;
    WINDOW* queueW;
    WINDOW* popupW = NULL;
    
    bool animating = false;
    bool paused = false;

    // Caching these values allows us to ensure we only render objects that are updated
    // The playfield, however, will always be rendered in full
    int cachedScore = -1;
    int cachedLevel = -1;
    const Tetromino* cachedSavedPiece = NULL;
    std::vector<const Tetromino*> cachedQueue;

    void setUpWindows();
    void refreshPlayFieldW();
    void refreshSavedPieceW();
    void refreshScoreW();
    void refreshLevelW();
    void refreshQueueW();
    void displayPopup(std::string message, attr_t attribute);
    void killPopup();
    
    void renderCenteredTetromino(WINDOW* window, int x, int y, const Rectangle* rectangle);
    void renderRectangle(WINDOW* window, int x, int y, const Rectangle* rectangle);
    void renderClearRectangle(WINDOW* window, int x, int y, const Rectangle* rectangle, Cell color);
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
    curs_set(0);

    start_color();
    init_color(8, 255, 127, 0); // Orange
    init_color(9, 63, 0, 255); // Purple

    init_pair(Cell::empty, COLOR_WHITE, COLOR_BLACK);
    init_pair(Cell::cyan, COLOR_CYAN, COLOR_BLACK);
    init_pair(Cell::blue, COLOR_BLUE, COLOR_BLACK);
    init_pair(Cell::orange, 8, COLOR_BLACK);
    init_pair(Cell::yellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Cell::green, COLOR_GREEN, COLOR_BLACK);
    init_pair(Cell::purple, 9, COLOR_BLACK);
    init_pair(Cell::red, COLOR_RED, COLOR_BLACK);

    setUpWindows();

    nodelay(playFieldW, true);
    keypad(playFieldW, true);
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
    char input = wgetch(playFieldW);
    if(inputMap.contains(input)) {
        return inputMap.at(input);
    } else {
        return Input::noInput;
    }
}

void NCursesUi::pause() {
    if(!paused) {
        paused = true;
        displayPopup("PAUSED", COLOR_PAIR(Cell::green));
    }
}

void NCursesUi::unpause() {
    if(paused) {
        paused = false;
        killPopup();
    }
}

void NCursesUi::render() {
    if(popupW != NULL) {
        wrefresh(popupW);
    } else {
        refreshPlayFieldW();
        refreshSavedPieceW();
        refreshScoreW();
        refreshLevelW();
        refreshQueueW();
    }
}

bool NCursesUi::isAnimating() {
    return false;
}

void NCursesUi::setUpWindows() {
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
    std::string savedPieceWTitle = "SAVED";
    int savedPieceWTitleX = savedPieceWWidth / 2 - savedPieceWTitle.length() / 2;

    int playFieldWStartX = savedPieceWStartX + savedPieceWWidth + 2;
    int playFieldWStartY = savedPieceWStartY;
    int playFieldWWidth = 22;
    int playFieldWHeight = 22;
    std::string playFieldWTitle = "TETRIS";
    int playFieldWTitleX = playFieldWWidth / 2 - playFieldWTitle.length() / 2;

    int scoreWStartX = savedPieceWStartX;
    int scoreWStartY = savedPieceWStartY + savedPieceWHeight + 1;
    int scoreWWidth = savedPieceWWidth;
    int scoreWHeight = 6;
    std::string scoreWTitle = "SCORE";
    int scoreWTitleX = scoreWWidth / 2 - scoreWTitle.length() / 2;

    int levelWStartX = scoreWStartX;
    int levelWStartY = scoreWStartY + scoreWHeight + 1;
    int levelWWidth = scoreWWidth;
    int levelWHeight = 6;
    std::string levelWTitle = "LEVEL";
    int levelWTitleX = levelWWidth / 2 - levelWTitle.length() / 2;

    int queueWStartX = playFieldWStartX + playFieldWWidth + 2;
    int queueWStartY = playFieldWStartY;
    int queueWWidth = savedPieceWWidth;
    int queueWHeight = playFieldWHeight;
    std::string queueWTitle = "QUEUE";
    int queueWTitleX = queueWWidth / 2 - queueWTitle.length() / 2;

    // Create windows
    savedPieceW = newwin(savedPieceWHeight, savedPieceWWidth, savedPieceWStartY, savedPieceWStartX);
    BORDER(savedPieceW);
    mvwaddstr(savedPieceW, 0, savedPieceWTitleX, savedPieceWTitle.data());

    scoreW = newwin(scoreWHeight, scoreWWidth, scoreWStartY, scoreWStartX);
    BORDER(scoreW);
    mvwaddstr(scoreW, 0, scoreWTitleX, scoreWTitle.data());

    levelW = newwin(levelWHeight, levelWWidth, levelWStartY, levelWStartX);
    BORDER(levelW);
    mvwaddstr(levelW, 0, levelWTitleX, levelWTitle.data());

    playFieldW = newwin(playFieldWHeight, playFieldWWidth, playFieldWStartY, playFieldWStartX);
    BORDER(playFieldW);
    mvwaddstr(playFieldW, 0, playFieldWTitleX, playFieldWTitle.data());

    queueW = newwin(17, 12, 1, 40);
    BORDER(queueW);
    mvwaddstr(queueW, 0, queueWTitleX, queueWTitle.data());

    wrefresh(savedPieceW);
    wrefresh(scoreW);
    wrefresh(playFieldW);
    wrefresh(levelW);
    wrefresh(queueW);
}

void NCursesUi::refreshPlayFieldW() {
    renderRectangle(playFieldW, 1, -1, &gameState.getPlayField());

    ActiveTetromino& activeTet = gameState.getActiveTetromino();
    const Rectangle* currentRotation = activeTet.getTetrominoRotation();

    int activeTetPlacementY = activeTet.getPlacementLocation().getY() - 1;
    int activeTetPlacementX = activeTet.getPlacementLocation().getX() * 2 + 1;

    int activeTetY = activeTet.getLocation().getY() - 1;
    int activeTetX = activeTet.getLocation().getX() * 2 + 1;

    Cell tetColour = Cell::empty;
    for(int i = 0;i < currentRotation->width && tetColour == Cell::empty;i++) {
        for(int j = 0;j < currentRotation->height && tetColour == Cell::empty;j++) {
            if(currentRotation->getCell(i, j) != Cell::empty) {
                tetColour = currentRotation->getCell(i, j);
            }
        }
    }

    renderClearRectangle(playFieldW, activeTetPlacementX, activeTetPlacementY, currentRotation, Cell::empty);
    renderClearRectangle(playFieldW, activeTetX, activeTetY, currentRotation, tetColour);

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
    if(cachedScore != gameState.getScore()) {
        mvwaddstr(scoreW, 2, 2, std::to_string(gameState.getScore()).data());

        wrefresh(scoreW);

        cachedScore = gameState.getScore();
    }
}

void NCursesUi::refreshLevelW() {
    if(cachedLevel != gameState.getLevel()) {
        mvwaddstr(levelW, 2, 2, std::to_string(gameState.getLevel()).data());

        wrefresh(levelW);

        cachedLevel = gameState.getLevel();
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

void NCursesUi::displayPopup(std::string message, attr_t attribute) {
    int popupWWidth = message.length() + 6;
    int popupWHeight = 5;
    int popupWStartX = 27 - popupWWidth / 2;
    int popupWStartY = 11;

    popupW = newwin(popupWHeight, popupWWidth, popupWStartY, popupWStartX);

    wattron(popupW, attribute);
    wattron(popupW, A_BOLD);

    BORDER(popupW);
    mvwaddstr(popupW, 2, 3, message.data());

    wattroff(popupW, attribute);
    wattroff(popupW, A_BOLD);
}

void NCursesUi::killPopup() {
    delwin(popupW);
    popupW = NULL;

    wrefresh(savedPieceW);
    wrefresh(scoreW);
    wrefresh(levelW);
    wrefresh(playFieldW);
    wrefresh(queueW);
}

void NCursesUi::renderCenteredTetromino(WINDOW* window, int x, int y, const Rectangle* rectangle) {
    // Used for saved and queued tetrominos
    // First draws an empty border of spaces around the centre to erase any existing shapes
    int offset = 0;

    switch (rectangle->width) {
    case 2:
        mvwaddstr(window, y, x, "        ");
        mvwaddstr(window, y + 1, x, "  ");
        mvwaddstr(window, y + 2, x, "  ");
        offset = 1;
        
    case 3:
        mvwaddstr(window, y + 3, x, "        ");
        mvwaddstr(window, y + 2, x + 6, "  ");
        mvwaddstr(window, y + 1, x + 6, "  ");
    
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

void NCursesUi::renderClearRectangle(WINDOW* window, int x, int y, const Rectangle* rectangle, Cell color) {
    wattron(window, COLOR_PAIR(color));

    for(int ry = 0;ry < rectangle->height;ry++) {

        // This lets us account for the two cut off rows in the playfield
        int ncursesY = y + ry;
        if(ncursesY <= 0) {
            continue;
        }
        
        for(int rx = 0;rx < rectangle->width;rx++) {
            Cell currentCell = rectangle->getCell(rx, ry);
            
            int ncursesX = x + rx * 2;

            if(currentCell != Cell::empty) {
                mvwaddstr(window, ncursesY, ncursesX, "[]");
            }
        }
    }
}
#include <span>

#pragma once

namespace tetrisBackend {

    enum class PlayFieldSquare {
        empty,
        lightBlue,
        blue,
        orange,
        yellow,
        green,
        red,
        purple
    };

    class Rectangle {
        public:
            int getWidth();
            int getHeight();
            PlayFieldSquare getPlayFieldSquare(int x, int y);
    };

    class Square : public Rectangle {
    };

    class Tetromino {
        public:
            Square getShape();
            Square rotateClockwise();
            Square rotateAntiClockwise();
    };

    class ActiveTetromino {
        public:
            Tetromino tetromino();
            int getXCoordinate();
            int getYCoordinate();
    };

    class Tetris {
        public:
            Tetris(int width, int height, int initialDifficulty = 0, int nextTetrominoListSize = 3);
            Tetris(): Tetris(10, 16, 0, 3) {}
            ~Tetris();

            int getScore();
            int getDifficulty();

            bool hasSavedPiece();
            Tetromino getSavedPiece();

            Rectangle getPlayField();

            // Defaults to incrementing by 1, but can increase or decrease level by arbitrary amount
            void updateDifficulty(int increment = 1);
            
            
    };
}
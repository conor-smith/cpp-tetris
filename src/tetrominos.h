#include <array>

#include "tetris_state.h"

#pragma once

// Constant tetrominos
const Tetromino I_PIECE = Tetromino({
    Rectangle(4, 4, {
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty,
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty,
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty,
        Cell::empty, Cell::empty, Cell::cyan,  Cell::empty
    }),
    Rectangle(4, 4, {
        Cell::empty, Cell::empty, Cell::empty, Cell::empty,
        Cell::empty, Cell::empty, Cell::empty, Cell::empty,
        Cell::cyan,  Cell::cyan,  Cell::cyan,  Cell::cyan,
        Cell::empty, Cell::empty, Cell::empty, Cell::empty,
    })
});
const Tetromino L_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty,  Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::orange
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::empty,
        Cell::orange, Cell::orange, Cell::orange,
        Cell::orange, Cell::empty,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::orange, Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::empty,
        Cell::empty,  Cell::orange, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::orange,
        Cell::orange, Cell::orange, Cell::orange,
        Cell::empty,  Cell::empty,  Cell::empty
    })
});
const Tetromino J_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty, Cell::blue,  Cell::empty,
        Cell::empty, Cell::blue,  Cell::empty,
        Cell::blue,  Cell::blue,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::blue,  Cell::empty, Cell::empty,
        Cell::blue,  Cell::blue,  Cell::blue,
        Cell::empty, Cell::empty, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::blue,  Cell::blue,
        Cell::empty, Cell::blue,  Cell::empty,
        Cell::empty, Cell::blue,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::empty,
        Cell::blue,  Cell::blue,  Cell::blue,
        Cell::empty, Cell::empty, Cell::blue
    })
});
const Tetromino O_PIECE = Tetromino({
    Rectangle(2, 2, {
        Cell::yellow, Cell::yellow,
        Cell::yellow, Cell::yellow
    })
});
const Tetromino S_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::empty,
        Cell::empty, Cell::green, Cell::green,
        Cell::green, Cell::green, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::green, Cell::empty, Cell::empty,
        Cell::green, Cell::green, Cell::empty,
        Cell::empty, Cell::green, Cell::empty
    })
});
const Tetromino Z_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty, Cell::empty, Cell::empty,
        Cell::red,   Cell::red,   Cell::empty,
        Cell::empty, Cell::red,   Cell::red
    }),
    Rectangle(3, 3, {
        Cell::empty, Cell::red,   Cell::empty,
        Cell::red,   Cell::red,   Cell::empty,
        Cell::red,   Cell::empty, Cell::empty
    })
});
const Tetromino T_PIECE = Tetromino({
    Rectangle(3, 3, {
        Cell::empty,  Cell::empty,  Cell::empty,
        Cell::purple, Cell::purple, Cell::purple,
        Cell::empty,  Cell::purple, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::purple, Cell::purple, Cell::empty,
        Cell::empty,  Cell::purple, Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::purple, Cell::purple, Cell::purple,
        Cell::empty,  Cell::empty,  Cell::empty
    }),
    Rectangle(3, 3, {
        Cell::empty,  Cell::purple, Cell::empty,
        Cell::empty,  Cell::purple, Cell::purple,
        Cell::empty,  Cell::purple, Cell::empty
    })
});

const std::array<const Tetromino*, 7> TETROMINOS = {
    &I_PIECE,
    &L_PIECE,
    &J_PIECE,
    &O_PIECE,
    &S_PIECE,
    &Z_PIECE,
    &T_PIECE
};
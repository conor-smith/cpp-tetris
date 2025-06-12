#include <vector>

#pragma once

enum class Cell {
    emtpy,
    border,
    cyan,
    blue,
    orange,
    yellow,
    green,
    purple,
    red
};

class Rectangle {
    public:

    Cell getCell(int x, int y);

    friend class TetrisState;
    
    private:
    
    Rectangle(int width, int height);
    Rectangle(int width, int height, std::vector<Cell> contents);

    const int width;
    const int height;

    std::vector<Cell> contents;
};

class TetrisState {

};
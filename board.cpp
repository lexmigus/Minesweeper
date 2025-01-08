#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 9
#define MINES 10

struct Cell {
    bool containsMine = false;
    int surroundingMines = 0;
    bool isRevealed = false;
    bool isFlagged = false;
};

class Board {
private:
    int rows;
    int cols;
    int totalMines;
    std::vector<std::vector<Cell>> board;
    
    bool isValidLocation() {
        return false;
    };
    
    void placeMines() {
        
    };

    void incrementSurroundingMineCount() {

    };

public:
    void flagCell() {
        
    };
    void revealCell() {
        
    };
    void checkCell() {

    };
};

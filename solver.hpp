#include "board.hpp"

//solveCell runs the solving logic for a single cell
void solveCell(Board& board, std::pair<int, int> cell) {
    int x = cell.first;
    int y = cell.second;
    if (board.allNeighboursKnown(x, y) or board.isHidden(x, y)) {
        return;
    }
    else {
        int surroundingFlags = board.countSurroundingFlags(x, y);
        int surroundingHidden = board.countSurroundingHidden(x, y);
        int numberOfMines = board.checkSurroundingCellCounts(x, y);

        if (surroundingFlags == numberOfMines) {
            std::cout << "revealing" << std::endl;
            board.revealAllNeighbors(x, y);
        }
        else if (surroundingFlags + surroundingHidden == numberOfMines) {
            std::cout << "flagging" << std::endl;
            board.flagAllNeighbors(x, y);
        }
    }
}


//solve runs the high level solving logic
void solve(Board& board) {
    for (int i=0; i<5; i++){
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                solveCell(board, {i, j});
            }
        }
        board.printBoard();
    }
}
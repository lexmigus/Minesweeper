#include "board.hpp"

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
        if (surroundingFlags == board.checkSurroundingCellCounts(x, y)) {
            board.revealNeighbors(x, y);
        }
        else if (surroundingFlags + surroundingHidden == board.checkSurroundingCellCounts(x, y)) {
            board.flagNeighbors(x, y);
        }
    }
}

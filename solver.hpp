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
        if (surroundingFlags == board.checkSurroundingCellCounts(x, y)) {
            board.revealNeighbors(x, y);
        }
        else if (surroundingFlags + surroundingHidden == board.checkSurroundingCellCounts(x, y)) {
            board.flagNeighbors(x, y);
        }
    }
}

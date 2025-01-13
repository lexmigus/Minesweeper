#include "board.hpp"

void solve(auto &board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board.checkSurroundingCellCounts(i, j) == 0) {
                board.revealSurroundingCells(i, j);
            }
        }
    }
}

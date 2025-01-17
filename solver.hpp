#include "board.hpp"

std::set<std::pair<int,int>> visited;

//solveCell runs the solving logic for a single cell
int solveCell(Board& board, std::pair<int, int> cell) {
    int x = cell.first;
    int y = cell.second;

    // Skip if the cell is already solved
    if (board.cellSolved(x, y)) {
        return 0;
    }

    // If all neighbors are known, mark the current cell as solved
    if (board.allNeighboursKnown(x, y)) {
        board.setSolved(x, y);
        return 0;
    }

    // If the cell is hidden, do nothing
    if (board.isHidden(x, y)) {
        return 0;
    } else {
        // Count the flags and hidden cells around the current cell
        int surroundingFlags = board.countSurroundingFlags(x, y);
        int surroundingHidden = board.countSurroundingHidden(x, y);

        // If the number of flags matches the count of surrounding cells, reveal neighbors
        if (surroundingFlags == board.checkSurroundingCellCounts(x, y)) {
            board.revealAllNeighbors(x, y);
            return 1;
        }
        // If the number of flags + hidden cells matches the count, flag neighbors
        else if (surroundingFlags + surroundingHidden == board.checkSurroundingCellCounts(x, y)) {
            // Ensure that we only flag cells that are not already flagged
            board.flagAllNeighbors(x, y);
            return 1;
        }
    }

    return 0;
}



void recursiveSolve(Board& board, std::pair<int, int> cell){
    int x = cell.first;
    int y = cell.second;
    if (board.isHidden(x,y) or visited.find(cell) != visited.end() or board.cellSolved(x,y)){
        return;
    }
    if (solveCell(board, cell)){
        visited.clear();
        board.printBoard();
    } else {
        visited.insert(cell);
    }

    const std::pair<int, int> offsets[] = OFFSETS;
    for (const auto& offset: offsets) {
        int dx = offset.first;
        int dy = offset.second;
        if(board.isValidLocation(x + dx, y + dy)) {
            std::cout << "Recursive solve: " << x + dx << ", " << y + dy << std::endl;
            recursiveSolve(board, {x+dx, y+dy});
        }
    }

};


//solve runs the high level solving logic
bool solve(Board& board) {
    std::cout << "Solving board" << std::endl;
    board.printBoard();


    for (int i=0; i<2; i++){
        std::cout << "Iteration: " << i+1 << std::endl;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                std::cout << "Solving cell: " << i << ", " << j << std::endl;
                recursiveSolve(board, {i, j});
            }
        }
        board.printBoard();
        if (board.hasWon()){
            return true;
        }
    }
    return false;
}
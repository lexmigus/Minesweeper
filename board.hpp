#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#define MINES 10
#define BOARD_SIZE 9
#define OFFSETS { \
    {-1, -1}, {-1, 0}, {-1, 1}, \
    { 0, -1},          { 0, 1}, \
    { 1, -1}, { 1, 0}, { 1, 1}  \
}

int squaresStillHidden = BOARD_SIZE * BOARD_SIZE;

// Cell structure to represent each cell in the board
struct Cell {
    bool containsMine = false;
    int surroundingMines = 0;
    bool isRevealed = false;
    bool isFlagged = false;
};

class Board {

public:
    Board(int r, int c, int mines) : rows(r), cols(c), totalMines(mines) {
        board.resize(rows, std::vector<Cell>(cols));
    }

    // Places mines on the board
    void placeMines(int mines, int x, int y) {
        int mineCount = 0;
        srand(time(0));

        while (mineCount < mines){
            int randomX = rand()%cols;
            int randomY = rand()%rows;

            if(not placesMineInStartSquare(x, y, randomX, randomY)) {
                if (!board[randomX][randomY].containsMine) {

                    board[randomX][randomY].containsMine = true;
                    mineCount++;
                    incrementSurroundingMineCount(randomX, randomY);
                };
            };
        };
    };

    // Marks a cell as flagged
    void flagCell(int x, int y) {
        board[x][y].isFlagged = true;
    };

    // Reveals a cell and surrounding cells if the cell has a mine, returns true if the cell contains mines, false otherwise
    bool revealCell(int x, int y) {
        // Mark the cell as revealed
        board[x][y].isRevealed = true;
        squaresStillHidden--; // Decrement the counter only once

        // If the cell contains a mine, return true (game over)
        if (board[x][y].containsMine) {
            std::cout << "Mine hit" << std::endl;
            return true;
        }
        // If the cell has no surrounding mines, reveal neighbors
        if (board[x][y].surroundingMines == 0) {
            const std::pair<int, int> offsets[] = OFFSETS;

            for (const auto& offset : offsets) {
                int neighborX = x + offset.first;
                int neighborY = y + offset.second;

                // Check if the neighbor is valid and not already revealed
                if (isValidLocation(neighborX, neighborY) && !board[neighborX][neighborY].isRevealed) {
                    revealCell(neighborX, neighborY); // Recursive call
                }
            }
        }
        // If no mine was hit, return false
        return false;
    }

    // Returns the number of mines surrounding a cell
    int checkSurroundingCellCounts(int x, int y) {
        return board[x][y].surroundingMines;
    };

    // Prints the board to terminal
    void printBoard() {
        for(int j = 0; j < cols; j++) {
            std::string rowString = "";
            for(int i = 0; i < rows; i++) {
                if(board[i][j].isFlagged) {
                    rowString.append("X");
                } else if(not board[i][j].isRevealed) {
                    rowString.append("-");
                } else if(board[i][j].surroundingMines == 0) {
                    rowString.append(".");
                } else {
                    rowString.append(std::to_string(board[i][j].surroundingMines));
                };
            };
            std::cout << rowString << std::endl;
        };
        std::cout << std::endl;
    };

    // Testing
    // Prints position of all mines to terminal
    void printMines() {
        for(int j = 0; j < cols; j++) {
            std::string rowString = "";
            for(int i = 0; i < rows; i++) {
                if(board[i][j].containsMine) {
                    rowString.append("X");
                } else {
                    rowString.append("-");
                };
            };
            std::cout << rowString << std::endl;
        };
    };

    // Prints the number mines surround each cell to the terminal
    void printCounts() {
        for(int j = 0; j < cols; j++) {
            std::string rowString = "";
            for(int i = 0; i < rows; i++) {
                if(board[i][j].containsMine) {
                    rowString.append("X");
                } else if(board[i][j].surroundingMines == 0) {
                    rowString.append("-");
                } else {
                    rowString.append(std::to_string(board[i][j].surroundingMines));
                };
            };
            std::cout << rowString << std::endl;
        };
    };

    // Returns true if all neighbours of a cell are known, no further solving possible
    bool allNeighboursKnown (int x, int y) {
        const std::pair<int, int> offsets[] = OFFSETS;
        for (const auto& offset: offsets) {
            int dx = offset.first;
            int dy = offset.second;
            if(isValidLocation(x + dx, y + dy)) {
                if (board[x+dx][y+dy].isRevealed == false and board[x+dx][y+dy].isFlagged == false) {
                    return false;
                };
            };
        };
        return true;
    };

    // Returns true if all neighbours of a cell are unknown, no further solving possible
    bool allNeighboursUnknown (int x, int y) {
        const std::pair<int, int> offsets[] = OFFSETS;
        for (const auto& offset: offsets) {
            int dx = offset.first;
            int dy = offset.second;
            if(isValidLocation(x + dx, y + dy)) {
                if (board[x+dx][y+dy].isRevealed == true or board[x+dx][y+dy].isFlagged == true) {
                    return false;
                };
            };
        };
        return true;
    };

private:
    int rows;
    int cols;
    int totalMines;
    std::vector<std::vector<Cell>> board;

    // Determines if cell is in the bounds of the board
    bool isValidLocation(int x, int y) {
        return (0 <= x and x < rows) and (0 <= y and y < cols);
    }


    // Makes sure mines not placed in start square. Returns true if position in start square, false otherwise
    bool placesMineInStartSquare(int x, int y, int x_placed, int y_placed) {
        return (x - 1 <= x_placed and x_placed <= x + 1) and
            (y - 1 <= y_placed and y_placed <= y + 1);
    }

    // Increments the surrounding mine count for a cell
    void incrementSurroundingMineCount(int x, int y) {
        const std::pair<int, int> offsets[] = OFFSETS;

        for (const auto& offset: offsets) {
            int dx = offset.first;
            int dy = offset.second;
            if(isValidLocation(x + dx, y + dy)) {
                        board[x+dx][y+dy].surroundingMines++;
                    };
        };
    };
};
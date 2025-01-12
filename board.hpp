#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#define MINES 1
#define BOARD_SIZE 9

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
                if (!hasMine(randomX, randomY)) {

                    board[randomX][randomY].containsMine = true;
                    mineCount++;
                    incrementSurroundingMineCount(randomX, randomY);
                };
            };
        };
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


    // Determines if cell contains a mine
    bool hasMine(int x, int y) {
        return board[x][y].containsMine;
    };

    // Increments the surrounding mine count for a cell
    void incrementSurroundingMineCount(int x, int y) {
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if(i == 0 and j == 0) {
                    continue;
                };
                if(isValidLocation(x + i, y + j)) {
                    board[x+i][y+j].surroundingMines++;
                };
            };
        };
    };

public:
    // Marks a cell as flagged
    void flagCell(int x, int y) {
        board[x][y].isFlagged = true;
    };

    // Reveals a cell and surrounding cells if the cell has a mine, returns true if the cell contains mines, false otherwise
    bool revealCell(int x, int y) {
    // Log the cell being revealed
    std::cout << "Revealing cell: " << x << " " << y << std::endl;

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
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                // Skip the current cell
                if (i == 0 && j == 0) {
                    continue;
                }

                int neighborX = x + i;
                int neighborY = y + j;

                // Check if the neighbor is valid and not already revealed
                if (isValidLocation(neighborX, neighborY) && !board[neighborX][neighborY].isRevealed) {
                    revealCell(neighborX, neighborY); // Recursive call
                }
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
};
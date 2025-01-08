#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

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

    bool isValidLocation(int x, int y) {
        return (0 <= x < rows) && (0 <= y < cols);
    };

    void placeMines(int mines) {
        int mineCount = 0;
        srand(time(0));

        while (mineCount < mines){
            int randomX = rand()%cols;
            int randomY = rand()%rows;

            if (!hasMine(randomX, randomY)){
                board[randomX][randomY].containsMine = true;
                mineCount++;
                incrementSurroundingMineCount(randomX, randomY);
            };
        };
    };

    bool hasMine(int x, int y){
        return board[x][y].containsMine;
    };

    void incrementSurroundingMineCount(int x, int y) {
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if(i != 0 and j != 0) {
                    if(isValidLocation(x + i, y + j)) {
                        board[x + i][y + j].surroundingMines++;
                    };
                };
            };
        };
    };

public:
    void flagCell(int x, int y) {
        board[x][y].isFlagged = true;
    };

    void revealCell(int x, int y) {
        board[x][y].isRevealed = true;
        if (board[x][y].surroundingMines == 0){
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if(i != 0 and j != 0) {
                        if(isValidLocation(x + i, y + j)) {
                            revealCell(x+i,y+j);
                        };
                    };
                };
            };
        }
    };

    int checkCell(int x, int y) {
        return board[x][y].surroundingMines;
    };

    void printBoard() {
        for(int i = 0; i < rows; i++) {
            std::string columnString = "";
            for(int j = 0; j < cols; j++) {
                if(board[i][j].isFlagged) {
                    columnString.append("X");
                } else if(board[i][j].isRevealed) {
                    columnString.append(" ");
                } else if(board[i][j].surroundingMines == 0) {
                    columnString.append("-");
                } else {
                    columnString.append(std::to_string(board[i][j].surroundingMines));
                };
            std::cout << columnString;
            };
        };
    };
};

main() {
    Board board;
    board.printBoard();
}
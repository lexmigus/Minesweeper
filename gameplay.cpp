#include "board.hpp"

bool playing = true;
bool firstMove = true;

// Request user input for x and y position to reveal
std::pair<int, int> requestCell() {
    int x, y;
    std::cout << "Enter x: ";
    std::cin >> x;
    std::cout << "Enter y: ";
    std::cin >> y;
    return {x - 1, y - 1};
}

main() {
    Board board(BOARD_SIZE, BOARD_SIZE, MINES);
    std::cout << std::endl;
    //board.printMines();
    while(playing) {
        int x, y;
        auto cell = requestCell();
        x = cell.first;
        y = cell.second;
        std::cout << "x: " << x << " y: " << y << std::endl;
        //Place mines after first move
        if (firstMove) {
            std::cout << "dropping mines"<< std::endl;
            board.placeMines(MINES, x, y);
            std::cout << "mines placed"<< std::endl;
            firstMove = false;
            board.printBoard();
            std::cout << "board generated"<< std::endl;
        }
        //Flip, if explodes, end game
        if (board.revealCell(x,y)){
            std::cout << "You lose!" << std::endl;
            firstMove = true;
            playing = false;
            break;
        };
        std::cout << "cell revealed"<< std::endl;
        if(squaresStillHidden == MINES) {
            std::cout << "You win!" << std::endl;
            playing = false;
        };
        std::cout << "Squares still hidden: " << squaresStillHidden << std::endl;
        board.printBoard();
        board.printCounts();
        std::cout << std::endl;
        board.printMines();
    }
}



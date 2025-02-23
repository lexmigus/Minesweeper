#include "solver.hpp"

bool playing = true;
bool firstMove = true;
// Request user input for x and y position to reveal
pair<int, int> requestCell() {
    int x, y;
    cout << "Enter x: ";
    cin >> x;
    cout << "Enter y: ";
    cin >> y;
    return {x - 1, y - 1};
}

int main() {
    int width, height, mines;
    cout << "Enter width: ";
    cin >> width;
    cout << "Enter height: ";
    cin >> height;
    cout << "Enter number of mines: ";
    cin >> mines;
    Board board(width, height, mines);
    cout << endl;
    //board.printMines();
    while(playing) {
        int x, y;
        auto cell = requestCell();
        x = cell.first;
        y = cell.second;
        //Place mines after first move
        if (firstMove) {
            cout << "dropping mines"<< endl;
            board.placeMines(mines, x, y);
            cout << "mines placed"<< endl;
            firstMove = false;
            board.printBoard();
            cout << endl;
            board.printCounts();
            cout << endl;
            board.printMines();
            cout << endl;
            cout << "board generated"<< endl;
        };
        board.revealCell(x,y);
        if (solve(board)){
            cout << "You win!" << endl;
            playing = false;
        }
        //Flip, if explodes, end game
        // if (board.revealCell(x,y)){
        //     cout << "You lose!" << endl;
        //     firstMove = true;
        //     playing = false;
        //     break;
        // };
        // cout << "cell revealed"<< endl;
        // if(squaresStillHidden == mines) {
        //     cout << "You win!" << endl;
        //     playing = false;
        // };
        // cout << "Squares still hidden: " << squaresStillHidden << endl;
        // board.printBoard();
        // board.printCounts();
        // cout << endl;
        // board.printMines();
    }
    return 0;
}

// To compile and run
//   g++ -std=c++11 gameplay.cpp -o test.exe
//   ./test.exe

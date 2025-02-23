#include "board.cpp"
#include <algorithm>

vector<pair<int,int>> visited;

struct square {
    int x;
    int y;
};

//solveCell runs the solving logic for a single cell
int solveCell(Board& board, pair<int, int> cell) {
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
};



void recursiveSolve(Board& board, pair<int, int> cell){
    int x = cell.first;
    int y = cell.second;
    auto it = find(visited.begin(), visited.end(), cell);
    
    if (board.isHidden(x, y) || it != visited.end() || board.cellSolved(x, y)) {
        return;
    }
    if (solveCell(board, cell)) {
        visited.clear();
        board.printBoard();
    } else {
        visited.push_back(cell);
    }
    
    const pair<int, int> offsets[] = OFFSETS;
    for (const auto& offset: offsets) {
        int dx = offset.first;
        int dy = offset.second;
        if(board.isValidLocation(x + dx, y + dy)) {
            cout << "Recursive solve: " << x + dx << ", " << y + dy << endl;
            recursiveSolve(board, {x+dx, y+dy});
        }
    }

};

vector<pair<int,int>> intersection(vector<pair<int,int>> v1, vector<pair<int,int>> v2){
    vector<pair<int,int>> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

vector<pair<int,int>> difference(vector<pair<int,int>> v1, vector<pair<int,int>> v2){
    vector<pair<int,int>> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_difference(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

vector<pair<int,int>> getCellGroup(Board& board, pair<int,int> cell){
    vector<pair<int,int>> cellGroup;
    int x = cell.first;
    int y = cell.second;
    const pair<int, int> offsets[] = OFFSETS;
    for (const auto& offset: offsets) {
        int dx = offset.first;
        int dy = offset.second;
        if(board.isValidLocation(x + dx, y + dy)) {
            if (board.isHidden(x+dx, y+dy) and !board.isFlagged(x+dx, y+dy)){
            cellGroup.push_back({x+dx, y+dy});
            }
        }
    }
    return cellGroup;
}

vector<pair<int, int>> intersection(vector<pair<int, int>> v1, vector<pair<int, int>> v2);
vector<pair<int, int>> difference(vector<pair<int, int>> v1, vector<pair<int, int>> v2);

void setSolve(Board& board, pair<int,int> cellA, pair<int,int> cellB){
    vector<pair<int,int>> alpha = getCellGroup(board, cellA);
    vector<pair<int,int>> beta = getCellGroup(board, cellB);

    vector<pair<int,int>> common = intersection(alpha, beta);
    alpha = difference(alpha, common);
    beta = difference(beta, common);

    int alphaMines = board.checkSurroundingCellCounts(cellA.first, cellA.second) - board.countSurroundingFlags(cellA.first, cellA.second);
    int betaMines = board.checkSurroundingCellCounts(cellB.first, cellB.second) - board.countSurroundingFlags(cellB.first, cellB.second);

};

//solve runs the high level solving logic
bool solve(Board& board) {
    cout << "Solving board" << endl;
    board.printBoard();


    for (int i=0; i<2; i++){
        cout << "Iteration: " << i+1 << endl;
        for (int i = 0; i < board.numCols(); i++) {
            for (int j = 0; j < board.numRows(); j++) {
                recursiveSolve(board, make_pair(i, j));
            }
        }
        board.printBoard();
        if (board.hasWon()){
            return true;
        }
    }
    return false;
};
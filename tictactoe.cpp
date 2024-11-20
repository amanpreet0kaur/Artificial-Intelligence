#include <iostream>
#include <vector>
using namespace std;

vector<int> board(10, 0); // Initialize board with 10 elements (index 0 unused)
int turn = 1; // Track the turn number

void displayBoard() {
    for (int i = 1; i < 10; i++) {
        if (board[i] == 0) {
            cout << "0 "; 
        } else if (board[i] == 3) {
            cout << "X ";
        } else {
            cout << "O ";
        }

        if (i % 3 == 0) 
            cout << endl; 
    }
    cout << endl;
}

int Make2() {
    if (board[5] == 0) 
        return 5;
    for (int i : {2, 4, 6, 8}) { 
        if (board[i] == 0)
            return i;
    }
    return -1; 
}

void Go(int n) {
    board[n] = (turn % 2 != 0) ? 3 : 5; // Assign X or O
    turn++; 
}

bool checkWin(int player) {
    vector<vector<int>> winningCombos = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9},
        {1, 4, 7}, {2, 5, 8}, {3, 6, 9},
        {1, 5, 9}, {3, 5, 7}
    };

    for (auto combo : winningCombos) {
        if (board[combo[0]] == player && board[combo[1]] == player && board[combo[2]] == player) {
            return true;
        }
    }
    return false;
}

int Posswin(int p) {
    vector<vector<int>> winningCombos = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9},
        {1, 4, 7}, {2, 5, 8}, {3, 6, 9},
        {1, 5, 9}, {3, 5, 7}
    };

    for (auto combo : winningCombos) {
        if (board[combo[0]] == p && board[combo[1]] == p && board[combo[2]] == 0) {
            return combo[2];
        }
        if (board[combo[0]] == p && board[combo[1]] == 0 && board[combo[2]] == p) {
            return combo[1]; 
        }
        if (board[combo[0]] == 0 && board[combo[1]] == p && board[combo[2]] == p) {
            return combo[0]; 
        }
    }
    return 0;
}

void userMove() {
    int move;
    cout << "Your turn (enter a number between 1-9): ";
    cin >> move;

    while (move < 1 || move > 9 || board[move] != 0) {
        cout << "Invalid move! Try again: ";
        cin >> move;
    }

    Go(move); 
}

void aiMove() {
    if (turn == 2) {
        if (board[5] == 0) {
            Go(5); // Center
        } else {
            Go(1); // Top-left
        }
    } else {
        int winMoveX = Posswin(3); 
        if (winMoveX != 0) {
            Go(winMoveX);
        } else {
            int blockMove = Posswin(5); 
            if (blockMove != 0) {
                Go(blockMove);
            } else {
                Go(Make2());
            }
        }
    }
}

void game() {
    while (turn <= 9) {
        displayBoard(); 

        if (turn % 2 != 0) {
            userMove();
        } else { 
            aiMove();
        }

        if (checkWin(3)) {
            displayBoard();
            cout << "X wins!" << endl;
            return;
        } else if (checkWin(5)) {
            displayBoard();
            cout << "O wins!" << endl; 
            return;
        }
    }

    displayBoard();
    cout << "It's a draw!" << endl; 
}

int main() {
    game(); 
    return 0;
}

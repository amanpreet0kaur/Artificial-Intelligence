#include <iostream>
#include <vector>
#include <limits>
using namespace std;

vector<int> board(9, 2);
int User = 3;
int Computer = 5;
int turn = 1;
bool userFirst = true;
int maxDepth = 8;
int count = 0;

void display() {
    for (int i = 0; i < 9; i++) {
        if (i == 3 || i == 6) cout << endl;
        cout << (board[i] == User ? "X " : (board[i] == Computer ? "O " : "- "));
    }
    cout << endl;
}

int checkWin(const vector<int>& board, int player) {
    for (int i = 0; i < 3; i++) {
        if ((board[i * 3] == player && board[i * 3 + 1] == player && board[i * 3 + 2] == player) ||
            (board[i] == player && board[i + 3] == player && board[i + 6] == player)) {
            return 10;
        }
    }
    if ((board[0] == player && board[4] == player && board[8] == player) ||
        (board[2] == player && board[4] == player && board[6] == player)) {
        return 10;
    }
    return 0;
}

bool isDraw() {
    for (int i = 0; i < 9; i++) {
        if (board[i] == 2) return false;
    }
    return true;
}

int rate(const vector<int>& board, int player, int turn) {
    int score = 0;
    if (turn <= 3) {
        if (board[4] == player) score += 10;
    } else {
        if (board[4] == player) score += 4;
    }
    int corners[] = {0, 2, 6, 8};
    for (int i : corners) {
        if (board[i] == player) score += 3;
    }
    int edges[] = {1, 3, 5, 7};
    for (int i : edges) {
        if (board[i] == player) score += 1;
    }
    return score;
}

vector<int> moves_left(const vector<int>& board) {
    vector<int> moves;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 2) {
            moves.push_back(i);
        }
    }
    return moves;
}

int minimax(vector<int>& board, int depth, int maxDepth) {
    count++;
    int userWin = checkWin(board, User);
    int computerWin = checkWin(board, Computer);
    if (userWin == 10) return -10;
    if (computerWin == 10) return 10;
    if (isDraw()) return 0;
    if (depth >= maxDepth) return rate(board, Computer, turn);

    int bestScore = -1000;
    vector<int> moves = moves_left(board);
    for (int move : moves) {
        board[move] = (depth % 2 == 0) ? Computer : User;
        int currentScore = -minimax(board, depth + 1, maxDepth);
        board[move] = 2;
        bestScore = max(bestScore, currentScore);
    }
    return bestScore;
}

void computerTurn() {
    for (int move : moves_left(board)) {
        board[move] = User;
        if (checkWin(board, User) == 10) {
            board[move] = Computer;
            display();
            return;
        }
        board[move] = 2;
    }

    for (int move : moves_left(board)) {
        board[move] = Computer;
        if (checkWin(board, Computer) == 10) {
            display();
            return;
        }
        board[move] = 2;
    }

    int bestScore = INT_MIN;
    int toMove = -1;
    for (int move : moves_left(board)) {
        board[move] = Computer;
        int score = minimax(board, 0, maxDepth) + rate(board, Computer, turn);
        board[move] = 2;
        if (score > bestScore) {
            bestScore = score;
            toMove = move;
        }
    }
    if (toMove != -1) {
        board[toMove] = Computer;
    }
    display();
}

void userTurn() {
    int n;
    while (true) {
        cout << "Enter the block number for your move (1-9): ";
        cin >> n;
        if (n >= 1 && n <= 9 && board[n - 1] == 2) {
            board[n - 1] = User;
            break;
        } else {
            cout << "Invalid move! Please try again." << endl;
        }
    }
}

int main() {
    cout << "Enter 1 to play first and 2 for second: ";
    int choice;
    cin >> choice;
    if (choice == 2) userFirst = false;
    while (turn <= 9) {
        display();
        if ((turn % 2 == 1 && userFirst) || (turn % 2 == 0 && !userFirst)) {
            userTurn();
        } else {
            cout << endl << "Computer's move:" << endl;
            computerTurn();
        }
        int userResult = checkWin(board, User);
        int computerResult = checkWin(board, Computer);
        if (userResult == 10) {
            display();
            cout << "User wins!" << endl;
            break;
        } else if (computerResult == 10) {
            display();
            cout << "Computer wins!" << endl;
            break;
        } else if (isDraw()) {
            display();
            cout << "It's a draw!" << endl;
            break;
        }
        turn++;
    }
    cout << "Total recursion steps: " << count << endl;
    return 0;
}

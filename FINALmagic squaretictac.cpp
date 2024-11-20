#include <iostream> 
#include <vector> 
 
using namespace std; 
 
int PLAYER_User = 3; 
int PLAYER_Computer = 5; 
const int BLANK = 2; 
 
vector<int> board(9, BLANK); 
int turn = 1; 
bool userFirst = true; 
vector<int> MagicSquare = {6, 1, 8, 7, 5, 3, 2, 9, 4}; 
void printBoard() 
{ 
    for (int i = 0; i < 9; i++) 
    { 
        if (board[i] == 3) 
            cout << "X "; 
        else if (board[i] == 5) 
            cout << "O "; 
        else 
            cout << "- "; 
        if (i % 3 == 2) 
            cout << endl; 
    } 
    cout << endl; 
} 
 
string checkWin() 
{ 
    for (int i = 0; i < 9; ++i) 
    { 
        if (board[i] == PLAYER_User) 
        { 
            for (int j = i + 1; j < 9; ++j) 
            { 
                if (board[j] == PLAYER_User) 
                { 
                    for (int k = j + 1; k < 9; ++k) 
                    { 
                        if (board[k] == PLAYER_User) 
                        { 
                            if (MagicSquare[i] + MagicSquare[j] + MagicSquare[k] == 
15) 
                            { 
                                return "PLAYER_User"; 
                            } 
                        } 
                    } 
                } 
            } 
        } 
    } 
 
    for (int i = 0; i < 9; ++i) 
    { 
        if (board[i] == PLAYER_Computer) 
        { 
            for (int j = i + 1; j < 9; ++j) 
            { 
                if (board[j] == PLAYER_Computer) 
                { 
                    for (int k = j + 1; k < 9; ++k) 
                    { 
                        if (board[k] == PLAYER_Computer) 
                        { 
                            if (MagicSquare[i] + MagicSquare[j] + MagicSquare[k] == 
15) 
                            { 
                                return "PLAYER_Computer"; 
                            } 
                        } 
                    } 
                } 
            } 
        } 
    } 
 
    return ""; 
} 
 
int goAnywhere() 
{ 
    if (board[0] == 2) 
        return 0; 
    if (board[1] == 2) 
        return 1; 
    if (board[2] == 2) 
        return 2; 
    if (board[3] == 2) 
        return 3; 
    if (board[4] == 2) 
        return 4; 
    if (board[5] == 2) 
        return 5; 
    if (board[6] == 2) 
        return 6; 
    if (board[7] == 2) 
        return 7; 
    if (board[8] == 2) 
        return 8; 
    return -1; 
} 
 
int posswin(int player) 
{ 
    for (int i = 0; i < 9; ++i) 
    { 
        if (board[i] == player) 
        { 
            for (int j = i + 1; j < 9; ++j) 
            { 
                if (board[j] == player) 
                { 
                    int requiredSum = 15 - MagicSquare[i] - MagicSquare[j]; 
                    for (int k = 0; k < 9; ++k) 
                    { 
                        if (MagicSquare[k] == requiredSum && board[k] == 
BLANK) 
                        { 
                            return k; 
                        } 
                    } 
                } 
            } 
        } 
    } 
    return -1; 
} 
 
void go(int move) 
{ 
    board[move] = userFirst ? PLAYER_User : PLAYER_Computer; 
    turn++; 
} 
 
int make2() 
{ 
    if (board[4] == BLANK) 
        return 4; 
    vector<int> options = {0, 2, 6, 8}; 
    for (int option : options) 
    { 
        if (board[option] == BLANK) 
            return option; 
    } 
    return -1; 
} 
 
void userMove() 
{ 
    int move; 
    cout << "Enter your move (1-9): "; 
    cin >> move; 
    if (board[move - 1] == BLANK) 
    { 
        go(move - 1); 
    } 
    else 
    { 
        cout << "Invalid move. Try again." << endl; 
        userMove(); 
    } 
} 
 
void computerMove() 
{ 
    int move = -1; 
    switch (turn) 
    { 
    case 1: 
        move = 0; 
        break; 
    case 2: 
        move = (board[4] == BLANK) ? 4 : 0; 
        break; 
    case 3: 
        move = (board[8] == BLANK) ? 8 : 2; 
        break; 
    case 4: 
        if (posswin(PLAYER_User) != -1) 
            move = posswin(PLAYER_User); 
        else 
            move = make2(); 
        break; 
    case 5: 
        if (posswin(PLAYER_Computer) != -1) 
            move = posswin(PLAYER_Computer); 
        else if (posswin(PLAYER_User) != -1) 
            move = posswin(PLAYER_User); 
        else if (board[6] == BLANK) 
            move = 6; 
        else 
            move = 2; 
        break; 
    case 6: 
        if (posswin(PLAYER_Computer) != -1) 
            move = posswin(PLAYER_Computer); 
        else if (posswin(PLAYER_User) != -1) 
            move = posswin(PLAYER_User); 
        else 
            move = make2(); 
        break; 
    default: 
        if (posswin(PLAYER_Computer) != -1) 
            move = posswin(PLAYER_Computer); 
        else if (posswin(PLAYER_User) != -1) 
            move = posswin(PLAYER_User); 
        else 
            move = goAnywhere(); 
        break; 
    } 
    go(move); 
} 
 
int main() 
{ 
    cout << "Do you want to play first? (y/n): "; 
    char choice; 
    cin >> choice; 
    if (choice == 'n') 
        userFirst = false; 
    if (userFirst) 
    { 
        PLAYER_User = 3; 
        PLAYER_Computer = 5; 
    } 
    else 
    { 
        PLAYER_Computer = 3; 
        PLAYER_User = 5; 
    } 
    while (turn <= 9) 
    { 
        printBoard(); 
 
        if (userFirst) 
        { 
            userMove(); 
            userFirst = false; 
        } 
        else 
        { 
            computerMove(); 
            userFirst = true; 
        } 
 
        if (checkWin() == "PLAYER_User") 
        { 
            printBoard(); 
            cout << "User wins!" << endl; 
            break; 
        } 
        else if (checkWin() == "PLAYER_Computer") 
        { 
            printBoard(); 
            cout << "Computer wins!" << endl; 
            break; 
        } 
        else if (turn > 9) 
        { 
            printBoard(); 
            cout << "It's a draw!" << endl; 
            break; 
        } 
    } 
 
    return 0; 
}
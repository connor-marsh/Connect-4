// Team 19: Jackson, Connor, and Bart

/*
 * This is a recreation of the game connect 4 with ascii graphics in the console.
 * Player 1 plays as X and player 2 plays as O. There is a single player option
 * available where you can play against an AI, but the AI is not very good.
 * I tried to make an AI using the minimax algorithm but I ran into some
 * difficulty because I'm used to programming AI's in higher level languages
 * like python where you can return arrays and do other things that make the code simpler*/

// All the function body's for the AI are mostly self-explanatory. A better description of the
// minimax algorithm can be found on wikipedia. This is actually an implementation of negamax (which is very similar and functionally identical to minimax).

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int nodesSearched;


void printBoard(char board[][6]) {
    printf("*****************************************\n");
    printf("1 2 3 4 5 6 7\n");
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 7; i++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    
    printf("*****************************************\n");
}

void createBoard(char board[][6]) {
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            board[i][j] = '_';
        }
    }
}
int isValidMove(char board[][6], int move) {
    return board[move-1][0] == '_';
}
int generateMoves(char board[][6], int movesOutput[]) { // returns the number of possible moves
    int numMoves = 0;
    for (int i = 0; i < 7; i++) {
        if (isValidMove(board, i+1)) {
            movesOutput[numMoves] = i+1;
            numMoves++;
        }
    }
    return numMoves;
}
int getNumMoves(char board[][6]) { // this function only exists because there is no function overloading in c
    int numMoves = 0;
    for (int i = 0; i < 7; i++) {
        if (isValidMove(board, i+1)) {
            numMoves++;
        }
    }
    return numMoves;
}

int gameIsWon(char board[][6], int XsTurn) { // return 1 if the game is over
    
    int dirs[8][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] != (XsTurn ? 'X' : 'O')) {
                continue;
            }
            for (int dir = 0; dir < 8; dir++) {
                
                if (i + dirs[dir][0]*3 < 0 || i + dirs[dir][0]*3 > 6 || j + dirs[dir][1]*3 < 0 || j + dirs[dir][1]*3 > 5) {
                    continue; // This means that we've hit an edge case
                }
                
                int inARow = 1;
                for (int count = 1; count < 4; count++) {
                    if (board[i+dirs[dir][0]*count][j+dirs[dir][1]*count] == (XsTurn ? 'X' : 'O')) {
                        inARow++;
                    }
                }
                if (inARow == 4) {
                    return 1;
                }
            }
        }
    }
    
    return 0;
            
            
    
}

void makeMove(char board[][6], int move, int XsTurn) {
    // because of error checking we can assume the top row is clear
    move--;
    for (int j = 0; j < 5; j++) {
        if (board[move][j+1] != '_') {
            board[move][j] = (XsTurn ? 'X' : 'O');
            return;
        }
    }
    board[move][5] = (XsTurn ? 'X' : 'O');
}

void unmakeMove(char board[][6], int move) {
    move--;
    for (int j = 0; j < 6; j++) {
        if (board[move][j] != '_') {
            board[move][j] = '_';
            return;
        }
    }
}

int evalBoard(char board[][6], int XsTurn) {
    int centeredSum = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == 'X') {
                if (i >1 && i < 5) {
                    centeredSum += 1 * (XsTurn ? 1 : -1);
                }
                else if (i == 0 || i == 6) {
                    centeredSum -= 1 * (XsTurn ? 1 : -1);
                }
            }
            else if (board[i][j] == 'O') {
                if (i >1 && i < 5) {
                    centeredSum += 1 * (XsTurn ? -1 : 1);
                }
                else if (i == 0 || i == 6) {
                    centeredSum -= 1 * (XsTurn ? -1 : 1);
                }
            }
        }
    }
    return centeredSum;
}

int minimax(char board[][6], int isRoot, int depth, int XsTurn, int alpha, int beta) {
    
    nodesSearched++;

    if (gameIsWon(board, !XsTurn)) {
        return -1*(1000000+depth*200);
    }
    else if (getNumMoves(board) == 0) {
        return 0;
    }
    else if (depth == 0) {
        return evalBoard(board, XsTurn);
    }
    
    int moves[7];
    int numMoves = generateMoves(board, moves);
    
    int bestScore = -9999999;
    int bestIndex = 0;
    for (int i = 0; i < numMoves; i++) {
        makeMove(board, moves[i], XsTurn);
        int search = -minimax(board, 0, depth-1, !XsTurn, -beta, -alpha);
        unmakeMove(board, moves[i]);
        
        
        if (search > bestScore) {
            bestScore = search;
            bestIndex = i;
        }
        if (bestScore > alpha) {
            alpha = bestScore;
        }
        if (alpha >= beta) {
            break;
        }
    }

    if (isRoot) {
        return bestIndex;
    }

    return bestScore;
        
    
}

int aiMove(char board[][6]) {
    // ai will use the minimax algorithm with alpha beta pruning
    int totalTime = 0;
    int clockElapsed;
    int depth = 0;
    int moves[7];
    int search;
    nodesSearched = 0;
    do {
        clock_t clockStart = clock();
        

        depth++;
        int numMoves = generateMoves(board, moves);
        search = minimax(board, 1, depth, 0, -9999999, 9999999);
        clock_t clockEnd = clock();

        totalTime += (int) clockEnd - clockStart;
    } while (totalTime < 1000);
    printf("Depth reached: %d\n", depth);
    printf("Time elapsed: %d milliseconds\n", totalTime);
    printf("Nodes searched: %d\n", nodesSearched);


    return moves[search];

    
}







int main() {
    
    
    int players;
    do {
        printf("Do you want 1 or 2 players?\n");
        scanf("%d", &players);
    } while (!(players == 1 || players == 2));
    
    char board[7][6];
    createBoard(board);
    
    do {
        printBoard(board);
        
        int move;
        // get move
        do {
            printf("Player 1: What Column do you want to place your piece in?\n");
            scanf("%d", &move);
        } while (!isValidMove(board, move));
        
        makeMove(board, move, 1);
        if (gameIsWon(board, 1)) {
            printf("PLAYER 1 WINS!!!!!!!\n\n");
            break;
        }
        else if (getNumMoves(board) == 0) {
            printf("ITS A DRAW!!!!!\n\n");
        }
        
        if (players == 1) {
            move = aiMove(board);
        }
        else {
            printBoard(board);
            do {
                printf("Player 2: What Column do you want to place your piece in?\n");
                scanf("%d", &move);
            } while (!isValidMove(board, move));
        }
        
        makeMove(board, move, 0);
        
        if (gameIsWon(board, 0)) {
            printf("PLAYER 2 WINS!!!!!!!\n\n");
            break;
        }
        else if (getNumMoves(board) == 0) {
            printf("ITS A DRAW!!!!!\n\n");
        }
        
        
    } while (1);
    printBoard(board);
    
    return 0;
    
}

/*
 * connectFour.c
 *
 * Created: 4/13/2022 1:49:42 PM
 *  Author: surim
 */ 
/*
 * connectFour.c
 *
 * Created: 4/13/2022 1:49:42 PM
 *  Author: surim
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int board[5][5]; 
int numTurns = 0; 
int player1Turns = 0;
int player2Turns = 0; 
int player1 = 1; 
//int player2 = 0; 
int gameOver = 0;
//int x[6][7];

void reset(){
  int i,j;
  //draw board
  for(i = 0; i < 5; i++){
    for(j = 0; j < 5; j++){
		board[i][j] = 0;  
    }
  }
	placeMines(3);
  }
  //add random mines on the board, if a ball lands in this slot then it switches to the opposing teams color 
  //also need to implement bonus ball feature if player throws from far enough   


void placeMines(int numMines){
	int minesUsed = 0; 
	//board[4][0] = 3; 
	board[4][1] = 3; 
	/*
	while(minesUsed < numMines){
		int row = rand() % (4 + 1 - 0) + 0;
		int col = rand() % (4 + 1 - 0) + 0;
		if(board[row][col] == 0){
			board[row][col] = 3;
			minesUsed++;
		}
	} */ 
}


/*
      getCurrentPlayer is a getter for the player
      whose turn it is in the game.
      
      @return true if it's Player 1's turn,
      false if it's Player 2's turn.
     */
     int getCurrentPlayer() {
        return player1;
    }
	void setCurrentPlayer(int player){
		player1 = player; 
	}
	
    
    /*
      getCell is a getter for the contents of the
      cell specified by the method arguments.
      
      @param c column to retrieve
      @param r row to retrieve
      @return an integer denoting the contents
              of the corresponding cell on the 
              game board.  0 = empty, 1 = Player 1,
              2 = Player 2
     */
  int getCell(int c, int r) {
        return board[r][c];
    }
  int getNumTurns() {
        return numTurns;
        
    }
 int getGameOver() {
        return gameOver;
        
    }
    
 void setGameOver(int x) {
        gameOver = x;
    }
	
int setCell(int r, int c, int val){
	board[r][c] = val; 
}


    /*
      playTurn allows players to play a turn. Returns true 
      if the move is successful and false if a player tries
      to play in a location that is taken or after the game
      has ended. If the turn is successful and the game has 
      not ended, the player is changed. If the turn is 
      unsuccessful or the game has ended, the player is not 
      changed.
      
      
      param- c column to play in
      param- r row to play in
      return whether the turn was successful
     */
    
    //drops the chip in and stacks them 
     int playTurn(int c) {
        if ((board[0][c] == 1) || (board[0][c] == 2) || gameOver) { // != 0 
            return 0;
        }
//		 if (board[0][c] != 0 || gameOver) { // != 0
//	              return 0;
//	         }

        if (player1 == 1) {
           int row,col;
            for (row = sizeof(board) - 1; row >= 0; row--) {
				if(board[row][c] == 3){
					board[row][c] = 2; 
					LCD_drawString(40,10, "You hit a mine!",65535,0);
					break; 
				}
                if (board[row][c] == 0) {
                    board[row][c] = 1;
                    break;
                }
            }
         
        } else {
          int row,col;
            for ( row = sizeof(board) - 1; row >= 0; row--) {
				if(board[row][c] == 3){
					board[row][c] = 1;
					LCD_drawString(40,10, "You hit a mine!",65535,0);
					break; 
				}
                 if (board[row][c] == 0) {
                    board[row][c] = 2;
                    break;
                }
            }
           
        }
        /*
        int x [6][7];
        int row,col;
        for ( row = 0; row < 6; row++) {
            for ( col = 0; col < 7; col++) {
                x[row][col] = board[row][col];
            }
        }
*/ 
        numTurns++;
        if (checkWinner() == 0) {
            if(player1 == 1){
				player1 = 0;
			}
			else{
				player1 = 1;
			}
        }
       
        //storedState.add(numTurns, x);
        return 1;
    }

    /*
      checkWinner checks whether the game has reached a win 
      condition. checkWinner only looks for horizontal wins.
      
      @return 0 if nobody has won yet, 1 if player 1 has won, and
                 2 if player 2 has won, 3 if the game hits stalemate
     */
     int checkWinner() {
       int row,col;
         //Check horizontal win
        for ( row = 0; row < sizeof(board); row++) {
            for ( col = 0;col < sizeof(board[0]) - 3;col++) {
                if (board[row][col] != 0 && board[row][col] == board[row][col + 1] && 
                    board[row][col] == board[row][col + 2] &&
                    board[row][col] == board[row][col + 3]) {
                    gameOver = 1;
                    if (player1) {
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }           
        } 
        
        //check vertical
        for ( row = 0; row < sizeof(board) - 3; row++) {
            for ( col = 0;col < sizeof(board[0]);col++) {
                if (board[row][col] != 0 && 
                    board[row + 1][col] == board[row][col] && 
                    board[row + 2][col] == board[row][col] &&
                    board[row + 3][col] == board[row][col]) {
                    gameOver = 1;
                    if (player1) {
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }           
        } 
        
        
        //check diagonal downward win
        for (int row = 0; row < sizeof(board) - 3; row++) {
            for (int col = 0; col < sizeof(board[0]) - 3; col++) {
                if (board[row][col] != 0 && 
                    board[row][col] == board[row + 1][col + 1] 
                            && 
                    board[row + 1][col + 1] == board[row + 2][col + 2] &&
                    board[row + 2][col + 2] == board[row + 3][col + 3] 
                    ) {
                    gameOver = 1;
                    if (player1) {
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }
        }
        
      //check upward diagonal
        for (int row = 3; row < sizeof(board); row++) {
            for (int col = 0; col < sizeof(board[0]) - 3; col++) {
                if (board[row][col] != 0 && 
                    board[row][col] == board[row - 1][col + 1] && 
                    board[row - 1][col + 1] == board[row - 2][col + 2] &&
                    board[row - 2][col + 2] == board[row - 3][col + 3]) {         
                    gameOver = 1;
                    if (player1) {
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }
        }
        
       

        
        if (numTurns >= 42) {
            gameOver = 1;
            return 3;
        } else {
            return 0;
        }
		}
		
		void drawBoard(){
			for(int row = 0; row < 5; row++){
				for(int col = 0; col < 5; col++){
					if(board[row][col] == 1){
						LCD_drawCircle(20*col + 20, 20*row + 20, 5, 0xF800);
					}
					else if(board[row][col] == 2){
						LCD_drawCircle(20*col + 20, 20*row + 20, 5, 0xFFE0);
					}
				}
			}
			
			
		}
			
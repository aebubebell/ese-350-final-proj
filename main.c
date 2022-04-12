typedef struct gameBoard {
//x and y midpt pos of paddle
int x,y;
//vertical velocity of paddle
int vy;
} paddle;

static void reset(){
  int i,j;
  //draw board
  for(i = 0; i < 20; i++){
    for(j = 5; j < 20; j++){
      LCD_drawBlock(i,j,i+5,j+5,black);
    }
  }
    
   int board[6][6];
   int numTurns = 0;
   static boolean player1 = true;
   static gameOver = false;
   int x[6][7];



}
/**
     * getCurrentPlayer is a getter for the player
     * whose turn it is in the game.
     * 
     * @return true if it's Player 1's turn,
     * false if it's Player 2's turn.
     */
     boolean getCurrentPlayer() {
        return player1;
    }
    
    /**
     * getCell is a getter for the contents of the
     * cell specified by the method arguments.
     * 
     * @param c column to retrieve
     * @param r row to retrieve
     * @return an integer denoting the contents
     *         of the corresponding cell on the 
     *         game board.  0 = empty, 1 = Player 1,
     *         2 = Player 2
     */
     int getCell(int c, int r) {
        return board[r][c];
    }
   int getNumTurns() {
        return numTurns;
        
    }
 boolean getGameOver() {
        return gameOver;
        
    }
    
     void setGameOver(boolean x) {
        gameOver = x;
    }


    /**
     * playTurn allows players to play a turn. Returns true 
     * if the move is successful and false if a player tries
     * to play in a location that is taken or after the game
     * has ended. If the turn is successful and the game has 
     * not ended, the player is changed. If the turn is 
     * unsuccessful or the game has ended, the player is not 
     * changed.
     * 
     * 
     * param- c column to play in
     * param- r row to play in
     * return whether the turn was successful
     */
    
    //drops the chip in and stacks them 
     boolean playTurn(int c) {
        if (board[0][c] != 0 || gameOver) {
            return false;
        }

        if (player1) {
           int row,col;
            for (row = sizeof(board) - 1; row >= 0; row--) {
                if (board[row][c] == 0) {
                    board[row][c] = 1;
                    break;
                }
            }
         
        } else {
          int row,col;
            for ( row = sizeof(board) - 1; row >= 0; row--) {
                if (board[row][c] == 0) {
                    board[row][c] = 2;
                    break;
                }
            }
           
        }
        
        int x [6][7];
        int row,col;
        for ( row = 0; row < 6; row++) {
            for ( col = 0; col < 7; col++) {
                x[row][col] = board[row][col];
            }
        }

        numTurns++;
        if (checkWinner() == 0) {
            player1 = !player1;
        }
       
        //storedState.add(numTurns, x);
        return true;
    }

    /**
     * checkWinner checks whether the game has reached a win 
     * condition. checkWinner only looks for horizontal wins.
     * 
     * @return 0 if nobody has won yet, 1 if player 1 has won, and
     *            2 if player 2 has won, 3 if the game hits stalemate
     */
    public int checkWinner() {
       int row,col;
         //Check horizontal win
        for ( row = 0; row < sizeof(board); row++) {
            for ( col = 0;col < sizeof(board[0]) - 3;col++) {
                if (board[row][col] != 0 && board[row][col] == board[row][col + 1] && 
                    board[row][col] == board[row][col + 2] &&
                    board[row][col] == board[row][col + 3]) {
                    gameOver = true;
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
                    gameOver = true;
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
                    gameOver = true;
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
                    gameOver = true;
                    if (player1) {
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }
        }
        
       

        
        if (numTurns >= 42) {
            gameOver = true;
            return 3;
        } else {
            return 0;
        }
    } 

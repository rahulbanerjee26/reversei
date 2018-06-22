#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct move{
    char row;
    char col;
    int  score1;
};


char board[26][26];
int n;
//Function for setting up the initial Board
void buildBoard(char board[][26],int n){
    int row,col;
    for(row=0;row<n;row++){
        for(col=0;col<n;col++){
            board[row][col]='U';
        }
    }
    board[n/2-1][n/2-1]='W';
    board[n/2-1][n/2]='B';
    board[n/2][n/2-1]='B';
    board[n/2][n/2]='W';
}

//Print the Board
void printBoard(char board[][26],int n){
    int row,col;
    printf("  ");
    for(row=0;row<n;row++){
        printf("%c",(char)(97+row));
    }
    printf("\n");
    for(row=0;row<n;row++){
        printf("%c ",(char)(97+row));
        for(col=0;col<n;col++){
            printf("%c",board[row][col]);
        }
        printf("\n");
    }
}

//Update the Board
void updateBoard(char board[][26],char color,char row,char column){
    int Row=row-97;
    int Col=column-97;
    board[Row][Col]=color;
}

//Checks if position is in bounds
bool positionInBounds(int n, char row, char col){
    bool legal;
    int Row=row-97;
    int Col=col-97;
    if(Row>n||Row<0||Col>n||Col<0)
        legal=false;
    else
        legal=true;
    return legal;
}

//Checks the legality of a move
bool checkLegalInDirection(char board[][26], int n, char row, char col,char colour, int deltaRow, int deltaCol){
    bool legal;
    legal=false;
    char opp;
    if(colour=='W')
        opp='B';
    else
        opp='W';
    if(board[(int)row-97][(int)col-97]=='U'){
        for(deltaRow=-1;deltaRow<=1;deltaRow++){
            for(deltaCol=-1;deltaCol<=1;deltaCol++){
                if((deltaRow==0&&deltaCol==0)||((int)row+deltaRow-97)<0||((int)col+deltaCol-97)<0);
                else{
                    if(positionInBounds(n,(char)((int)row+deltaRow),(char)((int)col+deltaCol))){
                        if(board[(int)row+deltaRow-97][(int)col+deltaCol-97]==opp){
                            int x=(int)(row)+deltaRow-97;int y=(int)col+deltaCol-97;
                            while(positionInBounds(n,(char)(97+x),(char)(97+y))){
                                x+=deltaRow;
                                y+=deltaCol;
                                if(positionInBounds(n,(char)(97+x),(char)(97+y))){
                                    if(board[x][y]=='U'){
                                        break;
                                    }    
                                    if(board[x][y]==colour){
                                        legal=true;
                                    }
                            
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return legal;
}


//Counts available move for White
int countMovesForHuman(char board[][26],int n,char color){
    int count=0;
    int row;int col;
    for(row=0;row<n;row++){
        for(col=0;col<n;col++){
            if(checkLegalInDirection(board,n,(char)(97+row),(char)(97+col),color,-1,-1)){
                count++;
			}
		}
	}
    return count;
}


//Counts available move for Black
int countMovesForComputer(char board[][26],int n,char color){
    int count=0;
    int row;int col;
    for(row=0;row<n;row++){
        for(col=0;col<n;col++){
            if(checkLegalInDirection(board,n,(char)(97+row),(char)(97+col),color,-1,-1)){
                count++;
			}
		}	
    }
    return count;
}


bool check(char board[][26],int n,int x,int y,int deltaCol,int deltaRow,char colour){
	bool legal=false; 
	while(positionInBounds(n,(char)(97+x),(char)(97+y))){
		if(board[x][y]=='U'){
			break;
		}
	x+=deltaRow;y+=deltaCol;
	}
	if(board[x-deltaRow][y-deltaCol]==colour){ legal =true; }
	return legal;
}

void flip(char board[][26], int n, char row, char col,char colour, int deltaRow, int deltaCol){

    char opp;
    if(colour=='W')
        opp='B';
    else
        opp='W';
    if(board[(int)row-97][(int)col-97]=='U'){
        for(deltaRow=-1;deltaRow<=1;deltaRow++){
            for(deltaCol=-1;deltaCol<=1;deltaCol++){
                if(deltaRow==0&&deltaCol==0);
                else{
                    if(positionInBounds(n,(char)((int)row+deltaRow),(char)((int)col+deltaCol))){
                        if(board[(int)row+deltaRow-97][(int)col+deltaCol-97]==opp){
                            int x=(int)(row)+deltaRow-97;int y=(int)col+deltaCol-97;
								if(check(board,n,x,y,deltaCol,deltaRow,colour)){
									do{
										updateBoard(board,colour,(char)(97+x),(char)(97+y));
										x+=deltaRow;
										y+=deltaCol;	
										if(!(positionInBounds(n,(char)(97+x),(char)(97+y)))){ break; }
									}while(board[x][y]!=colour);
								}		
						}
					}
				}
			}
		}	
	}
}


//Function to count the number of tiles flipped
int score(char board[][26], int n, char row, char col,char colour, int deltaRow, int deltaCol){
	int moveScore=0;
    char opp;
    if(colour=='W')
        opp='B';
    else
        opp='W';
    if(board[(int)row-97][(int)col-97]=='U'){
        for(deltaRow=-1;deltaRow<=1;deltaRow++){
            for(deltaCol=-1;deltaCol<=1;deltaCol++){
                if(deltaRow==0&&deltaCol==0);
                else{
                    if(positionInBounds(n,(char)((int)row+deltaRow),(char)((int)col+deltaCol))){
                        if(board[(int)row+deltaRow-97][(int)col+deltaCol-97]==opp){
                            int x=(int)(row)+deltaRow-97;int y=(int)col+deltaCol-97;
								if(check(board,n,x,y,deltaCol,deltaRow,colour)){
									do{
										moveScore++;
										x+=deltaRow;
										y+=deltaCol;	
										if(!(positionInBounds(n,(char)(97+x),(char)(97+y)))){ break; }
									}while(board[x][y]!=colour);
								}		
						}
					}
				}
			}
		}	
	}
	return moveScore;
}

//Function the stores all the possible moves with the number of tiles they flip in a structure
//It then choses the best move and updates board with that move
void computerMove(char board[][26],int n,char computerColor){
    struct move comp[26];
    int i,j,index=0;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(checkLegalInDirection(board,n,(char)(97+i),(char)(97+j),computerColor,-1,-1)){
                comp[index].row=(char)(97+i);
                comp[index].col=(char)(97+j);
                comp[index].score1=score(board,n,comp[index].row,comp[index].col,computerColor,-1,-1);
				index++;
			}    
        }
    }
    int maxScore=1;
    char maxScoreRow;
    char maxScoreCol;
    for(i=0;i<index;i++){
        if(comp[i].score1>=maxScore){
            maxScore=comp[i].score1;
            maxScoreCol=comp[i].col;
            maxScoreRow=comp[i].row;
        }
    }
    for(i=0;i<index;i++){
        if(maxScore==comp[i].score1){
            if(maxScoreRow>comp[i].row){
				maxScoreCol=comp[i].col;
				maxScoreRow=comp[i].row;
			}
            else if(maxScoreRow==comp[i].row&&maxScoreCol>comp[i].col){
					maxScoreCol=comp[i].col;
			}
        }
        
    }
    flip(board,n,maxScoreRow,maxScoreCol,computerColor,-1,-1);
	updateBoard(board,computerColor,maxScoreRow,maxScoreCol);
    printf("Computer places %c at %c%c. \n",computerColor,maxScoreRow,maxScoreCol);
}

int count(char board[][26],int n,char color){
	int i=0;int j=0;
	int count=0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(board[i][j]==color)
				count++;
		}
	}
	return count;
}

int main(int argc, char **argv)
{
    char row,column;
    printf("Enter the board dimension: ");
    scanf("%d",&n);
	buildBoard(board,n);
	char turn='B';
	char human;
	char computer;
	printf("Computer plays (B/W) :");
	scanf(" %c",&computer);
	printBoard(board,n);
	if(computer=='B') 
		human='W';
	else
		human='B';
 	int totalTiles=n*n;
	int whiteScore=count(board,n,'W');
	int blackScore=count(board,n,'B');
	int humanCount= countMovesForHuman(board,n,human);
	int computerCount= countMovesForComputer(board,n,computer);
	do{ 
		 if(computer==turn){
			if(computerCount==0){ 
				printf("%c player has no valid move.\n",computer);
				turn=human;
			}
			else{
				computerMove(board,n,computer);
				printBoard(board,n);
				turn=human;
			}
		}
		else{
			if(humanCount==0){
				printf("%c player has no valid move.\n",human);
				turn=computer;
			}
			else{
				printf("Enter move for colour %c (RowCol): ",human);
				scanf(" %c%c",&row,&column);
				if(checkLegalInDirection(board,n,row,column,human,-1,-1)){
					flip(board,n,row,column,human,-1,-1);
					updateBoard(board,human,row,column);
					printBoard(board,n);
					turn=computer;
				}
				else{
					printf("Invalid move.\n");
					if(turn=='W')
						blackScore=whiteScore+2;
					else
						whiteScore=blackScore+2;
					break;
				}
		
			}	
		}
		humanCount= countMovesForHuman(board,n,human);
		computerCount= countMovesForComputer(board,n,computer);
		whiteScore=count(board,n,'W');
		blackScore=count(board,n,'B');
	}while((humanCount+computerCount!=0)||(whiteScore+blackScore)>totalTiles);
	
    if(blackScore>whiteScore)
		printf("B player wins.\n");
	else
		printf("W player wins.\n");
    return 0;
}

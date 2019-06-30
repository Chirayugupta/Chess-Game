#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<dos.h>
#include<time.h>
struct COORDS       //this structure is used to represent coordinates of units on the board
{   int row;
    int col;
};


char square[8][8]={     'r','n','b','q','k','b','n','r',                //this is a 2-d array of 8*8 characters which represents the chessboard
                        'p','p','p','p','p','p','p','p',                //the small character represent that its a black unit
                        '\0','\0','\0','\0','\0','\0','\0','\0',        //the capital character represents that its a white unit
                        '\0','\0','\0','\0','\0','\0','\0','\0',        //the places which are empty stores a null character
                        '\0','\0','\0','\0','\0','\0','\0','\0',        //P -> White Pawn    p -> Black pawn
                        '\0','\0','\0','\0','\0','\0','\0','\0',        //R -> White Rook    r -> Black rook
                        'P','P','P','P','P','P','P','P',                //N -> White Knight  n -> Black knight
                        'R','N','B','Q','K','B','N','R'      };         //B -> White Bishop  b -> Black bishop
                                                                        //Q -> White Queen   q -> Black queen
                                                                        //K -> White King    k -> Black king
void display();
void delay(unsigned int);
int valid_move2(struct COORDS Curr,struct COORDS New);
int valid_move(struct COORDS,struct COORDS);
int bk_underattack2(struct COORDS);
int wk_underattack2(struct COORDS);
int bk_underattack();
int wk_underattack();
int b_total_valid_moves();
int w_total_valid_moves();
void b_checkmate();
void w_checkmate();
void b_check();
void w_check();
void b_stalemate();
void w_stalemate();
void w_promote();
void b_promote();

void delay(unsigned int mseconds)
{
        clock_t goal = mseconds + clock();
        while(goal>clock());
}

void display()
{   struct COORDS pos;
    printf("\n");
    printf("   0  1  2  3  4  5  6  7\n");                  //This function displays the current status of the board
    for(pos.row=0;pos.row<8;++pos.row)
    {   printf("%d |",pos.row);
        for(pos.col=0;pos.col<8;++pos.col)
            printf("%-2c|",square[pos.row][pos.col]);
        printf("\n");
    }

}

void main()
{   int turn=0;
    struct COORDS Curr,New;
    display();
    while(1)
    {   if(turn%2==0)
        {
            printf("\nEnter White's Move: ");
            scanf("%d.%d to %d.%d",&Curr.row,&Curr.col,&New.row,&New.col);
            if(isupper(square[Curr.row][Curr.col])>0)
            {   if(valid_move(Curr,New))
                {   square[New.row][New.col]=square[Curr.row][Curr.col];
                    square[Curr.row][Curr.col]='\0';
                    w_promote();
                    b_checkmate();
                    b_stalemate();
                    b_check();
                }
                else
                {   printf("\nInvalid Move!!Try Again\n");
                    delay(2000);
                    ++turn;
                }
            }
            else
            {   printf("\nTry Again!!It's White turn\n");
                delay(2000);
                ++turn;
            }
            system("cls");
            display();
            ++turn;

        }
        else
        {
            printf("\nEnter Black's Move: ");
            scanf("%d.%d to %d.%d",&Curr.row,&Curr.col,&New.row,&New.col);
            if(islower(square[Curr.row][Curr.col])>0)
            {   if(valid_move(Curr,New))
                {   square[New.row][New.col]=square[Curr.row][Curr.col];
                    square[Curr.row][Curr.col]='\0';
                    b_promote();
                    w_checkmate();
                    w_stalemate();
                    w_check();
                }
                else
                {   printf("\nInvalid Move!!Try Again\n");
                    delay(2000);
                    ++turn;
                }
            }
            else
            {   ++turn;
                printf("\nTry Again!!It's Black turn\n");
                delay(2000);
            }
            system("cls");
            display();
            ++turn;

        }

    }

}




int valid_move2(struct COORDS Curr,struct COORDS New)               //valid_move2() takes 2 input-->the current coordinates and new coordinates
{                                                                   //Player moves a piece from current coordinates to the new coordinates
    char unit,newunit;                                              //This function returns 1 if move is valid and 0 if the move is not valid
    int color,row,col,i,j;

    unit=square[Curr.row][Curr.col];        //unit stores the unit on the current coordinates
    newunit=square[New.row][New.col];       //newunit stores the unit on the new coordinates

    if(isalpha(newunit))
            color=islower(newunit);                 //color is a int which stores >0 if black unit is present at the new coordinates
    else                                            //it stores 0 if white unit is present at the new coordinates
            color=-1;                               //it stores <0 if no unit is present at the new coordinates
//////////////////////////////////////////////////////////////////////////////////
    if(unit=='K')
    {   if(New.row==Curr.row-1&&New.col==Curr.col&&color!=0)
            return 1;
        else if(New.row==Curr.row-1&&New.col==Curr.col-1&&color!=0)
            return 1;
        else if(New.row==Curr.row&&New.col==Curr.col-1&&color!=0)
            return 1;
        else if(New.row==Curr.row+1&&New.col==Curr.col-1&&color!=0)     //this code checks for the valid moves of a white king
            return 1;                                                   //it is checking at all the 8 positions around the king
        else if(New.row==Curr.row+1&&New.col==Curr.col&&color!=0)       //if those position contains a black piece or nothing...then its valid
            return 1;
        else if(New.row==Curr.row+1&&New.col==Curr.col+1&&color!=0)
            return 1;
        else if(New.row==Curr.row&&New.col==Curr.col+1&&color!=0)
            return 1;
        else if(New.row==Curr.row-1&&New.col==Curr.col+1&&color!=0)
            return 1;
        else
            return 0;
    }
//////////////////////////////////////////////////////////////////////////////////
    else if(unit=='k')
    {   if(New.row==Curr.row-1&&New.col==Curr.col&&color<=0)
            return 1;
        else if(New.row==Curr.row-1&&New.col==Curr.col-1&&color<=0)
            return 1;
        else if(New.row==Curr.row&&New.col==Curr.col-1&&color<=0)
            return 1;
        else if(New.row==Curr.row+1&&New.col==Curr.col-1&&color<=0)
            return 1;
        else if(New.row==Curr.row+1&&New.col==Curr.col&&color<=0)
            return 1;
        else if(New.row==Curr.row+1&&New.col==Curr.col+1&&color<=0)
            return 1;
        else if(New.row==Curr.row&&New.col==Curr.col+1&&color<=0)
            return 1;
        else if(New.row==Curr.row-1&&New.col==Curr.col+1&&color<=0)
            return 1;
        else
            return 0;
    }
//////////////////////////////////////////////////////////////////////////////////
    else if(unit=='Q')
    {  square[Curr.row][Curr.col]='R';
       i=valid_move2(Curr,New);

       square[Curr.row][Curr.col]='B';              //to check valid move for queen ... we replace the queen if with the bishop and check for valid move
       j=valid_move2(Curr,New);                     //by recursively calling the same function .....and then we replace the queen with a rook and
                                                    //check for valid move by recursively calling the same function...if move is not valid for both the
       square[Curr.row][Curr.col]='Q';              //conditions....then it return 0  otherwise 1.

       return i+j;
    }
////////////////////////////////////////////////////////////////////////////////////
    else if(unit=='q')
    {  square[Curr.row][Curr.col]='r';
       i=valid_move2(Curr,New);

       square[Curr.row][Curr.col]='b';
       j=valid_move2(Curr,New);

       square[Curr.row][Curr.col]='q';

       return i+j;
    }
////////////////////////////////////////////////////////////////////////////////////
    else if(unit=='P')
    {

            if(New.row==Curr.row-1&&New.col==Curr.col+1)
                    if(color>0)
                            return 1;
                    else
                            return 0;
            else if(New.row==Curr.row-1&&New.col==Curr.col-1)
                    if(color>0)                                 //its also valid to move diagonally one block if that block contains piece of opposite color
                            return 1;
                    else
                            return 0;
            else if(New.row==Curr.row-1&&New.col==Curr.col)
                    if(color<0)
                        return 1;                               //its a valid for a pawn to move by one block in the same column if that block doesnt contain any unit
                    else
                        return 0;
            else if(New.row==Curr.row-2&&New.col==Curr.col&&Curr.row==6)            //its also valid to move 2 blocks along the same column if its first move of the pawn...and the two blocks are empty
                    if(color<0&&square[New.row+1][New.col]=='\0')
                        return 1;
                    else
                        return 0;
            else
                        return 0;

    }

/////////////////////////////////////////////////////////////////////////////

    else if(unit=='p')
    {

            if(New.row==Curr.row+1&&New.col==Curr.col+1)
                    if(color==0)
                            return 1;
                    else
                            return 0;
            else if(New.row==Curr.row+1&&New.col==Curr.col-1)
                    if(color==0)
                            return 1;
                    else
                            return 0;
            else if(New.row==Curr.row+1&&New.col==Curr.col)
                    if(color<0)
                        return 1;
                    else
                        return 0;
            else if(New.row==Curr.row+2&&New.col==Curr.col&&Curr.row==1)
                    if(color<0&&square[New.row-1][New.col]=='\0')
                        return 1;
                    else
                        return 0;
            else
                        return 0;

    }


////////////////////////////////////////////////////////////////////////////

    else if(unit=='R')
    {
            if(New.col==Curr.col)
            {   if(New.row<Curr.row)
                {   for(row=Curr.row-1;row>New.row;--row)
                        if(square[row][Curr.col]!='\0')
                                return 0;
                    if(color>0||color<0)                            //first we check if the new coords are in the same row or in the same column...if not return 0
                        return 1;                                   //if in the same column , we check using a loop if all the blocks inbetween the current block
                    else                                            //and the new block are all empty or not..if not return 0...because rook cannot take a leap..
                        return 0;                                   //and if the new coords contain a unit of same color then also return 0....otherwise return 1
                }
                else if(New.row>Curr.row)                           //similarly if the new coords are in the same row...we check all the in between blocks and the color of the unit present in new coords
                {   for(row=Curr.row+1;row<New.row;++row)
                        if(square[row][Curr.col]!='\0')
                                return 0;
                    if(color>0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else
                    return 0;
            }
            else if(New.row==Curr.row)
            {   if(New.col<Curr.col)
                {   for(col=Curr.col-1;col>New.col;--col)
                        if(square[Curr.row][col]!='\0')
                                return 0;
                    if(color>0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else if(New.col>Curr.col)
                {   for(col=Curr.col+1;col<New.col;++col)
                        if(square[Curr.row][col]!='\0')
                                return 0;
                    if(color>0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else
                    return 0;
            }
            else
                return 0;

    }


//////////////////////////////////////////////////

    else if(unit=='r')
    {
            if(New.col==Curr.col)
            {   if(New.row<Curr.row)
                {   for(row=Curr.row-1;row>New.row;--row)
                        if(square[row][Curr.col]!='\0')
                                return 0;
                    if(color==0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else if(New.row>Curr.row)
                {   for(row=Curr.row+1;row<New.row;++row)
                        if(square[row][Curr.col]!='\0')
                                return 0;
                    if(color==0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else
                    return 0;
            }
            else if(New.row==Curr.row)
            {   if(New.col<Curr.col)
                {   for(col=Curr.col-1;col>New.col;--col)
                        if(square[Curr.row][col]!='\0')
                                return 0;
                    if(color==0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else if(New.col>Curr.col)
                {   for(col=Curr.col+1;col<New.col;++col)
                        if(square[Curr.row][col]!='\0')
                                return 0;
                    if(color==0||color<0)
                        return 1;
                    else
                        return 0;
                }
                else
                    return 0;
            }
            else
                return 0;

    }



///////////////////////////////////////////////////////////////////

    else if(unit=='N')
    {
        if((New.row==Curr.row-2)&&(New.col==Curr.col+1)&&color!=0)
                return 1;
        else if((New.row==Curr.row-2)&&(New.col==Curr.col-1)&&color!=0)
                return 1;
        else if((New.row==Curr.row-1)&&(New.col==Curr.col-2)&&color!=0)
                return 1;
        else if((New.row==Curr.row+1)&&(New.col==Curr.col-2)&&color!=0)         //knight is simple...we are checking if the new coords is one of the 8 possible coords where
                return 1;                                                       //a knight can move or not...if not return 0
        else if((New.row==Curr.row+2)&&(New.col==Curr.col-1)&&color!=0)         //if it is then we check whether the new coords contains a unit of same color..if yes return 0
                return 1;                                                       //if not then return 1
        else if((New.row==Curr.row+2)&&(New.col==Curr.col+1)&&color!=0)
                return 1;
        else if((New.row==Curr.row+1)&&(New.col==Curr.col+2)&&color!=0)
                return 1;
        else if((New.row==Curr.row-1)&&(New.col==Curr.col+2)&&color!=0)
                return 1;
        else
                return 0;

    }

///////////////////////////////////////////////////////////////////

    else if(unit=='n')
    {
        if((New.row==Curr.row-2)&&(New.col==Curr.col+1)&&color<=0)
                return 1;
        else if((New.row==Curr.row-2)&&(New.col==Curr.col-1)&&color<=0)
                return 1;
        else if((New.row==Curr.row-1)&&(New.col==Curr.col-2)&&color<=0)
                return 1;
        else if((New.row==Curr.row+1)&&(New.col==Curr.col-2)&&color<=0)
                return 1;
        else if((New.row==Curr.row+2)&&(New.col==Curr.col-1)&&color<=0)
                return 1;
        else if((New.row==Curr.row+2)&&(New.col==Curr.col+1)&&color<=0)
                return 1;
        else if((New.row==Curr.row+1)&&(New.col==Curr.col+2)&&color<=0)
                return 1;
        else if((New.row==Curr.row-1)&&(New.col==Curr.col+2)&&color<=0)
                return 1;
        else
                return 0;

    }

//////////////////////////////////////////////////////////////////////////////

   else if(unit=='B')
   {

        if(Curr.row-New.row==New.col-Curr.col)
        {
            if(New.col>Curr.col)
            {   for(row=Curr.row-1,col=Curr.col+1;row>New.row;--row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;                               //we check if the new coords is diagonally w.r.t current coords or not...if not return 0
                if(color!=0)                                            //if it is...then we check all the blocks in between whether  they are empty or not..if not return 0 because bishop cannot take a leap
                        return 1;
                else
                        return 0;                                          //if in between blocks are empty then check if the new position contains a unit of same color...if yes return 0 otherwise 1

            }
            else if(New.col<Curr.col)
            {   for(row=Curr.row+1,col=Curr.col-1;row<New.row;++row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color!=0)
                        return 1;
                else
                        return 0;

            }
            else
                return 0;
        }
        else if(Curr.row-New.row==Curr.col-New.col)
        {
            if(New.col>Curr.col)
            {   for(row=Curr.row+1,col=Curr.col+1;row<New.row;++row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color!=0)
                        return 1;
                else
                        return 0;

            }
            else if(New.col<Curr.col)
            {   for(row=Curr.row-1,col=Curr.col-1;row>New.row;--row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color!=0)
                        return 1;
                else
                        return 0;

            }
            else
                return 0;
        }
        else
            return 0;
   }
///////////////////////////////////////////////////////////////////////////////


   else if(unit=='b')
   {

        if(Curr.row-New.row==New.col-Curr.col)
        {
            if(New.col>Curr.col)
            {   for(row=Curr.row-1,col=Curr.col+1;row>New.row;--row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color<=0)
                        return 1;
                else
                        return 0;

            }
            else if(New.col<Curr.col)
            {   for(row=Curr.row+1,col=Curr.col-1;row<New.row;++row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color<=0)
                        return 1;
                else
                        return 0;

            }
            else
                return 0;
        }
        else if(Curr.row-New.row==Curr.col-New.col)
        {
            if(New.col>Curr.col)
            {   for(row=Curr.row+1,col=Curr.col+1;row<New.row;++row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color<=0)
                        return 1;
                else
                        return 0;

            }
            else if(New.col<Curr.col)
            {   for(row=Curr.row-1,col=Curr.col-1;row>New.row;--row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                if(color<=0)
                        return 1;
                else
                        return 0;

            }
            else
                return 0;
        }
        else
            return 0;
   }
   /////////////////////////////////////////////////////////////////////////
   else
        return 0;


///////////////////////////////////////////////////////////////////////////////
}





//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



int valid_move(struct COORDS Curr,struct COORDS New)
{//*********************************************************************
    int s;
    char unit,newunit;
    struct COORDS pos;                          //valid_move2() function checks if the move made is correct or not..but a correct move may or may not be
                                                //valid .....How??because a move is also invalid if making that move puts your king under attack...
    unit=square[Curr.row][Curr.col];            //valid_move2() func doesnt check for that....thats why we have valid_move() function
    newunit=square[New.row][New.col];

    s=valid_move2(Curr,New);                    //valid_move() first calls valid_move2() and checks whether the move is correct or not
                                                //if not then return 0 if it is ,then it makes the move and then runs a loop for each position on board
    if(s==0)                                    //and check whether the king is under attack or not by calling a particular function for each position
        return 0;                               //if king is under attack from even one position then move becomes invalid and 0 is returned
    else if(s==1)                               //in the last ,the move is undone
    {   /////////////////////////////////////////////
        if(islower(unit)==0)
        {   square[New.row][New.col]=unit;
            square[Curr.row][Curr.col]='\0';
            for(pos.row=0;pos.row<8;++pos.row)
                for(pos.col=0;pos.col<8;++pos.col)
                    if(wk_underattack2(pos))
                    {   square[New.row][New.col]=newunit;
                        square[Curr.row][Curr.col]=unit;
                        return 0;
                    }
             square[New.row][New.col]=newunit;
             square[Curr.row][Curr.col]=unit;
             return 1;
        }
        else if(islower(unit)>0)
        {   square[New.row][New.col]=unit;
            square[Curr.row][Curr.col]='\0';
            for(pos.row=0;pos.row<8;++pos.row)
                for(pos.col=0;pos.col<8;++pos.col)
                    if(bk_underattack2(pos))
                    {   square[New.row][New.col]=newunit;
                        square[Curr.row][Curr.col]=unit;
                        return 0;
                    }
             square[New.row][New.col]=newunit;
             square[Curr.row][Curr.col]=unit;
             return 1;
        }
        /////////////////////////////////////////////////

    }

//*********************************************************
}





int w_total_valid_moves()
{
    int no_of_validmoves=0;
    struct COORDS Curr,New;

    for(Curr.row=0;Curr.row<8;++Curr.row)
        for(Curr.col=0;Curr.col<8;++Curr.col)

            if(isupper(square[Curr.row][Curr.col])>0)        //this function counts the no. of valid moves that every white piece can make on the board

                for(New.row=0;New.row<8;++New.row)          //two struct COORDS are declared Curr-->for current position
                    for(New.col=0;New.col<8;++New.col)                                     //New--->for new position
                                                            //then a loop is run in which we traverse the Curr from all the 64 blocks
                        if(valid_move(Curr,New))            //if a white piece is found on some block then Curr is fixed on that block and
                            ++no_of_validmoves;             //another loop is started which traverse New through all the 64 blocks
                                                            //for each value of New...then valid_move(Curr,New) is called
                                                            //if valid_move() return 1 then we increment the count


    return no_of_validmoves;
}



int b_total_valid_moves()
{
    int no_of_validmoves=0;
    struct COORDS Curr,New;

    for(Curr.row=0;Curr.row<8;++Curr.row)
        for(Curr.col=0;Curr.col<8;++Curr.col)

            if(islower(square[Curr.row][Curr.col])>0)

                for(New.row=0;New.row<8;++New.row)
                    for(New.col=0;New.col<8;++New.col)

                        if(valid_move(Curr,New))
                            ++no_of_validmoves;



    return no_of_validmoves;
}



void w_checkmate()
{
    if(wk_underattack()==1&&w_total_valid_moves()==0)           //if total valid moves for white pieces are 0 and white king is under attack then it prints "checkmate black wins!"
    {
            system("cls");
            display();
            printf("\nIt's Checkmate!!Black Wins\n");
            exit(0);
    }

}

void b_checkmate()
{
    if((bk_underattack()==1)&&(b_total_valid_moves()==0))
    {
            system("cls");
            display();
            printf("\nIt's Checkmate!!White Wins\n");
            exit(0);
    }

}

void w_check()
{
    if(wk_underattack()==1&&w_total_valid_moves()>0)    //if white king is under attack but there are valid moves left for white pieces then it prints "check"
    {   system("cls");
        display();
        printf("\nIt's Check\n");
        delay(2000);
    }
}

void b_check()
{
    if(bk_underattack()==1&&b_total_valid_moves()>0)
    {   system("cls");
        display();
        printf("\nIt's Check\n");
        delay(2000);
    }
}

void w_stalemate()
{
    if(wk_underattack()==0&&w_total_valid_moves()==0)       //if white king is not under attack but no of valid moves for white pieces is 0....then its a draw
    {
        system("cls");
        display();
        printf("\nIts Draw!!Stalemate\n");
        exit(0);
    }

}


void b_stalemate()
{
    if(bk_underattack()==0&&b_total_valid_moves()==0)
    {
        system("cls");
        display();
        printf("\nIts Draw!!Stalemate\n");
        exit(0);
    }

}




int bk_underattack2(struct COORDS Curr)       //this function checks whether the black king is under attack from a particular position or not
{                                             //that position is passed to it in a struct COORDS variable Curr
//******************************************************************************************
    int i,j,row,col;
    char unit;
    struct COORDS bkpos;

    for(i=0;i<8;++i)
        for(j=0;j<8;++j)
            if(square[i][j]=='k')
                    goto out;
    out:
    bkpos.row=i;                        //the black king position is stored in bkpos (struct COORDS variable)
    bkpos.col=j;

    unit=square[Curr.row][Curr.col];        //unit stores the piece present at the postion passed to the function
    //////////////////////////////////////////////////////////
    if(unit=='K')
    {   if(bkpos.row==Curr.row-1&&bkpos.col==Curr.col)
            return 1;
        else if(bkpos.row==Curr.row-1&&bkpos.col==Curr.col-1)
            return 1;
        else if(bkpos.row==Curr.row&&bkpos.col==Curr.col-1)
            return 1;
        else if(bkpos.row==Curr.row+1&&bkpos.col==Curr.col-1)         //if it is a white king it checks whether the black king is present at a position where a white king can move or not
            return 1;                                                   //if yes then it return 1 otherwise 0
        else if(bkpos.row==Curr.row+1&&bkpos.col==Curr.col)
            return 1;
        else if(bkpos.row==Curr.row+1&&bkpos.col==Curr.col+1)
            return 1;
        else if(bkpos.row==Curr.row&&bkpos.col==Curr.col+1)
            return 1;
        else if(bkpos.row==Curr.row-1&&bkpos.col==Curr.col+1)
            return 1;
        else
            return 0;
    }
    //////////////////////////////////////////////////////////
    else if(unit=='Q')
    {   square[Curr.row][Curr.col]='R';
        i=bk_underattack2(Curr);

        square[Curr.row][Curr.col]='B';             //we replace the queen with a rook first...and make a recursive call to the same function
        j=bk_underattack2(Curr);                    //then we replace the queen with a bishop...and make a recursive call to the same function
                                                    //if in any of those case 1 is returned then black king is under attack from the white queen
        square[Curr.row][Curr.col]='Q';
                                                    //in the end we put the queen back in its position
        return i+j;
    }
    //////////////////////////////////////////////////////////
    else if(unit=='P')
    {
        if(bkpos.row==Curr.row-1&&bkpos.col==Curr.col+1)            //in case of white pawn we simply check if the black king is one block diagonally opposite to it or not..
                return 1;
        else if(bkpos.row==Curr.row-1&&bkpos.col==Curr.col-1)
                return 1;
        else
                return 0;
    }
    //////////////////////////////////////////////////////////
    else if(unit=='R')
    {
            if(bkpos.col==Curr.col)
            {   if(bkpos.row<Curr.row)                          //first we check whether the king is in same column or row as the rook or not...if not return 0
                {   for(row=Curr.row-1;row>bkpos.row;--row)
                        if(square[row][Curr.col]=='\0')         //if they are in same column then check whether all the blocks in b/w them are empty or not..if not return 0 otherwise 1
                                continue;
                        else
                                return 0;
                    return 1;
                }
                else if(bkpos.row>Curr.row)                     //if they are in same row then check whether all blocks in b/w them are empty or not...if not return 0 otherwise 1
                {   for(row=Curr.row+1;row<bkpos.row;++row)
                        if(square[row][Curr.col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }

            }
            else if(bkpos.row==Curr.row)
            {   if(bkpos.col<Curr.col)
                {   for(col=Curr.col-1;col>bkpos.col;--col)
                        if(square[Curr.row][col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }
                else if(bkpos.col>Curr.col)
                {   for(col=Curr.col+1;col<bkpos.col;++col)
                        if(square[Curr.row][col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }

            }
            else
                return 0;

    }
    //////////////////////////////////////////////////////////////
    else if(unit=='N')
    {
        if((bkpos.row==Curr.row-2)&&(bkpos.col==Curr.col+1))
                return 1;
        else if((bkpos.row==Curr.row-2)&&(bkpos.col==Curr.col-1))
                return 1;
        else if((bkpos.row==Curr.row-1)&&(bkpos.col==Curr.col-2))
                return 1;
        else if((bkpos.row==Curr.row+1)&&(bkpos.col==Curr.col-2))  //in case of white knight...we check whether the black king is present at one of the 8 possible danger positions
                return 1;                                           //(where a knight can move) or not....if not return 0 otherwise 1
        else if((bkpos.row==Curr.row+2)&&(bkpos.col==Curr.col-1))
                return 1;
        else if((bkpos.row==Curr.row+2)&&(bkpos.col==Curr.col+1))
                return 1;
        else if((bkpos.row==Curr.row+1)&&(bkpos.col==Curr.col+2))
                return 1;
        else if((bkpos.row==Curr.row-1)&&(bkpos.col==Curr.col+2))
                return 1;
        else
                return 0;

    }
    ////////////////////////////////////////////////////////////////

    else if(unit=='B')
    {

        if(Curr.row-bkpos.row==bkpos.col-Curr.col)
        {
            if(bkpos.col>Curr.col)
            {   for(row=Curr.row-1,col=Curr.col+1;row>bkpos.row;--row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;                                       //in case of white bishop , we first check whether the black king is diagonally related to bishop or not
                return 1;                                                   //if not,return 0...if yes then we check whether all the blocks in b/w them are empty or not if not return 0 otherwise 1

            }
            else if(bkpos.col<Curr.col)
            {   for(row=Curr.row+1,col=Curr.col-1;row<bkpos.row;++row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }

        }
        else if(Curr.row-bkpos.row==Curr.col-bkpos.col)
        {
            if(bkpos.col>Curr.col)
            {   for(row=Curr.row+1,col=Curr.col+1;row<bkpos.row;++row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }
            else if(bkpos.col<Curr.col)
            {   for(row=Curr.row-1,col=Curr.col-1;row>bkpos.row;--row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }

        }
        else
            return 0;
    }
   ////////////////////////////////////////////////////////
    else
        return 0;

//******************************************************************************************
}


//########################################################################################################################
//########################################################################################################################
//########################################################################################################################





int wk_underattack2(struct COORDS Curr)
{
//******************************************************************************************
    int i,j,row,col;
    char unit;
    struct COORDS wkpos;

    for(i=0;i<8;++i)
        for(j=0;j<8;++j)
            if(square[i][j]=='K')
                    goto out2;
    out2:
    wkpos.row=i;
    wkpos.col=j;

    unit=square[Curr.row][Curr.col];
    //////////////////////////////////////////////////////////
    if(unit=='k')
    {   if(wkpos.row==Curr.row-1&&wkpos.col==Curr.col)
            return 1;
        else if(wkpos.row==Curr.row-1&&wkpos.col==Curr.col-1)
            return 1;
        else if(wkpos.row==Curr.row&&wkpos.col==Curr.col-1)
            return 1;
        else if(wkpos.row==Curr.row+1&&wkpos.col==Curr.col-1)
            return 1;
        else if(wkpos.row==Curr.row+1&&wkpos.col==Curr.col)
            return 1;
        else if(wkpos.row==Curr.row+1&&wkpos.col==Curr.col+1)
            return 1;
        else if(wkpos.row==Curr.row&&wkpos.col==Curr.col+1)
            return 1;
        else if(wkpos.row==Curr.row-1&&wkpos.col==Curr.col+1)
            return 1;
        else
            return 0;
    }
    //////////////////////////////////////////////////////////
    else if(unit=='q')
    {   square[Curr.row][Curr.col]='r';
        i=wk_underattack2(Curr);

        square[Curr.row][Curr.col]='b';
        j=wk_underattack2(Curr);

        square[Curr.row][Curr.col]='q';

        return i+j;
    }
    //////////////////////////////////////////////////////////
    else if(unit=='p')
    {
        if(wkpos.row==Curr.row+1&&wkpos.col==Curr.col+1)
                return 1;
        else if(wkpos.row==Curr.row+1&&wkpos.col==Curr.col-1)
                return 1;
        else
                return 0;
    }
    //////////////////////////////////////////////////////////
    else if(unit=='r')
    {
            if(wkpos.col==Curr.col)
            {   if(wkpos.row<Curr.row)
                {   for(row=Curr.row-1;row>wkpos.row;--row)
                        if(square[row][Curr.col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }
                else if(wkpos.row>Curr.row)
                {   for(row=Curr.row+1;row<wkpos.row;++row)
                        if(square[row][Curr.col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }

            }
            else if(wkpos.row==Curr.row)
            {   if(wkpos.col<Curr.col)
                {   for(col=Curr.col-1;col>wkpos.col;--col)
                        if(square[Curr.row][col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }
                else if(wkpos.col>Curr.col)
                {   for(col=Curr.col+1;col<wkpos.col;++col)
                        if(square[Curr.row][col]=='\0')
                                continue;
                        else
                                return 0;
                    return 1;
                }

            }
            else
                return 0;

    }
    //////////////////////////////////////////////////////////////
    else if(unit=='n')
    {
        if((wkpos.row==Curr.row-2)&&(wkpos.col==Curr.col+1))
                return 1;
        else if((wkpos.row==Curr.row-2)&&(wkpos.col==Curr.col-1))
                return 1;
        else if((wkpos.row==Curr.row-1)&&(wkpos.col==Curr.col-2))
                return 1;
        else if((wkpos.row==Curr.row+1)&&(wkpos.col==Curr.col-2))
                return 1;
        else if((wkpos.row==Curr.row+2)&&(wkpos.col==Curr.col-1))
                return 1;
        else if((wkpos.row==Curr.row+2)&&(wkpos.col==Curr.col+1))
                return 1;
        else if((wkpos.row==Curr.row+1)&&(wkpos.col==Curr.col+2))
                return 1;
        else if((wkpos.row==Curr.row-1)&&(wkpos.col==Curr.col+2))
                return 1;
        else
                return 0;

    }
    ////////////////////////////////////////////////////////////////

    else if(unit=='b')
    {

        if(Curr.row-wkpos.row==wkpos.col-Curr.col)
        {
            if(wkpos.col>Curr.col)
            {   for(row=Curr.row-1,col=Curr.col+1;row>wkpos.row;--row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }
            else if(wkpos.col<Curr.col)
            {   for(row=Curr.row+1,col=Curr.col-1;row<wkpos.row;++row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }

        }
        else if(Curr.row-wkpos.row==Curr.col-wkpos.col)
        {
            if(wkpos.col>Curr.col)
            {   for(row=Curr.row+1,col=Curr.col+1;row<wkpos.row;++row,++col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }
            else if(wkpos.col<Curr.col)
            {   for(row=Curr.row-1,col=Curr.col-1;row>wkpos.row;--row,--col)
                        if(square[row][col]=='\0')
                                continue;
                        else
                                return 0;
                return 1;

            }

        }
        else
            return 0;
    }
   ////////////////////////////////////////////////////////
    else
        return 0;

//******************************************************************************************
}



int wk_underattack()
{   struct COORDS pos;

    for(pos.row=0;pos.row<8;++pos.row)                      //wk_underattack() runs a loop traversing all the 64 blocks...for each block it calls the
                for(pos.col=0;pos.col<8;++pos.col)          //wk_underattack2()
                    if(wk_underattack2(pos))                //if wk_underattack2() return 1 for even one block then then this function returns 1
                        return 1;                           //if  wk_underattack2() return 0 for all the blocks then only this function return 0
    return 0;
}



int bk_underattack()
{   struct COORDS pos;

    for(pos.row=0;pos.row<8;++pos.row)
                for(pos.col=0;pos.col<8;++pos.col)
                    if(bk_underattack2(pos))
                        return 1;
    return 0;
}





void w_promote()
{   int i=0,j;
    char unit;                          //this function enables the promotion rule for white pawns
    for(j=0;j<8;++j)                    //it traverse the last rank and look for white pawns...if found then it asks the user for promotion
        if(square[i][j]=='P')           //and then based on the user choice it replaces that white pawn with queen or knight or rook or bishop
           while(1)
            {    printf("\nPromote your pawn to any one of these:(Q R B N) ");fflush(stdin);
                 scanf("%c",&unit);
                 switch(unit)
                 {   case 'Q': square[i][j]='Q';
                               return;
                     case 'R': square[i][j]='R';
                               return;
                     case 'B': square[i][j]='B';
                               return;
                     case 'N': square[i][j]='N';
                               return;
                     default : printf("Invalid choice Try Again!!");
                 }

            }


}




void b_promote()
{   int i=7,j;
    char unit;
    for(j=0;j<8;++j)
        if(square[i][j]=='p')
           while(1)
            {    printf("\nPromote your pawn to any one of these:(q r b n) ");fflush(stdin);
                 scanf("%c",&unit);
                 switch(unit)
                 {   case 'q': square[i][j]='q';
                               return;
                     case 'r': square[i][j]='r';
                               return;
                     case 'b': square[i][j]='b';
                               return;
                     case 'n': square[i][j]='n';
                               return;
                     default : printf("Invalid choice Try Again!!");
                 }

            }


}




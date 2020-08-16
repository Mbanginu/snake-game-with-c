#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/*Implementing kbhit()*/
        #include <stdio.h>
        #include <termios.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/time.h>

        void changemode(int dir)
        {
        static struct termios oldt, newt;

        if ( dir == 1 )
        {
            tcgetattr( STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~( ICANON | ECHO );
            tcsetattr( STDIN_FILENO, TCSANOW, &newt);
        }
        else
            tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
        }

        int kbhit (void)
        {
        struct timeval tv;
        fd_set rdfs;

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        FD_ZERO(&rdfs);
        FD_SET (STDIN_FILENO, &rdfs);

        select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
        return FD_ISSET(STDIN_FILENO, &rdfs);

        }
/*Done implementing kbhit()*/
const int gColumns = 60; //game columns
const int gRows = 25;   //game rows

char key;
int iCol=12, iRow=20; //initial column and row //where the game begins
char show[2];
int scores;
char fruit[2] = {'f'};
int rRow = 8, rCol = 6; //Random row, random column
int rVal, cVal;
char name[30];
int tail = 0;   //adding a tail to the snake
char pDirection;
int sleepingTime;


int drawFruit(int rVal, int cVal);
void drawBorders ();
void printScore(int scr);
void addingTail(int irow, int icol, int tails, char butt);
void changeDirection(char pDirection);



int main(){
    initscr();
    
    mvprintw(1, 3, "LET'S PLAY SNAKE GAME...\n");
    printw("Your gamer name: ");
    scanw("%s", name);
    clear();
    key = '\0';
    drawBorders();
    while(key != 'q'){
        key = getch();
        
        if(key== 'd' || key == KEY_RIGHT){    //d --> right
            pDirection = 'r';  
        }
        else if(key == 'a' || key == KEY_LEFT){    //a --> left
            pDirection = 'l';
        }
        else if(key == 'w' || key == KEY_UP){    //w --> up
            pDirection = 'u';
        }
        else if(key == 's' || key == KEY_DOWN){    //s --> down
            pDirection = 'd';
        }

//Checking and changing the direction
            //changemode(1);        
        while ( !kbhit() ){
            if(rRow == iRow && rCol == iCol){
                drawFruit(iRow, iCol);
                scores += 1;
            }
            if (iRow == (gRows) || iRow == 2 || iCol == (gColumns+2) || iCol == 1){
                mvprintw(gRows/2, gColumns/4, "Ooops! you loose \t press q: ");
                break;
            }
            tail = scores;   
            changeDirection(pDirection);
            clear();
            drawBorders();
            attron(A_BOLD | A_BLINK);
            mvprintw(rRow, rCol, fruit);    /**/
	        attroff(A_BOLD | A_BLINK);           
            addingTail(iRow, iCol, tail, key);  /**/
            mvprintw(iRow,iCol,show);   /**/
            
            printScore(scores);
            
            refresh();
            
            usleep(sleepingTime);
        } 
        
    }
    mvprintw((gRows+3), 3, "\n Great! '%s' score: %d \t Thank you for playing...\n",name, scores);
  
    getch();
    endwin();
    return 0;
}

/*Generate random Fruits positions*/
int drawFruit(int rVal, int cVal){  //Using row and columnn value to generate a random figure
    rCol = (rand() % ((gColumns -1) - 2 + 1)) + 2;     // rCol = rand()%30;
    rRow = (rand() % ((gRows -1) - 3 + 1)) + 3;     //rRow = rand()%30;
    // number = (rand() % (upper - lower + 1)) + lower
}

/*Change directions from here*/
    void changeDirection(char pDir){
        switch (pDir)
        {
            case 'd': {
                show[0] = 'v';
                iRow += 1;
                break;
            }
            case 'l': {
                show[0] = '<';
                iCol -= 1;
                break;
            }
            case 'u': {
                show[0] = '^';
                iRow -= 1;
                break;
            }
            case 'r': {
                show[0] = '>';
                iCol += 1;
                break;
            }
        }
    }

/*Print the score on the top right corner*/
void printScore(int scr){
    mvprintw(1, gColumns - 8, "Score: %d", scr);
}

char determinePrintingCharacter(int tValue){
    char characters[5] = {'*', '+', 'o', 'e', '@'};
    char printCharacter;
    if(tValue < 5){
        printCharacter = characters[0];
        sleepingTime = 400000;
    }
    else if(tValue >= 5 && tValue < 10){
        printCharacter = characters[1];
        sleepingTime = 250000;
    }
    else if(tValue >= 10 && tValue < 15){
        printCharacter = characters[2];
        sleepingTime = 150000;
    }
    else if(tValue >= 15 && tValue <= 20){
        printCharacter = characters[3];
        sleepingTime = 100000;
    }
    else
    {
        printCharacter = characters[4];
        sleepingTime = 50000;
    }
    return printCharacter;
}

/*Printing tail to the snake*/
void addingTail(int irow, int icol, int tails, char butt){
    char showing = determinePrintingCharacter(tails);
    if (butt == 'a'){
        while (tails)
        {
            mvprintw(irow, icol + tails, "%c", showing);
            tails--;
        } 
    }
    else if (butt == 'd')
    {
        while (tails)
        {
            mvprintw(irow, icol - tails, "%c", showing);
            tails--;
        }
    }
    else if (butt == 's')
    {
        while (tails)
        {
            mvprintw(irow - tails, icol, "%c", showing);
            tails--;
        }   
    }
    else if (butt == 'w'){
        while (tails)
        {
            mvprintw(irow + tails, icol, "%c", showing);
            tails--;
        }
    }
    else
    {
        mvprintw(irow,icol, "%c", showing);
    }
    
}

/*Draw the borders of the Game and Show tips*/
void drawBorders (){
 //   mvprintw(1,3,"Designed by ALVINO \t Gamer: %s\n", name);
    for(int i=0; i<= (gRows); i++)
    {
        printw(" #");
        for(int j=0; j<=(gColumns); j++)
        {
            if(i==0 || i == 2 || j== (gColumns) || i == gRows)
            {
                printw("#");
            }
            else{
                printw(" ");
            }
        }
        printw("\n");
    }
    
    printw("  TIP:\t");
    printw("  d -> right: \t");
    printw("a -> left \t");
    printw("w -> up \t");
    printw("s -> down \t");
    printw("q -> quit \n");
    printw("  Arrow keys may also work...  \n \t \t");
    attron(A_BOLD | A_UNDERLINE);
	printw("https://github.com/mbanginu");
	attroff(A_BOLD | A_UNDERLINE);

    mvprintw(1,3,"Designed by: ALVIN \t  Gamer: %s\n", name);
}
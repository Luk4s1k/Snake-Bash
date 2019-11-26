//
//  main.cpp
//  Змея_ебаная_плюсы
//
//  Created by Luca Mitrovich on 18/11/2019.
//  Copyright © 2019 Luca Mitrović. All rights reserved.
//

#include <stdio.h>
#include <ncurses.h>
#include <time.h> //for clock()
#include <string>
#include <fstream> //working with files
#include <unistd.h> //for usleep()
#include <math.h> //for pow()


std::string filename = "/Users/luca/Desktop/Змея_ебаная_плюсы/Змея_ебаная_плюсы/Score.txt";
int seconds = 0, minutes = 0;
bool walls;
int speed = 10;
bool gameOver;
int width = 20, height = 20;
int x, y, FruitX, FruitY, SFruitX, SFruitY, score,score_random;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;
int nTail = 0;
int TailX[2500],TailY[2500];




int getScore() { // Best Score search in file Score.txt
    std::string str;
    int temp = 0;
    int bestscore = 0;
    
    std::ifstream scorefile(filename);
    if(scorefile.is_open()){
        
        while(getline(scorefile, str)) {
            temp = (int)str[0];
            
            if(temp > bestscore)
                bestscore = temp;
        }
    }
    scorefile.close();
    
    return bestscore - 48;
}

void UIwallSettings(){ // Changes wall settings
    clear();
    mvprintw(1,20,"Do you want a snake to go throught the wall?");
    mvprintw(5,30,"[Y] Yes");
    mvprintw(5,42,"[N] No");
    char wall_command = getch();
    if(wall_command == 'y') walls = true;
    if(wall_command == 'n') walls = false;
}

void UIinstrutcions(){ // Setup game Settings
    clear();
    mvprintw(1, 30, "[F] Field Size");
    mvprintw(3, 30, "[D] Snake Speed");
    mvprintw(5, 30, "[W] Walls Settings");
    mvprintw(7, 30, "[S] Start Game");
    char prop_command = getch();
    switch (prop_command) {
        case 'f':
                 clear();
                 echo();
                 curs_set(1);
                 mvprintw(1,30,"Enter the field width (10 - 50):");
                 scanw("%d",&width);
                 mvprintw(3,30,"Enter the field height (10 - 50):");
                 scanw("%d",&height);
                 noecho();
                 curs_set(0);
                 UIinstrutcions();
                 break;
        case 'w':
            UIwallSettings();
            UIinstrutcions();
            break;
       case 'd':     //snake speed change block
                 clear();
                 echo();
                 curs_set(1);
                 mvprintw(1,30, "Enter snake speed (1 - 5):");
                 scanw("%d",&speed);
                 speed *= pow(10,speed);
                 noecho();
                 curs_set(0);
                 UIinstrutcions();
                 break;
        case 's':
                break;
        default: UIinstrutcions();
            break;
    }
    
}


    
void UImainMenu(){ // main Menu call Function
    initscr();
    mvprintw(1,30,"The Snake Game");
    mvprintw(3,33,"[S] START");
    mvprintw(5,30,"[I] INSTRUCTIONS");
    mvprintw(7,33,"BEST SCORE: %d",getScore());
    mvprintw(9,33,"[Q] QUIT");
    
    int c = getch();
    switch(c){
        case 115:                     //pressed 'S'
            break;
        case 105: UIinstrutcions();   //pressed 'I'
            break;
        case 113 : clear();           //pressed 'Q'
                   gameOver = TRUE ;
                    close(0);
            break;
        default:UImainMenu();         //if another key is pressed
            break;
        
    }
}

void randomFruitSpawn(){ //to spawn Super Fruit
    if (score_random == 0){
        score_random = rand() % 30;
        SFruitX  = (rand() % width);
        SFruitY  = (rand() % width);
      
    }
}

void Setup(){ // Start variables and window settings

    initscr(); //initiates new window
    clear(); //clears the window
    noecho(); // don't show entered keys from the keyboard
    //cbreak();
    curs_set(0); //cursor visibility
    
    gameOver = false;
    dir = STOP;
    x = width/2;
    y = height/2;
    
    
    FruitX = (rand() % width)+1;
    FruitY = (rand() % height)+1;
    score = 0;
}



void Time(){  // Converts time to "MM:SS"
    unsigned long time  = (clock()/CLOCKS_PER_SEC)/1000;
    if(time % 61 == 0){ seconds++;}
    if (seconds/10 >= 60){minutes++;seconds = 0;}
}

void Draw(){
    
    clear();
    
    for(int i = 0; i < width; i++)
        mvprintw(0,i,"#");
    
    for(int i = 0; i <= height; i++){
        for(int j = 0; j <= width; j++){
            if(i == 0 || i == height)
                mvprintw(i,j,"#");
            else if(j == 0 || j == width)
                mvprintw(i,j,"#");
            else if(i == y && j == x)
                mvprintw(i,j,"0");                       // Head Skin
            else if(i == FruitY && j == FruitX)
                mvprintw(i,j,"@");                      // Fruit Skin
            else if(i == SFruitY && j == SFruitX)
                mvprintw(i,j,"$");                      // Super Fruit Skin
            else
                for(int k = 0;k < nTail;k++)
                    if(TailX[k] == j && TailY[k] == i)
                        mvprintw(i,j,"o");              // Tail Skin
                
            
        }
      }
    
    //Time();
    mvprintw(3, width + 2, "-$- Super Fruit Amount: %d ",score_random);
    mvprintw(height + 2, 0,"Score: %d",score);
    mvprintw(height + 3, 0, "Time: %d:%d", minutes,seconds/10);
    refresh();
}
void Input(){   // Control Settings
    
    keypad(stdscr,TRUE);
    halfdelay(1);
    int c = getch();
    
    switch(c){
        case KEY_LEFT:
        case 'a':
            if (dir != RIGHT || score == 0) dir = LEFT; break;
        case KEY_RIGHT:
        case 'd':
            if (dir != LEFT || score == 0) dir = RIGHT; break;
        case KEY_UP:
        case 'w':
            if (dir != DOWN || score == 0) dir = UP; break;
        case KEY_DOWN:
        case 's':
            if (dir != UP || score == 0) dir = DOWN; break;
        }
    
}
void Logic(){
    
    int prevX = TailX[0];
    int prevY = TailY[0];
    int prev2X, prev2Y;
    TailX[0] = x;
    TailY[0] = y;
    
    for(int i = 1; i < nTail; i++){
        prev2X = TailX[i];
        prev2Y = TailY[i];
        TailX[i] = prevX;
        TailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
        
    }
    switch(dir){
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
        break;
            
    }
    switch(walls) { // through the wall mode check
        case false: {
            if(x == width ) x = 0;
            else if(x == 0) x = width - 1;
            
            if(y == height) y = 0;
            else if(y == 0) y = height - 1;
            
            break;
        }
        case true: {
            if(x == width || x == 0 || y == height || y == 0)
                gameOver = true;
            break;
        }
    }
    
    
            if(x == FruitX && y == FruitY){ // if eats fruit  - score increases ,new Fruit spawned
                score++;
                FruitX = (rand() % width)+1;
                FruitY = (rand() % height)+1;
                nTail++;
            }
            if(x == SFruitX && y == SFruitY){   //super fruit has limited lifetime and add 5 points to the score
                score += score_random;
                nTail += 4;
                //SFruitX = width + 3;   //for deleting from the game field
                //SFruitY = height + 3;  //for deleting from the game field
                score_random = 0;
            }
    
    for (int i = 1; i < nTail; i++) // eats tail - dies
        if(TailX[i] == x && TailY[i] == y){
            gameOver = true;
        }
    
    if (score_random != 0){  // score_random countdown
        score_random -= 0.0001;
    }
    
    usleep(1000000/speed);  //snake speed control
}

void writeStats() { //Saving score into the Score.txt file format "Name score MM:SS"
    char name[20];
    echo();
    curs_set(1);
    mvprintw(1,width + 2,"Write your name: ");
    scanw("%s",name);
    std::ofstream out; // создаем новый поток для записи
    out.open(filename,std::ios::app);
    if(out.is_open()){
        out << score << " " << name << " " << minutes << ":"<< seconds/10 << std::endl;
    }
    out.close();
}

int main(){
     
    Setup();
    UImainMenu();
    
    
    while(!gameOver){
        Draw();
        Input();
        Logic();
        Time();
        randomFruitSpawn();
    }
    echo();
    writeStats();
    getch();
    endwin();
    return 0;
}

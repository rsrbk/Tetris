//
//  main.cpp
//  d5-1
//
//  Created by Руслан on 30.10.14.
//  Copyright (c) 2014 Руслан. All rights reserved.
//

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <curses.h>
#include <stdio.h>

const int startM=23;
const int N=23;
const int M=27;

void gen_figure(int figure[3][3])
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            (i==2) ? figure[i][j]=1 : figure[i][j]=0;
        }
    }
    figure[1][rand()%3]=1;
}

void rot_figure(int nowI, int nowJ, int figure[3][3])
{
    int new_figure[3][3];
    if((figure[2][0]==1 && figure[2][1]==1 && figure[2][2]==1))
    {
        for(int i = 0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                new_figure[i][j] = figure[2-j][i];
            }
        }
        
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                figure[i][j] = new_figure[i][j];
            }
        }
    }
    else if(figure[0][0]==0 && figure[0][1]==0 && figure[0][2]==0)
    {
        int new_temp_figure[3][3];
        
        for(int i = 0; i<2; i++)
        {
            for(int j=0; j<3; j++)
            {
                new_temp_figure[i][j] = figure[i+1][j];
            }
        }
        
        for(int i = 0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                new_figure[i][j] = new_temp_figure[2-j][i];
            }
        }
        
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                figure[i][j] = new_figure[i][j];
            }
        }
    }
    else if(figure[0][2]==0 && figure[1][2]==0 && figure[2][2]==0)
    {
        for(int i = 0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                new_figure[i][j] = figure[2-j][i];
            }
        }
        
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                if(i==0) figure[i][j]=0;
                else
                {
                    figure[i][j] = new_figure[i-1][j];
                }
            }
        }
    }
    else
    {
        for(int i = 0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                new_figure[i][j] = figure[2-j][i];
            }
        }
        
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                figure[i][j] = new_figure[i][j];
            }
        }
    }
    
}

bool check_line_for_exist(int pole[N][M], int i, int val)
{
    for(int j=1; j<M-2; j++)
    {
        if(pole[i][j]==val) return true;
    }
    return false;
}

void delete_line(int pole[N][M])
{
    for(int i=N-2; i>0; i--)
    {
        for(int j=0; j<M; j++)
        {
            pole[i][j]=pole[i-1][j];
        }
    }
}

void print_pole(int pole[N][M])
{
    for(int i=0; i<N; i++)
    {
        for(int j=startM; j<startM+M-1; j++)
        {
            move(i, j);
            if(i==0 || j==startM || i==N-1 || j==startM+M-2)
            {
                
                attron(COLOR_PAIR(2));
                printw(" ");
            }
            else
            {
                if(pole[i-1][j-startM]==1 && i>1)
                {
                    attron(COLOR_PAIR(1));
                    printw(" ");
                }
                else
                {
                    attron(COLOR_PAIR(3));
                    printw(" ");
                }
            }
        }
    }
}

void pull_figure(int moveI, int moveJ, int figure[3][3], int color)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(figure[i][j]==1)
            {
                move(moveI+i, moveJ+j+startM);
                attron(COLOR_PAIR(color));
                printw(" ");
            }
        }
    }
}

void create_pole(int pole[N][M])
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<M; j++)
        {
            pole[i][j]=0;
        }
    }
}

void game(int figure[3][3], int pole[N][M])
{
    char s;
    int i(1), j(M/2), score(0);
    bool is_end=false;
    bool game_over=false;
    
    initscr();
    curs_set(0);
    start_color();
    
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);
    
    create_pole(pole);
    
    move(1,startM+M+1);
    attron(COLOR_PAIR(5));
    printw("a: <-, d: ->, s: rotate");
    
    while(!game_over)
    {
        move(1,5);
        attron(COLOR_PAIR(5));
        printw("Score: %d", score);
        
        if(!check_line_for_exist(pole, N-3, 0))
        {
            delete_line(pole);
            score++;
        }
        else if(check_line_for_exist(pole, 2, 1))
        {
            move(3,5);
            attron(COLOR_PAIR(5));
            printw("Game over :(");
            move(0,0);
            attron(COLOR_PAIR(4));
            timeout(10000);
            char s=getch();
            if(s=='r')
            {
                move(3,5);
                attron(COLOR_PAIR(4));
                printw("            ");
                game(figure, pole);
                
            }
        }
        
        print_pole(pole);
        
        if(i>1) pull_figure(i-1, j, figure, 3);
        if(i==1) gen_figure(figure);
        
        attron(COLOR_PAIR(4));
        move(0,0);
        
        timeout(0);
        s=getch();
        if(s=='a' && j>1 && pole[i-1][j-3]!=1) j--;
        else if(s=='d' && j<M-5 && pole[i-1][j+3]!=1) j++;
        else if(s=='s') rot_figure(i, j, figure);
        else if(s=='q') exit(1);
        else if(s=='r') game(figure, pole);
        
        pull_figure(i, j, figure, 1);
        
        if((pole[i+2][j]==1 && figure[2][0]==1) || (pole[i+2][j+1]==1 && figure[2][1]==1) || (pole[i+2][j+2]==1 && figure[2][2]==1) || i==N-4)
            is_end=true;
        
        if(is_end)
        {
            for(int q=0; q<3; q++)
            {
                for(int w=0; w<3; w++)
                {
                    if(figure[q][w]==1){
                        pole[i-1+q][j+w]=1;
                    }
                }
            }
            is_end=false;
            i=1;
        }
        else i++;
        
        refresh();
        usleep(100000);
    }
}

int main(int argc,char *argv[])
{
    initscr();
    curs_set(0);
    start_color();
    
    int pole[N][M];
    int figure[3][3];
    game(figure, pole);
    
    getch();
    
}
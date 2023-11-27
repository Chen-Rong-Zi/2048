# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <time.h>
# include <ncurses.h>
# include "colors.h"

int print_map(int *map){
    for ( int i = 0; i < 16; ++i )
    {
        printf("%d ", map[i]);
        if ( (i + 1) % 4 == 0 ) printf("\n");
    }
    return 0;
}

int draw(int map[][4], int row, int column)
{
    for ( int i = 0; i < row; ++i )
    {
        for ( int j = 0; j < column; ++j )
        {
            attron(corr_color(map[i][j]));  // 启用颜色对
            for ( int k = i * 3; k < i * 3 + 3; ++k )
            {
                for ( int l = j * 6; l < j * 6 + 6; ++l )
                {
                    mvprintw(k, l, " ");  // 在指定位置打印字符串
                }
            }
            char number[10] = {};
            sprintf(number, "%d", map[i][j]);
            mvprintw(i * 3 + 1, j * 6 + 2, number);  // 在指定位置打印字符串
            attroff(corr_color(map[i][j]));  // 关闭颜色对
        }
    }
}

int init(){
    srand(time(NULL));
    initscr();              // start curses mode
    raw();                  // disable line buffering
    color_init();
    curs_set(0);            // make cursor invisiable
    cbreak();               // no idea how to use this!
    keypad(stdscr, TRUE);   // enable arrow keys and f1, f2, etc.
    noecho();               // Don't echo when we do getch
}

int game_map[4][4] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int move_down(int start)
{
    if ( start >= 3 )
        return 0;

    move_down(start + 1);
    for ( int i = 0; i < 4; ++i )
    {
        for ( int j = start; j < 3; ++j)
        {
            if ( game_map[j][i] == 0 )
                break;
            if ( game_map[j][i] == game_map[j+1][i] )
            {
                game_map[j+1][i] *= 2;
                game_map[j][i] = 0;
                break;
            }
            else if ( game_map[j+1][i] == 0 )
            {
                game_map[j+1][i] = game_map[j][i];
                game_map[j][i] = 0;
            }
        }
    }
    return 0;
}

int move_up(int start)
{
    if ( start <= 0 )
        return 0;

    move_up(start - 1);
    for ( int i = 0; i < 4; ++i )
    {
        for ( int j = start; j > 0; --j)
        {
            if ( game_map[j][i] == 0 )
                break;
            if ( game_map[j][i] == game_map[j-1][i] )
            {
                game_map[j-1][i] *= 2;
                game_map[j][i] = 0;
                break;
            }
            else if ( game_map[j-1][i] == 0 )
            {
                game_map[j-1][i] = game_map[j][i];
                game_map[j][i] = 0;
            }
        }
    }
    return 0;
}


int move_left(int start)
{
    if ( start <= 0 )
        return 0;

    move_left(start - 1);
    for ( int i = 0; i < 4; ++i )
    {
        for ( int j = start; j > 0; --j)
        {
            if ( game_map[i][j] == 0 )
                break;
            if ( game_map[i][j] == game_map[i][j-1] )
            {
                game_map[i][j-1] *= 2;
                game_map[i][j] = 0;
                break;
            }
            else if ( game_map[i][j-1] == 0 )
            {
                game_map[i][j-1] = game_map[i][j];
                game_map[i][j] = 0;
            }
        }
    }
    return 0;
}

int move_right(int start)
{
    if ( start >= 3 )
        return 0;

    move_right(start + 1);
    for ( int i = 0; i < 4; ++i )
    {
        for ( int j = start; j < 3; ++j)
        {
            if ( game_map[i][j] == 0 )
                break;
            if ( game_map[i][j] == game_map[i][j+1] )
            {
                game_map[i][j+1] *= 2;
                game_map[i][j] = 0;
                break;
            }
            else if ( game_map[i][j+1] == 0 )
            {
                game_map[i][j+1] = game_map[i][j];
                game_map[i][j] = 0;
            }
        }
    }
    return 0;
}

int random_new(int map[][4]){
    int i = 0, j = 0, times = 1, load = 0;
    while ( times != 0 && load != 15 )
    {
        i = rand() % 4;
        j = rand() % 4;
        if ( map[i][j] == 0 )
        {
            times -= 1;
            load = 0;
            map[i][j] = 2;
        }
        load += 1;
    }
}

int main() {
    char ch = 0;
    init();
    draw(game_map, 4, 4);
    while ( true )
    {
        refresh();  // 刷新屏幕
        if ( ch == 'q' ){
            char comfirm[20] = {};
            mvprintw(4 * 3, 0, "Do you want to quit? (y/n)");
            comfirm[0] = getch();
//             if ( (strcmp(comfirm, "y") == 0) || (strcmp(comfirm, "yes") == 0) )
//                 break;
            if ( comfirm[0] == 'y' )
                break;
        }
        if ( ch == 'j' ){
            move_down(0);
            random_new(game_map);
        }
        else if ( ch == 'k' ){
            move_up(3);
            random_new(game_map);
        }
        else if ( ch == 'l' ){
            move_right(0);
            random_new(game_map);
        }
        else if ( ch == 'h' ){
            move_left(3);
            random_new(game_map);
        }
        clear();
        draw(game_map, 4, 4);
        refresh();  // 刷新屏幕
        ch = getch();
    }
    endwin();  // 结束ncurses
    return 0;
}


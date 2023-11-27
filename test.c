# include <stdlib.h>
# include <ncurses.h>
# include "colors.h"
int draw(int map[][4], int row, int column)
{
    for ( int i = 0; i < row; ++i )
    {
        for ( int j = 0; j < column; ++j )
        {
            attron(COLOR_PAIR(1));  // 启用颜色对
            for ( int k = i * 3; k < i * 3 + 3; ++k )
            {
                for ( int l = j * 3; l < j * 3 + 3; ++l )
                {
                    mvprintw(k, l, " ");  // 在指定位置打印字符串
                }
            }
            char number[4] = {};
            sprintf(number, "%d", map[i][j]);
            mvprintw(i * 3 + 1, j * 3 + 1, number);  // 在指定位置打印字符串
            attroff(COLOR_PAIR(1));  // 关闭颜色对
            move(0, 0);
        }
    }
}

int initialize(){
    initscr();              // start curses mode
    raw();                  // disable line buffering
    start_color();          // 启用颜色
    curs_set(0);
    cbreak();               // no idea how to use this!
    keypad(stdscr, TRUE);   // enable arrow keys and f1, f2, etc.
//     noecho();               // Don't echo when we do getch
    init_pair(1, COLOR_BLACK, COLOR_GREEN);  // 创建颜色对，第一个参数是颜色对的编号
}

int main() {
    int map[4][4] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    draw(map, 4, 4);
    refresh();  // 刷新屏幕
    getch();  // 等待用户按键
    endwin();  // 结束ncurses

    return 0;
}


#include <ncurses.h>

int main() {
    initscr();  // 初始化 ncurses

    if (has_colors()) {
        start_color();  // 启用颜色

        // 打印终端支持的颜色对数量
        printw("numbers: %d\n", COLOR_PAIRS);
    } else {
        printw("not a chance\n");
    }

    refresh();  // 刷新屏幕
    getch();    // 等待用户按键

    endwin();   // 结束 ncurses

    return 0;
}


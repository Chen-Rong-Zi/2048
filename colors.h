# define COLOR_0    COLOR_PAIR(10)
# define COLOR_2    COLOR_PAIR(1)
# define COLOR_4    COLOR_PAIR(2)
# define COLOR_8    COLOR_PAIR(3)
# define COLOR_16   COLOR_PAIR(4)
# define COLOR_32   COLOR_PAIR(5)
# define COLOR_64   COLOR_PAIR(6)
# define COLOR_128  COLOR_PAIR(7)
# define COLOR_256  COLOR_PAIR(8)
# define COLOR_512  COLOR_PAIR(9)
# define COLOR_1024 COLOR_PAIR(10)

int corr_color(int n){
    if ( n == 0 )
        return COLOR_0;
    else if ( n == 2 )
        return COLOR_2;
    else if ( n == 4 )
        return COLOR_4;
    else if ( n == 8 )
        return COLOR_8;
    else if ( n == 16 )
        return COLOR_16;
    else if ( n == 32 )
        return COLOR_32;
    else if ( n == 64 )
        return COLOR_64;
    else if ( n == 128 )
        return COLOR_128;
    else if ( n == 256 )
        return COLOR_256;
    else if ( n == 512 )
        return COLOR_512;
    else if ( n == 1024 )
        return COLOR_1024;
    return 0;
}


int color_init(){
    start_color();          // 启用颜色
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);
    init_pair(6, COLOR_BLACK, COLOR_RED);
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
    init_pair(9, COLOR_BLACK, COLOR_RED);
    init_pair(10, COLOR_BLACK, COLOR_WHITE);
}

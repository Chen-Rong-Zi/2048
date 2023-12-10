# include "config.h"

int max_row = MAX_ROW;
int max_col = MAX_COL;
int startx = 0;
int starty = 0;
int unit_height = 3;
int unit_width = 7;
int map_width = 0;
int map_height = 0;
int extra_row = 3;


int draw_ui(void) {
    char *title0  = {" ▄▄▄▄▄▄▄▄▄▄▄   ▄▄▄▄▄▄▄▄▄   ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄ \n"};
    char *title1  = {"▐░░░░░░░░░░░▌ ▐░░░░░░░░░▌ ▐░▌       ▐░▌▐░░░░░░░░░░░▌\n"};
    char *title2  = {" ▀▀▀▀▀▀▀▀▀█░▌▐░█░█▀▀▀▀▀█░▌▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌\n"};
    char *title3  = {"          ▐░▌▐░▌▐░▌    ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌\n"};
    char *title4  = {"          ▐░▌▐░▌ ▐░▌   ▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌\n"};
    char *title5  = {" ▄▄▄▄▄▄▄▄▄█░▌▐░▌  ▐░▌  ▐░▌▐░░░░░░░░░░░▌ ▐░░░░░░░░░▌ \n"};
    char *title6  = {"▐░░░░░░░░░░░▌▐░▌   ▐░▌ ▐░▌ ▀▀▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌\n"};
    char *title7  = {"▐░█▀▀▀▀▀▀▀▀▀ ▐░▌    ▐░▌▐░▌          ▐░▌▐░▌       ▐░▌\n"};
    char *title8  = {"▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄█░█░▌          ▐░▌▐░█▄▄▄▄▄▄▄█░▌\n"};
    char *title9  = {"▐░░░░░░░░░░░▌ ▐░░░░░░░░░▌           ▐░▌▐░░░░░░░░░░░▌\n"};
    char *title10 = {" ▀▀▀▀▀▀▀▀▀▀▀   ▀▀▀▀▀▀▀▀▀             ▀  ▀▀▀▀▀▀▀▀▀▀▀ \n"};
    char *title[] = {title0, title1, title2, title3, title4, title5, title6, title7, title8, title9, title10};
    for ( int i = 0; i < 11; ++i ) {
        /*  mvprintw(int y, int x, const *char)  */
        mvprintw(starty - 11 - extra_row + i, startx + map_width / 2 - (53 / 2), title[i]);
    }

    char vertical[5] = "│";
    char horizontal[5] = "─";
    char left_down_cornor[5] = "└";
    char left_up_cornor[5] = "┌";
    char right_down_cornor[5] = "└";
    char right_up_cornor[5] = "┘";
    for ( int i = 0; i < map_width; ++i )
    {
        mvprintw(starty-1, startx + i, horizontal);
        mvprintw(starty + map_height, startx + i, horizontal);
    }
    for ( int j = 0; j < map_height; ++j )
    {
        mvprintw(starty + j, startx - 1, vertical);
        mvprintw(starty + j, startx + map_width, vertical);
    }
    mvprintw(starty - 1, startx - 1, left_up_cornor);
    mvprintw(starty - 1, startx + map_width, right_up_cornor);
    mvprintw(starty + map_height, startx - 1, left_down_cornor);
    mvprintw(starty + map_height, startx + map_width, right_down_cornor);

    refresh();
    return 0;
}

int restart(int *map, int row, int col) {
    /*  reseponse when 'r' was pressed or lose game  */
    attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_16);
    mvprintw(starty - 2, startx, "Do you want to restart? (Y/N)");
    attroff(COLOR_16);
    attroff(A_BOLD | A_UNDERLINE);
    refresh();

    int ch = getch(), flag = 0;
    while ( true )
    {
        if ( ch == 'y' || ch == 'Y' ){
            memset(map, 0, row * col * sizeof(map[0]));
            random_new(map, row, col);
            flag = 1;
            break;
        }
        else if ( ch == 'n' || ch == 'N' ){
            break;
        }
        else
            ch = getch();
    }
    /*  override prompt  */
    mvprintw(starty - 2, startx, "                             ");
    return flag;
}

int try_finish(int *map, int row, int col) {
    /*  response after every move  */
    int a1, a2, a3, a4, *map_copy = (int *) calloc(10000, sizeof(int));

    memcpy(map_copy, map, row * col * 4);
    if ( (a1 = move_down(map_copy, 0, row, col)) || (a2 = move_up(map_copy, row - 1, row, col)) || (a3 = move_left(map_copy, col - 1, row, col)) || (a4 = move_right(map_copy, 0, row, col)) ){
        return 0;
    }
    free(map_copy);
    mvprintw(starty - 3, startx, "you fail!");
    if ( restart(map, row, col) ){
        /*  override prompt  */
        mvprintw(starty - 3, startx, "         ");
        return 0;
    }
    else
    {
        quit_without_prompt();
        refresh();
        clear();
        endwin();
        exit(0);
    }
    return 1;
}

int read_backup(int **pmap, char *file_name, int *prow, int *pcol) {
    /*  read the file_name(has to be a json file) content if there exists  */
    FILE *fp = fopen(file_name, "r");
    char buffer[FILESIZE] = {};

    /*  case0 there is no such file  */
    if ( fp == NULL ){
        random_new(*pmap, *prow, *pcol);
        return 0;
    }

    fread(buffer, FILESIZE, 1, fp);
    fclose(fp);

    /*  case1 the file is not empty and we read it!  */
    free(*pmap);
    *pmap = json_to_map(file_name, prow, pcol);
    adjust_window(*prow, *pcol);

    return (*pmap != NULL)? 0:-1;
}

void init_ncurses(void) {
    /*  init ncurses  */
    setlocale(LC_ALL, "");
    initscr();              // start curses mode
    raw();                  // disable line buffering
    color_init();
    curs_set(0);            // make cursor invisiable
    keypad(stdscr, TRUE);   // enable arrow keys and f1, f2, etc.
    noecho();               // Don't echo when we do getch
    srand(time(NULL));
//     cbreak();               // able to be interrupted
}

void adjust_window(int row, int col) {
    /*  for singal unit  */
//     unit_height = 3;
//     unit_width = 7;
// 
    map_width = 7 * col;
    map_height = 3 * row;
//     extra_row = 3;

    /*  for all map  */
    int idealy = max_row / 2 - row / 2 * unit_height, idealx = max_col / 2 - col / 2 * unit_width;
    startx = (idealx > 4)? idealx:4;
    starty = (idealy > 11 + extra_row)? idealy:11 + extra_row;
    if ( DEBUG ){
        char cmd[100] = {};
        sprintf(cmd, "echo $(date) : startx = %d, starty = %d  >> /home/rongzi/test", startx, starty);
        system(cmd);
    }
}

void init(int **pgame_map, char *file_name, int *prow, int *pcol) {
    /*  init  */
    init_ncurses();
    getmaxyx(stdscr, max_row, max_col);
    adjust_window(*prow, *pcol);
    read_backup(pgame_map, file_name, prow, pcol);
    draw_ui();
}

int quit_without_prompt(void) {
    clear();
    endwin();
    fprintf(stderr, "quit game, no backup!\n");
    exit(0);
}

int make_backup(int *game_map, int row, int col) {
    char comfirm[20] = {};
    attron(A_BOLD | COLOR_2);
    mvprintw(starty - 2, startx, "Do you want to make backup? (y/n)");
    attroff(A_BOLD | COLOR_2);
    comfirm[0] = getch();
    /*  override prompt  */
    mvprintw(starty - 2, startx, "                                 ");
    if ( comfirm[0] == 'y' ){
        map_to_json(game_map, row, col);
        return 1;
    }
    return 0;
}

int quit(int *game_map, int row, int col) {
    /*  response function when 'q' was pressed  */
    char comfirm[20] = {};
    attron(A_BOLD | A_BLINK | COLOR_PAIR(12));
    mvprintw(starty - 2, startx, "Do you want to quit? (y/n)");
    attroff(A_BOLD | COLOR_PAIR(12));
    comfirm[0] = getch();
    /*  override prompt  */
    mvprintw(starty - 2, startx, "                          ");
    if ( comfirm[0] == 'y' )
    {
        int flag = make_backup(game_map, row, col);
        endwin();
        if ( flag )
            printf("quit game and backup\n");
        else
            printf("quit game! no backup!\n");
        exit(0);
    }
    return 0;
}

int arg_parse(char **arg_list, int arg_number, char (*pfile_name)[100], int *prow, int *pcol) {
    /*  parse -f -n flags  */
    int nflag = 0, fflag = 0;
    for ( int i = 1; i < arg_number - 1; ++i )
    {
        if ( nflag == 0 && strcmp(arg_list[i], "-n") == 0  ){
            int j = 0, xflag = 0;
            *pcol = *prow = 0;
            for ( j = 0; arg_list[i+1][j] != '\0'; ++j )
            {
                if ( arg_list[i+1][j] == 'x' )
                    xflag = 1;
                else if ( xflag == 0  )
                    *prow = (*prow) * 10 + arg_list[i+1][j] - 48;
                else if ( xflag == 1 )
                    *pcol = (*pcol) * 10 + arg_list[i+1][j] - 48;
            }
            *pcol = (*pcol == 0)? *prow:*pcol;

            if ( *prow * *pcol >= 10000 ){
                fprintf(stderr, "size too bjg!\n");
                exit(1);
            }
            else if ( *prow * *pcol <= 0 ){
                fprintf(stderr, "size too small!\n");
                exit(1);
            }
            nflag = 1;
        }

        else if ( fflag == 0 && strcmp(arg_list[i], "-f") == 0 ){
            /*  case0 not a json and we quit!  */
            if ( !is_json(arg_list[i + 1]) ){
                fprintf(stderr, "there is no such file!\n");
                exit(1);
            }

            /*  case1 is a json and we continue  */
            strcpy(*pfile_name, arg_list[i+1]);
            fflag = 1;
            return 0;
        }
    }

    /*  no flag setted! We read default backup last game we restore */
    if ( nflag == 0 && fflag == 0 ){
        strcpy(*pfile_name, "/home/rongzi/.Lectures/hw/2048/data.json");
        return 0;
    }
    /*  user defined the size and we do nothing  */
    else if ( nflag == 1 )
        strcpy(*pfile_name, "");
    return 0;
}

void update(int *game_map, int *prow, int *pcol) {
    static int last_max_row = 0, last_max_col = 0;
    getmaxyx(stdscr, max_row, max_col);
    if ( max_row != last_max_row || max_col != last_max_col ){
        if ( DEBUG ) system("echo $(date) updating >> ~/test");
        clear();
        adjust_window(*prow, *pcol);
        draw_ui();
        draw(game_map, *prow, *pcol, true);
        last_max_row = max_row;
        last_max_col = max_col;
    }
}

void play(int *game_map, int row, int col) {
    /*  this is where the game really start  */
    int flag = 1;
    char ch = 0;

    while ( true )
    {
        if ( ch == 'q' ){
            quit(game_map, row, col);
        }
        else if ( ch == 'b' ){
            make_backup(game_map, row, col);
        }
        else if ( ch == 'r' ){
            restart(game_map, row, col);
        }
        else if ( ch == 'j' ){
            if ( move_down(game_map, 0, row, col) )
                flag = random_new(game_map, row, col);
        }
        else if ( ch == 'k' ){
            if ( move_up(game_map, row - 1, row, col) )
                flag = random_new(game_map, row, col);
        }
        else if ( ch == 'l' ){
            if ( move_right(game_map, 0, row, col) )
                flag = random_new(game_map, row, col);
        }
        else if ( ch == 'h' ){
            if ( move_left(game_map, col - 1, row, col) )
                flag = random_new(game_map, row, col);
        }

        if ( flag == 0 ){
            try_finish(game_map, row, col);
        }
        update(game_map, &row, &col);
        draw(game_map, row, col, false);
        refresh();
        flag = 0;
        ch = getch();
    }
}

int main(int arg_number, char **arg_value) {
    /*  basic variables  */
    int row = 4, col = 4, *game_map = (int *) calloc(MAPSIZE, sizeof(int));
    char file_name[100] = {};

    /*  here we start!  */
    arg_parse(arg_value, arg_number, &file_name, &row, &col);
    init(&game_map, file_name, &row, &col);
    play(game_map, row, col);
    endwin();
    return 0;
}


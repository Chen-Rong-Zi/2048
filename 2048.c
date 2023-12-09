# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <ncurses.h>
# include <locale.h>
# include "config.h"

int max_row;
int max_col;
int startx;
int starty;
int height;
int width;
int quit(int *, int, int);
void init(int **, char *, int *, int *);
void adjust_window(int, int);


int restart(int *map, int row, int col) {
    /*  reseponse when 'r' was pressed or lose game  */
    attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_16);
    mvprintw(2, 0, "Do you want to restart? (Y/N)");
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
        else if ( ch == 'n' || ch == 'N' )
            break;
        else
            ch = getch();
    }
    clear();
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
    mvprintw(0, 0, "you fail!");
    if ( restart(map, row, col) ){
        refresh();
        return 0;
    }
    else
    {
        refresh();
        clear();
        endwin();
        exit(0);
    }
    return 1;
}

int read_backup(int **pmap, char *file_name, int *prow, int *pcol) {
    /*  case0 user clamed a new game and we pass */
    if ( *prow != 4 )
        return 0;

    /*  read the file_name(has to be a json file) content if there is  */
    FILE *fp = fopen(file_name, "r");
    char buffer[10240] = {};

    /*  case1 no filename passed in or wrong file was assigned and we try to read the backup last game we store */
    if ( fp == NULL ) {
        /*  assuming that there is a data.json  */
        read_backup(pmap, "/home/rongzi/.Lectures/hw/2048/data.json", prow, pcol);
        return 0;
    }

    fread(buffer, 10240, 1, fp);
    fclose(fp);
    /*  case2 there is no such file and we by default do regular start  */
    if ( strlen(buffer) <= 1 ) return 0;

    /*  case3 the filename exists and we read it!  */
    free(*pmap);
    adjust_window(*prow, *pcol);
    *pmap = json_to_map(file_name, prow, pcol);

    return (*pmap != NULL)? 0:-1;
}

void init_ncurses(void) {
    /*  init ncurses  */
    initscr();              // start curses mode
    raw();                  // disable line buffering
    color_init();
    curs_set(0);            // make cursor invisiable
    setlocale(LC_ALL, "");
    keypad(stdscr, TRUE);   // enable arrow keys and f1, f2, etc.
    noecho();               // Don't echo when we do getch
    srand(time(NULL));
//     cbreak();               // able to be interrupted
}

void adjust_window(int row, int col) {
    /*  for singal unit  */
    height = 3;
    width = 7;

    /*  for all map  */
    int idealx = max_row / 2 - row / 2 * height, idealy = max_col / 2 - col / 2 * width;
    startx = (idealx > 0)? idealx:4;
    starty = (idealy > 0)? idealy:10;
}

void init(int **pgame_map, char *file_name, int *prow, int *pcol) {
    /*  init  */
    init_ncurses();
    getmaxyx(stdscr, max_row, max_col);

    read_backup(pgame_map, file_name, prow, pcol);
    adjust_window(*prow, *pcol);
}

int quit_without_prompt(void) {
    clear();
    endwin();
    fprintf(stderr, "quit game, no backup!\n");
    exit(0);
}

int make_backup(int *game_map, int row, int col) {
    char comfirm[20] = {};
    mvprintw(2, 0, "Do you want to make backup? (y/n)");
    comfirm[0] = getch();
    clear();
    if ( comfirm[0] == 'y' ){
        map_to_json(game_map, row, col);
        return 1;
    }
    return 0;
}

int quit(int *game_map, int row, int col) {
    /*  response function when 'q' was pressed  */
    char comfirm[20] = {};
    mvprintw(2, 0, "Do you want to quit? (y/n)");
    comfirm[0] = getch();
    clear();
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

int arg_parse(char **arg_list, int arg_number, char (*pfile_name)[50], int *prow, int *pcol) {
    /*  parse -f -n flags  */
    int nflag = 0, fflag = 0;
    for ( int i = 1; i < arg_number - 1; ++i )
    {
        if ( nflag == 0 && strcmp(arg_list[i], "-n") == 0  ){
            *prow = *pcol = atoi(arg_list[i+1]);
            if ( *prow >= 101 ){
                printf("size too big!\n");
                exit(1);
            }
            else if ( *prow <= 0 ){
                printf("size too small!\n");
                exit(1);
            }
            nflag = 1;
        }

        else if ( fflag == 0 && strcmp(arg_list[i], "-f") == 0 ){
            strcpy(*pfile_name, arg_list[i+1]);
            fflag = 1;
            return 0;
        }
    }
    return 0;
}

void play(int *game_map, int row, int col) {
    /*  this where the game really start  */
    int flag = 0;
    char ch = 0;

    draw(game_map, row, col);
    random_new(game_map, row, col);
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
        draw(game_map, row, col);
        refresh();
        flag = 0;
        ch = getch();
    }
}

int main(int arg_number, char **arg_value) {
    /*  basic variables  */
    int row = 4, col = 4, *game_map = (int *) calloc(10000, sizeof(int));
    char file_name[50] = {};

    /*  here we start!  */
    arg_parse(arg_value, arg_number, &file_name, &row, &col);
    init(&game_map, file_name, &row, &col);
    play(game_map, row, col);
    endwin();
    return 0;
}


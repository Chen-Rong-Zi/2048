# ifndef CONFIG
# define CONFIG
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <assert.h>
# include <math.h>
# include <time.h>
# include <regex.h>
# include <ncurses.h>
# include <json-c/json.h>
# include <locale.h>

# define DEBUG 0
# define MAX_ROW 100
# define MAX_COL 100
/*  for singal unit  */
# define UNIT_HEIGHT 3
# define UNIT_WIDTH 7
# define STARTX 0       /*  x where title draw  */
# define STARTY 0       /*  x where title start  */
# define EXTRA_ROW 3    /*  where prompt start  */
# define FILESIZE 50000
# define MAPSIZE 10000

# define COLOR_0    COLOR_PAIR(11)
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
# define COLORS 1

extern int max_row;
extern int max_col;
extern int startx;
extern int starty;
extern int unit_height;
extern int unit_width;
extern int map_width;
extern int map_height;
extern int extra_row;
extern long times;

/*  functions.c  */
int random_new(int *, int, int);
int move_up(int *map, int start, int row, int col);
int move_down(int *map, int start, int row, int col);
int move_left(int *map, int start, int row, int col);
int move_right(int *map, int start, int row, int col);
int *json_to_map(char *file_name, int *row, int *col);
int map_to_json(int *array, int row, int col);
void draw(int *map, int row, int col, bool force);
bool is_json(char *);

/*  2048.c  */
int quit(int *, int, int);
int quit_without_prompt(void);
void init(int **, char *, int *, int *);
void adjust_window(int, int);

/*  for color init  */
void color_init();
int corr_color(int n);
# endif

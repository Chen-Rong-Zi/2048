# include <time.h>
# include <math.h>
# include <string.h>
# include <signal.h>
# include <ncurses.h>
# include <json-c/json.h>
# include "config.h"

int move_down(int *map, int start, int row, int col) {
    /*  move down */
    if ( start >= row - 1 )
        return 0;

    int flag = move_down(map, start + 1, row, col);
    for ( int i = 0; i < col; ++i )
    {
        for ( int j = start; j < row - 1; ++j)
        {
            if ( map[(j)*col+(i)] == 0 )
                break;
            if ( map[(j)*col+(i)] == map[(j+1)*col+(i)] )
            {
                map[(j+1)*col+(i)] *= 2;
                map[(j)*col+(i)] = 0;
                flag = 1;
                break;
            }
            else if ( map[(j+1)*col+(i)] == 0 )
            {
                map[(j+1)*col+(i)] = map[(j)*col+(i)];
                map[(j)*col+(i)] = 0;
                flag = 1;
            }
        }
    }
    return flag;
}

int move_up(int *map, int start, int row, int col) {
    /*  move up  */
    if ( start <= 0 )
        return 0;

    int flag = move_up(map, start - 1, row, col);;
    for ( int i = 0; i < col; ++i )
    {
        for ( int j = start; j > 0; --j)
        {
            if ( map[(j)*col+(i)] == 0 )
                break;
            if ( map[(j)*col+(i)] == map[(j-1)*col+(i)] )
            {
                map[(j-1)*col+(i)] *= 2;
                map[(j)*col+(i)] = 0;
                flag = 1;
                break;
            }
            else if ( map[(j-1)*col+(i)] == 0 )
            {
                map[(j-1)*col+(i)] = map[(j)*col+(i)];
                map[(j)*col+(i)] = 0;
                flag = 1;
            }
        }
    }
    return flag;
}

int move_left(int *map, int start, int row, int col) {
    /*  move left  */
    if ( start <= 0 )
        return 0;

    int flag = move_left(map, start - 1, row, col);
    for ( int i = 0; i < row; ++i )
    {
        for ( int j = start; j > 0; --j)
        {
            if ( map[(i)*col+(j)] == 0 )
                break;
            if ( map[(i)*col+(j)] == map[(i)*col+(j-1)] )
            {
                map[(i)*col+(j-1)] *= 2;
                map[(i)*col+(j)] = 0;
                flag = 1;
                break;
            }
            else if ( map[(i)*col+(j-1)] == 0 )
            {
                map[(i)*col+(j-1)] = map[(i)*col+(j)];
                map[(i)*col+(j)] = 0;
                flag = 1;
            }
        }
    }
    return flag;
}

int move_right(int *map, int start, int row, int col) {
    /*  move right  */
    if ( start >= col )
        return 0;

    int flag = move_right(map, start + 1, row, col);
    for ( int i = 0; i < row; ++i )
    {
        for ( int j = start; j < col - 1; ++j)
        {
            if ( map[i*col+j] == 0 )
                break;
            if ( map[i*col+j] == map[i*col+j+1] )
            {
                map[i*col+j+1] *= 2;
                map[i*col+j] = 0;
                flag = 1;
                break;
            }
            else if ( map[i*col+j+1] == 0 )
            {
                map[i*col+j+1] = map[i*col+j];
                map[i*col+j] = 0;
                flag = 1;
            }
        }
    }
    return flag;
}

int random_new(int *map, int row, int col) {
    /*  create new units after a move or at just begin  */
    int i = 0, j = 0, times = ((int)pow(sqrt(row * col), 2/3)), load = 0, flag = 0;

//     times = rand() % (row * 3);
    times = (rand() % row) | 1;
    while ( times != 0 && load != row * col )
    {
        i = rand() % row;
        j = rand() % col;
        if ( map[(i)*col+(j)] == 0 )
        {
            times -= 1;
            flag = 1;
            load = 0;
            map[(i)*col+(j)] = 2;
        }
        load += 1;
    }
    return flag;
}

void draw(int *map, int row, int col) {
    /* fulfill the screen according to map */
    for ( int i = 0; i < row; ++i )
    {
        for ( int j = 0; j < col; ++j )
        {
            attron(corr_color(map[i*col+j]));  // 启用颜色对
            for ( int k = i * 3; k < i * 3 + 3; ++k )
            {
                for ( int l = j * 7; l < j * 7 + 7; ++l )
                {
                    mvprintw(startx + k, starty + l, " ");  // 在指定位置打印字符串
                }
            }
            int posx = startx + i * height + height * 1/2;
            int posy = starty + j * width + width * 1/2;
            if ( map[i*col+j] == 0 )
                mvprintw(posx, posy, "+");     // 在指定位置打印字符串
            else
            {
                char number[10] = {0};
                sprintf(number, "%d", map[i*col+j]);
                if ( map[i*col+j] > 1000 )
                    mvprintw(posx, posy - 2, number);     // 在指定位置打印字符串
                else if ( map[i*col+j] > 10 )
                    mvprintw(posx, posy - 1, number);     // 在指定位置打印字符串
                else
                    mvprintw(posx, posy, number);     // 在指定位置打印字符串
            }
            attroff(corr_color(map[i*col+j]));          // 关闭颜色对
        }
    }
}

int* json_to_map(char *filename, int *row, int *col) {
    /* read json file */
    char file_name[100];
    sprintf(file_name, "%s", filename);
    FILE *fp = fopen(file_name, "r");
    char buffer[20480] = {};
    if ( fp == NULL ){
        perror("Error! Fail to open data.json");
        return 0;
    }
    fread(buffer, 10240, 1, fp);
    fclose(fp);

    struct json_object *parsed_json = json_tokener_parse(buffer);
    struct json_object *outter_array = NULL;
    json_object_object_get_ex(parsed_json, "array", &outter_array);

    /* parse json and save to array */
    int rows = json_object_array_length(outter_array);
    int *array = (int *) calloc(rows * rows, sizeof(int));
    *row = rows;
    for ( int r = 0; r < rows; ++r )
    {
        struct json_object *inner_array = json_object_array_get_idx(outter_array, r);
        int columns = json_object_array_length(inner_array);
        *col = columns;
        for ( int c = 0; c < columns; ++c ) {
            struct json_object *element = json_object_array_get_idx(inner_array, c);
            array[r * rows + c] = atoi(json_object_to_json_string(element));
        }
    }
    return array;
}

int map_to_json(int *array, int row, int col) {
    /*  make array to .json  */
    struct json_object *outter_json_array = json_object_new_array();

    for ( int i = 0; i < row; ++i )
    {
        struct json_object *inner_json_array = json_object_new_array();
        for ( int j = 0; j < col; ++j )
        {
            json_object_array_add(inner_json_array, json_object_new_int(array[i * col + j]));
        }
        json_object_array_add(outter_json_array, inner_json_array);
    }

    FILE *fp = fopen("/home/rongzi/.Lectures/hw/2048/data.json", "w");
    if ( fp != NULL ){
        size_t lenth = strlen(json_object_to_json_string(outter_json_array));
        char buffer[lenth + 10] = {};
        sprintf(buffer, "{\n    \"array\":%s\n}", json_object_to_json_string(outter_json_array));
        fprintf(fp, "%s", buffer);
    }
    else{
        fprintf(stderr, "Error! Can't write to data.json");
    }
    fclose(fp);
    return 0;
}

void color_init() {
    start_color();          // 启用颜色
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);
    init_pair(6, COLOR_BLACK, COLOR_RED);
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
    init_pair(9, COLOR_BLACK, COLOR_RED);
    init_pair(10, COLOR_BLACK, COLOR_WHITE);
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
}

int corr_color(int n) {
    /*  get corresbonding color code  */
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

# include "config.h"

int move_down(int *map, int start, int row, int col) {
    /*  move down */
    if ( start >= row - 1 )
        return 0;

    int flag = move_down(map, start + 1, row, col);

    /*  for every element in a row  */
    for ( int i = 0; i < col; ++i )
    {
        /*  for every element in a column  */
        for ( int j = start; j < row - 1; ++j)
        {
            if ( map[j * col + i] == 0 ) break;

            int curr_index = j * col + i, next_row_index = (j + 1) * col + i;
            if ( map[curr_index] == map[next_row_index] )
            {
                map[next_row_index] *= 2;
                map[curr_index] = 0;
                flag = 1;
                break;
            }
            else if ( map[next_row_index] == 0 )
            {
                map[next_row_index] = map[curr_index];
                map[curr_index] = 0;
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
            if ( map[j * col + i] == 0 ) break;

            int curr_index = j * col + i, prev_row_index = (j - 1) * col + i;
            if ( map[curr_index] == map[prev_row_index] )
            {
                map[prev_row_index] *= 2;
                map[curr_index] = 0;
                flag = 1;
                break;
            }
            else if ( map[prev_row_index] == 0 )
            {
                map[prev_row_index] = map[curr_index];
                map[curr_index] = 0;
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
            if ( map[i * col + j] == 0 ) break;

            int curr_index = i * col + j, prev_col_index = i * col + j - 1;
            if ( map[curr_index] == map[prev_col_index] )
            {
                map[prev_col_index] *= 2;
                map[curr_index] = 0;
                flag = 1;
                break;
            }
            else if ( map[prev_col_index] == 0 )
            {
                map[prev_col_index] = map[curr_index];
                map[curr_index] = 0;
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
            if ( map[i * col + j] == 0 ) break;

            int curr_index = i * col + j, next_col_index = i * col + j + 1;
            if ( map[curr_index] == map[next_col_index] )
            {
                map[next_col_index] *= 2;
                map[curr_index] = 0;
                flag = 1;
                break;
            }
            else if ( map[next_col_index] == 0 )
            {
                map[next_col_index] = map[curr_index];
                map[curr_index] = 0;
                flag = 1;
            }
        }
    }
    return flag;
}

int random_new(int *map, int row, int col) {
    /*  create new units after a move or at just begin  */
    long long i = 0, j = 0, load = 0, flag = 0;

    long long temp_times = times;
    if ( DEBUG ){
        char cmd[50];
        sprintf(cmd, "echo %lld ' ' >> /home/rongzi/test", temp_times);
        system(cmd);
    }
    while ( temp_times != 0 && load != row * col )
    {
        i = rand() % row;
        j = rand() % col;
        if ( map[i * col + j] == 0 )
        {
            temp_times -= 1;
            flag = 1;
            load = 0;
            map[i * col + j] = ((i & 1) == 1)?2:4;
        }
        load += 1;
    }
    return flag;
}

void draw(int *map, int row, int col, bool force) {
    /*   the array that store data from last game  */
    static int map_backup[MAPSIZE] = {};

    /* fulfill the screen according to map */
    for ( int i = 0; i < row; ++i )
    {
        for ( int j = 0; j < col; ++j )
        {
            int curr_index = i * col + j;
            /*  test if has drew once  */
            if ( map_backup[curr_index] == map[curr_index] && !force) continue;

            /* 启用颜色对 */
            attron(corr_color(map[curr_index]));
            /*  draw the background  */
            for ( int k = i * 3; k < i * 3 + 3; ++k ) {
                for ( int l = j * 7; l < j * 7 + 7; ++l ) {
                    mvprintw(starty + k, startx + l, " ");
                }
            }

            int posy = starty + i * unit_height + unit_height * 1/2;
            int posx = startx + j * unit_width + unit_width * 1/2;

            map_backup[curr_index] = map[curr_index];
            /*  draw the digits  */
            if ( map[curr_index] == 0 )
                mvprintw(posy, posx, "+");     // 在指定位置打印字符串
            else
            {
                char number[10] = {0};
                sprintf(number, "%d", map[curr_index]);
                if ( map[curr_index] < 10 )
                    mvprintw(posy, posx, number);     // 在指定位置打印字符串
                else if ( map[curr_index] > 10 && map[curr_index] < 1000 )
                    mvprintw(posy, posx - 1, number);     // 在指定位置打印字符串
                else if ( map[curr_index] > 1000 )
                    mvprintw(posy, posx - 2, number);     // 在指定位置打印字符串
            }
            attroff(corr_color(map[curr_index]));          // 关闭颜色对
        }
    }
}

int *json_to_map(char *filename, int *row, int *col) {
    /* read json file */
    char file_name[100];
    sprintf(file_name, "%s", filename);
    FILE *fp = fopen(file_name, "r");
    char buffer[FILESIZE] = {};
    if ( fp == NULL ){
        perror("Error! Fail to open data.json");
        return 0;
    }
    fread(buffer, sizeof(buffer), 1, fp);
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
            array[r * *col + c] = atoi(json_object_to_json_string(element));
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
    if ( fp == NULL ){
        fprintf(stderr, "Error! Can't write to data.json");
    }
    else{
        size_t lenth = strlen(json_object_to_json_string(outter_json_array));
        char buffer[lenth + 10] = {};
        sprintf(buffer, "{\n    \"array\":%s\n}", json_object_to_json_string(outter_json_array));
        fprintf(fp, "%s", buffer);
    }
    fclose(fp);
    return 0;
}

bool is_json(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    char buffer[FILESIZE] = {};

    if ( fp == NULL )
        return false;

    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    char pattern[] = ".*json";
    regex_t regex;
    regcomp(&regex, pattern, REG_EXTENDED);
    if ( regexec(&regex, file_name, 0, NULL, 0) != 0 )
        return false;

    struct json_object *outter_array;
    struct json_object *parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "array", &outter_array);
    if (json_object_object_get_ex(parsed_json, "array", &outter_array))
        return true;        /*  "array" exists  */
    else
        return false;       /*  no key "array"  */
    return false;
}

void color_init() {
    /*  enable color  */
    start_color();
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
    init_pair(12, COLOR_RED, COLOR_WHITE);
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

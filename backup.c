# include <stdio.h>
# include <string.h>
# include <json-c/json.h>

int *json_to_map(char *filename, int *row, int *col) {
    /* read json file */
    char file_name[50];
    sprintf(file_name, "%s", filename);
    FILE *fp = fopen(file_name, "r");
    char buffer[20480] = {};
    if ( fp == NULL ){
        perror("Error! Fail to open data.json");
        exit(1);
    }
    fread(buffer, 10240, 1, fp);
    fclose(fp);

    struct json_object *parsed_json = json_tokener_parse(buffer);
    struct json_object *outter_array = NULL;
    json_object_object_get_ex(parsed_json, "array", &outter_array);

    /* parse json and save to array */
    int rows = json_object_array_length(outter_array);
    int *array = (int *) malloc(sizeof(int) * 10000);
    *row = rows;
    for ( int r = 0; r < rows; ++r )
    {
        struct json_object *inner_array = json_object_array_get_idx(outter_array, r);
        int columns = json_object_array_length(inner_array);
        *col = columns;
        for ( int c = 0; c < columns; ++c ) {
            struct json_object *element = json_object_array_get_idx(inner_array, c);
            array[r * columns + c] = atoi(json_object_to_json_string(element));
        }
    }
    return array;
}

int map_to_json(int *array, int row, int col) {
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
        char buffer[lenth + 20] = {};
        sprintf(buffer, "{\n    \"array\":%s\n}", json_object_to_json_string(outter_json_array));
        fprintf(fp, "%s", buffer);
    }
    else{
        fprintf(stderr, "Error! Can't write to data.json");
    }
    fclose(fp);
    return 0;
}

int main(void) {
    int *array = (int *) calloc(20000, sizeof(int));
    map_to_json(array, 20, 10);
}

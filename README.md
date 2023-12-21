# 程序设计基础期末大作业--2048小游戏
> 本次大作业经过3次迭代，每次迭代都为该游戏增添了一到两个特性，下面是具体说明。
## 特性一：**N阶2048**小游戏
> 世界上不缺4x4的2048游戏，但是你一定没见过40x65的2048！相比4x4经典但是*烧脑*的版本，40x40听起来容易许多，起码我们有了能够轻易构造更大数值方块的能力。出乎意外的是，MxN阶的2048相比游戏性，观赏性更佳，似乎还有一定的解压效果，实用性大大增强，已经有了可以成为一个长久娱乐的工具的潜力而不只是一次完成教学目标的作业！

完成该特性的过程中，大量使用了过程抽象和模块化编程的理念，这才使得由4x4转向MxN成为可能。
本次大作业游戏源码由三部分组成：2048.c, functions.c, config.h
1. 其中config.h含有部分函数的声明以供其余模块使用，但实际上最重要的作用是提供用户一种修改游戏配置的方式，其中定义了有关游戏最大行列数，每个数字单元的宽和高，数字单元的颜色，存档文件的最大大小以及有关调试和绘图位置的参数。尽管这种修改配置的方式不是最简介直观的，但是由于~~助教没教~~能力不足，只能提供这样的方式供用户修改 ~~(反正只有我自己用，就不搞太复杂了:-))~~
2. functions.c中定义了数据处理相关的函数，包括处理上下左右移动时数组元素变化的函数，还有通过move_[deriction]的返回值判断是否需要随机产生新的数字单元并随即产生数字单元的random_new函数。在终端上绘画的draw函数，以及在其他特性加入后的其他有关数据处理的函数。
3. 2048.c中定义了主循环和有关运行逻辑的函数，而有关游戏数据处理的函数则统统放入了functions.c中，这使得游戏的可维护性大大增强。*事实上，在多次迭代中，就曾从这样的组织方式中收益，每次只有一个函数需要修改而不必考虑其余代码。大大简化了修改难度。*除此之外，2048.c中还定义了有关初始化，显示提示符的函数，显示ui的函数。

---

## 特性二：更高效的画面显示！
> N阶的2048哪里都好，只有一个尴尬的问题：太吃资源了！为实现将该作业做成真正使用的游戏这一目标，本作业还需要优化！
### 优化一：draw函数的解放
该作业4x4版本的2048实现中采用了***有数据更改就重绘屏幕的机制***，这意味着，用户的每一次操作都会导致整个终端的屏幕的绘制，即使两次的屏幕内容完全一致！这个机制在4x4下并没有太大问题，但在更为复杂的MxN版本中，重新绘制整个屏幕的代价就是会时常遇到屏幕闪烁的现象，这是由于重绘量过大导致的游戏低性能问题。第二个版本的2048为了解决这个问题，在draw函数中加入了以下语句。
```
/*   the array that store data from last game  */
static int map_backup[MAPSIZE] = {};

/* some for loop */
if ( map_backup[curr_index] == map[curr_index] && !force)
    continue;
else
    map_backup[curr_inded] = map[curr_index];
/* below are some action to draw the screen */
```
这个静态数组记录了上一次用户操作结束的屏幕内容，在进入该draw函数后，本次内容中和上一次相同的单元格不会再次重绘。如果不同则会更新该静态数组。而static保证了每一次draw函数留存的数组不会被重新初始化。draw函数记忆了上次屏幕内容，只去修改有差异的部分，这样就大大减少了重绘量，游戏也能在MxN阶下流畅运行！
### 优化二：数学题只算一遍！
> 在MxN阶的2048中，每次移动应该产生多少个随机单元呢？这应该是一个有关(m * c)的数学问题！如果4x4的2048下产生的随机单元数是2，那么40x40应该随机产生200个单元吗？实际测试中，如果随机单元数随(m*n)线性变化，则要么会导致低阶2048产生的单元数过于多，要么会导致高阶2048产生的单元数过于少，所以上一个版本的实现中，随机产生的单元数是由E ** x 和ln（x）这两个函数分段拟合而成的，这能保证低阶2048产生的单元数不至于过多，高阶2048产生的单元数不至于过少**(如果图上还有足够的空位的话)**。虽然解决了随机单元数不太对的问题，但是！这个函数也太耗费资源啦！
以下是为具体的拟合曲线：
```c
times = (*prow * *pcol > 2000)? ((long) log(sqrt(*prow * *pcol)) + ((*prow < *pcol)? *prow / 2: *pcol / 2)):(long) exp(sqrt(*prow * *pcol) / 10.0);
```
上一个版本中，上述代码被放在了random_new函数中，意味着同一局游戏里，这行代码会被反复执行(即使所有时刻的运算结果都为相同)！为了解决上述问题，第三个版本的2048中申明了以下变量。
```
long times;
```
并将求值times的操作，移动至了游戏初始化的位置，这样MxN阶的2048也有了不错的性能表现！

## 特性三：游戏存档
> 正经人谁在能玩游戏时玩2048啊，唯有工作学习之余~~(摸鱼时候)~~才能在2048中找到游戏的乐趣。但是！哪有老板喜欢员工上班玩游戏的，而如果此时正在玩2048的你由于老板的出现不得不紧急关闭2048，那么之前的进度.....
由于现实需要，本游戏看来必须要加上存档功能了。在第四版本的2048加入了存档游戏的功能，只需按下“b”就会显示是否保存进度的提示，再确认“y”保存！
以下是存档核心实现的函数
```
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
```
## 特性四：就要呆在中间！
> 游戏似乎是能玩了，但是，你这2048它不~~保熟~~正常啊！
本作业使用到的渲染屏幕的库默认x, y轴是绝对坐标，不能做到居中显示的效果~~也可能是这功能在文档的后面，我没翻到~~，一旦终端屏幕大小变化，整个游戏的显示就会不正常，第四特性是每当用户操作之后就会检查是否屏幕大小改变，如果改变就会重新调整，使得游戏显示正常。
以下是具体实现
```
void update(int *game_map, int *prow, int *pcol) {
    /*  detect whether the window size has changed  */
    static int last_max_row = 0, last_max_col = 0, is_first = 1;
    getmaxyx(stdscr, max_row, max_col);

    if ( is_first == 1 ){
        last_max_row = max_row;
        last_max_col = max_col;
        is_first = 0;
        return;
    }

    if ( (max_row != last_max_row || max_col != last_max_col) && is_first == 0 ){
        if ( DEBUG ) system("echo $(date) updating >> ~/test");
        clear();
        adjust_window(*prow, *pcol);
        draw_ui(*prow, *pcol);
        draw(game_map, *prow, *pcol, true);
        last_max_row = max_row;
        last_max_col = max_col;
    }
    return;
}
```

## 结语
***做大作业好累，求求助教多给点分数吧！***

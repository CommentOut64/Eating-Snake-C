/*
 * Please use GBK coding
 * 贪吃蛇游戏
 * 参考了 CSDN hzasrd的代码
 * 添加了WASD操作模式，添加了倒计时
 * 添加了暂停功能
 * 优化了交互
 * 修复了若干bug
 */

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> //windows编程头文件
#include <time.h>
#include <conio.h> //控制台输入输出头文件
#include <stdbool.h>
#include <string.h>

#define SNAKESIZE 100 // 蛇的身体最大节数
#define MAPWIDTH 118  // 宽度
#define MAPHEIGHT 29  // 高度

// 食物的坐标
struct
{
    int x;
    int y;
} food;

// 蛇的相关属性
struct
{
    int speed;        // 蛇移动的速度
    int len;          // 蛇的长度
    int x[SNAKESIZE]; // 组成蛇身的每一个小方块中x的坐标
    int y[SNAKESIZE]; // 组成蛇身的每一个小方块中y的坐标

} snake;

// 准备和倒计时
void prepare(void);
// 绘制游戏边框
void drawMap(void);
// 随机生成食物
void createFood(void);
// 按键操作
void keyDown(void);
// 蛇的状态
bool snakeStatus(void);
// 从控制台移动光标
void gotoxy(int x, int y);
// 暂停
void pause(int pre_key);
// 显示结束信息
void displayGameOver(void);

int key = 72;          // 表示蛇移动的方向，72为按下“↑”所代表的数字
int choice;            // 记录玩家选择的操控方式
int changeFlag = 0;    // 判断蛇是否吃掉了食物
int score = 0;         // 记录玩家的得分
bool isPaused = false; // 判断是否暂停

// 将控制台光标移到（x,y）处
void gotoxy(int x, int y)
{
    COORD coord; //<windows.h>中申明的坐标结构
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main(void)
{
    prepare();
    srand((unsigned int)time(NULL)); // 初始化随机数生成器的种子
    drawMap();
    while (1)
    {
        keyDown();
        if (!snakeStatus())
            break;
        createFood();
        Sleep(snake.speed);
    }
    Sleep(1000);
    system("cls"); // 清空屏幕
    displayGameOver();

    return 0;
}
void prepare(void)
{
    printf("请选择控制方式\n");
    printf("1)W A S D\n");
    printf("2)↑ ↓ ← →\n");

    char input[10];
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &choice);
    while (choice != 1 && choice != 2)
    {
        printf("请重新输入1或2\n");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &choice);
    }

    // 提示锁定大写
    if (choice == 1)
    {
        printf("你需要按 caps键 锁定大写\n");
        Sleep(1500);
    }

    printf("按 esc键 暂停游戏\n");
    Sleep(700);
    printf("按 任意键 开始游戏\n");
    _getch();
    for (int i = 3; i >= 1; i--)
    {
        printf("%d\n", i);
        Sleep(1000);
    }
    printf("GO!\n");
    Sleep(1000);
    system("cls"); // 清空屏幕
}

void drawMap()
{
    // 打印上下边框
    for (int i = 0; i <= MAPWIDTH; i += 2) // i+=2是因为横向占用的是两个位置
    {
        // 打印上边框
        gotoxy(i, 0);
        printf("■");
        // 打印下边框
        gotoxy(i, MAPHEIGHT);
        printf("■");
    }

    // 打印左右边框
    for (int i = 1; i < MAPHEIGHT; i++)
    {
        // 打印左边框
        gotoxy(0, i);
        printf("■");
        // 打印左边框
        gotoxy(MAPWIDTH, i);
        printf("■");
    }

    // 随机生成初试食物
    while (1)
    {

        food.x = rand() % (MAPWIDTH - 4) + 2;  // 在水平方向上，生成一个范围在2到MAPWIDTH-2之间的随机整数
        food.y = rand() % (MAPHEIGHT - 2) + 1; // 在垂直方向上，生成一个范围在1到MAPHEIGHT-1之间的随机整数
        // 生成的食物横坐标的奇偶必须和初试时蛇头所在坐标的奇偶一致，因为一个字符占两个字节位置，若不一致
        // 会导致吃食物的时候只吃到一半
        if (food.x % 2 == 0)
            break;
    }
    // 将光标移到食物的坐标处打印食物
    gotoxy(food.x, food.y);
    printf("★");

    // 初始化蛇的属性
    snake.len = 3;
    snake.speed = 200;

    // 在屏幕中间生成蛇头
    snake.x[0] = MAPWIDTH / 2 + 1; // x坐标为偶数
    snake.y[0] = MAPHEIGHT / 2;
    // 打印蛇头
    gotoxy(snake.x[0], snake.y[0]);
    printf("■");

    // 生成初试的蛇身
    for (int i = 1; i < snake.len; i++)
    {
        // 蛇身的打印，纵坐标不变，横坐标为上一节蛇身的坐标值+2
        snake.x[i] = snake.x[i - 1] + 2;
        snake.y[i] = snake.y[i - 1];
        gotoxy(snake.x[i], snake.y[i]);
        printf("■");
    }
    // 打印完蛇身后将光标移到屏幕最上方，避免光标在蛇身处一直闪烁
    gotoxy(MAPWIDTH - 2, 0);

    return;
}

void keyDown()
{
    int pre_key = key; // 记录前一个按键的方向
    if (_kbhit())      // 如果用户按下了键盘中的某个键
    {
        _flushall(); // 清空缓冲区的字符

        // getch()读取方向键的时候，会返回两次，第一次调用返回0或者224，第二次调用返回的才是实际值
        int ch = _getch();        // 第一次调用返回
        if (ch == 0 || ch == 224) // 第一次返回0或者224,说明读取的是方向键
            key = _getch();       // 第二次调用返回得到真实值
        else
            key = ch; // 读取的是WASD
        // printf("Key pressed: %d\n", key);

        // 忽略除 Esc 和方向控制键以外的按键
        if (key != 27 && key != 72 && key != 80 && key != 75 && key != 77 &&
            key != 87 && key != 65 && key != 83 && key != 68)
            key = pre_key;
    }

    if (key == 27) // 27是Esc键的ASCII 码
        pause(pre_key);

    /*
     *蛇移动时候先擦去蛇尾的一节
     *changeFlag为0表明此时没有吃到食物，因此每走一步就要擦除掉蛇尾，形成移动
     *changeFlag为1表明吃到了食物，就不需要擦除蛇尾，蛇身增长
     */
    if (changeFlag == 0)
    {
        gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
        printf("  "); // 在蛇尾处输出空格即擦去蛇尾
    }

    // 将蛇的每一节依次向前移动一节（蛇头除外）
    for (int i = snake.len - 1; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    // 蛇当前移动的方向不能和前一次的方向相反，比如蛇往左走的时候不能直接按右键往右走
    // 如果当前移动方向和前一次方向相反的话，把当前移动的方向改为前一次的方向
    /*
     * “↑” ：72     W 87
     * “↓” ：80     S 83
     * “←” ：75     A 65
     * “→” ：77     D 68
     */
    if (choice == 1)
    {
        if ((pre_key == 87 && key == 83) || (pre_key == 83 && key == 87) ||
            (pre_key == 65 && key == 68) || (pre_key == 68 && key == 65))
            key = pre_key;
    }
    else if (choice == 2)
    {
        if ((pre_key == 72 && key == 80) || (pre_key == 80 && key == 72) ||
            (pre_key == 75 && key == 77) || (pre_key == 77 && key == 75))
            key = pre_key;
    }

    // 判断蛇头应该往哪个方向移动
    switch (key)
    {
    case 65:
    case 75:
        snake.x[0] -= 2; // 向左
        break;
    case 68:
    case 77:
        snake.x[0] += 2; // 向右
        break;
    case 87:
    case 72:
        snake.y[0]--; // 向上
        break;
    case 83:
    case 80:
        snake.y[0]++; // 向下
        break;
    }
    // 打印出蛇头
    gotoxy(snake.x[0], snake.y[0]);
    printf("■");
    gotoxy(MAPWIDTH - 2, 0);
    // 由于目前没有吃到食物，changFlag值为0
    changeFlag = 0;

    return;
}

void createFood()
{
    if (snake.x[0] == food.x && snake.y[0] == food.y) // 蛇头碰到食物
    {
        // 再次生成一个食物
        int attempts = 0; // 限制循环次数
        while (attempts++ < 1000)
        {
            int flag = 1;                               // 1为生成有效
            srand((unsigned int)time(NULL) + attempts); // 重新初始化随机数生成器
            food.x = rand() % (MAPWIDTH - 4) + 2;
            food.y = rand() % (MAPHEIGHT - 2) + 1;

            // 随机生成的食物不能在蛇的身体上
            for (int i = 0; i < snake.len; i++)
            {
                if (snake.x[i] == food.x && snake.y[i] == food.y)
                {
                    flag = 0;
                    break;
                }
            }
            // 随机生成的食物不能横坐标为奇数，也不能在蛇身，否则重新生成
            if (flag && food.x % 2 == 0)
                break;
        }

        // 绘制食物
        gotoxy(food.x, food.y);
        printf("★");

        snake.len++;      // 吃到食物，蛇身长度加1
        score += 10;      // 每个食物得10分
        snake.speed -= 5; // 随着吃的食物越来越多，速度会越来越快
        changeFlag = 1;   // 很重要，因为吃到了食物，就不用再擦除蛇尾的那一节，以此来造成蛇身体增长的效果
    }
    return;
}
void pause(int pre_key)
{
    // 隐藏蛇
    for (int i = 0; i < snake.len; i++)
    {
        gotoxy(snake.x[i], snake.y[i]);
        printf("  ");
    }

    // 隐藏食物
    gotoxy(food.x, food.y);
    printf("  ");

    // 显示暂停提示
    char pauseMsg[] = "游戏已暂停";
    char continueMsg[] = "按任意键恢复";

    int pauseMsgLen = strlen(pauseMsg);
    int continueMsgLen = strlen(continueMsg);

    int pauseMsgX = (MAPWIDTH - pauseMsgLen) / 2;
    int continueMsgX = (MAPWIDTH - continueMsgLen) / 2;

    gotoxy(pauseMsgX, MAPHEIGHT / 2);
    printf("%s\n", pauseMsg);
    gotoxy(continueMsgX, MAPHEIGHT / 2 + 1);
    printf("%s", continueMsg);

    _getch(); // 等待输入
    // 清空缓冲区中的所有按键事件
    while (_kbhit())
        _getch();

    Sleep(500);
    // 清除暂停提示
    for (int i = 1; i < MAPWIDTH; i++)
    {
        gotoxy(i, MAPHEIGHT / 2);
        printf(" ");
        gotoxy(i, MAPHEIGHT / 2 + 1);
        printf(" ");
    }

    Sleep(700);
    // 重新绘制蛇
    for (int i = 0; i < snake.len; i++)
    {
        gotoxy(snake.x[i], snake.y[i]);
        printf("■");
    }

    // 重新绘制食物
    gotoxy(food.x, food.y);
    printf("★");

    gotoxy(MAPWIDTH - 2, 0); // 打印完蛇身后将光标移到屏幕最上方，避免光标在蛇身处一直闪烁

    key = pre_key; // 恢复方向
    Sleep(500);

    return;
}
bool snakeStatus()
{

    // 蛇头碰到上下边界，游戏结束
    if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
    {
        // 清空缓冲区中的所有按键事件
        while (_kbhit())
            _getch();
        return false;
    }

    // 蛇头碰到左右边界，游戏结束
    if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH)
    {
        // 清空缓冲区中的所有按键事件
        while (_kbhit())
            _getch();
        return false;
    }
    // 蛇头碰到蛇身，游戏结束
    for (int i = 3; i < snake.len; i++)
    {
        if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
        {
            // 清空缓冲区中的所有按键事件
            while (_kbhit())
                _getch();
            return false;
        }
    }
    return true;
}

void displayGameOver(void)
{
    char gameOverMsg[] = "Game Over!";
    char scoreMsg[50];
    char exitMsg[] = "按 esc键 退出";

    // 计算字符串长度
    int gameOverLen = strlen(gameOverMsg);
    int exitLen = strlen(exitMsg);
    snprintf(scoreMsg, sizeof(scoreMsg), "本次游戏得分为：%d", score); // 拼接字符串
    int scoreLen = strlen(scoreMsg);

    // 计算起始位置
    int gameOverX = (MAPWIDTH - gameOverLen) / 2;
    int scoreX = (MAPWIDTH - scoreLen) / 2;
    int exitX = (MAPWIDTH - exitLen) / 2;

    // 显示游戏结束信息
    gotoxy(gameOverX, MAPHEIGHT / 2);
    printf("%s\n", gameOverMsg);
    gotoxy(scoreX, MAPHEIGHT / 2 + 1);
    printf("%s\n", scoreMsg);
    gotoxy(exitX, MAPHEIGHT / 2 + 2);
    printf("%s", exitMsg);
    // 等待输入esc
    while (_getch() != 27)
        continue;

    return;
}

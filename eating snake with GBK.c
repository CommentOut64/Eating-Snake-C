/*
 * Please use GBK coding
 * ̰������Ϸ
 * �ο��� CSDN hzasrd�Ĵ���
 * �����WASD����ģʽ������˵���ʱ
 * �������ͣ����
 * �Ż��˽���
 * �޸�������bug
 */

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> //windows���ͷ�ļ�
#include <time.h>
#include <conio.h> //����̨�������ͷ�ļ�
#include <stdbool.h>
#include <string.h>

#define SNAKESIZE 100 // �ߵ�����������
#define MAPWIDTH 118  // ���
#define MAPHEIGHT 29  // �߶�

// ʳ�������
struct
{
    int x;
    int y;
} food;

// �ߵ��������
struct
{
    int speed;        // ���ƶ����ٶ�
    int len;          // �ߵĳ���
    int x[SNAKESIZE]; // ��������ÿһ��С������x������
    int y[SNAKESIZE]; // ��������ÿһ��С������y������

} snake;

// ׼���͵���ʱ
void prepare(void);
// ������Ϸ�߿�
void drawMap(void);
// �������ʳ��
void createFood(void);
// ��������
void keyDown(void);
// �ߵ�״̬
bool snakeStatus(void);
// �ӿ���̨�ƶ����
void gotoxy(int x, int y);
// ��ͣ
void pause(int pre_key);
// ��ʾ������Ϣ
void displayGameOver(void);

int key = 72;          // ��ʾ���ƶ��ķ���72Ϊ���¡����������������
int choice;            // ��¼���ѡ��Ĳٿط�ʽ
int changeFlag = 0;    // �ж����Ƿ�Ե���ʳ��
int score = 0;         // ��¼��ҵĵ÷�
bool isPaused = false; // �ж��Ƿ���ͣ

// ������̨����Ƶ���x,y����
void gotoxy(int x, int y)
{
    COORD coord; //<windows.h>������������ṹ
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main(void)
{
    prepare();
    srand((unsigned int)time(NULL)); // ��ʼ�������������������
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
    system("cls"); // �����Ļ
    displayGameOver();

    return 0;
}
void prepare(void)
{
    printf("��ѡ����Ʒ�ʽ\n");
    printf("1)W A S D\n");
    printf("2)�� �� �� ��\n");

    char input[10];
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &choice);
    while (choice != 1 && choice != 2)
    {
        printf("����������1��2\n");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &choice);
    }

    // ��ʾ������д
    if (choice == 1)
    {
        printf("����Ҫ�� caps�� ������д\n");
        Sleep(1500);
    }

    printf("�� esc�� ��ͣ��Ϸ\n");
    Sleep(700);
    printf("�� ����� ��ʼ��Ϸ\n");
    _getch();
    for (int i = 3; i >= 1; i--)
    {
        printf("%d\n", i);
        Sleep(1000);
    }
    printf("GO!\n");
    Sleep(1000);
    system("cls"); // �����Ļ
}

void drawMap()
{
    // ��ӡ���±߿�
    for (int i = 0; i <= MAPWIDTH; i += 2) // i+=2����Ϊ����ռ�õ�������λ��
    {
        // ��ӡ�ϱ߿�
        gotoxy(i, 0);
        printf("��");
        // ��ӡ�±߿�
        gotoxy(i, MAPHEIGHT);
        printf("��");
    }

    // ��ӡ���ұ߿�
    for (int i = 1; i < MAPHEIGHT; i++)
    {
        // ��ӡ��߿�
        gotoxy(0, i);
        printf("��");
        // ��ӡ��߿�
        gotoxy(MAPWIDTH, i);
        printf("��");
    }

    // ������ɳ���ʳ��
    while (1)
    {

        food.x = rand() % (MAPWIDTH - 4) + 2;  // ��ˮƽ�����ϣ�����һ����Χ��2��MAPWIDTH-2֮����������
        food.y = rand() % (MAPHEIGHT - 2) + 1; // �ڴ�ֱ�����ϣ�����һ����Χ��1��MAPHEIGHT-1֮����������
        // ���ɵ�ʳ����������ż����ͳ���ʱ��ͷ�����������żһ�£���Ϊһ���ַ�ռ�����ֽ�λ�ã�����һ��
        // �ᵼ�³�ʳ���ʱ��ֻ�Ե�һ��
        if (food.x % 2 == 0)
            break;
    }
    // ������Ƶ�ʳ������괦��ӡʳ��
    gotoxy(food.x, food.y);
    printf("��");

    // ��ʼ���ߵ�����
    snake.len = 3;
    snake.speed = 200;

    // ����Ļ�м�������ͷ
    snake.x[0] = MAPWIDTH / 2 + 1; // x����Ϊż��
    snake.y[0] = MAPHEIGHT / 2;
    // ��ӡ��ͷ
    gotoxy(snake.x[0], snake.y[0]);
    printf("��");

    // ���ɳ��Ե�����
    for (int i = 1; i < snake.len; i++)
    {
        // ����Ĵ�ӡ�������겻�䣬������Ϊ��һ�����������ֵ+2
        snake.x[i] = snake.x[i - 1] + 2;
        snake.y[i] = snake.y[i - 1];
        gotoxy(snake.x[i], snake.y[i]);
        printf("��");
    }
    // ��ӡ������󽫹���Ƶ���Ļ���Ϸ���������������һֱ��˸
    gotoxy(MAPWIDTH - 2, 0);

    return;
}

void keyDown()
{
    int pre_key = key; // ��¼ǰһ�������ķ���
    if (_kbhit())      // ����û������˼����е�ĳ����
    {
        _flushall(); // ��ջ��������ַ�

        // getch()��ȡ�������ʱ�򣬻᷵�����Σ���һ�ε��÷���0����224���ڶ��ε��÷��صĲ���ʵ��ֵ
        int ch = _getch();        // ��һ�ε��÷���
        if (ch == 0 || ch == 224) // ��һ�η���0����224,˵����ȡ���Ƿ����
            key = _getch();       // �ڶ��ε��÷��صõ���ʵֵ
        else
            key = ch; // ��ȡ����WASD
        // printf("Key pressed: %d\n", key);

        // ���Գ� Esc �ͷ�����Ƽ�����İ���
        if (key != 27 && key != 72 && key != 80 && key != 75 && key != 77 &&
            key != 87 && key != 65 && key != 83 && key != 68)
            key = pre_key;
    }

    if (key == 27) // 27��Esc����ASCII ��
        pause(pre_key);

    /*
     *���ƶ�ʱ���Ȳ�ȥ��β��һ��
     *changeFlagΪ0������ʱû�гԵ�ʳ����ÿ��һ����Ҫ��������β���γ��ƶ�
     *changeFlagΪ1�����Ե���ʳ��Ͳ���Ҫ������β����������
     */
    if (changeFlag == 0)
    {
        gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
        printf("  "); // ����β������ո񼴲�ȥ��β
    }

    // ���ߵ�ÿһ��������ǰ�ƶ�һ�ڣ���ͷ���⣩
    for (int i = snake.len - 1; i > 0; i--)
    {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    // �ߵ�ǰ�ƶ��ķ����ܺ�ǰһ�εķ����෴�������������ߵ�ʱ����ֱ�Ӱ��Ҽ�������
    // �����ǰ�ƶ������ǰһ�η����෴�Ļ����ѵ�ǰ�ƶ��ķ����Ϊǰһ�εķ���
    /*
     * ������ ��72     W 87
     * ������ ��80     S 83
     * ������ ��75     A 65
     * ������ ��77     D 68
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

    // �ж���ͷӦ�����ĸ������ƶ�
    switch (key)
    {
    case 65:
    case 75:
        snake.x[0] -= 2; // ����
        break;
    case 68:
    case 77:
        snake.x[0] += 2; // ����
        break;
    case 87:
    case 72:
        snake.y[0]--; // ����
        break;
    case 83:
    case 80:
        snake.y[0]++; // ����
        break;
    }
    // ��ӡ����ͷ
    gotoxy(snake.x[0], snake.y[0]);
    printf("��");
    gotoxy(MAPWIDTH - 2, 0);
    // ����Ŀǰû�гԵ�ʳ�changFlagֵΪ0
    changeFlag = 0;

    return;
}

void createFood()
{
    if (snake.x[0] == food.x && snake.y[0] == food.y) // ��ͷ����ʳ��
    {
        // �ٴ�����һ��ʳ��
        int attempts = 0; // ����ѭ������
        while (attempts++ < 1000)
        {
            int flag = 1;                               // 1Ϊ������Ч
            srand((unsigned int)time(NULL) + attempts); // ���³�ʼ�������������
            food.x = rand() % (MAPWIDTH - 4) + 2;
            food.y = rand() % (MAPHEIGHT - 2) + 1;

            // ������ɵ�ʳ�ﲻ�����ߵ�������
            for (int i = 0; i < snake.len; i++)
            {
                if (snake.x[i] == food.x && snake.y[i] == food.y)
                {
                    flag = 0;
                    break;
                }
            }
            // ������ɵ�ʳ�ﲻ�ܺ�����Ϊ������Ҳ����������������������
            if (flag && food.x % 2 == 0)
                break;
        }

        // ����ʳ��
        gotoxy(food.x, food.y);
        printf("��");

        snake.len++;      // �Ե�ʳ������ȼ�1
        score += 10;      // ÿ��ʳ���10��
        snake.speed -= 5; // ���ųԵ�ʳ��Խ��Խ�࣬�ٶȻ�Խ��Խ��
        changeFlag = 1;   // ����Ҫ����Ϊ�Ե���ʳ��Ͳ����ٲ�����β����һ�ڣ��Դ������������������Ч��
    }
    return;
}
void pause(int pre_key)
{
    // ������
    for (int i = 0; i < snake.len; i++)
    {
        gotoxy(snake.x[i], snake.y[i]);
        printf("  ");
    }

    // ����ʳ��
    gotoxy(food.x, food.y);
    printf("  ");

    // ��ʾ��ͣ��ʾ
    char pauseMsg[] = "��Ϸ����ͣ";
    char continueMsg[] = "��������ָ�";

    int pauseMsgLen = strlen(pauseMsg);
    int continueMsgLen = strlen(continueMsg);

    int pauseMsgX = (MAPWIDTH - pauseMsgLen) / 2;
    int continueMsgX = (MAPWIDTH - continueMsgLen) / 2;

    gotoxy(pauseMsgX, MAPHEIGHT / 2);
    printf("%s\n", pauseMsg);
    gotoxy(continueMsgX, MAPHEIGHT / 2 + 1);
    printf("%s", continueMsg);

    _getch(); // �ȴ�����
    // ��ջ������е����а����¼�
    while (_kbhit())
        _getch();

    Sleep(500);
    // �����ͣ��ʾ
    for (int i = 1; i < MAPWIDTH; i++)
    {
        gotoxy(i, MAPHEIGHT / 2);
        printf(" ");
        gotoxy(i, MAPHEIGHT / 2 + 1);
        printf(" ");
    }

    Sleep(700);
    // ���»�����
    for (int i = 0; i < snake.len; i++)
    {
        gotoxy(snake.x[i], snake.y[i]);
        printf("��");
    }

    // ���»���ʳ��
    gotoxy(food.x, food.y);
    printf("��");

    gotoxy(MAPWIDTH - 2, 0); // ��ӡ������󽫹���Ƶ���Ļ���Ϸ���������������һֱ��˸

    key = pre_key; // �ָ�����
    Sleep(500);

    return;
}
bool snakeStatus()
{

    // ��ͷ�������±߽磬��Ϸ����
    if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
    {
        // ��ջ������е����а����¼�
        while (_kbhit())
            _getch();
        return false;
    }

    // ��ͷ�������ұ߽磬��Ϸ����
    if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH)
    {
        // ��ջ������е����а����¼�
        while (_kbhit())
            _getch();
        return false;
    }
    // ��ͷ����������Ϸ����
    for (int i = 3; i < snake.len; i++)
    {
        if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
        {
            // ��ջ������е����а����¼�
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
    char exitMsg[] = "�� esc�� �˳�";

    // �����ַ�������
    int gameOverLen = strlen(gameOverMsg);
    int exitLen = strlen(exitMsg);
    snprintf(scoreMsg, sizeof(scoreMsg), "������Ϸ�÷�Ϊ��%d", score); // ƴ���ַ���
    int scoreLen = strlen(scoreMsg);

    // ������ʼλ��
    int gameOverX = (MAPWIDTH - gameOverLen) / 2;
    int scoreX = (MAPWIDTH - scoreLen) / 2;
    int exitX = (MAPWIDTH - exitLen) / 2;

    // ��ʾ��Ϸ������Ϣ
    gotoxy(gameOverX, MAPHEIGHT / 2);
    printf("%s\n", gameOverMsg);
    gotoxy(scoreX, MAPHEIGHT / 2 + 1);
    printf("%s\n", scoreMsg);
    gotoxy(exitX, MAPHEIGHT / 2 + 2);
    printf("%s", exitMsg);
    // �ȴ�����esc
    while (_getch() != 27)
        continue;

    return;
}

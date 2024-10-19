#include <windows.h>
#include <iostream>
#include <ctime>
#include <conio.h>
#include <string> 

using namespace std;


/* 1.	Реализовать перемещение персонажа по лабиринту стрелками влево, вправо, вверх и вниз. Персонаж не должен проходить сквозь стены. Если персонаж  дошёл до выхода из лабиринта в нижнем правом углу - игра заканчивается победой (вывести диалог с сообщением "победа - найден выход" в MessageBox).

2.	Реализовать подсчёт собранных монет, количество монет в наличии выводить в заголовок окна (начиная с нуля биткоинов). Если все монеты лабиринта собраны - игра заканчивается победой (вывести диалог с сообщением "победа - монеты собраны").

*/



enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
enum Colors { DARKGREEN = 2, RED = 12, YELLOW = 14, BLUE = 9, PURPLE = 5 };
enum Objects { HALL, WALL, COIN, ENEMY, HEALTH };



int main()
{
    int coins = 0;
    
    int sleep = 20000;
    int total_coins = 0;
    system(("title Bomber! - Bitcoins collected: " + to_string(coins)).c_str());
    srand(time(0));

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.dwSize = 100;
    SetConsoleCursorInfo(h, &info);

    const int WIDTH = 50;
    const int HEIGHT = 15;
    const int finish_y = HEIGHT - 3;
    const int finish_x = WIDTH - 1;
    int location[HEIGHT][WIDTH] = {};

    // генерация локации
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            location[y][x] = rand() % 5;

            if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
                location[y][x] = WALL;

            if ((x == 0 && y == 2) || (x == WIDTH - 1 && y == HEIGHT - 3))
                location[y][x] = HALL;

            if (location[y][x] == ENEMY) {
                int prob = rand() % 10;
                if (prob != 0)
                    location[y][x] = HALL;
            }

            if (location[y][x] == HEALTH) { // HEALTH (сердечки) как новый объект лабиринта
                int prob = rand() % 10;
                if (prob != 0)
                    location[y][x] = HALL;
            }

            if (location[y][x] == COIN) {
                int prob = rand() % 10;
                if (prob != 0)
                    location[y][x] = HALL;
                else
                    total_coins++;  // Если 0 - выпадает монетка - суммируем каждую на этапе генерации
            }
        }
    }

    // показ локации
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            switch (location[y][x]) {
            case HALL:
                cout << " ";
                break;
            case WALL:
                SetConsoleTextAttribute(h, DARKGREEN);
                cout << (char)177;
                break;
            case COIN:
                SetConsoleTextAttribute(h, YELLOW);
                cout << (char)15;
                break;
            case ENEMY:
                SetConsoleTextAttribute(h, RED);
                cout << (char)1;
                break;
            case HEALTH:
                SetConsoleTextAttribute(h, PURPLE);
                cout << (char)3;
                break;
            default:
                cout << location[y][x];
                break;
            }
        }
        cout << "\n";
    }

    COORD position;
    position.X = 1;
    position.Y = 2;
    SetConsoleCursorPosition(h, position);
    SetConsoleTextAttribute(h, BLUE);
    cout << (char)2;

    while (true) {
        int code = _getch();
        if (code == 224)
            code = _getch();

        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, BLUE);
        cout << " ";

        switch (code) {
        case ENTER:
            break;
        case SPACEBAR:
            break;
        case ESCAPE:
            MessageBoxA(NULL, "You decided to finish the game!", "GAME OVER", MB_OK | MB_ICONASTERISK);
            Sleep(sleep);
            return 0; // Если игрок захочет завершить игру
        case RIGHT:
            if (position.X < WIDTH - 1 && location[position.Y][position.X + 1] != WALL)
                position.X++;
            break;
        case LEFT:
            if (position.X > 0 && location[position.Y][position.X - 1] != WALL)
                position.X--;
            break;
        case UP:
            if (position.Y > 0 && location[position.Y - 1][position.X] != WALL)
                position.Y--;
            break;
        case DOWN:
            if (position.Y < HEIGHT - 1 && location[position.Y + 1][position.X] != WALL)
                position.Y++;
            break;
        default:
            cout << code << "\n";
            break;
        }

        SetConsoleCursorPosition(h, position);
        SetConsoleTextAttribute(h, BLUE);
        cout << (char)2;

        // Победа 1. Выход найден
        if (position.Y == finish_y && position.X == finish_x) {
            MessageBoxA(NULL, "Victory - found a way out!", "VICTORY", MB_OK | MB_ICONASTERISK);
            Sleep(sleep);
            break;
        }

        // Обработка сбора монет
        if (location[position.Y][position.X] == COIN) {
            coins++;
            system(("title Bomber! - Bitcoins collected: " + to_string(coins)).c_str()); // Обновляем заголовок окна
            location[position.Y][position.X] = HALL;
        }

        // Победа 2. Собраны все монетки
        if (coins == total_coins) {
            MessageBoxA(NULL, "Victory - collected all the coins!", "VICTORY", MB_OK | MB_ICONASTERISK);
            Sleep(sleep);
            break;
        }
    }




}

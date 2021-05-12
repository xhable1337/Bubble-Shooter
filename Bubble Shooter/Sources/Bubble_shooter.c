/*
Game «Bubble Shooter»
Designed & developed by @xhable

Repo link: https://github.com/xhable1337/Bubble-Shooter
 */

#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "resource.h"
#include "leaderboard.h"

#define DELAY 5

// --- Константы

// Константы разрешения экрана
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Ширина границы
const int BORDER = 24;

// Скорость передвижения шарика
const int MSPEED = 8;

// Радиус коллизии шарика
const int COLRADIUS = 8;

// Количество возможных цветов шариков
const int COLORS = 6;

// Размеры сетки
const int BALLX = 20;
const int BALLY = 20;

// Инициализация сетки
const int GRIDX = 19;
const int GRIDY = 6;

// true/false значения для удобства работы с условиями
const int false = 0;
const int true = 1;

// Разрешение текстуры шариков
const int IMAGE_WIDTH = 32;
const int IMAGE_HEIGHT = 32;

// Массив таблицы рекордов
int leaderboard[8];

typedef struct _BACKGROUND
{
    SDL_Surface* image;
} BACKGROUND;

typedef struct _PLAYER
{
    float posX;
    float posY;
    float stepX;
    float stepY;
    SDL_Surface* image;
    int color;
    int imgW;
    int imgH;
    float centerX;
    float centerY;
} PLAYER;

typedef struct _NPC
{
    float posX;
    float posY;
    int indexX;
    int indexY;
    int imgW;
    int imgH;
    SDL_Surface* image;
    int color;
    float distX;
    float distY;
    float dist;
    float centerX;
    float centerY;
    int coltype;
    int remain;
} NPC;

typedef struct _UIELEMENT
{
    float posX;
    float posY;
    int color;
    SDL_Surface* image;
} UIELEMENT;

// --- Глобальные переменные

int clicked = 0;
int play = 0;
int quit = 0;
int health = 0;
int maxhealth = 0;
int Sound;
int Score;
int musicVolume = 100;
int soundsVolume = 100;
char NickString[16];

SDL_Window* gWindow = NULL;

Mix_Music* bubblePop = NULL;
Mix_Music* synthPop = NULL;
Mix_Music* shot = NULL;
Mix_Music* music = NULL;

// Элемент-игрок
PLAYER ball;

// Следующий шарик
UIELEMENT nextball;

// Элементы жизней
UIELEMENT lifeballs[5];

// Элемент звука (вкл/выкл)
UIELEMENT SoundElement;

// Элемент завершения игры
UIELEMENT EGelement;

// Эффект при наведении на кнопки главного меню
UIELEMENT MenuHoverElement;

// Внутриигровой UI
UIELEMENT PMUI;

// UI при завершении игры (endGameUI)
UIELEMENT EGUI;

// Кнопка перезапуска игры (endGameUI)
UIELEMENT EGR;

// Кнопка возврата в главное меню (endGameUI)
UIELEMENT EGMen;

// Кнопка перехода к таблице рекордов (endGameUI)
UIELEMENT EGRank;

// Меню настроек
UIELEMENT SettingsElement;

// Таблица рекордов
UIELEMENT RankElement;

// Сетка шариков [x][y]
NPC ballgrid[20][20];

int ballCount = 0;
int currentCount = 0;

// Уровень угрозы в игре
int ThreatLevel = 1;

/*
    Коды интерфейсов
    1 = главное меню
    2 = игра
    3 = таблица рекордов
    4 = настройки
*/
int interface;

SDL_Surface* BallSurface;
SDL_Surface* gScreenSurface = NULL;
BACKGROUND backg;
NPC* destructionStart = NULL;

// --- Прототипы функций

// Запускает все подсистемы SDL и создаёт окно
int init();

// Отрисовывает таблицу рекордов в соответствующем пункте меню
void renderLeaderboard();

// Загружает шарики в память
int loadMedia();

int getstring = 1;

// Выгружает ресурсы из памяти и закрывает программу
void closing();

// Возвращает SDL_Surface с игровыми ассетами. Если не загружены, сначала их загружает в память.
SDL_Surface* loadSurface(char* path);

// Создаёт «игрока» - PLAYER
PLAYER createPLAYER(float posX, float posY, float stepX, float stepY, int color, SDL_Surface* image);

// Создаёт NPC - неиграбельные шарики
NPC createNPC(float posY, float posX, int indexY, int indexX, int color, SDL_Surface* image);

// Создаёт UI элемент
UIELEMENT createELEMENT(float posX, float posY, int color, SDL_Surface* image);

// Передвигает шар-«игрока»
void movePLAYER();

// Возвращает SDL_Surface шарика цвета color
SDL_Surface* getBallColor(int color);

// Отрисовывает количество очков
void drawScore();

// Меняет цвет интерфейса, основываясь на уровне угрозы
void changeUI();

// Меняет цвет жизней
void changeLifes();

// Проверяет коллизию с NPC
NPC* checkCollision();

// Проверяет коллизию со стенами
void WallCollision();

// Проверяет коллизию с потолком
NPC* CeilingCollision();

// Проверяет коллизию с NPC
NPC* NPCCollision();

// Проверяет все типы коллизий NPC
NPC* collision();

// Уничтожает стоящие рядом шарики одного цвета
void checkAround(NPC* npc, int checkcolor);

// Проверяет, достаточно ли шариков для вызова функции их уничтожения
void checkDestruction(NPC* npc, int checkcolor);

// Проверяет игровое поле на наличие «островков»
void checkIsland(NPC* npc);

// Уничтожает «островки»
void destroyIsland(int ScoreOn);

// Инициализирует переменные, необходимые для работы программы, и создает UI элементы
int PrepareGame();

// Завершает игру
int playEnd();

// Проверяет выигрыш
int playWin();

// Обновляет экран
void RefreshScreen();

// [Отладка] Выводит сетку шариков в консоль
void printGrid();

// Сдвигает сетку шариков вниз
void gridDown();

// Создаёт сетку шариков
void createGrid(int ballY);

// Загружает фон, основываясь на коде интерфейса
void makeBACKGROUND();

// Очищает сетку шариков
void cleanGrid();

// Отображает «игрока» на экране
void drawPLAYER(PLAYER p);

// Отображает шарики-NPC
void drawNPC(NPC n);

// Отображает фон
void drawBACKGROUND(BACKGROUND b);

// Отображает UI элемент на экране
void drawELEMENT(UIELEMENT u, int imageW, int imageH);

// Основная «входная» функция, которая обеспечивает менеджмент интерфейсов
void Game();

// Загружает UI завершения игры
void EndGameUI();

// Подготавливает и запускает игру
void PreparePlay();

// Основная игровая функция с логикой игры
void Play();

// Функция обработки кнопок в главном меню
void MainMenu();

// Функция обработки кнопок на странице рекордов
void Highscores();

// Функция обработки кнопок в меню настроек
void Settings();

// Функция обработки экранных кнопок
void Buttons(SDL_Event e);

// Выстреливает шариком в направлении курсора
void shoot();

// Позволяет изменять громкость звуков и музыки с шагом 10 единиц
void changeVolume(int type, int side);

// Отрисовывает уровни громкости в меню настроек
void renderVolume();

// =======================================================================

int main(int argc, char* args[])
{
    int error;

    // Подготовка к игре
    error = PrepareGame();

    // Выход при обнаружении ошибок на стадии подготовки
    if (error) return error;

    // Запуск музыки
    Mix_PlayMusic(music, -1);

    // Основной цикл игры
    while (!quit) Game();

    // Освобождение памяти, завершение работы подсистем SDL и всей программы
    closing();

    return 0;
}


void movePLAYER()
{
    ball.posX += ball.stepX;
    ball.posY += ball.stepY;
    ball.centerX = ball.posX + IMAGE_WIDTH / 2;
    ball.centerY = ball.posY + IMAGE_HEIGHT / 2;

    // Проверка коллизии со стеной
    WallCollision();

}

NPC* collision()
{
    NPC* n;

    // Проверка на коллизию NPC
    n = NPCCollision();

    // Проверка на коллизию с потолком
    if (n == NULL) n = CeilingCollision();
    else 
    {
        ball.posX = (SCREEN_WIDTH / 2 - IMAGE_WIDTH / 2) + 1;
        ball.posY = (SCREEN_HEIGHT - IMAGE_HEIGHT) - 5;
        ball.stepY = 0;
        ball.stepX = 0;
        clicked = 0;
        health--;
        changeUI();
    }
    return n;
}

void WallCollision()
{
    if ((ball.posX + IMAGE_WIDTH > SCREEN_WIDTH - BORDER) ||
        (ball.posX < BORDER))
    {
        ball.stepX = -ball.stepX;
        ball.posX += ball.stepX;
        Mix_PlayChannel(-1, synthPop, 0);
    }
}

NPC* CeilingCollision()
{
    int ballcolor;
    int newX;

    if (ball.posY < BORDER + 5)
    {
        newX = (int)((ball.posX) / IMAGE_WIDTH);
        if (ball.posX > newX * IMAGE_WIDTH + IMAGE_WIDTH / 2) newX++;

        ballgrid[1][newX] = createNPC(
            IMAGE_HEIGHT - 5,
            newX * IMAGE_WIDTH + IMAGE_WIDTH / 4,
            1,
            newX,
            ball.color,
            ball.image
        );

        ball.image = NULL;

        ballCount = 0;
        currentCount = 0;
        checkDestruction(&ballgrid[1][newX], ballgrid[1][newX].color);
        drawNPC(ballgrid[1][newX]);

        ball.color = nextball.color;
        ball.image = nextball.image;
        ballcolor = rand() % COLORS + 1;
        nextball.color = ballcolor;
        nextball.image = getBallColor(ballcolor);

        return &ballgrid[0][newX];
    }
    return NULL;
}

NPC* checkCollision()
{
    int i, j;
    float dist, distX, distY;

    for (i = 1; i < BALLY; i++)
        for (j = 1; j <= BALLX; j++)
        {
            if (ballgrid[i][j].color) {

                distX = ballgrid[i][j].centerX - (ball.centerX);
                distY = ballgrid[i][j].centerY - (ball.centerY);
                dist = sqrt(pow(distX, 2) + pow(distY, 2));

                ballgrid[i][j].distX = distX;
                ballgrid[i][j].distY = distY;
                ballgrid[i][j].dist = dist;

                if (dist < IMAGE_WIDTH - COLRADIUS)
                {
                    /*
                    Коды типов коллизии по сторонам (О - шарик в центре):
                       6 1
                      5 O 2
                       4 3
                    */
                    if (ball.centerX > ballgrid[i][j].centerX)
                    {
                        if (ball.centerY < IMAGE_HEIGHT / 5 + ballgrid[i][j].posY) ballgrid[i][j].coltype = 1;
                        else if (ball.centerY > (IMAGE_HEIGHT / 3) * 2 + ballgrid[i][j].posY + 1) ballgrid[i][j].coltype = 3;
                        else ballgrid[i][j].coltype = 2;
                    }

                    else
                    {
                        if (ball.centerY < IMAGE_HEIGHT / 5 + ballgrid[i][j].posY) ballgrid[i][j].coltype = 6;
                        else if (ball.centerY > (IMAGE_HEIGHT / 3) * 2 + ballgrid[i][j].posY + 1) ballgrid[i][j].coltype = 4;
                        else ballgrid[i][j].coltype = 5;
                    }
                    return &ballgrid[i][j];
                }
            }
        }

    return NULL;
}

NPC* NPCCollision()
{
    int m, n;
    int ballcolor;
    NPC* colNPC, * newNPC;

    colNPC = checkCollision();

    /*
    Коды типов коллизии по сторонам (О - шарик в центре):
           6 1
          5 O 2
           4 3
    */

    if (colNPC)
    {
        // Коллизия есть!
        switch (colNPC->coltype)
        {
        case 1:
            if (colNPC->indexY % 2 == 0) {
                m = -1;
                n = 0;
            }
            else {
                m = -1;
                n = +1;
            }
            break;
        case 2:
            m = 0;
            n = +1;
            break;
        case 3:
            if (colNPC->indexY % 2 == 0 || colNPC->indexX == 18) {
                m = +1;
                n = 0;
            }
            else {
                m = +1;
                n = +1;
            }
            break;
        case 4:
            if (colNPC->indexY % 2 == 0 && colNPC->indexX != 1) {
                m = +1;
                n = -1;
            }
            else {
                m = +1;
                n = 0;
            }
            break;
        case 5:
            m = 0;
            n = -1;
            break;
        case 6:
            if (colNPC->indexY % 2 == 0) {
                m = -1;
                n = -1;
            }
            else {
                m = -1;
                n = 0;
            }
        }

        // Создаём новый NPC
        ballgrid[(colNPC->indexY) + m][(colNPC->indexX) + n] = createNPC(
            ((colNPC->indexY) + m) * (IMAGE_HEIGHT - 5),
            (colNPC->indexX) * IMAGE_WIDTH,
            ((colNPC->indexY) + m),
            ((colNPC->indexX) + n),
            ball.color,
            ball.image
        );
        newNPC = &ballgrid[(colNPC->indexY) + m][(colNPC->indexX) + n];

        ball.image = NULL;

        // Передвигаем NPC
        if (colNPC->coltype <= 3)
            newNPC->posX += IMAGE_WIDTH / 2;
        if ((colNPC->coltype == 2 || colNPC->coltype == 5) && ((colNPC->indexY) % 2 == 1))
            newNPC->posX += IMAGE_WIDTH / 2;
        if (colNPC->coltype == 5) {
            newNPC->posX -= IMAGE_WIDTH / 2;
        }
        newNPC->posX += n * IMAGE_WIDTH / 2;

        newNPC->posX -= IMAGE_WIDTH / 4;


        newNPC->centerX = newNPC->posX + IMAGE_WIDTH / 2;
        newNPC->centerY = newNPC->posY + IMAGE_WIDTH / 2;

        // Проверка на выход шарика за пределы игрового поля
        if (newNPC->posX < IMAGE_WIDTH && (newNPC->indexY) % 2 == 1) {
            newNPC->posX += IMAGE_WIDTH / 2;
        }
        if (newNPC->posX > SCREEN_WIDTH - (BORDER + 3 * IMAGE_WIDTH / 2) && (newNPC->indexY) % 2 == 0) {
            newNPC->posX -= IMAGE_WIDTH / 2;
        }

        ballCount = 0;
        currentCount = 0;
        checkDestruction(newNPC, newNPC->color);
        colNPC->coltype = 0;

        ball.color = nextball.color;
        ball.image = nextball.image;
        ballcolor = rand() % COLORS + 1;
        nextball.color = ballcolor;
        nextball.image = getBallColor(ballcolor);

        return newNPC;
    }
    return NULL;
}

UIELEMENT createELEMENT(float posX, float posY, int color, SDL_Surface* image)
{
    UIELEMENT u;

    u.posX = posX;
    u.posY = posY;
    u.color = color;
    u.image = image;
    return u;
}

PLAYER createPLAYER(float posX, float posY,
    float stepX, float stepY,
    int color, SDL_Surface* image)
{
    PLAYER p;

    p.posX = posX;
    p.posY = posY;
    p.stepX = stepX;
    p.stepY = stepY;
    p.color = color;
    p.image = image;
    p.centerX = posX + IMAGE_WIDTH / 2;
    p.centerY = posY + IMAGE_HEIGHT / 2;
    return p;
}

NPC createNPC(float posY, float posX,
    int indexY, int indexX,
    int color, SDL_Surface* image)
{
    NPC n;

    n.posX = posX;
    n.posY = posY;
    n.indexY = indexY;
    n.indexX = indexX;
    n.color = color;
    n.image = image;
    n.centerX = posX + IMAGE_WIDTH / 2;
    n.centerY = posY + IMAGE_HEIGHT / 2;
    n.coltype = 0;
    n.remain = 0;

    return n;

}

void makeBACKGROUND()
{
    if (interface == 1) backg.image = loadSurface(menuBG);
    if (interface == 2) backg.image = loadSurface(gameBG);
    if (interface == 3 || interface == 4) backg.image = loadSurface(menuBG2);
}

SDL_Surface* getBallColor(int color)
{
    /*
    Коды цветов шариков
    0 = null
    1 = red
    2 = orange
    3 = yellow
    4 = green
    5 = blue
    6 = purple
    */

    switch (color)
    {
    case 1: return loadSurface(color1);
    case 2: return loadSurface(color2);
    case 3: return loadSurface(color3);
    case 4: return loadSurface(color4);
    case 5: return loadSurface(color5);
    case 6: return loadSurface(color6);
    }
    
    return NULL;
}

void cleanGrid() {
    int i, j;

    for (i = 1; i < BALLY; i++) {
        for (j = 1; j < BALLX; j++) {
            ballgrid[i][j].indexX = 0;
            ballgrid[i][j].indexY = 0;
            ballgrid[i][j].posX = 0;
            ballgrid[i][j].posY = 0;
            ballgrid[i][j].centerX = 0;
            ballgrid[i][j].centerY = 0;
            ballgrid[i][j].color = 0;
        }
    }
}

void createGrid(int ballY)
{
    int i;

    /*LEMBRAR DE TROCAR ISTO QUANDO FOR PARA MATRIZ*/
    for (i = 1; i < ballY; i++)
    {
        gridDown();
        SDL_Delay(10 * DELAY);
        RefreshScreen();
    }
}

void drawPLAYER(PLAYER p)
{
    SDL_Rect srcRect, dstRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = IMAGE_WIDTH;
    srcRect.h = IMAGE_HEIGHT;
    dstRect.x = p.posX;
    dstRect.y = p.posY;
    SDL_BlitSurface(p.image, &srcRect, gScreenSurface, &dstRect);
}

void drawBACKGROUND(BACKGROUND b)
{
    SDL_Rect srcRect, dstRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = SCREEN_WIDTH;
    srcRect.h = SCREEN_HEIGHT;
    dstRect.x = 0;
    dstRect.y = 0;
    SDL_BlitSurface(b.image, &srcRect, gScreenSurface, &dstRect);
}

void drawELEMENT(UIELEMENT u, int imageW, int imageH)
{
    SDL_Rect srcRect, dstRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = imageW;
    srcRect.h = imageH;
    dstRect.x = u.posX;
    dstRect.y = u.posY;
    SDL_BlitSurface(u.image, &srcRect, gScreenSurface, &dstRect);
}

void drawNPC(NPC n)
{
    if (n.color)
    {
        SDL_Rect srcRect, dstRect;
        srcRect.x = 0; srcRect.y = 0;
        srcRect.w = IMAGE_WIDTH;
        srcRect.h = IMAGE_HEIGHT;
        dstRect.x = n.posX;
        dstRect.y = n.posY;
        SDL_BlitSurface(n.image, &srcRect, gScreenSurface, &dstRect);
    }
}

void RefreshScreen()
{
    int i, j;
    drawBACKGROUND(backg);

    /*Main Menu Refresh Screen*/
    if (interface == 1) {
        drawELEMENT(SoundElement, 38, 38);
        drawELEMENT(MenuHoverElement, 210, 73);
    }

    /*Play Refresh Screen*/
    if (interface == 2) {
        drawScore();
        drawELEMENT(PMUI, SCREEN_WIDTH, SCREEN_HEIGHT);
        drawPLAYER(ball);
        drawELEMENT(SoundElement, 38, 38);
        drawELEMENT(EGelement, 38, 38); 
        for (i = 0; i < health; i++) {
            drawELEMENT(lifeballs[i], 10, 10);
        }

        for (i = 0; i < BALLY; i++)
            for (j = 0; j < BALLX; j++)
                drawNPC(ballgrid[i][j]);
        drawELEMENT(nextball, IMAGE_WIDTH, IMAGE_HEIGHT);

        if (play == -1) {
            drawELEMENT(EGUI, 200, 200);
            drawELEMENT(EGMen, 38, 38);
            drawELEMENT(EGR, 38, 38);
            drawELEMENT(EGRank, 38, 38);
            drawScore();
        }

    }

    /*Highscores Refresh Screen*/
    if (interface == 3) {
        drawELEMENT(RankElement, 440, 350);
        renderLeaderboard();
        drawELEMENT(EGelement, 38, 38);
    }

    /*Settings Refresh Screen*/
    if (interface == 4) {
        drawELEMENT(SettingsElement, 440, 350);
        drawELEMENT(EGelement, 38, 38);
        renderVolume();
    }

    /*Update the surface*/
    SDL_UpdateWindowSurface(gWindow);

    //SDL_FreeSurface(gScreenSurface);

    /* Not so good solution, depends on your computer*/
    SDL_Delay(DELAY);
}

void shoot() {
    int Mx, My;

    SDL_GetMouseState(&Mx, &My);

    if (My > SCREEN_HEIGHT - 45) return;

    Mx = Mx - SCREEN_WIDTH / 2;
    My = SCREEN_HEIGHT - My - IMAGE_HEIGHT / 2;

    ball.stepX = Mx / sqrt((Mx * Mx) + (My * My));
    ball.stepY = -My / sqrt((Mx * Mx) + (My * My));


    if (ball.stepX > 0.997)
    {
        ball.stepX = 0.997;
        ball.stepY = -0.0774;
    }

    if (ball.stepX < -0.997)
    {
        ball.stepX = -0.997;
        ball.stepY = -0.0774;
    }

    ball.stepY *= MSPEED;
    ball.stepX *= MSPEED;
    clicked = 1;
    Mix_PlayChannel(-1, shot, 0);
}

void Game() {
    if (!Sound)
    {
        Mix_VolumeChunk(bubblePop, 0);
        Mix_VolumeChunk(synthPop, 0);
        Mix_VolumeChunk(shot, 0);
        Mix_VolumeMusic(0);
    }
    else 
    { 
        Mix_VolumeMusic(musicVolume); 
        Mix_VolumeChunk(bubblePop, soundsVolume);
        Mix_VolumeChunk(synthPop, soundsVolume);
        Mix_VolumeChunk(shot, soundsVolume);
    }

    switch (interface) {
    case 1: /*Main Menu*/
        MainMenu();
        break;
    case 2: /*Play*/
        Play();
        break;
    case 3: /*Highscores*/
        Highscores();
        break;
    case 4: /*Settings*/
        Settings();
        break;
    }
    RefreshScreen();
}

void MainMenu() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        Buttons(e);
        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            else if (e.key.keysym.sym == SDLK_KP_MINUS)
                changeVolume('m', '-');
            else if (e.key.keysym.sym == SDLK_KP_PLUS)
                changeVolume('m', '+');

            break;
        }
    }
}

void Highscores() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        Buttons(e);
        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            break;
        }
    }
}

void Settings() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        Buttons(e);
        switch (e.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
            break;
        }
    }
}

void changeVolume(int type, int side) {
    /*
    type == 's': громкость звуков
    type == 'm': громкость музыки
    
    side == '+': добавить 10 процентов громкости
    side == '-': вычесть 10 процентов громкости 
    */

    switch (type)
    {
    case 's':
        if      (side == '+' && soundsVolume < 100) soundsVolume += 10;
        else if (side == '-' && soundsVolume > 0) soundsVolume -= 10;
        break;
    case 'm':
        if      (side == '+' && musicVolume < 100) musicVolume += 10;
        else if (side == '-' && musicVolume > 0) musicVolume -= 10;
        break;
    default:
        break;
    }

    Mix_VolumeChunk(bubblePop, soundsVolume);
    Mix_VolumeChunk(synthPop, soundsVolume);
    Mix_VolumeChunk(shot, soundsVolume);
    Mix_VolumeMusic(musicVolume);

}

void Buttons(SDL_Event e) {
    int Mx, My;
    int ballcolor;

    /*
    Коды интерфейсов
    1 = главное меню
    2 = игра
    3 = таблица рекордов
    4 = настройки
    */

    SDL_GetMouseState(&Mx, &My);

    // interface == 1: Главное меню
    if (interface == 1) {
        // Кнопка звука (вкл/выкл)
        SoundElement.posX = 3;
        SoundElement.posY = (SCREEN_HEIGHT - 41) + 4;
        if (Mx < (41) && Mx >(3)
            && My > (SCREEN_HEIGHT - 41) && My < (SCREEN_HEIGHT - 41 + 38)) {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (Sound) Sound = false;
                else Sound = true;
            }
            if (Sound == true)
                SoundElement.image = loadSurface(soundOnHoverBlue);
            else if (Sound == false)
                SoundElement.image = loadSurface(soundOffHoverBlue);
        }
        else {
            if (Sound == true)
                SoundElement.image = loadSurface(soundOnBlue);
            else if (Sound == false)
                SoundElement.image = loadSurface(soundOffBlue);
        }

        MenuHoverElement.posX = 56;

        // Кнопки главного меню
        if (!(Mx > 70 && Mx < 250 && My > 210 && My < 359)) {
            MenuHoverElement.image = NULL;
        }
        else if (Mx > 70 && Mx < 250) {
            MenuHoverElement.image = loadSurface(arrow);
            if (My > 210 && My < 235) {
                MenuHoverElement.posY = 184;
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    interface = 2;
                    makeBACKGROUND();
                }
            }
            if (My > 250 && My < 275) {
                MenuHoverElement.posY = 228;
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    interface = 3;
                    makeBACKGROUND();
                }
            }
            if (My > 290 && My < 317) {
                MenuHoverElement.posY = 269;
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    interface = 4;
                    makeBACKGROUND();
                }
            }
            if (My > 332 && My < 359) {
                MenuHoverElement.posY = 311;
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    quit = true;
                }
            }
        }


    }

    /*
    Координаты кнопок
    Формат:
    Левый верхний угол x y || Правый нижний угол x y
    -------------------------------------------------
    Music  -: 190 160 || 217 186
    Music  +: 426 160 || 454 186
    Sounds -: 190 212 || 217 240
    Sounds +: 426 212 || 454 240
    */

    // interface == 4: Меню настроек
    if (interface == 4)
    {

        /*printf(
            "-------------------\n"
            "Mx: %3d |+| My: %3d\n", Mx, My);*/
        
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            if (Mx > 426 && Mx < 454) // Кнопки «+»
            {
                if (My > 160 && My < 186) // Музыка +
                {
                    changeVolume('m', '+');
                    printf("Music + 10 = %d\%.\n", musicVolume);
                }
                else if (My > 212 && My < 240) // Звуки +
                {
                    changeVolume('s', '+');
                    printf("Sounds + 10 = %d\%.\n", soundsVolume);

                }
            }
            else if (Mx > 190 && Mx < 228) // Кнопки «-»
            {
                if (My > 160 && My < 186) // Музыка -
                {
                    changeVolume('m', '-');
                    printf("Music - 10 = %d\%.\n", musicVolume);
                }
                else if (My > 212 && My < 240) // Звуки -
                {
                    changeVolume('s', '-');
                    printf("Sounds - 10 = %d\%.\n", soundsVolume);

                }
            }

        }
    }


    if ((interface == 2 || interface == 3 || interface == 4)) {
        EGelement.posX = 577;
        EGelement.posY = (SCREEN_HEIGHT - 41) + 4;

        if ((Mx < (SCREEN_WIDTH - 2 * IMAGE_WIDTH + 38) && Mx > (SCREEN_WIDTH - 2 * IMAGE_WIDTH - 2)
            && My > (SCREEN_HEIGHT - 41) && My < (SCREEN_HEIGHT - 41 + 38))
            || (((Mx < 268) && (Mx > 238) && (My < 274) && (My > 238)) && (play == -1))) {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                interface = 1;
                play = 0;
                ball.posX = (SCREEN_WIDTH / 2 - IMAGE_WIDTH / 2);
                ball.posY = (SCREEN_HEIGHT - IMAGE_HEIGHT) - 4;
                ball.stepX = 0;
                ball.stepY = 0;
                ballcolor = rand() % COLORS + 1;
                ball.color = ballcolor;
                ball.image = getBallColor(ballcolor);
                ballcolor = rand() % COLORS + 1;
                nextball.image = getBallColor(ballcolor);
                nextball.color = ballcolor;
                MenuHoverElement.image = NULL;
                ThreatLevel = 1;

                cleanGrid();
                makeBACKGROUND();
                EGelement.image = loadSurface(menuBlue);

            }

            if (!(Mx < (268) && Mx >(238) && My < (274) && My >(238))) {
                switch (ThreatLevel) {
                case 1: EGelement.image = loadSurface(menuHoverBlue); break;
                case 2: EGelement.image = loadSurface(menuHoverYellow); break;
                case 3: EGelement.image = loadSurface(menuHoverRed); break;
                }
            }
        }
        else {
            switch (ThreatLevel) {
            case 1: EGelement.image = loadSurface(menuBlue); break;
            case 2: EGelement.image = loadSurface(menuYellow); break;
            case 3: EGelement.image = loadSurface(menuRed); break;
            }
        }

        // Кнопка звука (вкл/выкл)
        SoundElement.posX = 30;
        SoundElement.posY = (SCREEN_HEIGHT - 41) + 4;
        if (Mx < (66) && Mx >(28)
            && My > (SCREEN_HEIGHT - 41) && My < (SCREEN_HEIGHT - 41 + 38)) {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (Sound) Sound = false;
                else Sound = true;
            }
            if (Sound == true) {
                switch (ThreatLevel) {
                case 1: SoundElement.image = loadSurface(soundOnHoverBlue); break;
                case 2: SoundElement.image = loadSurface(soundOnHoverYellow); break;
                case 3: SoundElement.image = loadSurface(soundOnHoverRed); break;
                }
            }
            else if (Sound == false) {
                switch (ThreatLevel) {
                case 1: SoundElement.image = loadSurface(soundOffHoverBlue); break;
                case 2: SoundElement.image = loadSurface(soundOffHoverYellow); break;
                case 3: SoundElement.image = loadSurface(soundOffHoverRed); break;
                }
            }
        }
        else {
            if (Sound == true) {
                switch (ThreatLevel) {
                case 1: SoundElement.image = loadSurface(soundOnBlue); break;
                case 2: SoundElement.image = loadSurface(soundOnYellow); break;
                case 3: SoundElement.image = loadSurface(soundOnRed); break;
                }
            }
            else if (Sound == false) {
                switch (ThreatLevel) {
                case 1: SoundElement.image = loadSurface(soundOffBlue); break;
                case 2: SoundElement.image = loadSurface(soundOffYellow); break;
                case 3: SoundElement.image = loadSurface(soundOffRed); break;
                }
            }
        }

        // Кнопка возврата в главное меню
        if (Mx < (268) && Mx >(238)
            && My < (274) && My >(238)) {
            if (ThreatLevel == 3) EGMen.image = loadSurface(menuHoverRed);
            if (ThreatLevel == 1) EGMen.image = loadSurface(menuHoverBlue);
        }
        else {
            if (ThreatLevel == 3) EGMen.image = loadSurface(menuRed);
            if (ThreatLevel == 1) EGMen.image = loadSurface(menuBlue);
        }

        // Кнопка перезапуска игры
        if (Mx < (335) && Mx >(305)
            && My < (306) && My >(272)) {
            if (ThreatLevel == 3) EGR.image = loadSurface(returnHoverRed);
            if (ThreatLevel == 1) EGR.image = loadSurface(returnHoverBlue);
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && play == -1) {
                EGUI.image = NULL;
                EGR.image = NULL;
                EGRank.image = NULL;
                EGMen.image = NULL;
                play = 0;
            }
        }
        else {
            if (ThreatLevel == 3) EGR.image = loadSurface(returnRed);
            if (ThreatLevel == 1) EGR.image = loadSurface(returnBlue);
        }

        if (Mx < (402) && Mx >(372)
            && My < (274) && My >(238)) {
            if (ThreatLevel == 3) EGRank.image = loadSurface(rankHoverRed);
            if (ThreatLevel == 1) EGRank.image = loadSurface(rankHoverBlue);
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && play == -1) {
                EGUI.image = NULL;
                EGR.image = NULL;
                EGRank.image = NULL;
                EGMen.image = NULL;
                interface = 3;
                ThreatLevel = 1;
                makeBACKGROUND();
                play = 0;
            }
        }
        else {
            if (ThreatLevel == 3) EGRank.image = loadSurface(rankRed);
            if (ThreatLevel == 1) EGRank.image = loadSurface(rankBlue);
        }
    }
}

void Play() {
    SDL_Event e;
    interface = 2;

    if (!play) {
        PreparePlay();
    }

    while (SDL_PollEvent(&e) != 0)
    {
        Buttons(e);
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE: quit = true;
                break;
            }
        }
        else if ((e.type == SDL_MOUSEBUTTONDOWN) && e.button.button == SDL_BUTTON_LEFT && !clicked) shoot();
    }

    if (clicked)
    {
        movePLAYER();
        collision();
    }

    if (maxhealth == 0) {
        maxhealth = 6;
    }

    if (health == 0) {
        gridDown();
        maxhealth--;
        health = maxhealth;
    }

    playWin();
    playEnd();
}

int init() {
    int success = true;

    srand(time(NULL));

    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("Bubble Shooter | Ilyushina Darya", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }

        else
        {
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }

            else
            {
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }

            // Инициализация SDL_ttf
            if (TTF_Init() == -1) {
                printf("SDL could not initialize TTF! SDL Error: %s\n", SDL_GetError());
                success = false;
            }

            // Инициализация SDL_mixer
            if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
                success = false;

            // Загрузка звуковых эффектов
            bubblePop = Mix_LoadWAV(bubblePop_path);
            if (bubblePop == NULL)
                success = false;

            synthPop = Mix_LoadWAV(synthPop_path);
            if (synthPop == NULL)
                success = false;

            shot = Mix_LoadWAV(shot_path);
            if (shot == NULL)
                success = false;

            // Загрузка музыки
            music = Mix_LoadMUS(music_path);
            if (music == NULL)
                success = false;
        }
    }

    return success;
}

int loadMedia()
{
    int success = true;

    if (BallSurface == NULL)
    {
        printf("Failed to load image! SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    else
    {
        Uint32 colorkey = SDL_MapRGBA(BallSurface->format, 0x00, 0x00, 0x00, 0xFF);
        SDL_SetColorKey(BallSurface, 1, colorkey);
    }
    return success;
}

void closing()
{
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;

    if (BallSurface != NULL)
        SDL_FreeSurface(BallSurface);

    if (SoundElement.image != NULL)
        SDL_FreeSurface(SoundElement.image);

    if (EGelement.image != NULL)
        SDL_FreeSurface(EGelement.image);

    if (EGelement.image != NULL)
        SDL_FreeSurface(MenuHoverElement.image);

    if (EGelement.image != NULL)
        SDL_FreeSurface(backg.image);

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Закрытие подсистем SDL
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface(char* path)
{
    if (requestSurface(path) == NULL) {
        SDL_Surface* loadedSurface = IMG_Load(path);
        if (loadedSurface == NULL)
        {
            printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
            exit(502);
        }
        return loadedSurface;
    }
    else return requestSurface(path);
}

int PrepareGame()
{
    int ballcolor;
    int i;

    if (!init())
    {
        printf("Failed to initialize!\n");
        return 1;
    }

    ballcolor = rand() % COLORS + 1;
    BallSurface = getBallColor(ballcolor);

    resourceInit();
    leaderboardInit(&leaderboard);
    
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        return 2;
    }


    interface = 1;
    /*####*/
    Sound = true;

    makeBACKGROUND();

    // Создание «игрока»
    ball = createPLAYER(
        (SCREEN_WIDTH / 2 - IMAGE_WIDTH / 2) + 1,
        (SCREEN_HEIGHT - IMAGE_HEIGHT) - 5,
        0,
        0,
        ballcolor,
        getBallColor(ballcolor));

    // Создание следующего шарика
    ballcolor = rand() % COLORS + 1;
    BallSurface = getBallColor(ballcolor);

    nextball = createELEMENT(
        135,
        (SCREEN_HEIGHT - IMAGE_HEIGHT) - 5,
        ballcolor,
        getBallColor(ballcolor));

    for (i = 0; i < 5; i++) {
        lifeballs[i] = createELEMENT(
            (SCREEN_WIDTH / 3) - 24 + (i * 7) + 56,
            (i % 2) ? (SCREEN_HEIGHT - IMAGE_HEIGHT) + 12 : (SCREEN_HEIGHT - IMAGE_HEIGHT) + 0,
            0,
            loadSurface(lifeBlue));
    }

    SoundElement = createELEMENT(
        28 + 4,
        (SCREEN_HEIGHT - 41) + 6,
        0,
        NULL);

    EGelement = createELEMENT(
        SCREEN_WIDTH - 2 * IMAGE_WIDTH - 2 + 4,
        (SCREEN_HEIGHT - 41) + 6,
        0,
        NULL);

    MenuHoverElement = createELEMENT(
        79,
        -38,
        0,
        NULL);

    PMUI = createELEMENT(
        0,
        0,
        0,
        NULL);

    EGUI = createELEMENT(
        SCREEN_WIDTH / 2 - 100,
        SCREEN_HEIGHT / 2 - 125,
        0,
        NULL);

    EGMen = createELEMENT(
        238,
        238,
        0,
        NULL);

    EGR = createELEMENT(
        305,
        272,
        0,
        NULL);

    EGRank = createELEMENT(
        373,
        238,
        0,
        NULL);

    SettingsElement = createELEMENT(
        SCREEN_WIDTH / 2 - 220,
        SCREEN_HEIGHT / 2 - 220,
        0,
        loadSurface(settings));

    RankElement = createELEMENT(
        SCREEN_WIDTH / 2 - 220,
        SCREEN_HEIGHT / 2 - 220,
        0,
        loadSurface(topPlayers));

    return 0;
}

void PreparePlay() {
    maxhealth = 5;
    ThreatLevel = 1;
    health = maxhealth;
    
    changeLifes();
    Score = 0;
    clicked = 0;
    createGrid(GRIDY);
    ball.image = getBallColor(ball.color);
    nextball.image = getBallColor(nextball.color);
    play = 1;
}

void renderVolume() {
    SDL_Surface* fontSurface;
    TTF_Font* font;
    SDL_Color fontColor = { 52,152,219 };

    SDL_Surface* shadowSurface;
    TTF_Font* shadow;
    SDL_Color shadowColor = { 0,0,0 };

    SDL_Rect scoreRect;

    scoreRect.x = SCREEN_WIDTH / 2 + 50;
    scoreRect.y = 153;
    scoreRect.w = 100;
    scoreRect.h = 38;

    int volumes[2] = { musicVolume, soundsVolume };
    char volumeString[16];
    font = TTF_OpenFont(font_path, 27);
    shadow = TTF_OpenFont(font_path, 27);

    TTF_SetFontOutline(shadow, 1);

    for (int i = 0; i < 2; i++)
    {
        sprintf(volumeString, u8"%d%%", volumes[i]);
        shadowSurface = TTF_RenderUTF8_Blended(shadow, volumeString, shadowColor);
        fontSurface = TTF_RenderUTF8_Blended(font, volumeString, fontColor);
        SDL_BlitSurface(shadowSurface, NULL, gScreenSurface, &scoreRect);
        SDL_BlitSurface(fontSurface, NULL, gScreenSurface, &scoreRect);
        SDL_FreeSurface(shadowSurface);
        SDL_FreeSurface(fontSurface);
        scoreRect.y += 55;
    }

    TTF_CloseFont(font);
    TTF_CloseFont(shadow);
}

void renderLeaderboard() {
    SDL_Surface* fontSurface;
    TTF_Font* font;
    SDL_Color fontColor = { 52,152,219 };

    SDL_Surface* shadowSurface;
    TTF_Font* shadow;
    SDL_Color shadowColor = { 0,0,0 };

    SDL_Rect scoreRect;

    scoreRect.x = SCREEN_WIDTH / 2 - 50;
    scoreRect.y = 95;
    scoreRect.w = 100;
    scoreRect.h = 38;

    char scoreString[16];
    font = TTF_OpenFont(font_path, 27);
    shadow = TTF_OpenFont(font_path, 27);

    TTF_SetFontOutline(shadow, 1);
    
    for (int i = 0; i < 8; i++)
    {
        sprintf(scoreString, u8"%012d", leaderboard[i]);
        shadowSurface = TTF_RenderUTF8_Blended(shadow, scoreString, shadowColor);
        fontSurface = TTF_RenderUTF8_Blended(font, scoreString, fontColor);
        SDL_BlitSurface(shadowSurface, NULL, gScreenSurface, &scoreRect);
        SDL_BlitSurface(fontSurface, NULL, gScreenSurface, &scoreRect);
        SDL_FreeSurface(shadowSurface);
        SDL_FreeSurface(fontSurface);
        scoreRect.y += 33;
    }

    TTF_CloseFont(font);
    TTF_CloseFont(shadow);
}

int playWin() {
    int i, j;
    for (i = 1, j = 0; j < BALLX; j++) {
        if (ballgrid[i][j].color) {
            return 0;
        }
    }
    ball.image = NULL;
    nextball.image = NULL;
    clicked = 1;
    play = -1;
    Score <<= 1;
    cleanGrid();
    EndGameUI();
    return 1;
}

int playEnd() {
    int i, j, stop = false;
    for (i = 16, j = 0; j < BALLX; j++) {
        if (ballgrid[i][j].color) {
            stop = true;
            break;
        }
    }
    if (!stop) return 0;
    for (i = 1; i < BALLY - 1; i++) {
        for (j = 1; j < GRIDX; j++) {
            ballgrid[i][j].color = 0;
        }
        SDL_Delay(DELAY * 5);
        RefreshScreen();
    }
    ball.image = NULL;
    nextball.image = NULL;
    clicked = 1;
    play = -1;
    cleanGrid();
    EndGameUI();
    leaderboardUpdate(&leaderboard, Score);
    return 1;
}

void printGrid() {
    int i, j;
    for (i = 0; i < BALLY; i++) {
        for (j = 0; j < BALLX; j++) {
            if (i % 2 == 0) printf("%d ", ballgrid[i][j].color);
            else printf(" %d", ballgrid[i][j].color);
        }
        printf("\n");
    }
    printf("---\n");
}

void gridDown()
{
    int i, j, ballcolor;

    for (i = BALLY - 1; i > 0; i--)
    {
        for (j = 1; j < GRIDX; j++)
        {
            if (ballgrid[i][j].color) {
                ballgrid[i + 1][j] = createNPC(
                    (i + 1) * (IMAGE_HEIGHT - 5),
                    j * IMAGE_WIDTH + ((i + 1) % 2 * IMAGE_WIDTH / 2) - IMAGE_WIDTH / 4,
                    i + 1,
                    j,
                    ballgrid[i][j].color,
                    getBallColor(ballgrid[i][j].color)
                );
                ballgrid[i][j].color = 0;
                drawNPC(ballgrid[i + 1][j]);
            }
        }
    }
    /* ### */
    for (j = 1; j <= GRIDX; j++)
    {
        ballcolor = rand() % COLORS + 1;
        BallSurface = getBallColor(ballcolor);
        ballgrid[1][j] = createNPC(
            1 * (IMAGE_HEIGHT - 5),
            j * IMAGE_WIDTH + (1 % 2 * IMAGE_WIDTH / 2) - IMAGE_WIDTH / 4,
            1,
            j,
            ballcolor,
            BallSurface
        );
        drawNPC(ballgrid[1][j]);
    }
    ballgrid[1][GRIDX].color = 0;
    for (j = 1; j < GRIDX; j++) {
        if (ballgrid[1][j].color) {
            checkIsland(&ballgrid[1][j]);
        }

    }

    destroyIsland(false);
    changeUI();
    changeLifes();
}

void checkIsland(NPC* npc)
{
    int n;
    npc->remain = 1;

    /*
      Коды типов коллизии:
           6 1
          5 O 2
           4 3
    */

    if (npc->indexX < 0 && npc->indexX > BALLX) return;

    for (n = 0; n <= 1; n++) {
        if ((npc->indexY) % 2 == n) {
            // Тип коллизии: 3
            if (ballgrid[(npc->indexY) + 1][(npc->indexX) + n].color && ballgrid[(npc->indexY) + 1][(npc->indexX) + n].remain == 0) {
                checkIsland(&ballgrid[(npc->indexY) + 1][(npc->indexX) + n]);
            }
            // Тип коллизии: 4
            if (ballgrid[(npc->indexY) - 1][(npc->indexX) + n].color && ballgrid[(npc->indexY) - 1][(npc->indexX) + n].remain == 0) {
                checkIsland(&ballgrid[(npc->indexY) - 1][(npc->indexX) + n]);
            }
            // Тип коллизии: 6
            if (ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1].color && ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1].remain == 0) {
                checkIsland(&ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1]);
            }
            // Тип коллизии: 4
            if (ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1].color && ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1].remain == 0) {
                checkIsland(&ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1]);
            }
        }
    }
    // Тип коллизии: 2
    if (ballgrid[(npc->indexY)][(npc->indexX) - 1].color && ballgrid[(npc->indexY)][(npc->indexX) - 1].remain == 0) {
        checkIsland(&ballgrid[(npc->indexY)][(npc->indexX) - 1]);
    }
    // Тип коллизии: 5
    if (ballgrid[(npc->indexY)][(npc->indexX) + 1].color && ballgrid[(npc->indexY)][(npc->indexX) + 1].remain == 0) {
        checkIsland(&ballgrid[(npc->indexY)][(npc->indexX) + 1]);
    }

    return;
}

void checkAround(NPC* npc, int checkcolor)
{
    int n;

    SDL_Delay(5 * DELAY);
    RefreshScreen();

    /*
      Коды типов коллизии:
           6 1
          5 O 2
           4 3
    */

    if (npc->indexX < 0 && npc->indexX > BALLX) {
        return;
    }

    for (n = 0; n <= 1; n++) {
        if ((npc->indexY) % 2 == n) {
            // Тип коллизии: 3
            if (ballgrid[(npc->indexY) + 1][(npc->indexX) + n].color == checkcolor) {
                npc->color = 0;
                Score += 20;
                ballgrid[(npc->indexY) + 1][(npc->indexX) + n].color = 0;
                checkAround(&ballgrid[(npc->indexY) + 1][(npc->indexX) + n], checkcolor);
            }
            // Тип коллизии: 1
            if (ballgrid[(npc->indexY) - 1][(npc->indexX) + n].color == checkcolor) {
                npc->color = 0;
                Score += 20;
                ballgrid[(npc->indexY) - 1][(npc->indexX) + n].color = 0;
                checkAround(&ballgrid[(npc->indexY) - 1][(npc->indexX) + n], checkcolor);
            }
            // Тип коллизии: 6
            if (ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1].color == checkcolor) {
                npc->color = 0;
                Score += 20;
                ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1].color = 0;
                checkAround(&ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1], checkcolor);
            }
            // Тип коллизии: 4
            if (ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1].color == checkcolor) {
                npc->color = 0;
                Score += 20;
                ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1].color = 0;
                checkAround(&ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1], checkcolor);
            }
        }
    }
    // Тип коллизии: 2
    if (ballgrid[(npc->indexY)][(npc->indexX) - 1].color == checkcolor) {
        npc->color = 0;
        Score += 20;
        ballgrid[(npc->indexY)][(npc->indexX) - 1].color = 0;
        checkAround(&ballgrid[(npc->indexY)][(npc->indexX) - 1], checkcolor);
    }
    // Тип коллизии: 5
    if (ballgrid[(npc->indexY)][(npc->indexX) + 1].color == checkcolor) {
        npc->color = 0;
        Score += 20;
        ballgrid[(npc->indexY)][(npc->indexX) + 1].color = 0;
        checkAround(&ballgrid[(npc->indexY)][(npc->indexX) + 1], checkcolor);
    }

    return;
}

void checkDestruction(NPC* npc, int checkcolor)
{
    int n, j;

    currentCount++;

    if (currentCount == 1 && ballCount == 1) return;

    if (ballCount == 0)
    {
        destructionStart = npc;
    }
    else if (currentCount >= 3)
    {
        currentCount = 0;
        return;
    }
    if (ballCount > 2)
    {
        ballCount = 0;
        Score -= 10;

        checkAround(destructionStart, destructionStart->color);
        Mix_PlayChannel(-1, bubblePop, 0);

        for (j = 1; j < GRIDX; j++) {
            if (ballgrid[1][j].color) {
                checkIsland(&ballgrid[1][j]);
            }

        }
        destroyIsland(true);
        destructionStart = NULL;
        currentCount = 0;
        health++;
        playWin();
        return;
    }

    /*
      Коды типов коллизии:
           6 1
          5 O 2
           4 3
    */

    if (npc->indexX < 0 && npc->indexX > BALLX) return;

    for (n = 0; n <= 1; n++) {
        if ((npc->indexY) % 2 == n) {
            // Тип коллизии: 3
            if (ballgrid[(npc->indexY) + 1][(npc->indexX) + n].color == checkcolor) {
                ballCount++;
                checkDestruction(&ballgrid[(npc->indexY) + 1][(npc->indexX) + n], checkcolor);
            }
            // Тип коллизии: 1
            if (ballgrid[(npc->indexY) - 1][(npc->indexX) + n].color == checkcolor) {
                ballCount++;
                checkDestruction(&ballgrid[(npc->indexY) - 1][(npc->indexX) + n], checkcolor);
            }
            // Тип коллизии: 6
            if (ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1].color == checkcolor) {
                ballCount++;
                checkDestruction(&ballgrid[(npc->indexY) - 1][(npc->indexX) + n - 1], checkcolor);
            }
            // Тип коллизии: 4
            if (ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1].color == checkcolor) {
                ballCount++;
                checkDestruction(&ballgrid[(npc->indexY) + 1][(npc->indexX) + n - 1], checkcolor);
            }
        }
    }
    // Тип коллизии: 2
    if (ballgrid[(npc->indexY)][(npc->indexX) - 1].color == checkcolor) {
        ballCount++;
        checkDestruction(&ballgrid[(npc->indexY)][(npc->indexX) - 1], checkcolor);
    }
    // Тип коллизии: 5
    if (ballgrid[(npc->indexY)][(npc->indexX) + 1].color == checkcolor) {
        ballCount++;
        checkDestruction(&ballgrid[(npc->indexY)][(npc->indexX) + 1], checkcolor);
    }


    return;
}

void destroyIsland(int scoreOn) {
    int i, j;
    for (i = 1; i < BALLY - 1; i++)
        for (j = 1; j < GRIDX; j++) {
            if (ballgrid[i][j].remain) 
                ballgrid[i][j].remain = 0;
            else 
            {
                if (ballgrid[i][j].color) 
                {
                    SDL_Delay(5 * DELAY);
                    RefreshScreen();
                    if (scoreOn) Score += 100;
                }

                ballgrid[i][j].indexX = 0;
                ballgrid[i][j].indexY = 0;
                ballgrid[i][j].posX = 0;
                ballgrid[i][j].posY = 0;
                ballgrid[i][j].centerX = 0;
                ballgrid[i][j].centerY = 0;
                ballgrid[i][j].color = 0;
            }
        }
}

void changeUI() {
    int i, j, stop = false;
    for (i = BALLY - 1; (i > 0 && stop == false); i--)
        for (j = 1; j < GRIDX; j++) 
        {
            if (ballgrid[i][j].color && i == 13) { ThreatLevel = 3; stop = true; break; }
            else if (ballgrid[i][j].color && i == 10) { ThreatLevel = 2; stop = true; break; }
            else if (ballgrid[i][j].color && i < 10) { ThreatLevel = 1; stop = true; break; }
        }
    switch (ThreatLevel) {
    case 1:
        PMUI.image = loadSurface(uiBlue);
        EGelement.image = loadSurface(menuBlue);
        if (Sound) SoundElement.image = loadSurface(soundOnBlue);
        else SoundElement.image = loadSurface(soundOffBlue);

        break;
    case 2:
        PMUI.image = loadSurface(uiYellow);
        EGelement.image = loadSurface(menuYellow);
        if (Sound) SoundElement.image = loadSurface(soundOnYellow);
        else SoundElement.image = loadSurface(soundOffYellow);

        break;
    case 3:
        PMUI.image = loadSurface(uiRed);
        EGelement.image = loadSurface(menuRed);
        if (Sound) SoundElement.image = loadSurface(soundOnRed);
        else SoundElement.image = loadSurface(soundOffRed);

        break;
    }
}

void changeLifes() {
    int i;
    switch (ThreatLevel) {
    case 1:
        for (i = 0; i < health; i++)
            lifeballs[i].image = loadSurface(lifeBlue);

        break;
    case 2:
        for (i = 0; i < health; i++)
            lifeballs[i].image = loadSurface(lifeYellow);

        break;
    case 3:
        for (i = 0; i < health; i++)
            lifeballs[i].image = loadSurface(lifeRed);

        break;
    }
}

void drawScore() {
    // Шрифты
    TTF_Font* font = NULL;
    TTF_Font* shadow = NULL;

    // Цвета щрифтов
    SDL_Color fontColor = { 255, 255, 255 };
    SDL_Color shadowColor = { 0, 0, 0 };

    // SDL_Surface для текста
    SDL_Surface* scoreSurface = NULL;
    SDL_Surface* scoreShadowSurface = NULL;
    SDL_Surface* endScoreSurface = NULL;
    SDL_Surface* endScoreShadowSurface = NULL;

    // SDL_Rect для сообщения с количеством очков
    SDL_Rect messageRect;

    char scoreString[16];

    font = TTF_OpenFont(font_path, 24);
    shadow = TTF_OpenFont(font_path, 24);
    TTF_SetFontOutline(shadow, 1);

    if (font == NULL)
        exit(404);

    switch (ThreatLevel) {
    case 1: // rgb(52, 152, 219)
        fontColor.r = 52;
        fontColor.g = 152;
        fontColor.b = 219;
        break;
    case 2: // rgb(241, 196, 15)
        fontColor.r = 241;
        fontColor.g = 196;
        fontColor.b = 15;
        break;
    case 3: // rgb(231, 76, 60)
        fontColor.r = 231;
        fontColor.g = 76;
        fontColor.b = 60;
        break;
    }

    messageRect.x = SCREEN_WIDTH / 2 + 98;
    messageRect.y = SCREEN_HEIGHT - 37;
    messageRect.w = 100;
    messageRect.h = 38;
    
    sprintf(scoreString, u8"%012d", Score);

    scoreShadowSurface = TTF_RenderUTF8_Blended(shadow, scoreString, shadowColor);
    scoreSurface = TTF_RenderUTF8_Blended(font, scoreString, fontColor);

    if (!scoreSurface) {
        printf("Failed to render text!\n");
        exit(502);
    }

    SDL_BlitSurface(scoreShadowSurface, NULL, gScreenSurface, &messageRect);
    SDL_BlitSurface(scoreSurface, NULL, gScreenSurface, &messageRect);

    if (play == -1) {
        messageRect.x = SCREEN_WIDTH / 2 - 10;
        messageRect.y = SCREEN_HEIGHT / 2 - 69;
        messageRect.w = 200;
        messageRect.h = 76;
        sprintf(scoreString, "%08d", Score);
        endScoreShadowSurface = TTF_RenderUTF8_Blended(shadow, scoreString, shadowColor);
        endScoreSurface = TTF_RenderUTF8_Blended(font, scoreString, fontColor);
        SDL_BlitSurface(endScoreShadowSurface, NULL, gScreenSurface, &messageRect); 
        SDL_BlitSurface(endScoreSurface, NULL, gScreenSurface, &messageRect);

    }

    SDL_FreeSurface(scoreShadowSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(endScoreSurface);
    SDL_FreeSurface(endScoreShadowSurface);
    TTF_CloseFont(font);
    TTF_CloseFont(shadow);
}

void EndGameUI() {
    int Mx, My;
    switch (ThreatLevel) {
    case 1:
        EGUI.image = loadSurface(victory);
        EGR.image = loadSurface(returnBlue);
        EGMen.image = loadSurface(menuBlue);
        EGRank.image = loadSurface(rankBlue);
        break;
    case 3:
        EGUI.image = loadSurface(defeat);
        EGR.image = loadSurface(returnRed);
        EGMen.image = loadSurface(menuRed);
        EGRank.image = loadSurface(rankRed);
        break;
    }
    SDL_GetMouseState(&Mx, &My);
}

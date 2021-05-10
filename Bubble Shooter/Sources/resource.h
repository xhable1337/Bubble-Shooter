#pragma once
#include <SDL.h>
#include <SDL_image.h>

//// Шрифт
// Acrobat Bold
extern char* font_path;

//// Звуки
// Фоновая музыка
extern char* music_path;
// Звук лопающегося пузыря (для взрыва островов)
extern char* bubblePop_path;
// Звук синтезаторного пузыря (для отскока от стен)
extern char* synthPop_path;
// Звук шипения (для выстрела шариком)
extern char* shot_path;

//// Файл сохранения таблицы рекордов
extern char* leaderboardPath;

//// Ассеты для меню
extern char* menuBG;               // Фон для меню
extern char* menuBG2;              // Фон в пунктах меню
extern char* arrow;                // Стрелка для меню
extern char* topPlayers;           // Таблица рекордов
extern char* settings;             // Экран информации

//// Внутриигровые ассеты интерфейса
// Фон
extern char* gameBG;              // Фон во время игры

// Низкий уровень угрозы (синий)
extern char* uiBlue;              // Синий игровой UI
extern char* menuBlue;            // Синяя кнопка главного меню
extern char* menuHoverBlue;       // Синяя кнопка главного меню при наведении мыши
extern char* soundOnBlue;         // Синяя кнопка "звук включен"
extern char* soundOnHoverBlue;    // Синяя кнопка "звук включен" при наведении мыши
extern char* soundOffBlue;        // Синяя кнопка "звук включен"
extern char* soundOffHoverBlue;   // Синяя кнопка "звук выключен" при наведении мыши
extern char* lifeBlue;            // Синий индикатор жизней
                             
// Средний уровень угрозы (жёлтый)
extern char* uiYellow;            // Жёлтый игровой UI
extern char* menuYellow;          // Жёлтая кнопка главного меню
extern char* menuHoverYellow;     // Жёлтая кнопка главного меню при наведении мыши
extern char* soundOnYellow;       // Жёлтая кнопка "звук включен"
extern char* soundOnHoverYellow;  // Жёлтая кнопка "звук включен" при наведении мыши
extern char* soundOffYellow;      // Жёлтая кнопка "звук включен"
extern char* soundOffHoverYellow; // Жёлтая кнопка "звук включен" при наведении мыши
extern char* lifeYellow;          // Жёлтый индикатор жизней

// Высокий уровень угрозы (красный)
extern char* uiRed;               // Красный игровой UI
extern char* menuRed;             // Красная кнопка главного меню
extern char* menuHoverRed;        // Красная кнопка главного меню при наведении мыши
extern char* soundOnRed;          // Красная кнопка "звук включен"
extern char* soundOnHoverRed;     // Красная кнопка "звук включен" при наведении мыши
extern char* soundOffRed;         // Красная кнопка "звук включен"
extern char* soundOffHoverRed;    // Красная кнопка "звук включен" при наведении мыши
extern char* lifeRed;             // Красный индикатор жизней

// Проигрыш
extern char* defeat;              // Сплэш-скрин при поражении
extern char* returnRed;           // Красная кнопка перезапуска 
extern char* returnHoverRed;      // Красная кнопка перезапуска при наведении
extern char* rankRed;             // Красная кнопка таблицы рекордов
extern char* rankHoverRed;        // Красная кнопка таблицы рекордов при наведении

// Выигрыш
extern char* victory;             // Сплэш-скрин при победе
extern char* returnBlue;          // Синяя кнопка перезапуска 
extern char* returnHoverBlue;     // Синяя кнопка перезапуска при наведении
extern char* rankBlue;	          // Синяя кнопка таблицы рекордов
extern char* rankHoverBlue;       // Синяя кнопка таблицы рекордов при наведении


                                  
//// Внутриигровые модели		  
// Шарики						  
extern char* color1;              // Цвет шарика под номером 1
extern char* color2;              // Цвет шарика под номером 2
extern char* color3;              // Цвет шарика под номером 3
extern char* color4;              // Цвет шарика под номером 4
extern char* color5;              // Цвет шарика под номером 5
extern char* color6;              // Цвет шарика под номером 6

void resourceInit();
SDL_Surface* requestSurface(char* path);
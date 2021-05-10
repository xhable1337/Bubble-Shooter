#include <SDL.h>
#include <SDL_image.h>

//// Шрифт
// Acrobat Bold
char* font_path = "./TTF/font.ttf";

//// Звуки
// Фоновая музыка
char* music_path = "./Sounds/n1ghtliner - slighty observer.mp3";
// Звук лопающегося пузыря (для взрыва островов)
char* bubblePop_path = "./Sounds/bubblePop.wav";
// Звук синтезаторного пузыря (для отскока от стен)
char* synthPop_path = "./Sounds/synthPop.wav";
// Звук шипения (для выстрела шариком)
char* shot_path = "./Sounds/shot.mp3";

//// Файл сохранения таблицы рекордов
char* leaderboardPath = "./Misc/leaderboard.txt";

//// Ассеты для меню
char* menuBG = "./Images/menuBG.png";                           // Фон для меню			            
char* menuBG2 = "./Images/skyMenuBG640x480.png";                // Фон в пунктах меню
char* arrow = "./Images/menuButtonHover.png";                   // Стрелка для меню
char* topPlayers = "./Images/topPlayers.png";                   // Таблица рекордов
char* settings = "./Images/settings.png";                       // Экран информации

//// Внутриигровые ассеты интерфейса	
// Фон
char* gameBG = "./Images/skyBG640x480.jpg";                     // Фон во время игры

// Низкий уровень угрозы							            
char* uiBlue = "./Images/uiBlue.png";                           // Синий игровой UI
char* menuBlue = "./Images/menuBlue.png";                       // Синяя кнопка главного меню
char* menuHoverBlue = "./Images/menuHoverBlue.png";             // Синяя кнопка главного меню при наведении мыши
char* soundOnBlue = "./Images/soundOnBlue.png";                 // Синяя кнопка "звук включен"
char* soundOnHoverBlue = "./Images/soundOnHoverBlue.png";       // Синяя кнопка "звук включен" при наведении мыши
char* soundOffBlue = "./Images/soundOffBlue.png";               // Синяя кнопка "звук выключен"
char* soundOffHoverBlue = "./Images/soundOffHoverBlue.png";     // Синяя кнопка "звук включен" при наведении мыши
char* lifeBlue = "./Images/lifeBlue.png";                       // Синий индикатор жизней
                                                                
// Средний уровень угрозы							            
char* uiYellow = "./Images/uiYellow.png";                       // Жёлтый игровой UI
char* menuYellow = "./Images/menuYellow.png";                   // Жёлтая кнопка главного меню
char* menuHoverYellow = "./Images/menuHoverYellow.png";         // Жёлтая кнопка главного меню при наведении мыши
char* soundOnYellow = "./Images/soundOnYellow.png";             // Жёлтая кнопка "звук включен"
char* soundOnHoverYellow = "./Images/soundOnHoverYellow.png";   // Жёлтая кнопка "звук включен" при наведении мыши
char* soundOffYellow = "./Images/soundOffYellow.png";           // Жёлтая кнопка "звук выключен"
char* soundOffHoverYellow = "./Images/soundOffHoverYellow.png"; // Жёлтая кнопка "звук выключен" при наведении мыши
char* lifeYellow = "./Images/lifeYellow.png";                   // Жёлтый индикатор жизней
                                                        
// Высокий уровень угрозы							    
char* uiRed = "./Images/uiRed.png";                             // Красный игровой UI
char* menuRed = "./Images/menuRed.png";                         // Красная кнопка главного меню
char* menuHoverRed = "./Images/menuHoverRed.png";               // Красная кнопка главного меню при наведении мыши
char* soundOnRed = "./Images/soundOnRed.png";                   // Красная кнопка "звук включен"
char* soundOnHoverRed = "./Images/soundOnHoverRed.png";         // Красная кнопка "звук включен" при наведении мыши
char* soundOffRed = "./Images/soundOffRed.png";                 // Красная кнопка "звук выключен"
char* soundOffHoverRed = "./Images/soundOffHoverRed.png";       // Красная кнопка "звук выключен" при наведении мыши
char* lifeRed = "./Images/lifeRed.png";                         // Красный индикатор жизней

// Проигрыш
char* defeat = "./Images/defeat.png";                           // Сплэш-скрин при поражении
char* returnRed = "./Images/returnRed.png";                     // Красная кнопка перезапуска 
char* returnHoverRed = "./Images/returnHoverRed.png";           // Красная кнопка перезапуска при наведении
char* rankRed = "./Images/rankRed.png";                         // Красная кнопка таблицы рекордов
char* rankHoverRed = "./Images/rankHoverRed.png";               // Красная кнопка таблицы рекордов при наведении

// Выигрыш
char* victory = "./Images/victory.png";                         // Сплэш-скрин при победе
char* returnBlue = "./Images/returnBlue.png";                   // Синяя кнопка перезапуска 
char* returnHoverBlue = "./Images/returnHoverBlue.png";         // Синяя кнопка перезапуска при наведении
char* rankBlue = "./Images/rankBlue.png";                       // Синяя кнопка таблицы рекордов
char* rankHoverBlue = "./Images/rankHoverBlue.png";             // Синяя кнопка таблицы рекордов при наведении

//// Внутриигровые модели
// Шарики
char* color1 = "./Images/red.png";
char* color2 = "./Images/orange.png";
char* color3 = "./Images/yellow.png";
char* color4 = "./Images/green.png";
char* color5 = "./Images/blue.png";
char* color6 = "./Images/purple.png";

// Дальше находятся вспомогательные функции загрузки текстур.
// Во избежание проблем со зрением, просьба не заглядывать в код.
// Спасибо!

SDL_Surface* load(char* path)
{
    /*Load image at specified path*/
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("[%s]: OK", path);
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        exit(502);
    }
    else 
    {
        printf("[%s]: OK\n", path);
    }

    return loadedSurface;
}

// Элементы меню
SDL_Surface* mBG, * mBG2, * arr, * tP, * creds;

// Фон
SDL_Surface* gBG;

// 6 цветов шариков
SDL_Surface* c1, * c2, * c3, * c4, * c5, * c6;

// Элементы синего интерфейса
SDL_Surface* uiB, * sOnB, * SOnHB, * SOffB, * SOffHB, * lB, * mB, * mHB;

// Элементы жёлтого интерфейса
SDL_Surface* uiY, * sOnY, * SOnHY, * SOffY, * SOffHY, * lY, * mY, * mHY;

// Элементы красного интерфейса
SDL_Surface* uiR, * sOnR, * SOnHR, * SOffR, * SOffHR, * lR, * mR, * mHR;

// Элементы интерфейса проигрыша
SDL_Surface* def, * retR, * retHR, * rankR, * rankHR;

// Элементы интерфейса выигрыша
SDL_Surface* vic, * retB, * retHB, * rankB, * rankHB;


// Предзагружает все surface в память. (Исправлена утечка памяти)
void resourceInit() {
    // Загрузка ассетов меню
    {
        mBG = load(menuBG);
        mBG2 = load(menuBG2);
        arr = load(arrow);
        tP = load(topPlayers);
        creds = load(settings);
    }

    // Загрузка фона
    load(gameBG);

    // Загрузка шариков
    {
        c1 = load(color1);
        c2 = load(color2);
        c3 = load(color3);
        c4 = load(color4);
        c5 = load(color5);
        c6 = load(color6);
    }

    // Загрузка элементов синего интерфейса
    {
        uiB = load(uiBlue);
        sOnB = load(soundOnBlue);
        SOnHB = load(soundOnHoverBlue);
        SOffB = load(soundOffBlue);
        SOffHB = load(soundOffHoverBlue);
        lB = load(lifeBlue);
        mB = load(menuBlue);
        mHB = load(menuHoverBlue);
    }

    // Загрузка элементов жёлтого интерфейса
    {
        uiY = load(uiYellow);
        sOnY = load(soundOnYellow);
        SOnHY = load(soundOnHoverYellow);
        SOffY = load(soundOffYellow);
        SOffHY = load(soundOffHoverYellow);
        lY = load(lifeYellow);
        mY = load(menuYellow);
        mHY = load(menuHoverYellow);
    }

    // Загрузка элементов красного интерфейса
    {
        uiR = load(uiRed);
        sOnR = load(soundOnRed);
        SOnHR = load(soundOnHoverRed);
        SOffR = load(soundOffRed);
        SOffHR = load(soundOffHoverRed);
        lR = load(lifeRed);
        mR = load(menuRed);
        mHR = load(menuHoverRed);
    }

    // Загрузка элементов интерфейса проигрыша
    {
        def = load(defeat);
        retR = load(returnRed);
        retHR = load(returnHoverRed);
        rankR = load(rankRed);
        rankHR = load(rankHoverRed);
    }

    // Загрузка элементов интерфейса выигрыша
    {
        vic = load(victory);
        retB = load(returnBlue);
        retHB = load(returnHoverBlue);
        rankB = load(rankBlue);
        rankHB = load(rankHoverBlue);
    }
}

// Возвращает предзагруженный surface. (Исправлена утечка памяти)
SDL_Surface* requestSurface(char* path) {
    // Возврат фона
    if (path == gameBG) return gBG;

    // Возврат ассетов меню
    else if (path == menuBG) return mBG;
    else if (path == menuBG2) return mBG2;
    else if (path == arrow) return arr;
    else if (path == topPlayers) return tP;
    else if (path == settings) return creds;
    
    // Возврат шариков
    else if (path == color1) return c1;
    else if (path == color2) return c2;
    else if (path == color3) return c3;
    else if (path == color4) return c4;
    else if (path == color5) return c5;
    else if (path == color6) return c6;

    // Возврат элементов синего интерфейса
    else if (path == uiBlue) return uiB;
    else if (path == soundOnBlue) return sOnB;
    else if (path == soundOnHoverBlue) return SOnHB;
    else if (path == soundOffBlue) return SOffB;
    else if (path == soundOffHoverBlue) return SOffHB;
    else if (path == lifeBlue) return lB;
    else if (path == menuBlue) return mB;
    else if (path == menuHoverBlue) return mHB;

    // Возврат элементов жёлтого интерфейса
    else if (path == uiYellow) return uiY;
    else if (path == soundOnYellow) return sOnY;
    else if (path == soundOnHoverYellow) return SOnHY;
    else if (path == soundOffYellow) return SOffY;
    else if (path == soundOffHoverYellow) return SOffHY;
    else if (path == lifeYellow) return lY;
    else if (path == menuYellow) return mY;
    else if (path == menuHoverYellow) return mHY;

    // Возврат элементов красного интерфейса
    else if (path == uiRed) return uiR;
    else if (path == soundOnRed) return sOnR;
    else if (path == soundOnHoverRed) return SOnHR;
    else if (path == soundOffRed) return SOffR;
    else if (path == soundOffHoverRed) return SOffHR;
    else if (path == lifeRed) return lR;
    else if (path == menuRed) return mR;
    else if (path == menuHoverRed) return mHR;

    // Загрузка элементов интерфейса проигрыша
    else if (path == defeat) return def;
    else if (path == returnRed) return retR;
    else if (path == returnHoverRed) return retHR;
    else if (path == rankRed) return rankR;
    else if (path == rankHoverRed) return rankHR;

    // Загрузка элементов интерфейса выигрыша
    else if (path == victory) return vic;
    else if (path == returnBlue) return retB;
    else if (path == returnHoverBlue) return retHB;
    else if (path == rankBlue) return rankB;
    else if (path == rankHoverBlue) return rankHB;

    

    else return NULL;
}
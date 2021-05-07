#define MODELS 2 // Выбор моделей для игры (1 - планеты, 2 - шарики)

//// Ассеты для меню
char* menuBG = "./Images/menuBG.png";                           // Фон для меню			            
char* menuBG2 = "./Images/BG.png";                              // Фон в пунктах меню
char* arrow = "./Images/arrow.png";                             // Стрелка для меню
char* topPlayers = "./Images/topPlayers.png";                   // Таблица рекордов
char* credits = "./Images/credits.png";                         // Экран информации


//// Внутриигровые ассеты интерфейса	
// Фон
char* gameBG = "./Images/BG2.png";                              // Фон во время игры

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
#if (MODELS == 1)
char* color1 = "./Images/Earth.png";
char* color2 = "./Images/Saturn.png";
char* color3 = "./Images/Neptune.png";
char* color4 = "./Images/Venus.png";
char* color5 = "./Images/Jupiter.png";
char* color6 = "./Images/Mars.png";
#elif (MODELS == 2)
char* color1 = "./Images/red.png";
char* color2 = "./Images/orange.png";
char* color3 = "./Images/yellow.png";
char* color4 = "./Images/green.png";
char* color5 = "./Images/blue.png";
char* color6 = "./Images/purple.png";
#endif
#pragma once

// Сортирует массив таблицы рекордов по возрастанию методом «пузырька».
void leaderboardSort(int* leaderboard);

// Инициализация массива таблицы рекордов.
void leaderboardInit(int* leaderboard);

// Записывает массив таблицы рекордов в файл.
void leaderboardWrite(int* leaderboard);

// Считывает массив таблицы рекордов из файла.
void leaderboardRead(int* leaderboard);

// Добавляет score в массив таблицы рекордов, если значение score является рекордом.
void leaderboardUpdate(int* leaderboard, int score);
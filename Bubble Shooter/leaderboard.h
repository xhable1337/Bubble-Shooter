#pragma once

// ��������� ������ ������� �������� �� ����������� ������� ���������.
void leaderboardSort(int* leaderboard);

// ������������� ������� ������� ��������.
void leaderboardInit(int* leaderboard);

// ���������� ������ ������� �������� � ����.
void leaderboardWrite(int* leaderboard);

// ��������� ������ ������� �������� �� �����.
void leaderboardRead(int* leaderboard);

// ��������� score � ������ ������� ��������, ���� �������� score �������� ��������.
void leaderboardUpdate(int leaderboard, int score);


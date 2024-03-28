#pragma once

void Screensaver(); // отображает начальный экран(заставку)
void Menu(); // отображает меню игры 
void StartNewGame(); // отображает запрос на ввод количества строк и столбцов
void PlayGame(int row, int col); // начинает ход игры, устанавливая начальные значения времени и выбранной позиции.
void CheckMatch(int firstCardRow, int firstCardCol, int secondCardRow, int secondCardCol, int& moves, clock_t startTime, int row, int col); //  проверяет, совпадают ли две выбранные карты.
void ShowResults(clock_t startTime, int moves); // отображает результаты игры.
void InitializeBoard(int row, int col); // инициализирует игровое поле картами.
void AllCardsOpen(int row, int col); // открывает все карты на игровом поле.
void PrintBoard(int rows, int columns, int selectedRow, int selectedCol); // выводит текущее состояние игрового поля.
bool IsGameOver(int row, int col); // проверяет, завершена ли игра.
void SelectCard(int& selectedRow, int& selectedCol, int row, int col, clock_t startTime, int& moves); // позволяет игроку выбирать карту.
char GetDirection(); // получает ввод направления от игрока.
void PlaySoundFromProjectFolder(const char* soundName); // воспроизводит звук Windows, указанный именем звука.
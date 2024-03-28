#include <iostream>
#include <ctime>
#include <conio.h> // Для _getch()
#include <cstdlib>
#include <windows.h>
#include <string>
#include "function.h"


using std::cout;
using std::cin;
using std::endl;
using std::swap;
using std::string;

// глобальные переменные
int** board; // двумерный массив для хранения значений карт
bool** revealed; // двумерный массив для хранения информации о том, открыта ли карта
int moves = 0; // количество ходов (переворотов карт)


void PlaySoundFromProjectFolder(const char* soundName) {
    char fullPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, fullPath); // Получаем текущую директорию
    string pathStr(fullPath); // Конвертируем в string для удобства работы со строками

    // Строим полный путь к файлу звука
    pathStr += "\\sounds\\";
    pathStr += soundName;

    // Конвертируем string в std::wstring
    int size_needed = MultiByteToWideChar(CP_ACP, 0, pathStr.c_str(), -1, NULL, 0);
    std::wstring wFullPath(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, pathStr.c_str(), -1, &wFullPath[0], size_needed);

    // Воспроизводим звук
    PlaySound(wFullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void Screensaver() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // меняем цвет шрифта
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    PlaySoundFromProjectFolder("menu.wav");
    cout << endl << endl;
    cout << "                                              Загружаю игру..." << endl;
    Sleep(1000);

    system("cls");
    cout << endl << endl;
    cout << "                                 *******************************************" << endl;
    Sleep(500);
    cout << "                                 *       *************************         *" << endl;
    Sleep(500);
    cout << "                                 *       *                       *         *" << endl;
    Sleep(500);
    cout << "                                 *       *       Игра Память     *         *" << endl;
    Sleep(500);
    cout << "                                 *       *                       *         *" << endl;
    Sleep(500);
    cout << "                                 *       *************************         *" << endl;
    Sleep(500);
    cout << "                                 *******************************************" << endl;
    Sleep(2000);

    system("cls");
    cout << endl << endl;
    cout << "                                                 Загрузка";
    for (int i = 0; i < 5; ++i) {
        Sleep(500);
        cout << ".";
    }
    Sleep(500);
    system("cls");

    cout << endl << endl;
    cout << "                                             Игра начинается!" << endl;
    Sleep(1500);
}

void Menu() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    PlaySoundFromProjectFolder("start.wav");
    system("cls");
    cout << endl << endl;
    cout << "                                         *******************************\n"
        "                                         *                             *\n"
        "                                         *         МЕНЮ ИГРЫ           *\n"
        "                                         *                             *\n"
        "                                         *******************************\n\n"
        "                                         Нажмите 1, чтобы начать Новую игру\n"
        "                                         Нажмите 2, чтобы Выйти\n\n"
        "                                               Введите свой выбор: ";

    string input; // изменено на строку для предварительного считывания
    int choice;
    getline(cin, input); // считываем строку вместо числа

    //  преобразовываем введенную строку в число(конструкция позволяет обрабатывать исключения, ошибки и вылавливать их в catch)
    try {
        choice = stoi(input); // преобразование строки в число
    }
    catch (...) {
        choice = -1; // если преобразование не удалось, устанавливаем choice в некорректное значение
    }
    cout << endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    while (choice != 1 && choice != 2) {

        cout << "Пожалуйста, сделайте правильный выбор.\n";
        Sleep(60);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Введите свой выбор: ";
        getline(cin, input); // считываем строку вместо числа

        try {
            choice = stoi(input); // преобразование строки в число
        }
        catch (...) {
            choice = -1; // если преобразование не удалось, устанавливаем choice в некорректное значение
        }
        cout << endl;
    }

    switch (choice) {

    case 1:
        StartNewGame();
        break;
    case 2:
        cout << "Выхожу из игры..." << endl;
        Sleep(60);
        exit(0);
    }
}

void StartNewGame() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    int row, col;
    cout << "Пожалуйста, введите количество строк в игровом поле: ";
    cin >> row;
    cout << "Пожалуйста, введите количество столбцов в игровом поле: ";
    cin >> col;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    while (row * col % 2 != 0) {

        cout << "Игровое поле должно иметь четное количество строк и столбцов.\n"
            "Пожалуйста, введите количество строк и столбцов для игрового поля еще раз.\n";
        Sleep(180);
        cout << "Пожалуйста, введите количество строк в игровом поле: ";
        cin >> row;
        cout << "Пожалуйста, введите количество столбцов в игровом поле: ";
        cin >> col;
    }

    InitializeBoard(row, col);
    AllCardsOpen(row, col);
    Sleep(5000);

    PlayGame(row, col);
}

void PlayGame(int row, int col) {

    clock_t startTime = clock();
    int selectedRow = 0, selectedCol = 0; // начальная позиция выбора

    while (!IsGameOver(row, col)) {
        // выбор первой карты
        SelectCard(selectedRow, selectedCol, row, col, startTime, moves);

        int firstCardRow = selectedRow, firstCardCol = selectedCol; // сохраняем позицию первой выбранной карты

        // выбор второй карты
        SelectCard(selectedRow, selectedCol, row, col, startTime, moves);

        if (firstCardRow != selectedRow || firstCardCol != selectedCol) {
            CheckMatch(firstCardRow, firstCardCol, selectedRow, selectedCol, moves, startTime, row, col);
        }
        else {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Вы выбрали ту же самую карту. Пожалуйста, выберите другую.\n";
            Sleep(1000); // даем время прочитать сообщение
        }
    }

    ShowResults(startTime, moves);
}

void CheckMatch(int firstCardRow, int firstCardCol, int secondCardRow, int secondCardCol, int& moves, clock_t startTime, int row, int col) {

    system("cls"); // очищаем экран перед показом результатов
    PrintBoard(row, col, -1, -1); // печатаем доску для визуализации выбора

    if (board[firstCardRow][firstCardCol] == board[secondCardRow][secondCardCol]) {
        cout << "Совпадение найдено!" << endl;

        PlaySoundFromProjectFolder("tada.wav");

        revealed[secondCardRow][secondCardCol] = true; // оставляем карточки открытыми
        revealed[firstCardRow][firstCardCol] = true;
    }
    else {
        cout << "Совпадение не найдено." << endl;

        PlaySoundFromProjectFolder("chord.wav");

        Sleep(2000); // даём время пользователю увидеть карточки

        revealed[firstCardRow][firstCardCol] = false; // закрываем карточки
        revealed[secondCardRow][secondCardCol] = false;
    }
}

void ShowResults(clock_t startTime, int moves) {

    int totalTime = static_cast<int>(clock() - startTime) / CLOCKS_PER_SEC;

    system("cls"); // Очищаем экран перед показом результатов

    cout << "Поздравляю! Вы завершили игру.\n";
    cout << "Вы выиграли игру за " << moves << " ходов.\n";
    cout << "Общее затраченное время: " << totalTime << " секунд.\n";
    cout << "Нажмите любую клавишу для возвращения в меню...";

    _getch(); // Ожидаем нажатия клавиши пользователем, чтобы вернуться в меню

    return Menu();
}

void InitializeBoard(int rows, int columns) {

    int area = rows * columns;
    int* numbers = new int[area];

    for (int i = 0; i < area / 2; ++i) {

        numbers[i] = i;
        numbers[i + (area / 2)] = i;
    }

    srand(time(0));

    for (int i = area - 1; i > 0; --i) {

        int j = rand() % (i + 1);
        swap(numbers[i], numbers[j]);
    }

    board = new int* [rows];
    revealed = new bool* [rows];

    for (int i = 0; i < rows; ++i) {

        board[i] = new int[columns];
        revealed[i] = new bool[columns];

        for (int j = 0; j < columns; ++j) {

            board[i][j] = numbers[(i * columns) + j];
            revealed[i][j] = false;
        }
    }
    delete[] numbers;
}

void PrintBoard(int rows, int columns, int selectedRow, int selectedCol) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    const int cardHeight = 5; // высота карточки
    const int cardWidth = 7; // ширина карточки
    const int spaceBetween = 3; // расстояние между карточками и вокруг них

    int totalHeight = rows * (cardHeight + spaceBetween) + spaceBetween;
    int totalWidth = columns * (cardWidth + spaceBetween) + spaceBetween;

    for (int i = 0; i < totalHeight; ++i) {
        for (int j = 0; j < totalWidth; ++j) {
            int cardRow = i / (cardHeight + spaceBetween);
            int cardCol = j / (cardWidth + spaceBetween);

            // вычисляем относительные координаты внутри карточки
            int relY = i % (cardHeight + spaceBetween) - spaceBetween;
            int relX = j % (cardWidth + spaceBetween) - spaceBetween;

            bool isCardArea = relY >= 0 && relX >= 0;
            bool isBorder = relY == 0 || relY == (cardHeight - 1) || relX == 0 || relX == (cardWidth - 1);
            bool isCenter = relY == (cardHeight / 2) && relX == ((cardWidth / 2) - 1);

            if (isCardArea) {
                if (cardRow == selectedRow && cardCol == selectedCol && isBorder) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
                    cout << "#"; // выделяем границу выбранной карточки
                }
                else if (isBorder) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    cout << "+"; // отрисовываем границу карточки
                }
                else if (isCenter && revealed[cardRow][cardCol]) {
                    cout << board[cardRow][cardCol]; // выводим число карточки, если она открыта
                }
                else {
                    cout << " "; // заполняем внутреннее пространство карточки                    
                }
            }
            else {
                cout << " "; // заполняем пространство между карточками
            }
        }
        cout << endl;
    }
}

void AllCardsOpen(int rows, int columns) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    for (int i = 0; i < rows; ++i) {
        cout << " " << endl;
        for (int j = 0; j < columns; ++j) {
            cout << board[i][j] << "   ";
        }
        cout << endl;
    }
}

bool IsGameOver(int rows, int columns) {

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (!revealed[i][j]) {
                return false;
            }
        }
    }
    return true;
}

char GetDirection() {

    char c = _getch();
    if (c == -32) { // для стрелок
        c = _getch();
        return c;
    }
    return c;
}

void SelectCard(int& selectedRow, int& selectedCol, int row, int col, clock_t startTime, int& moves) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char direction;

    do {
        system("cls");
        PrintBoard(row, col, selectedRow, selectedCol);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        cout << "Количество переворотов: " << moves << endl;
        cout << "Время: " << static_cast<int>(clock() - startTime) / CLOCKS_PER_SEC << endl;
        cout << "Используйте клавиши со стрелками для перемещения, нажмите Enter, чтобы выбрать карту." << endl;

        direction = GetDirection();

        switch (direction) {
        case 72: // Стрелка вверх
            if (selectedRow > 0) selectedRow--;
            break;
        case 80: // Стрелка вниз
            if (selectedRow < row - 1) selectedRow++;
            break;
        case 75: // Стрелка влево
            if (selectedCol > 0) selectedCol--;
            break;
        case 77: // Стрелка вправо
            if (selectedCol < col - 1) selectedCol++;
            break;
        case 13: // Enter
            if (!revealed[selectedRow][selectedCol]) {
                revealed[selectedRow][selectedCol] = true; // Открываем карточку
                moves++; // увеличиваем количество ходов
            }
            else {
                cout << "Эта карточка уже открыта. Выберите другую." << endl;
                Sleep(1000); // даем время прочитать сообщение
            }
            break;
        }
    } while (direction != 13); // повторяем, пока не будет нажат Enter
}
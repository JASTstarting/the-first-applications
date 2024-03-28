#include "lib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

struct Song {
    string title;
    string author;
    string lyrics;
    int year = 0;
};

int main() {
    SetConsoleOutputCP(CP_UTF8); //  устанавливает кодировку консоли на UTF-8. UTF-8 позволяет корректно отображать символы Unicode, включая русские буквы.
    setvbuf(stdout, nullptr, _IOFBF, 1000); //  устанавливает буферизацию стандартного вывода stdout на размер 1000 байтов с полной буферизацией.
    setlocale(LC_ALL, "RUS");

    Song* songs = nullptr; // объявляем указатель songs на объект типа Song
    int count = 0;
    int choice; // для использования в switch
    string input; // Изменено на строку для предварительного считывания

    do {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // для изменения  цвета шрифта
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // желтый цвет
        system("cls"); // Очистка экрана перед отображением меню
        cout << endl;
        cout << "                                     ********************###    МЕНЮ    ###***************************" << endl << endl;
        cout << "                                     *                   ДОБАВЛЕНО ПЕСЕН : " << count << "                         * " << endl << endl;
        cout << "                                     *****************************************************************" << endl;
        cout << "                                     *    1. Добавить песню                                          *" << endl;
        cout << "                                     *    2. Удалить песню                                           *" << endl;
        cout << "                                     *    3. Изменить название,автора и текст песни                  *" << endl;
        cout << "                                     *    4. Показать список всех песен                              *" << endl;
        cout << "                                     *    5. Вывести текст песни на экран из списка                  *" << endl;
        cout << "                                     *    6. Сохранить песню в файл                                  *" << endl;
        cout << "                                     *    7. Поиск и отображение всех песен одного автора            *" << endl;
        cout << "                                     *    8. Поиск и отображение всех песен, с определенным словом   *" << endl;
        cout << "                                     *    9. Выход                                                   *" << endl;
        cout << "                                     *****************************************************************" << endl << endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // зеленый цвет

        cout << "Выберите действие: ";
        getline(cin, input); // Считываем строку вместо числа

        //  преобразовываем введенную строку в число(конструкция позволяет обрабатывать исключения, ошибки и вылавливать их в catch)
        try {
            choice = stoi(input); // Преобразование строки в число
        }
        catch (...) {
            choice = -1; // Если преобразование не удалось, устанавливаем choice в некорректное значение
        }
        cout << endl;

        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // белый цвет
        switch (choice) {

        case 1:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << endl;
            cout << "                                                  ***************************" << endl;
            cout << "                                                  * 1. Ввести с клавиатуры  *" << endl;
            cout << "                                                  * 2. Загрузить из файла   *" << endl;
            cout << "                                                  ***************************" << endl << endl;
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << "Выберите способ: ";
            int method;
            cin >> method;
            cout << endl;
            cin.ignore(); // Чистим буфер
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
            if (method == 1) {
                addSongManually(songs, count);
                pauseScreen(); // Пауза перед очисткой экрана
            }
            else if (method == 2) {
                loadSongFromFile(songs, count);
                pauseScreen(); // Пауза перед очисткой экрана
            }
            else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << "Неверный выбор." << endl;
                pauseScreen(); // Пауза перед очисткой экрана
            }
            break;
        case 2:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
            deleteSong(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 3:
            editSong(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 4:
            displaySongs(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 5:
            printLyrics(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 6:
            saveSongToFile(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 7:
            searchByAuthor(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 8:
            searchByWord(songs, count);
            pauseScreen(); // Пауза перед очисткой экрана
            break;
        case 9:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << "Выход из программы..." << endl;
            break;
        default:
            cout << "Неверный выбор. Пожалуйста, попробуйте еще раз." << endl;
            pauseScreen(); // Пауза перед очисткой экрана
        }
    } while (choice != 9);

    delete[] songs; // освобождаем память
}

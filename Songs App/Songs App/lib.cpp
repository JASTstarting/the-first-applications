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

bool isSongExists(const Song* songs, int count, const Song& newSong) {
    for (int i = 0; i < count; ++i) {
        if (songs[i].title == newSong.title && songs[i].author == newSong.author && songs[i].year == newSong.year) {
            return true; // Песня уже существует
        }
    }
    return false; // Песня уникальна
}

void pauseScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // меняем цвет шрифта
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "Нажмите Enter, чтобы продолжить...";
    cin.ignore();
    cin.get();
}

void addSongManually(Song*& songs, int& count) {
    Song newSong; // создаем новый объект Song, куда сохранятся новые песни
    cout << "Введите название песни: ";
    getline(cin, newSong.title);

    cout << "Введите автора песни: ";
    getline(cin, newSong.author);

    string yearInput;
    cout << "Введите год создания песни (0 если неизвестен): ";
    // проверяем является ли введенное значение числом
    while (true) {
        getline(cin, yearInput);
        if (isNumber(yearInput)) {
            // Если входная строка является числом, преобразуем её в int
            newSong.year = stoi(yearInput);
            break; // Выходим из цикла, так как получили корректный ввод
        }
        else {
            cout << "Некорректный ввод. Пожалуйста, используйте только цифры: ";
        }
    }

    cout << "Введите текст песни: ";
    getline(cin, newSong.lyrics);

    // Проверка на существование песни
    if (isSongExists(songs, count, newSong)) {
        cout << "Песня с таким названием, автором и годом уже существует. Добавление отменено." << endl;
    }
    else {
        // Если песня уникальна, добавляем её
        addSong(songs, count, newSong);
    }
}

void loadSongFromFile(Song*& songs, int& count) {
    string filename;
    cout << "Введите имя файла для загрузки: ";
    getline(cin, filename);

    filename += ".txt"; // Добавляем расширение файла
    cout << endl;
    // открываем файл для чтения
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл." << endl;
        return;
    }

    // чтение данных о песне из файла
    Song newSong;
    getline(file, newSong.title); // Считываем название
    getline(file, newSong.author); // Считываем автора
    file >> newSong.year; // Считываем год

    // Считываем текст песни до конца файла
    string line;
    while (getline(file, line)) {
        newSong.lyrics += line + "\n"; // Добавляем каждую строку к тексту песни
    }
    file.close();
    // проверяем есть ли уже такая песня
    if (isSongExists(songs, count, newSong)) {
        cout << "Песня '" << newSong.title << "' уже существует. Загрузка отменена." << endl;
    }
    else {
        addSong(songs, count, newSong);
        cout << "Песня '" << newSong.title << "' загружена из файла '" << filename << "'." << endl;
    }
}

void addSong(Song*& songs, int& count, const Song& newSong) {
    // делаем проверку на наличие одинаковых песен
    if (isSongExists(songs, count, newSong)) {
        cout << "Такая песня уже существует. Добавление отменено." << endl;
        return;
    }
    // создаем временный массив песен на одну запись больше
    Song* temp = new Song[count + 1];

    // копируем существующие песни из исходного массива во временный
    for (int i = 0; i < count; ++i) {
        temp[i] = songs[i];
    }
    temp[count] = newSong; // добавляем новую песню в конец временного массива

    delete[] songs;
    songs = temp; // Переносим указатель на новый массив песен в переменную songs
    count++;

    cout << "Песня '" << newSong.title << "' успешно добавлена." << endl;
}

void displaySongs(const Song* songs, int count) {
    // проверка на наличие песен в каталоге
    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }
    // выводим список песен на экран
    for (int i = 0; i < count; ++i) {
        cout << i + 1 << ": " << "Название песни: " << songs[i].title << " (Автор песни: " << songs[i].author;
        if (songs[i].year != 0) {
            cout << ", Год создания: " << songs[i].year;
        }
        cout << ")" << endl;
    }
}

void editSong(Song* songs, int count) {

    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }

    for (int i = 0; i < count; ++i) {
        cout << i + 1 << ": " << "Название песни: " << songs[i].title << " (Автор песни: " << songs[i].author;
        if (songs[i].year != 0) {
            cout << ", Год создания: " << songs[i].year;
        }
        cout << ")" << endl;
    }
    cout << endl;
    // запрашиваем у пользователя номер песни для редактирования
    int songNumber;
    cout << "Введите номер песни для редактирования: ";
    cin >> songNumber;
    // проверяем на корректность
    if (songNumber < 1 || songNumber > count) {
        cout << "Неверный номер песни." << endl;
        return;
    }
    songNumber--; // для индексации с 1

    // запрашиваем новые данные о песне 
    cout << "Введите новое название песни: ";
    cin.ignore();
    getline(cin, songs[songNumber].title);

    cout << "Введите нового автора текста: ";
    getline(cin, songs[songNumber].author);

    cout << "Введите новый год создания песни (0 если неизвестен): ";
    cin >> songs[songNumber].year;
    cin.ignore();

    cout << "Введите новый текст песни: ";
    getline(cin, songs[songNumber].lyrics);

    cout << "Песня успешно изменена" << endl;
}

void deleteSong(Song*& songs, int& count) {

    cout << "Список песен:" << endl;

    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }
    // вывод списка песен перед удалением
    else {
        for (int i = 0; i < count; ++i) {
            cout << i + 1 << ": " << "Название песни: " << songs[i].title << " (Автор песни: " << songs[i].author;
            if (songs[i].year != 0) {
                cout << ", Год создания: " << songs[i].year;
            }
            cout << ")" << endl;
        }
        cout << endl;

        int songNumber;
        cout << "Введите номер песни для удаления: ";
        cin >> songNumber;
        cin.ignore(); // Очистка буфера ввода после cin

        if (songNumber < 1 || songNumber > count) {
            cout << "Неверный номер песни." << endl;
            return deleteSong(songs, count);
        }
        songNumber--; // для индексации с 1

        // Сохраняем название песни, которую собираемся удалить
        string removedSongTitle = songs[songNumber].title;

        // смещаем оставшиеся песни на одну позицию влево, удаляя выбранную песню
        for (int i = songNumber; i < count - 1; ++i) {
            songs[i] = songs[i + 1];
        }
        cout << "Песня " << removedSongTitle << " успешно удалена" << endl;
        count--;
    }
}

void saveSongToFile(const Song* songs, int count) {

    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }

    displaySongs(songs, count); // показать список песен для выбора

    cout << "Введите номер песни для сохранения: ";
    int songNumber;
    cin >> songNumber;
    cin.ignore(); // Очищаем буфер после ввода числа

    if (songNumber < 1 || songNumber > count) {
        cout << "Неверный номер песни." << endl;
        return;
    }
    songNumber--; // для индексации с 1

    // получаем ссылку на выбранную песню
    const Song& song = songs[songNumber];
    string filename = song.title + ".txt"; // используем название песни как имя файла и добавляем расширение

    // заменяем недопустимых символов в имени файла
    for (char& c : filename) {
        if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '\"' || c == '<' || c == '>' || c == '|') {
            c = '_';
        }
    }

    // проверяем, что файл с таким именем не существует
    string fullPath = filename;
    ifstream testFile(fullPath);
    if (testFile.good()) {
        // если файл существует, закрываем его и выводим сообщение
        testFile.close();
        cout << "Файл '" << filename << "' уже существует. Песня не была сохранена." << endl;
        return;
    }

    // создаем и открываем файл для записи и сохраняем информацию о песне
    ofstream file(fullPath);
    if (file.is_open()) {
        file << song.title << endl;
        file << song.author << endl;
        file << song.year << endl;
        file << song.lyrics;
        file.close();
        cout << "Песня '" << song.title << "' сохранена в файл '" << filename << "'." << endl << endl;
    }
    else {
        cout << "Не удалось открыть файл для записи." << endl;
    }
}

bool isNumber(const string& str) {
    // Проверяем, является ли каждый символ строки числом(в цикле проходим по каждому символу c в строке str)
    for (char const& c : str) {
        if (isdigit(c) == 0) // проверяем, является ли текущий символ числом
            return false;  // если символ не является числом, возвращаем false
    }
    return true; // // если все символы строки являются числами, возвращаем true
}

void searchByAuthor(const Song* songs, int count) {

    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }

    string author;
    cout << "Введите автора для поиска: ";
    getline(cin, author);
    cout << endl;
    // если нашли песни одного автора, то выводим в консоль
    for (int i = 0; i < count; ++i) {
        if (songs[i].author == author) {
            cout << songs[i].title << " (" << songs[i].year << ")" << endl;
            cout << songs[i].lyrics << endl << endl;
        }
    }
}

void searchByWord(const Song* songs, int count) {

    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }

    string word;
    cout << "Введите слово для поиска в текстах песен: ";
    getline(cin, word);
    cout << endl;

    // если нашли,то выводим на консоль
    for (int i = 0; i < count; ++i) {
        // проверяем, содержится ли подстрока word в тексте всех песен
        if (songs[i].lyrics.find(word) != string::npos) { // string::npos - обозначает, что подстрока не была найдена в тексте.
            cout << songs[i].title << " (" << songs[i].author << ", " << songs[i].year << ")" << endl;
            cout << songs[i].lyrics << endl << endl;
        }
    }
}

void printLyrics(const Song* songs, int count) {

    if (count == 0) {
        cout << "Каталог пуст." << endl;
        return;
    }

    // Вывод списка песен перед выводом
    cout << "Список песен:" << endl;
    for (int i = 0; i < count; ++i) {
        cout << i + 1 << ": " << "Название песни: " << songs[i].title << " (Автор песни: " << songs[i].author;
        if (songs[i].year != 0) {
            cout << ", Год создания: " << songs[i].year;
        }
        cout << ")" << endl;
    }
    cout << endl;

    int songNumber;
    cout << "Введите номер песни для вывода текста: ";
    cin >> songNumber;
    cout << endl;
    if (songNumber < 1 || songNumber > count) {
        cout << "Неверный номер песни." << endl;
        return;
    }
    songNumber--; // для индексации с 1

    cout << "Текст песни '" << songs[songNumber].title << "':" << endl;
    cout << songs[songNumber].lyrics << endl;
}
#pragma once


#include<iostream>
using std::string;

struct Song;
void addSongManually(Song*& songs, int& count); // для добавления песни вручную в массив песен 
void loadSongFromFile(Song*& songs, int& count); // загружает песни из файла и добавляет их в массив песен 
void addSong(Song*& songs, int& count, const Song& newSong); // добавляет новую песню (newSong) в массив песен
void displaySongs(const Song* songs, int count); // отображает список всех песен, содержащихся в массиве
void editSong(Song* songs, int count); // позволяет редактировать информацию о песне в массиве
void deleteSong(Song*& songs, int& count); // удаляет песню из массива
void saveSongToFile(const Song* songs, int count); // сохраняет все песни из массива songs в текстовый файл.
void searchByAuthor(const Song* songs, int count); // ищет песни по имени автора в массиве
void searchByWord(const Song* songs, int count); // выполняет поиск песен по ключевому слову в тексте песни.
void printLyrics(const Song* songs, int count); //  отображает тексты выбраных песен из массива 
bool isSongExists(const Song* songs, int count, const Song& newSong); // проверяет, существует ли песня newSong в массиве songs
bool isNumber(const string& str); // проверяет, является ли строка str числом.
void pauseScreen(); // приостанавливает выполнение программы и ожидает ввода пользователя для продолжения.
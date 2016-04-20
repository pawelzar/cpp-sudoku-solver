#ifndef MENU_H
#define MENU_H

#include <iostream>

using namespace std;

inline void Menu() {
	system("cls");
	cout << " MENU:\n"
		<< " A) Graj\n"
		<< " B) Pomoc\n"
		<< " C) Informacje\n"
		<< " ESC) Wyjscie\n";
}

inline void MenuWithBoard() {
	system("cls");
	cout << " MENU:\n"
		<< " A) Rozwiaz\n"
		<< " B) Generuj nowe\n"
		<< " C) Zapisz aktualne sudoku\n"
		<< " D) Wczytaj ostatnie zapisane sudoku\n"
		<< " ESC) Powrot do MENU\n\n";
}

#endif  // MENU_H

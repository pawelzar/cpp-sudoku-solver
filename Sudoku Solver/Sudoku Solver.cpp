#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include "Menu.h"

using namespace std;

short int Sud[9][9];
short int square[3][3];
short int ver[9];
short int hor[9];

vector<short int> Sudo[9][9];

short int index;
short int counter;
bool again;

void Wait();
void Print();
void Check(short int, int, int); // wpisuje cyfre
void Uncheck(short int, int, int); // kasuje cyfre
bool isValid(short int, int, int); // sprawdza czy mozna wpisac cyfre
void GetReady(); // wypelnia kazdy kontener Sudo cyframi od 1 do 9
void ClearnSeed(); // czysci tablice Sud, square, ver, hor i miesza cyfry w kontenerze
bool Generate(); // wypelnia cala tablice 2-wymiarowa zgodnie z zasadami sudoku
bool Solve(); // sprawdza czy sudoku jest rozwiazywalne
void Prepare(); // usuwa elementy z rozwiazanego sudoku
bool SolveSudoku();
void Move(); // pozwala poruszac sie po sudoku
void Save();
void Load();

int main() {
	int znak;
	bool esc = 0;
	GetReady();
	while (!esc) {
		Menu();
		znak = _getch();
		switch (znak) {
		case 97:
			do {
				again = 0;
				ClearnSeed();
				Generate();
				Prepare();
				Print();
				Move();
			} while (again);
			break;
		case 98:
			cout << "\n Do przemieszczania wskaznika uzywaj strzalek. W puste miejsca wpisz cyfre"
				<< "\n od 1 do 9 tak, aby nie powtarzala sie w wierszu, kolumnie i kwadracie 3x3.\n";
			Wait();
			break;
		case 99:
			cout << "\n Sudoku: Wersja 25.01.2014"
				<< "\n Autor: Pawel Zarebski\n";
			Wait();
			break;
		case 27:
			esc = 1;
			return 0;
		default:
			cout << "\n\n Niepoprawny klawisz.";
			Sleep(500);
		}
	}
	return 0;
}

void Wait() {
	cout << "\n\n Aby kontynuowac wcisnij dowolny klawisz...";
	_getch();
}

void Print() {
	MenuWithBoard();
	for (int i = 0; i<9; i++){
		cout << " ";
		if (i % 3 == 0) cout << "+-------+-------+-------+\n ";
		for (int j = 0; j<9; j++){
			if (j % 3 == 0) cout << "| ";
			if (index / 9 == i && index % 9 == j){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				cout << (char)(Sud[i][j] == 0 ? 43 : Sud[i][j] + 48) << " ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else cout << (char)(Sud[i][j] == 0 ? 32 : Sud[i][j] + 48) << " ";
		}
		cout << "|\n";
	}
	cout << " +-------+-------+-------+\n";
}

void Check(short int n, int i, int j) {
	Sud[i][j] = n;
	square[i / 3][j / 3] |= (1 << (n - 1));
	ver[i] |= (1 << (n - 1));
	hor[j] |= (1 << (n - 1));
}

void Uncheck(short int n, int i, int j) {
	Sud[i][j] = 0;
	square[i / 3][j / 3] ^= (1 << (n - 1));
	ver[i] ^= (1 << (n - 1));
	hor[j] ^= (1 << (n - 1));
}

bool isValid(short int n, int i, int j) {
	if ((square[i / 3][j / 3] & (1 << (n - 1))) ||
		(ver[i] & (1 << (n - 1))) ||
		(hor[j] & (1 << (n - 1)))) return false;

	return true;
}

void GetReady() {
	for (int i = 0; i<9; i++){
		for (int j = 0; j<9; j++){
			Sudo[i][j].clear();
			for (int k = 1; k <= 9; k++){
				Sudo[i][j].push_back(k);
			}
		}
	}
}

void ClearnSeed() {
	memset(Sud, 0, sizeof(short int) * 81);
	memset(square, 0, sizeof(short int) * 9);
	memset(ver, 0, sizeof(short int) * 9);
	memset(hor, 0, sizeof(short int) * 9);
	srand(time(NULL));
	for (int i = 0; i<9; i++){
		for (int j = 0; j<9; j++){
			random_shuffle(Sudo[i][j].begin(), Sudo[i][j].end());
		}
	}
	index = 40;
}

bool Generate() {
	for (int i = 0; i<9; i++){
		for (int j = 0; j<9; j++){
			if (Sud[i][j] != 0) continue;
			for (int num = 0; num<9; num++){
				if (isValid(Sudo[i][j][num], i, j)){
					Check(Sudo[i][j][num], i, j);
					if (Generate()) return true;
					else Uncheck(Sudo[i][j][num], i, j);
				}
			}
			return false;
		}
	}
	return true;
}

bool Solve() {
	if (counter>1) return 0;
	for (int i = 8; i >= 0; i--){
		for (int j = 8; j >= 0; j--){
			if (Sud[i][j] != 0) continue;
			for (int num = 9; num >= 1; num--){
				if (isValid(num, i, j)){
					Check(num, i, j);
					Solve();
					Uncheck(num, i, j);
				}
			}
			return false;
		}
	}
	counter++;
	return true;
}

void Prepare() {
	int val;
	for (int i = 0; i<9; i++){
		for (int j = 0; j<9; j++){
			val = Sud[i][j];
			Uncheck(val, i, j);

			int a = 0, b = 0, c = 0;
			for (int k = 0; k <= 8; k++){
				if (Sud[i][k] != 0) a++;
				if (Sud[k][j] != 0) b++;
				if (Sud[3 * (i / 3) + k / 3][3 * (j / 3) + k % 3] != 0) c++;
			}
			if (a<1 || b<1 || c<2){
				Check(val, i, j);
				continue;
			}

			counter = 0;
			Solve();
			if (counter>1) Check(val, i, j);
		}
	}
}

bool SolveSudoku() {
	for (int i = 8; i >= 0; i--){
		for (int j = 8; j >= 0; j--){
			if (Sud[i][j] != 0) continue;
			for (int num = 9; num >= 1; num--){
				if (isValid(num, i, j)){
					Check(num, i, j);
					if (SolveSudoku()) return true;
					else Uncheck(num, i, j);
				}
			}
			return false;
		}
	}
	return true;
}

void Move() {
	int znak;
	bool esc = 0;
	bool cons[81];
	for (int i = 0; i<9; i++){
		for (int j = 0; j<9; j++){
			if (Sud[i][j] != 0) cons[i * 9 + j] = 1;
			else cons[i * 9 + j] = 0;
		}
	}
	while (!esc){
		znak = _getch();
		if (!cons[index]){
			if (znak == 'm'){
				Uncheck(Sud[index / 9][index % 9], index / 9, index % 9);
				Print();
			}
			else if (znak>'0' && znak <= '9'){
				if (isValid(znak - 48, index / 9, index % 9)){
					Uncheck(Sud[index / 9][index % 9], index / 9, index % 9);
					Check(znak - 48, index / 9, index % 9);
					Print();
				}
			}
		}
		switch (znak){
		case 72:
			index -= 9;
			if (index<0) index += 81;
			Print();
			break;
		case 75:
			index -= 1;
			if (index<0) index += 81;
			Print();
			break;
		case 80:
			index += 9;
			index %= 81;
			Print();
			break;
		case 77:
			index += 1;
			index %= 81;
			Print();
			break;
		case 97:
			SolveSudoku();
			Print();
			break;
		case 98:
			again = 1;
			esc = 1;
			break;
		case 99:
			Save();
			break;
		case 100:
			Load();
			break;
		case 27:
			esc = 1;
			break;
		}
	}
}

void Save() {
	ofstream outfile("sudoku.txt");
	for (int i = 0; i<9; i++){
		for (int j = 0; j<9; j++){
			outfile << Sud[i][j] << " ";
		}
		outfile << endl;
	}
	outfile.close();
}

void Load() {
	ifstream infile;
	infile.open("sudoku.txt");
	if (infile.is_open()){
		memset(Sud, 0, sizeof(short int) * 81);
		memset(square, 0, sizeof(short int) * 9);
		memset(ver, 0, sizeof(short int) * 9);
		memset(hor, 0, sizeof(short int) * 9);
		for (int i = 0; i<9; i++){
			for (int j = 0; j<9; j++){
				infile >> Sud[i][j];
				Check(Sud[i][j], i, j);
			}
		}
		infile.close();
		Print();
	}
	else cout << "\n Brak pliku.";
}

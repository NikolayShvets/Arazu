#include "pch.h"
#include <iostream>
#include <string>
#include <conio.h>
#include "windows.h"
#include <fstream>
#include <vector>
using namespace std;

bool map_change{ true };
bool gameOver{ false }; //признак законченности игры
ifstream current_map; // файл текущей карты 
string current_map_name = "_start_location.txt";
vector<string> current_draw_map; // текущая карта на отрисовку
int width{0}, height{ 0 };// ширина длина карты

//int ch_x{ 0 }, ch_y{ 0 }; //координаты персонажа

enum move_var { STOP = 0, LEFT, RIGHT, UP, DOWN }; // перечисление возможных перемещений


enum ch_race {HUMAN = 0, ELF, DWARF}; // вариант расы
enum ch_gender {MALE = 0, FEMALE}; // пол персонажа (м, ж)
enum ch_class {WIZARD = 0, ASSASIN, PRIST, WARRIOR, NECROMANCER}; // вариант класса персонажа (маг, вор, жрец, воин, некромант)
enum ch_armour {DRESS = 0, LEATHER, CHAIN, PLATE, NAKED}; // броня (тряпки, кожа, кольчуга, латы, голый)
enum ch_weapon {DAGGER = 0, DDAGGER, SWORD, DSWORD, AXE, DAXE, MACE, THSWORD, THAXE, BOW, SHIELD,
	GRIMUAR, MAGIC_STAFF, MAGIC_STICK, MAGIC_SCROLL, NONWEAPON}; // оружие (кинжал, два кинажал, меч, два меча, 
                                                                        //топор, два топора, булава, двуручный меч, 
																		//двуручный топор, лук, щит, гримуар, волшебный посох, 
                                                                        //волшебная палочка, свиток заклинаний, безоружный)
/*ch_weapon weapon;
ch_armour armour;
enum ch_dressed_armour {HELMET = 0, BODY, GLOVES, TROUSERS, BOOTS}; // надетая одежда (шлем, тело, перчатки, штаны, сапоги)
enum ch_dressed_weapons {LEFT_HAND = 0, RIGHT_HAND}; // надетое оружие (левая рука, правая рука)
ch_dressed_armour dressed_armour;
ch_dressed_weapons dressed_weapons;*/

struct  Character
{
	char ch_skin{ 'T' }; //внешний вид персонажа
	string _name{ "" }; //имя персонажа
	ch_race _race{ HUMAN }; //раса
	ch_gender _gender{ MALE }; //пол
	ch_class _class{ WIZARD }; //класс
	ch_weapon _weapon{ NONWEAPON }; //оружие
	ch_armour _armour{ NAKED }; //броня
	string _autobiography{ "" }; //предыстория
	double _coins{ 0.0 }; //деньги
	double _heals{ 100.0 }; //количество здоровья
	double _mana{ 100.0 }; //количество маны(выносливости)
	int ch_x{ 0 }; //вниз
	int ch_y{ 0 }; //вправо
	move_var ch_move;
};
Character character;

void Setup(); //функция подгрузки стартовых параметров игры (запускается один раз при старте)
void DrawMap(char skin, string map_name, move_var ch_move); // функция отрисовки карты (вызывается каждый раз при перемещении персонажа)
void Input(); // функция обработки пользовательского ввода
void Logic(); // логика игры

int main()
{
	Setup();
	while (!gameOver)
	{
		Input();
		Logic();
	}
	if (gameOver)
	{
		system("cls");
		cout << "Game over!";
	}
	return 0;
}
void Setup()
{
	system("cls");
	int temp{ 0 };
	cout << "Character editor" << endl;
	cout << "Input the name of your character: " << endl;
	getline(cin, character._name);
	cout << "The name of your character "<< character._name << endl;
	cout << "Select the race of your character: \n    0 - Human\n    1 - Elf\n    2 - Dwarf" << endl;
	cin >> temp;
	switch (temp)
	{
	case 0: character._race = HUMAN; cout << "Character race is Human" <<endl; break;
	case 1: character._race = ELF; cout << "Character race is Elf" <<endl; break;
	case 2: character._race = DWARF; cout << "Character race is Dwarf" <<endl; break;
	default: character._race = HUMAN; cout << "Character race is Human" <<endl; break;
	}
	cout << "Select the gender of your character: \n    0 - Male\n    1 - Female" << endl;
	cin >> temp;
	switch (temp)
	{
	default: character._gender = MALE; cout << "The gender of your character male" << endl; character.ch_skin = 'T'; break;
	case 0: character._gender = MALE; cout << "The gender of your character male" << endl; character.ch_skin = 'T'; break;
	case 1: character._gender = FEMALE; cout << "The gender of your character female" << endl; character.ch_skin = 'Y'; break;
	}
	cout << "Select the class of your character: \n    0 - Wizard\n    1 - Assasin\n    2 - Prist\n    3 - Warrior\n    4 - Necromancer" << endl;
	cin >> temp;
	switch (temp)
	{
	default: character._class = WIZARD; cout << "The class of your character Wizard" << endl; break;
	case 0: character._class = WIZARD; cout << "The class of your character Wizard" << endl; break;
	case 1: character._class = ASSASIN; cout << "The class of your character Assasin" << endl; break;
	case 2: character._class = PRIST; cout << "The class of your character Prist" << endl; break;
	case 3: character._class = WARRIOR; cout << "The class of your character Warrior" << endl; break;
	case 4: character._class = NECROMANCER; cout << "The class of your character Necromancer" << endl; break;
	}
	cout << "Biography of your character: " << endl;
	getline(cin, character._autobiography, ';');
	cout << "Biography:" << character._autobiography << endl;
	character.ch_move = STOP;
	character.ch_x = 2;
	character.ch_y = 2;
	DrawMap(character.ch_skin, current_map_name, character.ch_move);
	//cout << "Character: " << character._name << " Race: " << character._race << " Class: " <<character._class<< " Gender: " << character._gender << endl;
}

void DrawMap(char skin, string map_name, move_var ch_move)
{
	system("cls");//or cls
	string str{""};
	if (map_change)
	{
		current_map.open(map_name);
		if (!current_map.is_open())
		{
			cout << "Error. Map file " << map_name << " not open." << endl;
		}
		while (getline(current_map, str))
		{
			current_draw_map.push_back(str);
			//cout << current_draw_map.back()<<endl;
		}
		map_change = false;
		height = current_draw_map.size(); //167 Х
		width = current_draw_map.back().size();//38 Y
	/*cout << "width = " << width;
	cout << "height = " << height;*/
	}
	for (int i = 0; i < height/*current_draw_map.size()*/; ++i)
	{
		for (int j = 0; j < width/*current_draw_map.back().size()*/; ++j)
		{
			if ((i == character.ch_x) && (j == character.ch_y))
			{
				current_draw_map[i][j] = character.ch_skin;
				switch (character.ch_move)
				{
				case LEFT: current_draw_map[i][j + 1] = ' '; break;
				case UP: current_draw_map[i + 1][j] = ' '; break;
				case RIGHT: current_draw_map[i][j - 1] = ' '; break;
				case DOWN: current_draw_map[i - 1][j] = ' '; break;
				}
			}
		}
		cout << current_draw_map[i] << endl;
	}
	current_map.close();
	cout << "Character: " << character._name << " Race: " << character._race << " Class: " << character._class << " Gender: " << character._gender << endl;
	cout << "Coordinates: " << character.ch_x << "	" << character.ch_y << endl;
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a': character.ch_move = LEFT; break;
		case 'w': character.ch_move = UP; break;
		case 'd': character.ch_move = RIGHT; break;
		case 's': character.ch_move = DOWN; break;
		case 'x': gameOver = true; break;
		}
	}
}

void Logic()
{
	switch (character.ch_move)
	{
	case LEFT: if(character.ch_y > 1) --character.ch_y; DrawMap(character.ch_skin, current_map_name, character.ch_move); break;
	case UP: if(character.ch_x > 1) --character.ch_x; DrawMap(character.ch_skin, current_map_name, character.ch_move); break;
	case RIGHT: if(character.ch_y < width - 2) ++character.ch_y; DrawMap(character.ch_skin, current_map_name, character.ch_move); break;
	case DOWN: if(character.ch_x < height - 2) ++character.ch_x; DrawMap(character.ch_skin, current_map_name, character.ch_move); break;
	}
	character.ch_move = STOP;
}
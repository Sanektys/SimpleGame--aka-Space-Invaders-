#pragma once

#include "renderSystem.h"


//////////////////////////
// Константы
const int LEVEL_ROWS = SCREEN_ROWS;
const int LEVEL_COLUMNS = SCREEN_COLUMNS;

const float SHIP_SPEED = 35.0;
const float SHIP_FIRE_COOLDOWN = 0.2;

const float BULLET_SPEED = 40.0;

const float ALIEN_AMPLITUDE = 6.0;
const float ALIEN_SPEED = 0.23;


const unsigned char CELL_SYMBOL_SHIP   = 'S';
const unsigned char CELL_SYMBOL_BULLET = '|';
const unsigned char CELL_SYMBOL_ALIEN  = 'X';

const unsigned char FIRST_LEVEL_DATA[LEVEL_ROWS][LEVEL_COLUMNS + 1] =
{
	"                                                                               ",
	"      XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX      ",
	"        X  XXXX XX XXXXXXX  XXXXXXXXXXXXXXXXXXXXXXX  XXXXXXX XX XXXX  X        ",
	"      XXXXXXXXX    X  X  X  X  XXX  XXXXXXX  XXX  X  X  X  X    XXXXXXXXX      ",
	"           XXXX    X  X  X  XXXXXXX  XXXXX  XXXXXXX  X  X  X    XXXX           ",
	"           XXXX             X   XXXXXXXXXXXXXXX   X             XXXX           ",
	"           X  X                  XXXXXXXXXXXXX                  X  X           ",
	"                                 XX  XXXXX  XX                                 ",
	"                                 XX  X   X  XX                                 ",
	"                                 X           X                                 ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                       S                                       ",
};


///////////////////////////
// Функции
unsigned char GetRenderCellSymbol(unsigned char cellSymbol)
{
	switch (cellSymbol) {
		case CELL_SYMBOL_SHIP :
			return 202;

		case CELL_SYMBOL_BULLET :
			return 179;

		case CELL_SYMBOL_ALIEN :
			return 203;
	}

	return '?';
}

ConsoleColor GetRenderCellSymbolColor(unsigned char cellSymbol)
{
	switch (cellSymbol) {
		case CELL_SYMBOL_SHIP :	
			return ConsoleColor::WHITE;

		case CELL_SYMBOL_BULLET : 
			return ConsoleColor::RED;

		case CELL_SYMBOL_ALIEN :	
			return ConsoleColor::GREEN;
	}

	return ConsoleColor::GRAY;
}

ConsoleColor GetRenderCellSymbolBackgroundColor(unsigned char cellSymbol)
{
	switch (cellSymbol) {
		case CELL_SYMBOL_SHIP :
			return ConsoleColor::DARK_GRAY;
	}

	return ConsoleColor::BLACK;
}
#pragma once

#include "renderSystem.h"


//////////////////////////
// Константы
const int levelRows = screenRows;
const int levelColumns = screenColumns;

const float shipSpeed = 35.0;
const float shipFireCooldown = 0.2;

const float bulletSpeed = 40.0;

const float alienAmplitude = 6.0;
const float alienSpeed = 0.23;


const unsigned char CellSymbol_Ship		= 'S';
const unsigned char CellSymbol_Bullet	= '|';
const unsigned char CellSymbol_Alien	= 'X';

const unsigned char levelData0[levelRows][levelColumns + 1] =
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
	switch (cellSymbol)
	{
		case CellSymbol_Ship:	return 202;
		case CellSymbol_Bullet: return 179;
		case CellSymbol_Alien:	return 203;
	}
	return '?';
}

ConsoleColor GetRenderCellSymbolColor(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
		case CellSymbol_Ship:	return ConsoleColor::White;
		case CellSymbol_Bullet: return ConsoleColor::Red;
		case CellSymbol_Alien:	return ConsoleColor::Green;
	}
	return ConsoleColor::Gray;
}

ConsoleColor GetRenderCellSymbolBackgroundColor(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
		case CellSymbol_Ship:	return ConsoleColor::DarkGray;
	}

	return ConsoleColor::Black;
}
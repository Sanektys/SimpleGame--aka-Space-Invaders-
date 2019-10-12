#pragma once

#include "renderSystem.h"


//////////////////////////
// Константы
const int LEVEL_ROWS = SCREEN_ROWS;
const int LEVEL_COLUMNS = SCREEN_COLUMNS;
const int LEVELS_NUMBER = 4;

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
	"      XXXXXXXXXXXXXXXXXXXXXXX    XX XXXXXXX XX    XXXXXXXXXXXXXXXXXXXXXXX      ",
	"        X  X XX XX XXXXXXX  XXXXXXXXX  X  XXXXXXXXX  XXXXXXX XX XX X  X        ",
	"      XXXXXXX X    X  X  X  X  XXX  XXXXXXX  XXX  X  X  X  X    X XXXXXXX      ",
	"           X XX    X  X  X  XXXXXXX  X X X  XXXXXXX  X  X  X    XX X           ",
	"           XXX              X   XXXXXX X XXXXXX   X              XXX           ",
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
	"                                       S                                       "
};

const unsigned char SECOND_LEVEL_DATA[LEVEL_ROWS][LEVEL_COLUMNS + 1] =
{
	"                                                                               ",
	"     XX  XXXXXX  XX          XXXX XX       XX XXXX          XX  XXXXXX  XX     ",
	"       XXX    XXX              XXX  XX   XX  XXX              XXX    XXX       ",
	"        XX    XX                 X    X X    X                 XX    XX        ",
	"         XX  XX                  XX    X    XX                  XX  XX         ",
	"          X  X                    X    X    X                    X  X          ",
	"          X  X                    X   X X   X                    X  X          ",
	"         X    X                 XX  XX   XX  XX                 X    X         ",
	"         X    X                 X XX  XXX  XX X                 X    X         ",
	"        X      X               XXX   X X X   XXX               X      X        ",
	"                              XX    X     X    XX                              ",
	"                            XXX   XX       XX   XXX                            ",
	"                         XXX   XXX  XX   XX  XXX   XXX                         ",
	"                        X  XXXX       XXX       XXXX  X                        ",
	"                       XXXX                         XXXX                       ",
	"                      X                                 X                      ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                       S                                       "
};

const unsigned char THIRD_LEVEL_DATA[LEVEL_ROWS][LEVEL_COLUMNS + 1] =
{
	"                                                                               ",
	"                              XXXXXXXXXXXXXXXXXXX                              ",
	"                    XXXXXXXXXXXXX             XXXXXXXXXXXXX                    ",
	"            XXXXXXXXXX                XXX                XXXXXXXXXX            ",
	"      XXXXXXXX           XXX        XX   XX        XXX           XXXXXXXX      ",
	"                        XX XX      X       X      XX XX                        ",
	"          XXXXXXXX     X     X                   X     X     XXXXXXXX          ",
	"               XXXXXXXXX             X   X             XXXXXXXXX               ",
	"                      XXXXXXXXX       X X       XXXXXXXXX                      ",
	"                              XXXXXXX     XXXXXXX                              ",
	"                                   XXX   XXX                                   ",
	"                                     X   X                                     ",
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
	"                                       S                                       "
};

const unsigned char FOURTH_LEVEL_DATA[LEVEL_ROWS][LEVEL_COLUMNS + 1] =
{
	"                                                                               ",
	"                                    XX   XX                                    ",
	"                                    X XXX X                                    ",
	"                          XXXXXXX  X   X   X  XXXXXXX                          ",
	"                       XXXX     XXX    X    XXX     XXXX                       ",
	"                    XXX            X  X X  X            XXX                    ",
	"                  XX                XX   XX                XX                  ",
	"                 X        XXXXXXX     XXX     XXXXXXX        X                 ",
	"                X      XXXX     XXX         XXX     XXXX      X                ",
	"               X      XXX         XXX     XXX         XXX      X               ",
	"               X     XX            X X   X X            XX     X               ",
	"              X     XX              X X X X              XX     X              ",
	"             X     X                X     X                X     X             ",
	"             X    X                  X   X                  X    X             ",
	"            X   XX                    X X                    XX   X            ",
	"            XXXX                       X                       XXXX            ",
	"           XX                                                     XX           ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                       S                                       "
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
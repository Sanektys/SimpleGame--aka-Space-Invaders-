#pragma once

#include "renderSystem.h"


//////////////////////////
// Константы
const int LEVEL_ROWS    = SCREEN_ROWS;
const int LEVEL_COLUMNS = SCREEN_COLUMNS;
const int LEVELS_NUMBER = 4;

const float SHIP_SPEED         = 28.6;
const float SHIP_FIRE_COOLDOWN = 0.2;

const float BULLET_SPEED = 40.0;

const float BONUS_SPEED  = 1.6;
const float BONUS_FREEZING_TIME = 8.0;
const float BONUS_FAST_RECHARGE_TIME = 6.0;
const float BONUS_TRIPLE_FIRE_TIME = 5.0;

const float ALIEN_AMPLITUDE = 6.0;
const float ALIEN_SPEED     = 0.24;

const int ARMORED_ALIEN_HEALTH = 9;
const int HEAVY_ALIEN_HEALTH   = 3;

const unsigned char CELL_SYMBOL_SHIP             = 'S';
const unsigned char CELL_SYMBOL_BULLET           = '|';
const unsigned char CELL_SYMBOL_ALIEN_BULLET     = '/';
const unsigned char CELL_SYMBOL_ALIEN            = 'X';
const unsigned char CELL_SYMBOL_MOTHERSHIP_ALIEN = 'M';
const unsigned char CELL_SYMBOL_HEAVY_ALIEN      = 'H';
const unsigned char CELL_SYMBOL_ARMORED_ALIEN    = 'T';
const unsigned char CELL_SYMBOL_SHOOTING_ALIEN   = 'G';
const unsigned char CELL_SYMBOL_B_FREEZING       = 'F';
const unsigned char CELL_SYMBOL_B_TRIPLE_FIRE    = 'P';
const unsigned char CELL_SYMBOL_B_FAST_RECHARGE  = 'R';

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
	"         MX  XM                  XX    X    XX                  MX  XM         ",
	"          X  X                    X    X    X                    X  X          ",
	"          X  X                    X   XMX   X                    X  X          ",
	"         X    X                 XX  XXMMMXX  XX                 X    X         ",
	"         X    X                 X XX  XXX  XX X                 X    X         ",
	"        M      M               XXX   X X X   XXX               M      M        ",
	"                              XX    X  M  X    XX                              ",
	"                            XXXM  XX  MXM  XX  MXXX                            ",
	"                         XXXMX XXX  XX   XX  XXX XMXXX                         ",
	"                        XM XXXX       XXX       XXXX MX                        ",
	"                       XXXX                         XXXX                       ",
	"                      M                                 M                      ",
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
	"                              XXXXXXHHXHXHHXXXXXX                              ",
	"                    XXXXXXXXXXHHH             HHHXXXXXXXXXX                    ",
	"            XXXXXXXXHH                HHH                HHXXXXXXXX            ",
	"      XXXXXXHH           HHH        HHTTTHH        HHH           HHXXXXXX      ",
	"                        HHTHH      HTT   TTH      HHTHH                        ",
	"          XXXXXHHH     HTT TTH     T T   T T     HTT TTH     HHHXXXXX          ",
	"               XXXXXXXHT     T       XT TX       T     THXXXXXXX               ",
	"                      XXXXXXXXH       X X       HXXXXXXXX                      ",
	"                              XXXXHHH     HHHXXXX                              ",
	"                                   XHH   HHX                                   ",
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
	"                                    HT   TH                                    ",
	"                                    H THT H                                    ",
	"                          HHHHHHH  H   T   H  HHHHHHH                          ",
	"                       HHHH     HHH    H    HHH     HHHH                       ",
	"                    HHH            H  T T  H            HHH                    ",
	"                  HH                HT   TH                HH                  ",
	"                 H        HHHHHHH     TTT     HHHHHHH        H                 ",
	"                H      HHHH     HHH         HHH     HHHH      H                ",
	"               H      HHH         HMM     MMH         HHH      H               ",
	"               H     HH            H M   M H            HH     H               ",
	"              H     HH              H  T  H              HH     H              ",
	"             H     H                H T T H                H     H             ",
	"             H    H                  H   H                  H    H             ",
	"            H   HH                    H H                    HH   H            ",
	"            HMMH                       H                       HMMH            ",
	"           MM                                                     MM           ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                                                               ",
	"                                       S                                       "
};


///////////////////////////
unsigned char GetRenderCellSymbol(unsigned char cellSymbol)
{
	switch (cellSymbol) {
		case CELL_SYMBOL_SHIP :
			return 202;

		case CELL_SYMBOL_BULLET :
			return 179;

		case CELL_SYMBOL_ALIEN_BULLET :
			return 179;

		case CELL_SYMBOL_ALIEN :
			return 203;

		case CELL_SYMBOL_MOTHERSHIP_ALIEN :
			return 197;

		case CELL_SYMBOL_HEAVY_ALIEN :
			return 203;

		case CELL_SYMBOL_ARMORED_ALIEN :
			return 206;

		case CELL_SYMBOL_SHOOTING_ALIEN :
			return 210;

		case CELL_SYMBOL_B_FREEZING :
			return 35;

		case CELL_SYMBOL_B_FAST_RECHARGE :
			return 60;

		case CELL_SYMBOL_B_TRIPLE_FIRE :
			return 33;
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

		case CELL_SYMBOL_ALIEN_BULLET :
			return ConsoleColor::BLUE;

		case CELL_SYMBOL_MOTHERSHIP_ALIEN :
			return ConsoleColor::DARK_GREEN;

		case CELL_SYMBOL_HEAVY_ALIEN :
			return ConsoleColor::GREEN;

		case CELL_SYMBOL_ARMORED_ALIEN :
			return ConsoleColor::GREEN;

		case CELL_SYMBOL_SHOOTING_ALIEN :
			return ConsoleColor::GREEN;

		case CELL_SYMBOL_B_FREEZING:
			return ConsoleColor::WHITE;

		case CELL_SYMBOL_B_FAST_RECHARGE:
			return ConsoleColor::WHITE;

		case CELL_SYMBOL_B_TRIPLE_FIRE:
			return ConsoleColor::WHITE;
	}

	return ConsoleColor::GRAY;
}

ConsoleColor GetRenderCellSymbolBackgroundColor(unsigned char cellSymbol)
{
	switch (cellSymbol) {
		case CELL_SYMBOL_SHIP :
			return ConsoleColor::DARK_GRAY;

		case CELL_SYMBOL_ARMORED_ALIEN :
			return ConsoleColor::DARK_BLUE;

		case CELL_SYMBOL_B_FREEZING:
			return ConsoleColor::DARK_CYAN;

		case CELL_SYMBOL_B_FAST_RECHARGE:
			return ConsoleColor::DARK_YELLOW;

		case CELL_SYMBOL_B_TRIPLE_FIRE:
			return ConsoleColor::DARK_RED;
	}

	return ConsoleColor::BLACK;
}
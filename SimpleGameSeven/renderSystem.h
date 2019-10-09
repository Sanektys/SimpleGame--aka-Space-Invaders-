#pragma once

#include "Windows.h"
#include "consoleColor.h"


/////////////////////////
// Константы
const int screenRows = 25;
const int screenColumns = 80;


/////////////////////////
// Структура
struct ConsoleSymbolData
{
	char symbol;
	ConsoleColor symbolColor;
	ConsoleColor backgroundColor;
};


/////////////////////////
// Класс системы отрисовки
class RenderSystem
{
public:
	RenderSystem();

	void initialize();
	void clear();
	void drawChar(int r, int c, char symbol, ConsoleColor symbolColor, ConsoleColor backgroundColor);
	void drawText(int r, int c, const char* text, ConsoleColor symbolColor, ConsoleColor backgroundColor);
	void flush();

private:
	void setConsoleCursor(int r, int c);
	void setConsoleColor(ConsoleColor symbolColor, ConsoleColor backgroundColor);

private:
	HANDLE m_consoleHandle;
	ConsoleSymbolData m_backBuffer[screenRows][screenColumns];
	ConsoleSymbolData m_screenBuffer[screenRows][screenColumns];
};
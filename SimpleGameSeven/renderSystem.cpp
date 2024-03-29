#include <cstdio>

#include "renderSystem.h"


//////////////////////////
RenderSystem::RenderSystem()
{
	m_consoleHandle = 0;

	for (int r = 0; r < SCREEN_ROWS; r++) {
		for (int c = 0; c < SCREEN_COLUMNS; c++) {
			m_backBuffer[r][c].symbol = 0;
			m_backBuffer[r][c].symbolColor = ConsoleColor::GRAY;
			m_backBuffer[r][c].backgroundColor = ConsoleColor::BLACK;

			m_screenBuffer[r][c] = m_backBuffer[r][c];
		}
	}
}

void RenderSystem::initialize()
{
	// ��������� ����������� �������
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// ������� ������� �������
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(m_consoleHandle, &cursorInfo);
}

void RenderSystem::clear()
{
	for (int r = 0; r < SCREEN_ROWS; r++) {
		for (int c = 0; c < SCREEN_COLUMNS; c++) {
			m_backBuffer[r][c].symbol = 0;
			m_backBuffer[r][c].backgroundColor = ConsoleColor::BLACK;
		}
	}
}

void RenderSystem::drawChar(int r, int c, char symbol,
	                        ConsoleColor symbolColor, ConsoleColor backgroundColor)
{
	if (r < 0 || c < 0 || r >= SCREEN_ROWS || c >= SCREEN_COLUMNS)
		return;

	m_backBuffer[r][c].symbol = symbol;
	m_backBuffer[r][c].symbolColor = symbolColor;
	m_backBuffer[r][c].backgroundColor = backgroundColor;
}

void RenderSystem::drawText(int r, int c, const char* text,
	                        ConsoleColor symbolColor, ConsoleColor backgroundColor)
{
	int column = c;
	char symbol = *text;

	while (symbol != 0) {
		drawChar(r, column, symbol, symbolColor, backgroundColor);

		text++;
		column++;
		symbol = *text;
	}
}

void RenderSystem::flush()
{
	bool screenBufferModified = false;

	for (int r = 0; r < SCREEN_ROWS; r++) {
		for (int c = 0; c < SCREEN_COLUMNS; c++) {
			bool isSymbolDifferent = m_backBuffer[r][c].symbol != m_screenBuffer[r][c].symbol;
			bool isColorDifferent = m_backBuffer[r][c].symbolColor != m_screenBuffer[r][c].symbolColor;
			bool isBgColorDifferent = m_backBuffer[r][c].backgroundColor != m_screenBuffer[r][c].backgroundColor;
			if ((isSymbolDifferent) || (isColorDifferent) || (isBgColorDifferent)) {
				// ����������� ���������� � ������� �� ��������������� �� �������� �����
				m_screenBuffer[r][c] = m_backBuffer[r][c];

				// ��������� ������� �� ������� (r,c)
				setConsoleCursor(r, c);
				setConsoleColor(m_screenBuffer[r][c].symbolColor, m_screenBuffer[r][c].backgroundColor);
				printf("%c", m_screenBuffer[r][c].symbol);

				screenBufferModified = true;
			}
		}
	}

	// ������� ������� ������� �� ������� �������
	if (screenBufferModified)
		setConsoleCursor(0,0);
}

void RenderSystem::setConsoleCursor(int r, int c)
{
	COORD cursorCoord;
	cursorCoord.X = c;
	cursorCoord.Y = r;
	SetConsoleCursorPosition(m_consoleHandle, cursorCoord);
}

void RenderSystem::setConsoleColor(ConsoleColor symbolColor, ConsoleColor backgroundColor)
{
	short consoleColor = (int)symbolColor | ((int)backgroundColor << 4);
	SetConsoleTextAttribute(m_consoleHandle, consoleColor);
}
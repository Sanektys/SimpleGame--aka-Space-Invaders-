#include <cstdlib>
#include <cstdio>

#include "game.h"
#include "level.h"
#include "input.h"


///////////////////////
Game::Game()
{
	m_isGameActive = true;
	m_isLosing = false;
	m_isExitGame = false;
	m_clockLastFrame = 0;
	m_frameTime = 0.0;
	m_frameCount = 0;
	m_fps = 0;
	m_gameTime = 0.0;

	m_currentLevel = 1;

	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++)
		m_objects[i] = 0;

	m_shipFireCooldownTime = 0.0;
	m_alienAmplitudeTime = 0.0;

	m_gamePoints = 0;
	m_lastLevelPoints = 0;
}

void Game::setupSystem()
{
	srand(time(0));

	m_renderSystem.initialize();
}

void Game::initialize()
{
	m_shipFireCooldownTime = 0.0;
	m_alienAmplitudeTime = 0.0;
	if (m_currentLevel == 1) {
		m_gamePoints = 0;
		m_gameTime = 0.0;
	}

	// Загрузка уровня
	for (int r = 0; r < LEVEL_ROWS; r++) {
		for (int c = 0; c < LEVEL_COLUMNS; c++) {
			unsigned char cellSymbol = ' ';
			switch (m_currentLevel) {
				case 1 :
					cellSymbol = FIRST_LEVEL_DATA[r][c];
					break;

				case 2 :
					cellSymbol = SECOND_LEVEL_DATA[r][c];
					break;

				case 3 :
					cellSymbol = THIRD_LEVEL_DATA[r][c];
					break;

				case 4 :
					cellSymbol = FOURTH_LEVEL_DATA[r][c];
					break;
			}

			switch (cellSymbol) {
				// Инициализация корабля
				case CELL_SYMBOL_SHIP : {
					createObject(GameObjectType::SHIP, (c + 0.5), r,
                                 GetRenderCellSymbol(cellSymbol),
                                 GetRenderCellSymbolColor(cellSymbol),
                                 GetRenderCellSymbolBackgroundColor(cellSymbol));
					break;
				}

				// Инициализация корабля пришельца
				case CELL_SYMBOL_ALIEN : {
					GameObject* alien = createObject(GameObjectType::ALIEN, (c + 0.5), r,
                                                     GetRenderCellSymbol(cellSymbol),
                                                     GetRenderCellSymbolColor(cellSymbol),
                                                     GetRenderCellSymbolBackgroundColor(cellSymbol));
					alien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					alien->setYSpeed(ALIEN_SPEED);
					break;
				}
			}
		}
	}
}

void Game::canIncreaseLevel()
{
	if (m_isExitGame) {
		m_isGameActive = false;
		return;
	}

	if (!m_isGameActive && !m_isLosing) {
		if (m_currentLevel < 4) {
			m_currentLevel++;
			m_isGameActive = true;
			m_lastLevelPoints = m_gamePoints;
			deleteAllObjects();
			initialize();
		}
		else {
			m_isGameActive = false;
		}
	}
	else if (m_isLosing) {
		m_isGameActive = false;
	}
}

bool Game::frame()
{
	// Расчёт дельты фремени
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_clockLastFrame;
	float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
	m_clockLastFrame = clockNow;
	m_gameTime += deltaTime;

	// Расчёт FPS
	m_frameTime += deltaTime;
	m_frameCount++;
	if (m_frameTime >= 1.0) {
		m_frameTime = 0;
		m_fps = m_frameCount;
		m_frameCount = 0;
	}

	render();
	update(deltaTime);
	canIncreaseLevel();

	return m_isGameActive;
}

void Game::render()
{
	// Начало кадра
	m_renderSystem.clear();

	// Отрисовка всех игровых объектов
	int objectsCount = 0;
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++)
		if (m_objects[i] != 0) {
			m_objects[i]->render(&m_renderSystem);
			objectsCount++;
		}

	// Вывод счётчика объектов
	char buff[64];
	sprintf_s(buff, "Objects: %d", objectsCount);
	m_renderSystem.drawText(0, 0, buff, ConsoleColor::DARK_GRAY, ConsoleColor::BLACK);

	// Вывод счётчика кадров
	sprintf_s(buff, "FPS: %i", m_fps);
	m_renderSystem.drawText(0, 14, buff, ConsoleColor::DARK_GRAY, ConsoleColor::BLACK);

	// Вывод счётчика игровых очков
	sprintf_s(buff, "Points: %i", m_gamePoints);
	m_renderSystem.drawText(0, 65, buff, ConsoleColor::CYAN, ConsoleColor::BLACK);

	// Конец кадра
	m_renderSystem.flush();
}

void Game::update(float dt)
{
	bool haveAliveAliens = false;

	// Обновление всех игровых объектов
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
		GameObject* object = m_objects[i];

		if (object != 0) {
			object->update(dt);

			switch (object->getType()) {
				// Корабль героя
				case GameObjectType::SHIP : {
					// Захождение за границы
					if (object->getX() < 0)
						object->setX(0);
					else if (object->getX() > SCREEN_COLUMNS - 1)
						object->setX(SCREEN_COLUMNS - 1);

					// Движение
					if (IsKeyDown(VK_LEFT))
						object->setXSpeed(-SHIP_SPEED);
					else if (IsKeyDown(VK_RIGHT))
						object->setXSpeed(SHIP_SPEED);
					else
						object->setXSpeed(0.0);
	
					// Стрельба
					if (IsKeyDown(VK_SPACE)) {
						if (m_shipFireCooldownTime <= 0.0) {
							m_shipFireCooldownTime = SHIP_FIRE_COOLDOWN;

							// Инициализация пули
							GameObject* bullet = createObject(GameObjectType::BULLET,
								                              object->getX(), object->getY() - 1,
                                                              GetRenderCellSymbol(CELL_SYMBOL_BULLET),
				                                              GetRenderCellSymbolColor(CELL_SYMBOL_BULLET),
						                                      GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_BULLET));
							bullet->setYSpeed(-BULLET_SPEED);
						}
					}
				}

				// Пуля
				case GameObjectType::BULLET : {
					if (object->getY() < 0) {
						destroyObject(object);
					}
					else {
						for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
							GameObject* anotherObject = m_objects[i];

							if (anotherObject != 0) {
								if (anotherObject->getType() == GameObjectType::ALIEN) {
									if (anotherObject->intersects(object)) {
										destroyObject(anotherObject);
										destroyObject(object);
										m_gamePoints += 2;
										break;
									}
								}
							}
						}
					}
					break;
				}

				// Корабль пришельца
				case GameObjectType::ALIEN : {
					haveAliveAliens = true;

					if (object->getY() >= SCREEN_ROWS - 1)
						m_isLosing = true;
					else
						object->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					break;
				}
			}
		}
	}

	// Перезарядка орудия
	if (m_shipFireCooldownTime > 0.0)
		m_shipFireCooldownTime -= dt;

	// Время колебательного движения пришельцев
	m_alienAmplitudeTime += dt;

	if (IsKeyDown(VK_ESCAPE))
		m_isExitGame = true;

	// Победа
	if (!haveAliveAliens)
		m_isGameActive = false;
}

GameObject* Game::createObject(GameObjectType type, float x, float y, char symbol,
	                           ConsoleColor color, ConsoleColor bgColor)
{
	// Нахождение свободного указателя и инициализация объекта
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
		if (m_objects[i] == 0) {
			GameObject* object = new GameObject();

			object->setType(type);
			object->setX(x);
			object->setY(y);
			object->setSymbol(symbol);
			object->setColor(color);
			object->setBackgroundColor(bgColor);

			m_objects[i] = object;
			return object;
		}
	}

	return 0;
}

void Game::destroyObject(GameObject* object)
{
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
		if (m_objects[i] == object) {
			delete m_objects[i];
			m_objects[i] = 0;
			return;
		}
	}
}

void Game::deleteAllObjects()
{
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++)
		if (m_objects[i] != 0) {
			delete m_objects[i];
			m_objects[i] = 0;
		}
}

bool Game::shutdown()
{
	deleteAllObjects();
	m_renderSystem.clear();
	m_renderSystem.flush();

	if (m_isExitGame)
		return true;

	// Расчёт потраченного времени
	int spendedMinutes = m_gameTime / 60;
	int spendedSeconds = m_gameTime - (spendedMinutes * 60);

	char buff[64];

	if (!m_isGameActive && !m_isLosing) {
		Sleep(400);
		sprintf_s(buff, "You are...");
		m_renderSystem.drawText(4, 34, buff, ConsoleColor::YELLOW, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(1200);
		sprintf_s(buff, "W I N N E R !!!");
		m_renderSystem.drawText(7, 32, buff, ConsoleColor::GREEN, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(600);
		sprintf_s(buff, "Spended time: %i min %i sec", spendedMinutes, spendedSeconds);
		m_renderSystem.drawText(11, 26, buff, ConsoleColor::WHITE, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(600);
		sprintf_s(buff, "Earned points: %i", m_gamePoints);
		m_renderSystem.drawText(13, 30, buff, ConsoleColor::CYAN, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(800);
		sprintf_s(buff, "Press ENTER or ESCAPE for exit...");
		m_renderSystem.drawText(16, 24, buff, ConsoleColor::DARK_GRAY, ConsoleColor::BLACK);
		m_renderSystem.flush();

		while (!IsKeyDown(VK_RETURN) && !IsKeyDown(VK_ESCAPE))
			;

		return true;
	}
	else if (m_isLosing) {
		Sleep(600);
		sprintf_s(buff, "You were defeated");
		m_renderSystem.drawText(4, 31, buff, ConsoleColor::RED, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(600);
		sprintf_s(buff, "Spended time: %i min %i sec", spendedMinutes, spendedSeconds);
		m_renderSystem.drawText(7, 27, buff, ConsoleColor::WHITE, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(600);
		sprintf_s(buff, "Earned points: %i", m_gamePoints);
		m_renderSystem.drawText(9, 31, buff, ConsoleColor::CYAN, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(800);
		sprintf_s(buff, "Press R to repeat or ESCAPE for exit...");
		m_renderSystem.drawText(12, 21, buff, ConsoleColor::DARK_GRAY, ConsoleColor::BLACK);
		m_renderSystem.flush();

		do {
			if (IsKeyDown(0x52)) {
				m_clockLastFrame = clock();
				m_isLosing = false;
				m_isGameActive = true;
				m_gamePoints = m_lastLevelPoints;
				return false;
			}
			else if (IsKeyDown(VK_ESCAPE))
				return true;
		} while (true);
	}

	return true;
}
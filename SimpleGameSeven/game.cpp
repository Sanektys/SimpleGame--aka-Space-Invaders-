#include <cstdlib>
#include <cstdio>
#include "game.h"
#include "level.h"
#include "input.h"


///////////////////////
Game::Game()
{
	m_isGameActive = true;
	m_clockLastFrame = 0;
	m_frameTime = 0.0;
	m_frameCount = 0;
	m_FPS = 0;

	for (int i = 0; i < gameObjectsCountMax; i++)
		m_objects[i] = 0;

	m_shipFireCooldownTime = 0.0;
	m_alienAmplitudeTime = 0.0;
}

void Game::setupSystem()
{
	srand(time(NULL));

	m_renderSystem.initialize();
}

void Game::initialize()
{
	m_shipFireCooldownTime = 0.0;
	m_alienAmplitudeTime = 0.0;

	// Загрузка уровня
	for (int r = 0; r < levelRows; r++)
	{
		for (int c = 0; c < levelColumns; c++)
		{
			unsigned char cellSymbol = levelData0[r][c];

			switch (cellSymbol)
			{
				// Инициализация корабля
				case CellSymbol_Ship:
				{
					createObject(GameObjectType::Ship, (c + 0.5), r,
									GetRenderCellSymbol(cellSymbol),
									GetRenderCellSymbolColor(cellSymbol),
									GetRenderCellSymbolBackgroundColor(cellSymbol));
					break;
				}

				//
				case CellSymbol_Alien:
				{
					GameObject* alien = createObject(GameObjectType::Alien, (c + 0.5), r,
														GetRenderCellSymbol(cellSymbol),
														GetRenderCellSymbolColor(cellSymbol),
														GetRenderCellSymbolBackgroundColor(cellSymbol));
					alien->setXSpeed(alienAmplitude * cos(m_alienAmplitudeTime));
					alien->setYSpeed(alienSpeed);
					break;
				}
			}
		}
	}
}

bool Game::frame()
{
	// Расчёт дельты фремени
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_clockLastFrame;
	float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
	m_clockLastFrame = clockNow;

	// Расчёт FPS
	m_frameTime += deltaTime;
	m_frameCount++;
	if (m_frameTime >= 1.0)
	{
		m_frameTime = 0;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	render();
	update(deltaTime);

	return m_isGameActive;
}

void Game::shutdown()
{
	for (int i = 0; i < gameObjectsCountMax; i++)
		if (m_objects[i] != 0)
		{
			delete m_objects[i];
			m_objects[i] = 0;
		}
}

void Game::render()
{
	// Начало кадра
	m_renderSystem.clear();

	// Отрисовка всех игровых объектов
	int objectsCount = 0;
	for (int i = 0; i < gameObjectsCountMax; i++)
		if (m_objects[i] != 0)
		{
			m_objects[i]->render(&m_renderSystem);
			objectsCount++;
		}

	// Вывод счётчика объектов
	char buff[64];
	sprintf_s(buff, "Objects: %d", objectsCount);
	m_renderSystem.drawText(0, 0, buff, ConsoleColor::Gray, ConsoleColor::Black);

	// Вывод счётчика кадров
	sprintf_s(buff, "FPS: %i", m_FPS);
	m_renderSystem.drawText(0, 16, buff, ConsoleColor::Gray, ConsoleColor::Black);

	// Конец кадра
	m_renderSystem.flush();
}

void Game::update(float dt)
{
	bool haveAliveAliens = false;

	// Обновление всех игровых объектов
	for (int i = 0; i < gameObjectsCountMax; i++)
	{
		GameObject* object = m_objects[i];

		if (object != 0)
		{
			object->update(dt);

			switch (object->getType())
			{
				// Корабль героя
				case GameObjectType::Ship:
				{
					// Захождение за границы
					if (object->getX() < 0)
						object->setX(0);
					else if (object->getX() > screenColumns - 1)
						object->setX(screenColumns - 1);

					// Движение
					if (IsKeyDown(VK_LEFT))
						object->setXSpeed(-shipSpeed);
					else if (IsKeyDown(VK_RIGHT))
						object->setXSpeed(shipSpeed);
					else
						object->setXSpeed(0.0);
	
					// Стрельба
					if (IsKeyDown(VK_SPACE))
					{
						if (m_shipFireCooldownTime <= 0.0)
						{
							m_shipFireCooldownTime = shipFireCooldown;

							// Инициализация пули
							GameObject* bullet = createObject(GameObjectType::Bullet, object->getX(), object->getY() - 1,
																GetRenderCellSymbol(CellSymbol_Bullet),
																GetRenderCellSymbolColor(CellSymbol_Bullet),
																GetRenderCellSymbolBackgroundColor(CellSymbol_Bullet));
							bullet->setYSpeed(-bulletSpeed);
						}
					}
				}

				// Пуля
				case GameObjectType::Bullet:
				{
					if (object->getY() < 0)
					{
						destroyObject(object);
					}
					else
					{
						for (int i = 0; i < gameObjectsCountMax; i++)
						{
							GameObject* anotherObject = m_objects[i];

							if (anotherObject != 0)
							{
								if (anotherObject->getType() == GameObjectType::Alien)
								{
									if (anotherObject->intersects(object))
									{
										destroyObject(anotherObject);
										destroyObject(object);
										break;
									}
								}
							}
						}
					}
					break;
				}

				// Корабль пришельца
				case GameObjectType::Alien:
				{
					haveAliveAliens = true;

					if (object->getY() >= screenRows)
						m_isGameActive = false;
					else
						object->setXSpeed(alienAmplitude * cos(m_alienAmplitudeTime));
					break;
				}
			}
		}
	}

	//
	if (m_shipFireCooldownTime > 0.0)
		m_shipFireCooldownTime -= dt;

	// Время колебательного движения пришельцев
	m_alienAmplitudeTime += dt;

	// Победа
	if (!haveAliveAliens)
		m_isGameActive = false;
}

GameObject* Game::createObject(GameObjectType type, float x, float y, char symbol, ConsoleColor color, ConsoleColor bgColor)
{
	// Нахождение свободного указателя и инициализация объекта
	for (int i = 0; i < gameObjectsCountMax; i++)
	{
		if (m_objects[i] == 0)
		{
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
	for (int i = 0; i < gameObjectsCountMax; i++)
	{
		if (m_objects[i] == object)
		{
			delete m_objects[i];
			m_objects[i] = 0;
			return;
		}
	}
}
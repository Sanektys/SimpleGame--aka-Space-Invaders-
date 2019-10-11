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
	m_fps = 0;

	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++)
		m_objects[i] = 0;

	m_shipFireCooldownTime = 0.0;
	m_alienAmplitudeTime = 0.0;
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

	// �������� ������
	for (int r = 0; r < LEVEL_ROWS; r++) {
		for (int c = 0; c < LEVEL_COLUMNS; c++) {
			unsigned char cellSymbol = FIRST_LEVEL_DATA[r][c];

			switch (cellSymbol) {
				// ������������� �������
				case CELL_SYMBOL_SHIP :
				{
					createObject(GameObjectType::SHIP, (c + 0.5), r,
                                 GetRenderCellSymbol(cellSymbol),
                                 GetRenderCellSymbolColor(cellSymbol),
                                 GetRenderCellSymbolBackgroundColor(cellSymbol));
					break;
				}

				// ������������� ������� ���������
				case CELL_SYMBOL_ALIEN :
				{
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

bool Game::frame()
{
	// ������ ������ �������
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_clockLastFrame;
	float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
	m_clockLastFrame = clockNow;

	// ������ FPS
	m_frameTime += deltaTime;
	m_frameCount++;
	if (m_frameTime >= 1.0) {
		m_frameTime = 0;
		m_fps = m_frameCount;
		m_frameCount = 0;
	}

	render();
	update(deltaTime);

	return m_isGameActive;
}

void Game::shutdown()
{
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++)
		if (m_objects[i] != 0) {
		    delete m_objects[i];
			m_objects[i] = 0;
		}
}

void Game::render()
{
	// ������ �����
	m_renderSystem.clear();

	// ��������� ���� ������� ��������
	int objectsCount = 0;
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++)
		if (m_objects[i] != 0) {
			m_objects[i]->render(&m_renderSystem);
			objectsCount++;
		}

	// ����� �������� ��������
	char buff[64];
	sprintf_s(buff, "Objects: %d", objectsCount);
	m_renderSystem.drawText(0, 0, buff, ConsoleColor::GRAY, ConsoleColor::BLACK);

	// ����� �������� ������
	sprintf_s(buff, "FPS: %i", m_fps);
	m_renderSystem.drawText(0, 16, buff, ConsoleColor::GRAY, ConsoleColor::BLACK);

	// ����� �����
	m_renderSystem.flush();
}

void Game::update(float dt)
{
	bool haveAliveAliens = false;

	// ���������� ���� ������� ��������
	for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
		GameObject* object = m_objects[i];

		if (object != 0) {
			object->update(dt);

			switch (object->getType()) {
				// ������� �����
				case GameObjectType::SHIP : {
					// ���������� �� �������
					if (object->getX() < 0)
						object->setX(0);
					else if (object->getX() > SCREEN_COLUMNS - 1)
						object->setX(SCREEN_COLUMNS - 1);

					// ��������
					if (IsKeyDown(VK_LEFT))
						object->setXSpeed(-SHIP_SPEED);
					else if (IsKeyDown(VK_RIGHT))
						object->setXSpeed(SHIP_SPEED);
					else
						object->setXSpeed(0.0);
	
					// ��������
					if (IsKeyDown(VK_SPACE)) {
						if (m_shipFireCooldownTime <= 0.0) {
							m_shipFireCooldownTime = SHIP_FIRE_COOLDOWN;

							// ������������� ����
							GameObject* bullet = createObject(GameObjectType::BULLET,
								                              object->getX(), object->getY() - 1,
                                                              GetRenderCellSymbol(CELL_SYMBOL_BULLET),
				                                              GetRenderCellSymbolColor(CELL_SYMBOL_BULLET),
						                                      GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_BULLET));
							bullet->setYSpeed(-BULLET_SPEED);
						}
					}
				}

				// ����
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
										break;
									}
								}
							}
						}
					}
					break;
				}

				// ������� ���������
				case GameObjectType::ALIEN : {
					haveAliveAliens = true;

					if (object->getY() >= SCREEN_ROWS)
						m_isGameActive = false;
					else
						object->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					break;
				}
			}
		}
	}

	// ����������� ������
	if (m_shipFireCooldownTime > 0.0)
		m_shipFireCooldownTime -= dt;

	// ����� �������������� �������� ����������
	m_alienAmplitudeTime += dt;

	// ������
	if (!haveAliveAliens)
		m_isGameActive = false;
}

GameObject* Game::createObject(GameObjectType type, float x, float y, char symbol,
	                           ConsoleColor color, ConsoleColor bgColor)
{
	// ���������� ���������� ��������� � ������������� �������
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
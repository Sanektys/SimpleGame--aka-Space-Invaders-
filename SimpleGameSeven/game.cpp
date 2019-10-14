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
	m_alienFireCooldownTime = 0.0;
	m_alienAmplitudeTime = 0.0;
	m_bonusFreezingTime = 0.0;
	m_bonusFastRechargeTime = 0.0;
	m_bonusTripleFireTime = 0.0;

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
				// Инициализация корабля игрока
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

                // Инициализация корабля-носителя пришельцев
				case CELL_SYMBOL_MOTHERSHIP_ALIEN : {
					GameObject* mothership = createObject(GameObjectType::MOTHERSHIP_ALIEN, (c + 0.5), r,
						                                  GetRenderCellSymbol(cellSymbol),
						                                  GetRenderCellSymbolColor(cellSymbol),
						                                  GetRenderCellSymbolBackgroundColor(cellSymbol));
					mothership->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					mothership->setYSpeed(ALIEN_SPEED);
					break;
				}

				// Инициализация "тяжёлого" корабля пришельцев
				case CELL_SYMBOL_HEAVY_ALIEN : {
					GameObject* armoredAlien = createObject(GameObjectType::HEAVY_ALIEN, (c + 0.5), r,
						                                    GetRenderCellSymbol(cellSymbol),
						                                    GetRenderCellSymbolColor(cellSymbol),
						                                    GetRenderCellSymbolBackgroundColor(cellSymbol));
					armoredAlien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					armoredAlien->setYSpeed(ALIEN_SPEED);
					armoredAlien->setHealth(HEAVY_ALIEN_HEALTH);
					break;
				}

				// Инициализация "бронированного" корабля пришельцев
				case CELL_SYMBOL_ARMORED_ALIEN : {
					GameObject* armoredAlien = createObject(GameObjectType::ARMORED_ALIEN, (c + 0.5), r,
						                                    GetRenderCellSymbol(cellSymbol),
						                                    GetRenderCellSymbolColor(cellSymbol),
						                                    GetRenderCellSymbolBackgroundColor(cellSymbol));
					armoredAlien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					armoredAlien->setYSpeed(ALIEN_SPEED);
					armoredAlien->setHealth(ARMORED_ALIEN_HEALTH);
					break;
				}

				// Инициализация стреляющего пришельца
				case CELL_SYMBOL_SHOOTING_ALIEN : {
					GameObject* shootingAlien = createObject(GameObjectType::SHOOTING_ALIEN, (c + 0.5), r,
						                                     GetRenderCellSymbol(cellSymbol),
						                                     GetRenderCellSymbolColor(cellSymbol),
						                                     GetRenderCellSymbolBackgroundColor(cellSymbol));
					shootingAlien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
					shootingAlien->setYSpeed(ALIEN_SPEED);
					break;
				}
			}
		}
	}
}

void Game::spawnBonus(bool order, GameObject* object)
{
	if (!order)
		return;

	int chance = 1 + rand() % 100;

	if (chance >= 1 && chance < 60) {
		GameObject* bonus = createObject(GameObjectType::BONUS_FREEZING,
			                             object->getX(), object->getY(),
			                             GetRenderCellSymbol(CELL_SYMBOL_B_FREEZING),
			                             GetRenderCellSymbolColor(CELL_SYMBOL_B_FREEZING),
			                             GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_B_FREEZING));
		bonus->setYSpeed(BONUS_SPEED);
	}
	else if (chance >= 60 && chance < 90) {
		GameObject* bonus = createObject(GameObjectType::BONUS_FAST_RECHARGE,
			                             object->getX(), object->getY(),
			                             GetRenderCellSymbol(CELL_SYMBOL_B_FAST_RECHARGE),
			                             GetRenderCellSymbolColor(CELL_SYMBOL_B_FAST_RECHARGE),
			                             GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_B_FAST_RECHARGE));
		bonus->setYSpeed(BONUS_SPEED);
	}
	else {
		GameObject* bonus = createObject(GameObjectType::BONUS_TRIPLE_FIRE,
			                             object->getX(), object->getY(),
			                             GetRenderCellSymbol(CELL_SYMBOL_B_TRIPLE_FIRE),
			                             GetRenderCellSymbolColor(CELL_SYMBOL_B_TRIPLE_FIRE),
			                             GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_B_TRIPLE_FIRE));
		bonus->setYSpeed(BONUS_SPEED);
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
			m_bonusFreezingTime = 0.0;
			m_bonusFastRechargeTime = 0.0;
			m_bonusTripleFireTime = 0.0;
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

	char buff[64];
	// Вывод счётчика кадров
	sprintf_s(buff, "FPS: %i", m_fps);
	m_renderSystem.drawText(0, 0, buff, ConsoleColor::DARK_GRAY, ConsoleColor::BLACK);

	// Вывод уведомления о бонусах
	if (m_bonusFreezingTime > 0.0) {
		sprintf_s(buff, "Freezing: %i", (int)m_bonusFreezingTime);
		m_renderSystem.drawText(0, 15, buff, ConsoleColor::BLACK, ConsoleColor::DARK_CYAN);
	}
	
	if (m_bonusFastRechargeTime > 0.0) {
		sprintf_s(buff, "Fast recharge: %i", (int)m_bonusFastRechargeTime);
		m_renderSystem.drawText(0, 28, buff, ConsoleColor::BLACK, ConsoleColor::DARK_YELLOW);
	}
	
	if (m_bonusTripleFireTime > 0.0) {
		sprintf_s(buff, "Triple fire: %i", (int)m_bonusTripleFireTime);
		m_renderSystem.drawText(0, 46, buff, ConsoleColor::BLACK, ConsoleColor::DARK_RED);
	}

	// Вывод счётчика игровых очков
	sprintf_s(buff, "Points: %i", m_gamePoints);
	m_renderSystem.drawText(0, 66, buff, ConsoleColor::CYAN, ConsoleColor::BLACK);

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
				// Корабль игрока
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
							if (m_bonusFastRechargeTime > 0.0)
								m_shipFireCooldownTime = SHIP_FIRE_COOLDOWN / 1.8;
							else
								m_shipFireCooldownTime = SHIP_FIRE_COOLDOWN;

							// Инициализация пули
							GameObject* bullet = createObject(GameObjectType::BULLET,
								                              object->getX(), object->getY(),
                                                              GetRenderCellSymbol(CELL_SYMBOL_BULLET),
				                                              GetRenderCellSymbolColor(CELL_SYMBOL_BULLET),
						                                      GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_BULLET));
							bullet->setYSpeed(-BULLET_SPEED);
							
							if (m_bonusTripleFireTime > 0.0) {
								bullet = createObject(GameObjectType::BULLET,
									                              object->getX() - 1, object->getY() + 1,
									                              GetRenderCellSymbol(CELL_SYMBOL_BULLET),
									                              GetRenderCellSymbolColor(CELL_SYMBOL_BULLET),
									                              GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_BULLET));
								bullet->setYSpeed(-BULLET_SPEED);
								bullet = createObject(GameObjectType::BULLET,
									                              object->getX() + 1, object->getY() + 1,
									                              GetRenderCellSymbol(CELL_SYMBOL_BULLET),
									                              GetRenderCellSymbolColor(CELL_SYMBOL_BULLET),
									                              GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_BULLET));
								bullet->setYSpeed(-BULLET_SPEED);
							}
						}
					}
				}

				// Пуля
				case GameObjectType::BULLET : {
					if (object->getY() < 0)
						hitObject(object);
					else {
						for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
							GameObject* anotherObject = m_objects[i];
							// Определение типа противника, с которым пересеклась пуля,
							// и его уничтожение/повреждение
							if (anotherObject != 0) {
								// Уничтожение обычного пришельца
								if (anotherObject->getType() == GameObjectType::ALIEN) {
									if (anotherObject->intersects(object)) {
										spawnBonus(anotherObject->canDropBonus(), anotherObject);
										hitObject(anotherObject);
										hitObject(object);
										m_gamePoints += 2;
										break;
									}
								}
								// Уничтожение корабля-носителя пришельцев
								if (anotherObject->getType() == GameObjectType::MOTHERSHIP_ALIEN) {
									if (anotherObject->intersects(object)) {
										GameObject* alien = createObject(GameObjectType::HEAVY_ALIEN,
											                             anotherObject->getX() + 1,
											                             anotherObject->getY(),
											                             GetRenderCellSymbol(CELL_SYMBOL_ALIEN),
											                             GetRenderCellSymbolColor(CELL_SYMBOL_ALIEN),
											                             GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_ALIEN));
										alien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
										alien->setYSpeed(ALIEN_SPEED);
										alien->setHealth(HEAVY_ALIEN_HEALTH);
										alien = createObject(GameObjectType::HEAVY_ALIEN,
											                 anotherObject->getX() - 1,
											                 anotherObject->getY(),
											                 GetRenderCellSymbol(CELL_SYMBOL_ALIEN),
											                 GetRenderCellSymbolColor(CELL_SYMBOL_ALIEN),
											                 GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_ALIEN));
										alien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
										alien->setYSpeed(ALIEN_SPEED);
										alien->setHealth(HEAVY_ALIEN_HEALTH);
										alien = createObject(GameObjectType::HEAVY_ALIEN,
											                 anotherObject->getX(),
											                 anotherObject->getY() + 1,
											                 GetRenderCellSymbol(CELL_SYMBOL_ALIEN),
											                 GetRenderCellSymbolColor(CELL_SYMBOL_ALIEN),
											                 GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_ALIEN));
										alien->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
										alien->setYSpeed(ALIEN_SPEED);
										alien->setHealth(HEAVY_ALIEN_HEALTH);
										spawnBonus(anotherObject->canDropBonus(), anotherObject);
										hitObject(anotherObject);
										hitObject(object);
										m_gamePoints += 5;
										break;
									}
								}
								// Уничтожение/повреждение "тяжёлого" корабля пришельцев
								if (anotherObject->getType() == GameObjectType::HEAVY_ALIEN) {
									if (anotherObject->intersects(object)) {
										spawnBonus(anotherObject->canDropBonus(), anotherObject);
										hitObject(anotherObject);
										hitObject(object);
										m_gamePoints += 4;
										break;
									}
								}
								// Уничтожение/повреждение "бронированного" корабля пришельцев
								if (anotherObject->getType() == GameObjectType::ARMORED_ALIEN) {
									if (anotherObject->intersects(object)) {
										spawnBonus(anotherObject->canDropBonus(), anotherObject);
										hitObject(anotherObject);
										hitObject(object);
										m_gamePoints += 12;
										break;
									}
								}
								// Уничтожение "стреляющего" корабля пришельцев
								if (anotherObject->getType() == GameObjectType::SHOOTING_ALIEN) {
									if (anotherObject->intersects(object)) {
										spawnBonus(anotherObject->canDropBonus(), anotherObject);
										hitObject(anotherObject);
										hitObject(object);
										m_gamePoints += 20;
										break;
									}
								}
							}
						}
					}
					break;
				}

				// Пуля пришельца
				case GameObjectType::ALIEN_BULLET : {
					if (object->getY() > SCREEN_ROWS)
						hitObject(object);
					else {
						// Уничтожение корабля игрока при соприкосновении
						for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
							GameObject* anotherObject = m_objects[i];
							if (anotherObject != 0) {
								if (anotherObject->getType() == GameObjectType::SHIP) {
									if (anotherObject->intersects(object)) {
										hitObject(anotherObject);
										hitObject(object);
										m_isLosing = true;
										break;
									}
								}
							}
						}
					}
					break;
				}

				// Логика бонуса заморозки
				case GameObjectType::BONUS_FREEZING : {
					if (object->getY() >= SCREEN_ROWS)
						hitObject(object);
					for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
						GameObject* anotherObject = m_objects[i];
						if (anotherObject != 0) {
							if (anotherObject->getType() == GameObjectType::SHIP)
								if (anotherObject->intersects(object)) {
									if (m_bonusFreezingTime <= 0.0) {
										m_bonusFreezingTime = BONUS_FREEZING_TIME;
											hitObject(object);
									}
								}
						}
					}
					break;
				}
				// Логика бонуса ускоренной перезарядки
				case GameObjectType::BONUS_FAST_RECHARGE : {
					if (object->getY() >= SCREEN_ROWS)
						hitObject(object);
					for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
						GameObject* anotherObject = m_objects[i];
						if (anotherObject != 0) {
							if (anotherObject->getType() == GameObjectType::SHIP)
								if (anotherObject->intersects(object)) {
									if (m_bonusFastRechargeTime <= 0.0) {
										m_bonusFastRechargeTime = BONUS_FAST_RECHARGE_TIME;
										hitObject(object);
									}
								}
						}
					}
					break;
				}
			    // Логика бонуса "тройного огня"
				case GameObjectType::BONUS_TRIPLE_FIRE : {
					if (object->getY() >= SCREEN_ROWS)
						hitObject(object);
					for (int i = 0; i < GAME_OBJECTS_COUNT_MAX; i++) {
						GameObject* anotherObject = m_objects[i];
						if (anotherObject != 0) {
							if (anotherObject->getType() == GameObjectType::SHIP)
								if (anotherObject->intersects(object)) {
									if (m_bonusTripleFireTime <= 0.0) {
										m_bonusTripleFireTime = BONUS_TRIPLE_FIRE_TIME;
										hitObject(object);
									}
								}
						}
					}
					break;
				}

                // Действия для всех кораблей пришельцев
				default : {
					haveAliveAliens = true;

					// Расчёт вероятности выстрела для "стреляющего" пришельца
					if (object->getType() == GameObjectType::SHOOTING_ALIEN) {
						if (m_alienFireCooldownTime <= 0.0) {
							int potentiallyShoot = 1 + rand() % 20;
							if (potentiallyShoot == 1) {
								m_alienFireCooldownTime = ALIEN_FIRE_COOLDOWN;
								GameObject* alienBullet = createObject(GameObjectType::ALIEN_BULLET,
									object->getX(), object->getY(),
									GetRenderCellSymbol(CELL_SYMBOL_ALIEN_BULLET),
									GetRenderCellSymbolColor(CELL_SYMBOL_ALIEN_BULLET),
									GetRenderCellSymbolBackgroundColor(CELL_SYMBOL_ALIEN_BULLET));
								alienBullet->setYSpeed(ALIEN_BULLET_SPEED);
							}
						}
					}

					if (object->getY() >= SCREEN_ROWS - 1)
						m_isLosing = true;
					else
						if (m_bonusFreezingTime > 0.0) {
							object->setXSpeed(0.0);
							object->setYSpeed(0.0);
						}
						else {
							object->setXSpeed(ALIEN_AMPLITUDE * cos(m_alienAmplitudeTime));
							object->setYSpeed(ALIEN_SPEED);
						}
					break;
				}
			}
		}
	}

	// Перезарядка орудия
	if (m_shipFireCooldownTime > 0.0)
		m_shipFireCooldownTime -= dt;
	
	// Отсчёт времени действия бонусов
	if (m_bonusFreezingTime > 0.0)
		m_bonusFreezingTime -= dt;
	
	if (m_bonusFastRechargeTime > 0.0)
		m_bonusFastRechargeTime -= dt;
	
	if (m_bonusTripleFireTime > 0.0)
		m_bonusTripleFireTime -= dt;

	// Время колебательного движения пришельцев
	// с временем перезарядки оружия
	if (m_bonusFreezingTime <= 0.0) {
		m_alienAmplitudeTime += dt;
		m_alienFireCooldownTime -= dt;
	}

	// Выход из игры по ESC
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

void Game::hitObject(GameObject* object)
{
	int health = object->getHealth();
	object->setHealth(--health);
	
	health = object->getHealth();
	if (health <= 0)
		destroyObject(object);
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
		m_renderSystem.drawText(9, 30, buff, ConsoleColor::CYAN, ConsoleColor::BLACK);
		m_renderSystem.flush();

		Sleep(800);
		sprintf_s(buff, "Press R to repeat level %i or ESCAPE for exit...", m_currentLevel);
		m_renderSystem.drawText(12, 17, buff, ConsoleColor::DARK_GRAY, ConsoleColor::BLACK);
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
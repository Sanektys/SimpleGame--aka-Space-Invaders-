#include "gameObject.h"


GameObject::GameObject()
{
	m_type = GameObjectType::NONE;

	m_health = 1;

	m_x = 0.0;
	m_y = 0.0;
	m_xSpeed = 0.0;
	m_ySpeed = 0.0;

	m_symbol = '?';
	m_color = ConsoleColor::WHITE;
	m_backgroundColor = ConsoleColor::BLACK;
}

void GameObject::render(RenderSystem* rs)
{
	int row = int(m_y);
	int column = int(m_x);

	// Окраска "тяжёлого" корабля пришельцев в зависимости от его здоровья
	if (m_type == GameObjectType::HEAVY_ALIEN) {
		switch (m_health) {
		    case 3 :
				m_color = ConsoleColor::GREEN;
				break;
			case 2 :
				m_color = ConsoleColor::YELLOW;
				break;
			case 1 :
				m_color = ConsoleColor::RED;
				break;
		}
	}

	// Окраска "бронированного" корабля пришельцев в зависимости от его здоровья
	if (m_type == GameObjectType::ARMORED_ALIEN) {
		if (m_health <= 9 && m_health > 6)
			m_color = ConsoleColor::GREEN;
		if (m_health <= 6 && m_health > 3)
			m_color = ConsoleColor::YELLOW;
		if (m_health <= 3 && m_health > 0)
			m_color = ConsoleColor::RED;
	}

	rs->drawChar(row, column, m_symbol, m_color, m_backgroundColor);
}

void GameObject::update(float dt)
{
	m_y += m_ySpeed * dt;
	m_x += m_xSpeed * dt;
}

bool GameObject::intersects(GameObject* object)
{
	if (int(m_y) == int(object->m_y) && int(m_x) == int(object->m_x))
		return true;

	return false;
}

bool GameObject::canDropBonus()
{
	if (m_type == GameObjectType::ALIEN) {
		int chance = 1 + rand() % 100;
		if (chance == 1)
			return true;
	}
	if (m_type == GameObjectType::MOTHERSHIP_ALIEN) {
		int chance = 1 + rand() % 75;
		if (chance == 1)
			return true;
	}
	if (m_type == GameObjectType::HEAVY_ALIEN) {
		int chance = 1 + rand() % 42;
		if (chance == 1)
			return true;
	}
	if (m_type == GameObjectType::ARMORED_ALIEN) {
		int chance = 1 + rand() % 16;
		if (chance == 1)
			return true;
	}

	return false;
}
#include "gameObject.h"


GameObject::GameObject()
{
	m_type = GameObjectType::NONE;

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
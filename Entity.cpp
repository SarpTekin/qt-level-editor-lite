#include "Entity.h"

Entity::Entity(int id, const QString &name, const QPoint &position)
    : m_id(id)
    , m_name(name)
    , m_position(position)
    , m_rect(position.x(), position.y(), DEFAULT_WIDTH, DEFAULT_HEIGHT)
    , m_color(QColor(100, 150, 255))  // Default blue color
{
}

void Entity::setPosition(const QPoint &pos)
{
    m_position = pos;
    // Update the rectangle's position (keep size the same)
    m_rect.moveTopLeft(pos);
}

void Entity::setName(const QString &name)
{
    m_name = name;
}

void Entity::setColor(const QColor &color)
{
    m_color = color;
}
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

void Entity::setSize(int width, int height)
{
    m_rect.setSize(QSize(width, height));
}

QJsonObject Entity::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["name"] = m_name;
    json["x"] = m_position.x();
    json["y"] = m_position.y();
    json["width"] = m_rect.width();
    json["height"] = m_rect.height();
    
    // Save color as RGB values
    json["color_r"] = m_color.red();
    json["color_g"] = m_color.green();
    json["color_b"] = m_color.blue();
    
    return json;
}

Entity Entity::fromJson(const QJsonObject &json)
{
    int id = json["id"].toInt();
    QString name = json["name"].toString();
    int x = json["x"].toInt();
    int y = json["y"].toInt();
    
    QPoint position(x, y);
    Entity entity(id, name, position);
    
    // Restore size if present
    if (json.contains("width") && json.contains("height")) {
        int width = json["width"].toInt();
        int height = json["height"].toInt();
        entity.setSize(width, height);
    }
    
    // Restore color if present
    if (json.contains("color_r") && json.contains("color_g") && json.contains("color_b")) {
        int r = json["color_r"].toInt();
        int g = json["color_g"].toInt();
        int b = json["color_b"].toInt();
        entity.setColor(QColor(r, g, b));
    }
    
    return entity;
}
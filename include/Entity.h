#ifndef ENTITY_H
#define ENTITY_H

#include <QPoint>
#include <QRect>
#include <QString>
#include <QColor>
#include <QJsonObject>

class Entity
{
public:
    // Constructor
    Entity(int id, const QString &name, const QPoint &position);
    
    // Getters
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QPoint position() const { return m_position; }
    QRect rect() const { return m_rect; }
    QColor color() const { return m_color; }
    
    // Setters
    void setPosition(const QPoint &pos);
    void setName(const QString &name);
    void setColor(const QColor &color);
    void setSize(int width, int height);
    
    // JSON serialization
    QJsonObject toJson() const;
    static Entity fromJson(const QJsonObject &json);

private:
    int m_id;                    // Unique identifier
    QString m_name;              // Display name
    QPoint m_position;           // Position on canvas
    QRect m_rect;                // Bounding rectangle (position + size)
    QColor m_color;              // Visual color
    
    static const int DEFAULT_WIDTH = 60;
    static const int DEFAULT_HEIGHT = 60;
};

#endif // ENTITY_H
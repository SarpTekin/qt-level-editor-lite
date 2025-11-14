#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <vector>
#include "Entity.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    
    // Public accessors for the object list
    int entityCount() const { return static_cast<int>(m_entities.size()); }
    Entity* getEntity(int index);
    const Entity* getEntity(int index) const;
    int selectedEntityIndex() const { return m_selectedEntityIndex; }
    
    // Set selection from external source (like the list widget)
    void setSelectedEntityIndex(int index);

signals:
    // Signals emitted when entities change (for updating the list)
    void entityAdded(int index);
    void entityRemoved(int index);
    void entitySelectionChanged(int index);

private:
    
    // Background color for the canvas
    QColor m_backgroundColor;
    
    // Container to store all entities on the canvas
    std::vector<Entity> m_entities;
    
    // Counter for generating unique entity IDs
    int m_nextEntityId;
    
    // Selection state
    int m_selectedEntityIndex;  // -1 if nothing selected, otherwise index in m_entities
    
    // Dragging state
    bool m_isDragging;
    QPoint m_dragStartPos;      // Mouse position when drag started
    QPoint m_entityStartPos;    // Entity position when drag started

    // Helper function to find entity at a given point
    // Returns index in m_entities, or -1 if none found
    int findEntityAt(const QPoint &pos) const;
    
    // Delete the currently selected entity
    void deleteSelectedEntity();    

protected:
    // Override paintEvent to draw on the canvas
    void paintEvent(QPaintEvent *event) override;
    
    // Mouse events for interaction
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    
    // Keyboard events
    void keyPressEvent(QKeyEvent *event) override;    
};

#endif // CANVAS_H
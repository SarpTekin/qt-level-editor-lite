#include "Canvas.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_backgroundColor(QColor(240, 240, 240))  // Light gray background
    , m_nextEntityId(1)  // Start entity IDs at 1
    , m_selectedEntityIndex(-1)  // No entity selected initially
    , m_isDragging(false)
{
    // Enable mouse tracking for drag operations
    setMouseTracking(true);

    // Enable keyboard focus so we can receive key events
    setFocusPolicy(Qt::StrongFocus);    
    
    // Set a minimum size
    setMinimumSize(400, 300);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    
    // Draw the background
    painter.fillRect(rect(), m_backgroundColor);
    
    // Draw a subtle grid pattern (optional, for visual reference)
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    const int gridSize = 20;
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
    
    // Draw all entities as rectangles
    for (size_t i = 0; i < m_entities.size(); ++i) {
        const Entity &entity = m_entities[i];
        bool isSelected = (static_cast<int>(i) == m_selectedEntityIndex);
        
        // Set the brush (fill color) from entity's color
        painter.setBrush(QBrush(entity.color()));
        
        // Set the pen (border) - thicker and darker if selected
        QColor borderColor = entity.color().darker(120);
        int borderWidth = isSelected ? 4 : 2;
        painter.setPen(QPen(borderColor, borderWidth));
        
        // Draw the rectangle
        painter.drawRect(entity.rect());
        
        // Draw selection highlight (yellow border) for selected entity
        if (isSelected) {
            painter.setPen(QPen(QColor(255, 200, 0), 3));  // Yellow highlight
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(entity.rect().adjusted(-2, -2, 2, 2));  // Slightly larger
        }
        
        // Draw the entity name as text
        painter.setPen(QPen(Qt::black, 1));
        painter.drawText(entity.rect(), Qt::AlignCenter, entity.name());
    }
}

int Canvas::findEntityAt(const QPoint &pos) const
{
    // Check entities in reverse order (top to bottom visually)
    // so entities drawn on top are selected first
    for (int i = static_cast<int>(m_entities.size()) - 1; i >= 0; --i) {
        if (m_entities[i].rect().contains(pos)) {
            return i;
        }
    }
    return -1;  // No entity at this position
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && m_selectedEntityIndex >= 0 && 
        m_selectedEntityIndex < static_cast<int>(m_entities.size())) {
        
        // Calculate how far the mouse has moved
        QPoint delta = event->pos() - m_dragStartPos;
        
        // Update entity position
        QPoint newPos = m_entityStartPos + delta;
        m_entities[m_selectedEntityIndex].setPosition(newPos);
        
        // Request a repaint to show the entity moving
        update();
    }
    
    QWidget::mouseMoveEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    
    QWidget::mouseReleaseEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();
        
        // Check if we clicked on an existing entity
        int entityIndex = findEntityAt(clickPos);
        
        if (entityIndex >= 0) {
            // Clicked on an entity - select it and start dragging
            m_selectedEntityIndex = entityIndex;
            m_isDragging = true;
            m_dragStartPos = clickPos;
            m_entityStartPos = m_entities[entityIndex].position();
            emit entitySelectionChanged(m_selectedEntityIndex);
        } else {
            // Clicked on empty space - create new entity
            QString entityName = QString("Entity_%1").arg(m_nextEntityId);
            Entity newEntity(m_nextEntityId, entityName, clickPos);
            
            // Add it to our container
            int newIndex = static_cast<int>(m_entities.size());
            m_entities.push_back(newEntity);
            
            // Select the newly created entity
            m_selectedEntityIndex = newIndex;
            m_isDragging = false;
            
            // Emit signals
            emit entityAdded(newIndex);
            emit entitySelectionChanged(m_selectedEntityIndex);
            
            // Increment ID for next entity
            m_nextEntityId++;
        }
        
        // Request a repaint to update selection highlight
        update();
    }
    
    QWidget::mousePressEvent(event);
}

void Canvas::deleteSelectedEntity()
{
    // Check if there's a selected entity
    if (m_selectedEntityIndex < 0 || m_selectedEntityIndex >= static_cast<int>(m_entities.size())) {
        return;  // Nothing selected or invalid index
    }
    
    int removedIndex = m_selectedEntityIndex;
    
    // Remove the selected entity from the vector
    m_entities.erase(m_entities.begin() + m_selectedEntityIndex);
    
    // Clear selection
    m_selectedEntityIndex = -1;
    
    // Emit signal
    emit entityRemoved(removedIndex);
    emit entitySelectionChanged(-1);
    
    // Request a repaint to remove the deleted entity
    update();
}

Entity* Canvas::getEntity(int index)
{
    if (index < 0 || index >= static_cast<int>(m_entities.size())) {
        return nullptr;
    }
    return &m_entities[index];
}

const Entity* Canvas::getEntity(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_entities.size())) {
        return nullptr;
    }
    return &m_entities[index];
}

void Canvas::setSelectedEntityIndex(int index)
{
    // Validate index
    if (index < -1 || index >= static_cast<int>(m_entities.size())) {
        index = -1;
    }
    
    if (m_selectedEntityIndex != index) {
        m_selectedEntityIndex = index;
        update();  // Repaint to show new selection
        emit entitySelectionChanged(m_selectedEntityIndex);
    }
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteSelectedEntity();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}
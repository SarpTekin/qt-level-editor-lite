#include "Canvas.h"
#include "AddEntityCommand.h"
#include "DeleteEntityCommand.h"  
#include "MoveEntityCommand.h" 
#include <QKeyEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QUndoStack>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_backgroundColor(QColor(240, 240, 240))
    , m_gridVisible(true)
    , m_gridSize(20)
    , m_snapToGrid(false)
    , m_undoStack(nullptr)
    , m_currentMoveCommand(nullptr)  // Initialize to nullptr
    , m_nextEntityId(1)
    , m_selectedEntityIndex(-1)
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
    
    // Draw grid if visible
    if (m_gridVisible) {
        painter.setPen(QPen(QColor(220, 220, 220), 1));
        for (int x = 0; x < width(); x += m_gridSize) {
            painter.drawLine(x, 0, x, height());
        }
        for (int y = 0; y < height(); y += m_gridSize) {
            painter.drawLine(0, y, width(), y);
        }
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

QPoint Canvas::snapToGrid(const QPoint &point) const
{
    if (!m_snapToGrid || m_gridSize < 1) {
        return point;  // No snapping if disabled or invalid grid size
    }
    
    // Snap to nearest grid point
    int snappedX = (point.x() / m_gridSize) * m_gridSize;
    int snappedY = (point.y() / m_gridSize) * m_gridSize;
    
    return QPoint(snappedX, snappedY);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && m_selectedEntityIndex >= 0 && 
        m_selectedEntityIndex < static_cast<int>(m_entities.size())) {
        
        // Calculate how far the mouse has moved
        QPoint delta = event->pos() - m_dragStartPos;
        
        // Update entity position
        QPoint newPos = m_entityStartPos + delta;
        
        // Snap to grid if enabled
        if (m_snapToGrid) {
            newPos = snapToGrid(newPos);
        }
        
        // Update entity position
        m_entities[m_selectedEntityIndex].setPosition(newPos);
        
        // Update move command if it exists
        if (m_currentMoveCommand) {
            m_currentMoveCommand->setNewPosition(newPos);
        }
        
        // Request a repaint to show the entity moving
        update();
    }
    
    QWidget::mouseMoveEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_isDragging && m_currentMoveCommand) {
            // Check if position actually changed
            QPoint finalPos = m_entities[m_selectedEntityIndex].position();
            if (finalPos != m_currentMoveCommand->m_oldPos) {
                // Position changed - push command to undo stack
                if (m_undoStack) {
                    m_undoStack->push(m_currentMoveCommand);
                }
            } else {
                // No change - discard command
                delete m_currentMoveCommand;
            }
            m_currentMoveCommand = nullptr;
        }
        m_isDragging = false;
    }
    
    QWidget::mouseReleaseEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();

        // Snap click position to grid if enabled
        if (m_snapToGrid) {
            clickPos = snapToGrid(clickPos);
        }
        
        // Check if we clicked on an existing entity
        int entityIndex = findEntityAt(clickPos);
        
        if (entityIndex >= 0) {
            // Clicked on an entity - select it and start dragging
            m_selectedEntityIndex = entityIndex;
            m_isDragging = true;
            m_dragStartPos = clickPos;
            m_entityStartPos = m_entities[entityIndex].position();
            emit entitySelectionChanged(m_selectedEntityIndex);
            
            // Create move command if undo stack available
            if (m_undoStack) {
                QPoint currentPos = m_entities[entityIndex].position();
                m_currentMoveCommand = new MoveEntityCommand(this, entityIndex, currentPos, currentPos);
            }
        } else {
            // Clicked on empty space - create new entity using command if undo stack available
            if (m_undoStack) {
                // Create command and push to undo stack (redo() will be called automatically)
                AddEntityCommand *command = new AddEntityCommand(this, clickPos);
                m_undoStack->push(command);
            } else {
                // Fallback: create directly if no undo stack (backward compatibility)
                QString entityName = QString("Entity_%1").arg(m_nextEntityId);
                Entity newEntity(m_nextEntityId, entityName, clickPos);
                
                int newIndex = static_cast<int>(m_entities.size());
                m_entities.push_back(newEntity);
                
                m_selectedEntityIndex = newIndex;
                m_isDragging = false;
                
                emit entityAdded(newIndex);
                emit entitySelectionChanged(m_selectedEntityIndex);
                
                m_nextEntityId++;
                update();
            }
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
        if (m_selectedEntityIndex >= 0 && m_selectedEntityIndex < static_cast<int>(m_entities.size())) {
            if (m_undoStack) {
                // Use command for undoable delete
                DeleteEntityCommand *command = new DeleteEntityCommand(this, m_selectedEntityIndex);
                m_undoStack->push(command);
            } else {
                // Fallback: direct delete
                deleteSelectedEntity();
            }
        }
        event->accept();
    } else if (event->key() == Qt::Key_D && event->modifiers() & Qt::ControlModifier) {
        // Ctrl+D: Duplicate selected entity
        duplicateSelectedEntity();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

bool Canvas::saveToFile(const QString &filePath) const
{
    // Create root JSON object
    QJsonObject root;
    root["version"] = "1.0";
    root["next_entity_id"] = m_nextEntityId;
    
    // Create JSON array of entities
    QJsonArray entitiesArray;
    for (const Entity &entity : m_entities) {
        entitiesArray.append(entity.toJson());
    }
    root["entities"] = entitiesArray;
    
    // Write to file
    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QTextStream stream(&file);
    stream << doc.toJson(QJsonDocument::Indented);
    file.close();
    
    return true;
}

bool Canvas::loadFromFile(const QString &filePath)
{
    // Read file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    // Parse JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // Clear existing entities
    m_entities.clear();
    m_selectedEntityIndex = -1;
    
    // Restore next entity ID if present
    if (root.contains("next_entity_id")) {
        m_nextEntityId = root["next_entity_id"].toInt();
    } else {
        m_nextEntityId = 1;
    }
    
    // Load entities
    if (root.contains("entities") && root["entities"].isArray()) {
        QJsonArray entitiesArray = root["entities"].toArray();
        for (const QJsonValue &value : entitiesArray) {
            if (value.isObject()) {
                Entity entity = Entity::fromJson(value.toObject());
                m_entities.push_back(entity);
            }
        }
    }
    
    // Request repaint
    update();
    
    // Emit signals for each loaded entity (for list widget update)
    for (int i = 0; i < static_cast<int>(m_entities.size()); ++i) {
        emit entityAdded(i);
    }
    
    return true;
}

void Canvas::setGridVisible(bool visible)
{
    if (m_gridVisible != visible) {
        m_gridVisible = visible;
        update();  // Repaint to show/hide grid
    }
}

void Canvas::setGridSize(int size)
{
    if (size < 1) size = 1;  // Minimum grid size
    if (m_gridSize != size) {
        m_gridSize = size;
        if (m_gridVisible) {
            update();  // Repaint if grid is visible
        }
    }
}

void Canvas::setSnapToGrid(bool snap)
{
    if (m_snapToGrid != snap) {
        m_snapToGrid = snap;
        // Optionally, snap all entities when enabling snap (for now, just set the flag)
        update();
    }
}

int Canvas::addEntityAt(const QPoint &position)
{
    QString entityName = QString("Entity_%1").arg(m_nextEntityId);
    Entity newEntity(m_nextEntityId, entityName, position);
    
    int newIndex = static_cast<int>(m_entities.size());
    m_entities.push_back(newEntity);
    
    m_selectedEntityIndex = newIndex;
    m_nextEntityId++;
    
    // Emit signal for UI updates
    emit entityAdded(newIndex);
    emit entitySelectionChanged(m_selectedEntityIndex);
    
    update();
    
    return newIndex;
}

void Canvas::removeEntityAt(int index)
{
    if (index < 0 || index >= static_cast<int>(m_entities.size())) {
        return;
    }
    
    m_entities.erase(m_entities.begin() + index);
    
    // Clear selection if removed entity was selected
    if (m_selectedEntityIndex == index) {
        m_selectedEntityIndex = -1;
    } else if (m_selectedEntityIndex > index) {
        // Adjust selection index if entity before it was removed
        m_selectedEntityIndex--;
    }
    
    // Emit signal
    emit entityRemoved(index);
    emit entitySelectionChanged(m_selectedEntityIndex);
    
    update();
}

int Canvas::insertEntity(const Entity &entity, int index)
{
    if (index < 0) {
        index = static_cast<int>(m_entities.size());
    } else if (index > static_cast<int>(m_entities.size())) {
        index = static_cast<int>(m_entities.size());
    }
    
    m_entities.insert(m_entities.begin() + index, entity);
    
    // Adjust selection if needed
    if (m_selectedEntityIndex >= index) {
        m_selectedEntityIndex++;
    }
    
    // Emit signal
    emit entityAdded(index);
    emit entitySelectionChanged(m_selectedEntityIndex);
    
    update();
    
    return index;
}

void Canvas::setUndoStack(QUndoStack *undoStack)
{
    m_undoStack = undoStack;
}

void Canvas::duplicateSelectedEntity()
{
    if (m_selectedEntityIndex < 0 || m_selectedEntityIndex >= static_cast<int>(m_entities.size())) {
        return;  // No entity selected
    }
    
    const Entity *sourceEntity = getEntity(m_selectedEntityIndex);
    if (!sourceEntity) {
        return;
    }
    
    // Calculate offset position (offset by entity width + 10 pixels)
    QPoint offsetPos = sourceEntity->position() + QPoint(sourceEntity->rect().width() + 10, 0);
    
    // Snap to grid if enabled
    if (m_snapToGrid) {
        offsetPos = snapToGrid(offsetPos);
    }
    
    // Use AddEntityCommand to create new entity at offset position
    if (m_undoStack) {
        AddEntityCommand *command = new AddEntityCommand(this, offsetPos);
        m_undoStack->push(command);
        
        // After command executes, update the new entity's properties
        // Get the newly created entity (it will be the last one)
        int newIndex = entityCount() - 1;
        Entity *newEntity = getEntity(newIndex);
        if (newEntity) {
            QString entityName = QString("%1 (Copy)").arg(sourceEntity->name());
            newEntity->setName(entityName);
            newEntity->setColor(sourceEntity->color());
            newEntity->setSize(sourceEntity->rect().width(), sourceEntity->rect().height());
            
            // Select the new entity
            setSelectedEntityIndex(newIndex);
            update();
        }
    } else {
        // Fallback: create directly
        QString entityName = QString("%1 (Copy)").arg(sourceEntity->name());
        Entity newEntity(m_nextEntityId, entityName, offsetPos);
        
        // Copy properties
        newEntity.setColor(sourceEntity->color());
        newEntity.setSize(sourceEntity->rect().width(), sourceEntity->rect().height());
        
        int newIndex = static_cast<int>(m_entities.size());
        m_entities.push_back(newEntity);
        
        m_selectedEntityIndex = newIndex;
        
        emit entityAdded(newIndex);
        emit entitySelectionChanged(m_selectedEntityIndex);
        
        m_nextEntityId++;
        update();
    }
}
#include "AddEntityCommand.h"
#include "Canvas.h"

AddEntityCommand::AddEntityCommand(Canvas *canvas, const QPoint &position, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_canvas(canvas)
    , m_entity(-1, "Placeholder", QPoint(0, 0))  // Match declaration order
    , m_entityIndex(-1)
    , m_firstRedo(true)
    , m_position(position)
{
    setText("Add Entity");
}

void AddEntityCommand::redo()
{
    if (!m_canvas) return;
    
    if (m_firstRedo) {
        // First time: create the entity at the stored position
        m_entityIndex = m_canvas->addEntityAt(m_position);
        // Store the created entity for undo
        const Entity *entity = m_canvas->getEntity(m_entityIndex);
        if (entity) {
            m_entity = *entity;
        }
        m_firstRedo = false;
    } else {
        // Subsequent redos: re-add the entity
        m_entityIndex = m_canvas->insertEntity(m_entity, m_entityIndex);
    }
}

void AddEntityCommand::undo()
{
    if (!m_canvas || m_entityIndex < 0) return;
    
    // Store the entity before removing it
    const Entity *entity = m_canvas->getEntity(m_entityIndex);
    if (entity) {
        m_entity = *entity;
    }
    
    // Remove the entity
    m_canvas->removeEntityAt(m_entityIndex);
}
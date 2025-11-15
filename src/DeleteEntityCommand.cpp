#include "DeleteEntityCommand.h"
#include "Canvas.h"

DeleteEntityCommand::DeleteEntityCommand(Canvas *canvas, int entityIndex, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_canvas(canvas)
    , m_entity(-1, "Placeholder", QPoint(0, 0))
    , m_entityIndex(entityIndex)
    , m_firstRedo(true)
{
    setText("Delete Entity");
}

void DeleteEntityCommand::redo()
{
    if (!m_canvas || m_entityIndex < 0) return;
    
    if (m_firstRedo) {
        // First time: store entity and remove it
        const Entity *entity = m_canvas->getEntity(m_entityIndex);
        if (entity) {
            m_entity = *entity;
        }
        m_canvas->removeEntityAt(m_entityIndex);
        m_firstRedo = false;
    } else {
        // Subsequent redos: remove entity again
        m_canvas->removeEntityAt(m_entityIndex);
    }
}

void DeleteEntityCommand::undo()
{
    if (!m_canvas || m_entityIndex < 0) return;
    
    // Re-insert the entity at its original position
    m_entityIndex = m_canvas->insertEntity(m_entity, m_entityIndex);
}
#include "MoveEntityCommand.h"
#include "Canvas.h"

MoveEntityCommand::MoveEntityCommand(Canvas *canvas, int entityIndex, const QPoint &oldPos, const QPoint &newPos, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_canvas(canvas)
    , m_entityIndex(entityIndex)
    , m_oldPos(oldPos)
    , m_newPos(newPos)
{
    setText("Move Entity");
}

void MoveEntityCommand::undo()
{
    if (!m_canvas || m_entityIndex < 0) return;
    
    Entity *entity = m_canvas->getEntity(m_entityIndex);
    if (entity) {
        entity->setPosition(m_oldPos);
        m_canvas->update();
    }
}

void MoveEntityCommand::redo()
{
    if (!m_canvas || m_entityIndex < 0) return;
    
    Entity *entity = m_canvas->getEntity(m_entityIndex);
    if (entity) {
        entity->setPosition(m_newPos);
        m_canvas->update();
    }
}

void MoveEntityCommand::setNewPosition(const QPoint &newPos)
{
    m_newPos = newPos;
}
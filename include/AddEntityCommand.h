#ifndef ADDENTITYCOMMAND_H
#define ADDENTITYCOMMAND_H

#include <QUndoCommand>
#include <QPoint>
#include "Entity.h"

class Canvas;

class AddEntityCommand : public QUndoCommand
{
public:
    AddEntityCommand(Canvas *canvas, const QPoint &position, QUndoCommand *parent = nullptr);
    
    void undo() override;
    void redo() override;

private:
    Canvas *m_canvas;
    Entity m_entity;
    int m_entityIndex;
    bool m_firstRedo;
    QPoint m_position;
};

#endif // ADDENTITYCOMMAND_H
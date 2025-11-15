#ifndef DELETEENTITYCOMMAND_H
#define DELETEENTITYCOMMAND_H

#include <QUndoCommand>
#include "Entity.h"

class Canvas;

class DeleteEntityCommand : public QUndoCommand
{
public:
    DeleteEntityCommand(Canvas *canvas, int entityIndex, QUndoCommand *parent = nullptr);
    
    void undo() override;
    void redo() override;

private:
    Canvas *m_canvas;
    Entity m_entity;
    int m_entityIndex;
    bool m_firstRedo;
};

#endif // DELETEENTITYCOMMAND_H
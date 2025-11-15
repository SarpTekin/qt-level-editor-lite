#ifndef MOVEENTITYCOMMAND_H
#define MOVEENTITYCOMMAND_H

#include <QUndoCommand>
#include <QPoint>

class Canvas;

class MoveEntityCommand : public QUndoCommand
{
public:
    MoveEntityCommand(Canvas *canvas, int entityIndex, const QPoint &oldPos, const QPoint &newPos, QUndoCommand *parent = nullptr);
    
    void undo() override;
    void redo() override;
    
    // Update the new position (called while dragging)
    void setNewPosition(const QPoint &newPos);

private:
    Canvas *m_canvas;
    int m_entityIndex;
    QPoint m_oldPos;
    QPoint m_newPos;

    friend class Canvas;  // Allow Canvas to access m_oldPos
};

#endif // MOVEENTITYCOMMAND_H
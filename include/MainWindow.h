#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoStack> 

class QDockWidget;
class QListWidget;
class Canvas;
class InspectorPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot to handle entity selection from the list
    void onEntityListItemSelected(int row);
    
    // Slots to update the list when entities change
    void onEntityAdded(int index);
    void onEntityRemoved(int index);
    void onEntitySelectionChanged(int index);

    // File menu actions
    void onSaveScene();
    void onLoadScene();   
       
    // View menu actions
    void toggleGridVisibility();
    void toggleSnapToGrid();

private:
    void setupObjectListPanel();
    void updateObjectList();
    
    Canvas *m_canvas;
    QDockWidget *m_objectListDock;
    QListWidget *m_objectListWidget;
    InspectorPanel *m_inspectorPanel;  
    QDockWidget *m_inspectorDock;     
    QUndoStack *m_undoStack; 
};

#endif // MAINWINDOW_H
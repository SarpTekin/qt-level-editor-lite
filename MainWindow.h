#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDockWidget;
class QListWidget;
class Canvas;

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

private:
    void setupObjectListPanel();
    void updateObjectList();
    
    Canvas *m_canvas;
    QDockWidget *m_objectListDock;
    QListWidget *m_objectListWidget;
};

#endif // MAINWINDOW_H
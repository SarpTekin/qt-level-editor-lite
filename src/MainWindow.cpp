#include "MainWindow.h"
#include "Canvas.h"
#include "InspectorPanel.h" 
#include "AddEntityCommand.h"
#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QUndoStack> 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_canvas(nullptr)
    , m_objectListDock(nullptr)
    , m_objectListWidget(nullptr)
    , m_inspectorPanel(nullptr)
    , m_inspectorDock(nullptr)
    , m_undoStack(new QUndoStack(this))
{
    // Set window title and size
    setWindowTitle("Qt Level Editor Lite");
    resize(1000, 700);
    
    // Create and set the canvas as the central widget
    m_canvas = new Canvas(this);
    m_canvas->setUndoStack(m_undoStack); // Pass the undo stack to the canvas
    setCentralWidget(m_canvas);
    
    // Set up the object list panel
    setupObjectListPanel();

    // Set up the inspector panel
    m_inspectorPanel = new InspectorPanel(this);
    m_inspectorPanel->setCanvas(m_canvas); 
    m_inspectorDock = new QDockWidget("Inspector", this);
    m_inspectorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_inspectorDock->setWidget(m_inspectorPanel);
    addDockWidget(Qt::LeftDockWidgetArea, m_inspectorDock);
    
    // Set up the menu bar
    QMenuBar *menuBar = this->menuBar();
    QMenu *fileMenu = menuBar->addMenu("&File");

    // View menu
    QMenu *viewMenu = menuBar->addMenu("&View");
    
    
    QAction *toggleGridAction = viewMenu->addAction("Show &Grid");
    toggleGridAction->setCheckable(true);
    toggleGridAction->setChecked(true);  // Grid visible by default
    toggleGridAction->setShortcut(QKeySequence("Ctrl+G"));
    connect(toggleGridAction, &QAction::triggered, this, &MainWindow::toggleGridVisibility);

     // Edit menu
     QMenu *editMenu = menuBar->addMenu("&Edit");
    
     QAction *undoAction = m_undoStack->createUndoAction(this, "&Undo");
     undoAction->setShortcut(QKeySequence::Undo);
     editMenu->addAction(undoAction);
     
     QAction *redoAction = m_undoStack->createRedoAction(this, "&Redo");
     redoAction->setShortcut(QKeySequence::Redo);
     editMenu->addAction(redoAction);

     // Duplicate action
    QAction *duplicateAction = editMenu->addAction("&Duplicate");
    duplicateAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(duplicateAction, &QAction::triggered, m_canvas, &Canvas::duplicateSelectedEntity);

    // Snap-to-grid toggle
    QAction *toggleSnapAction = viewMenu->addAction("Snap to &Grid");
    toggleSnapAction->setCheckable(true);
    toggleSnapAction->setChecked(false);  // Snap disabled by default
    toggleSnapAction->setShortcut(QKeySequence("Ctrl+Shift+G"));
    connect(toggleSnapAction, &QAction::triggered, this, &MainWindow::toggleSnapToGrid);
    
    // Save action
    QAction *saveAction = fileMenu->addAction("&Save Scene...");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveScene);
    
    // Load action
    QAction *loadAction = fileMenu->addAction("&Load Scene...");
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadScene);
    
    fileMenu->addSeparator();
    
    // Exit action
    QAction *exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Connect canvas signals to our slots
    connect(m_canvas, &Canvas::entityAdded, this, &MainWindow::onEntityAdded);
    connect(m_canvas, &Canvas::entityRemoved, this, &MainWindow::onEntityRemoved);
    connect(m_canvas, &Canvas::entitySelectionChanged, this, &MainWindow::onEntitySelectionChanged);

    // Connect inspector to selection changes
    connect(m_canvas, &Canvas::entitySelectionChanged, m_inspectorPanel, &InspectorPanel::onSelectionChanged);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupObjectListPanel()
{
    // Create the dock widget
    m_objectListDock = new QDockWidget("Objects", this);
    m_objectListDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    // Create the list widget
    m_objectListWidget = new QListWidget(m_objectListDock);
    m_objectListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Connect list widget selection to our slot
    connect(m_objectListWidget, &QListWidget::currentRowChanged, 
            this, &MainWindow::onEntityListItemSelected);
    
    // Set the list widget as the dock's widget
    m_objectListDock->setWidget(m_objectListWidget);
    
    // Add the dock to the main window
    addDockWidget(Qt::RightDockWidgetArea, m_objectListDock);
}

void MainWindow::updateObjectList()
{
    // Clear the list
    m_objectListWidget->clear();
    
    // Block signals while updating to prevent recursive selection changes
    m_objectListWidget->blockSignals(true);
    
    // Add all entities to the list
    for (int i = 0; i < m_canvas->entityCount(); ++i) {
        const Entity *entity = m_canvas->getEntity(i);
        if (entity) {
            QString itemText = QString("%1: %2").arg(entity->id()).arg(entity->name());
            m_objectListWidget->addItem(itemText);
        }
    }
    
    // Restore signals
    m_objectListWidget->blockSignals(false);
    
    // Update selection in the list to match canvas selection
    if (m_canvas->selectedEntityIndex() >= 0) {
        m_objectListWidget->setCurrentRow(m_canvas->selectedEntityIndex());
    } else {
        m_objectListWidget->clearSelection();
    }
}

void MainWindow::onEntityListItemSelected(int row)
{
    if (row >= 0 && row < m_canvas->entityCount()) {
        m_canvas->setSelectedEntityIndex(row);
    } else {
        m_canvas->setSelectedEntityIndex(-1);
    }
}

void MainWindow::onEntityAdded(int index)
{
    Q_UNUSED(index)
    updateObjectList();
}

void MainWindow::onEntityRemoved(int index)
{
    Q_UNUSED(index)
    updateObjectList();
}

void MainWindow::onEntitySelectionChanged(int index)
{
    // Block signals to prevent recursive updates
    m_objectListWidget->blockSignals(true);
    
    if (index >= 0 && index < m_objectListWidget->count()) {
        m_objectListWidget->setCurrentRow(index);
    } else {
        m_objectListWidget->clearSelection();
    }

    // Update inspector with entity data
    if (index >= 0 && index < m_canvas->entityCount()) {
        const Entity *entity = m_canvas->getEntity(index);
        if (entity) {
            m_inspectorPanel->updateFromEntity(
                entity->id(),
                entity->name(),
                entity->color(),
                entity->rect().width(),
                entity->rect().height()
            );
        }
    } else {
        m_inspectorPanel->onSelectionChanged(-1);
    }
    
    m_objectListWidget->blockSignals(false);
}

void MainWindow::onSaveScene()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save Scene",
        "",
        "JSON Files (*.json);;All Files (*)"
    );
    
    if (filePath.isEmpty()) {
        return;  // User cancelled
    }
    
    // Ensure .json extension
    if (!filePath.endsWith(".json", Qt::CaseInsensitive)) {
        filePath += ".json";
    }
    
    if (m_canvas->saveToFile(filePath)) {
        QMessageBox::information(this, "Success", "Scene saved successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save scene to file.");
    }
}

void MainWindow::onLoadScene()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Load Scene",
        "",
        "JSON Files (*.json);;All Files (*)"
    );
    
    if (filePath.isEmpty()) {
        return;  // User cancelled
    }
    
    if (m_canvas->loadFromFile(filePath)) {
        updateObjectList();
        QMessageBox::information(this, "Success", "Scene loaded successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to load scene from file.");
    }
}

void MainWindow::toggleGridVisibility()
{
    bool isVisible = m_canvas->isGridVisible();
    m_canvas->setGridVisible(!isVisible);
    
    // Update menu action checked state
    QList<QAction*> viewActions = menuBar()->actions();
    for (QAction *action : viewActions) {
        if (action->menu() && action->menu()->title() == "View") {
            QList<QAction*> subActions = action->menu()->actions();
            for (QAction *subAction : subActions) {
                if (subAction->text().contains("Grid")) {
                    subAction->setChecked(!isVisible);
                    break;
                }
            }
            break;
        }
    }
}

void MainWindow::toggleSnapToGrid()
{
    bool isSnapping = m_canvas->isSnapToGrid();
    m_canvas->setSnapToGrid(!isSnapping);
    
    // Update menu action checked state
    QList<QAction*> viewActions = menuBar()->actions();
    for (QAction *action : viewActions) {
        if (action->menu() && action->menu()->title() == "View") {
            QList<QAction*> subActions = action->menu()->actions();
            for (QAction *subAction : subActions) {
                if (subAction->text().contains("Snap")) {
                    subAction->setChecked(!isSnapping);
                    break;
                }
            }
            break;
        }
    }
}
#include "MainWindow.h"
#include "Canvas.h"
#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_canvas(nullptr)
    , m_objectListDock(nullptr)
    , m_objectListWidget(nullptr)
{
    // Set window title and size
    setWindowTitle("Qt Level Editor Lite");
    resize(1000, 700);
    
    // Create and set the canvas as the central widget
    m_canvas = new Canvas(this);
    setCentralWidget(m_canvas);
    
    // Set up the object list panel
    setupObjectListPanel();
    
    // Connect canvas signals to our slots
    connect(m_canvas, &Canvas::entityAdded, this, &MainWindow::onEntityAdded);
    connect(m_canvas, &Canvas::entityRemoved, this, &MainWindow::onEntityRemoved);
    connect(m_canvas, &Canvas::entitySelectionChanged, this, &MainWindow::onEntitySelectionChanged);
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
    
    m_objectListWidget->blockSignals(false);
}
#include "MainWindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window title and size
    setWindowTitle("Qt Level Editor Lite");
    resize(800, 600);
    
    // Add a simple label to verify the window works
    auto *label = new QLabel("Level Editor - Ready!", this);
    label->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);
}

MainWindow::~MainWindow() = default;
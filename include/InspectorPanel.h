#ifndef INSPECTORPANEL_H
#define INSPECTORPANEL_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

class Canvas;  // Forward declaration


class InspectorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit InspectorPanel(QWidget *parent = nullptr);

    // Set the canvas pointer (called by MainWindow)
    void setCanvas(Canvas *canvas);    

public slots:
    // Called when selection changes in the canvas
    // entityPtr can be nullptr if nothing selected
    void onSelectionChanged(int entityIndex);
    
    // Called with actual entity pointer from canvas
    void updateFromEntity(int id, const QString &name, const QColor &color, int width, int height);

private slots:
    // Property change handlers (for next step - add empty implementations for now)
    void onNameChanged();
    void onColorChanged();
    void onWidthChanged(int value);
    void onHeightChanged(int value);

private:
    void setupUI();
    void clearDisplay();  // Clear all fields when nothing selected
    void blockSignals(bool block);  // Helper to block signals during programmatic updates

    
    // UI Elements
    QLabel *m_statusLabel;
    
    // Property widgets
    QLineEdit *m_nameEdit;
    QPushButton *m_colorButton;
    QSpinBox *m_widthSpin;
    QSpinBox *m_heightSpin;
    
    // Current entity index (for tracking)
    int m_currentEntityIndex;
    Canvas *m_canvas;  // Add canvas pointer
};

#endif // INSPECTORPANEL_H
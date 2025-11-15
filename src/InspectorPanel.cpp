#include "InspectorPanel.h"
#include "Canvas.h"  
#include "Entity.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QColorDialog>

InspectorPanel::InspectorPanel(QWidget *parent)
    : QWidget(parent)
    , m_currentEntityIndex(-1)
    , m_canvas(nullptr)  // Initialize to nullptr
{
    setupUI();
}

void InspectorPanel::setCanvas(Canvas *canvas)
{
    m_canvas = canvas;
}

void InspectorPanel::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("<b>Inspector</b>", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Properties group
    QGroupBox *propertiesGroup = new QGroupBox("Properties", this);
    QFormLayout *formLayout = new QFormLayout();
    
    // Name field - NOW ENABLED
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Entity name");
    // REMOVED: setEnabled(false) - now editable!
    formLayout->addRow("Name:", m_nameEdit);
    
    // Color button - NOW ENABLED
    m_colorButton = new QPushButton(this);
    m_colorButton->setText("Color");
    m_colorButton->setStyleSheet("background-color: rgb(100, 150, 255);");
    // REMOVED: setEnabled(false) - now editable!
    formLayout->addRow("Color:", m_colorButton);
    
    // Size fields - NOW ENABLED
    m_widthSpin = new QSpinBox(this);
    m_widthSpin->setRange(10, 500);
    m_widthSpin->setSuffix(" px");
    // REMOVED: setEnabled(false) - now editable!
    formLayout->addRow("Width:", m_widthSpin);
    
    m_heightSpin = new QSpinBox(this);
    m_heightSpin->setRange(10, 500);
    m_heightSpin->setSuffix(" px");
    // REMOVED: setEnabled(false) - now editable!
    formLayout->addRow("Height:", m_heightSpin);
    
    propertiesGroup->setLayout(formLayout);
    mainLayout->addWidget(propertiesGroup);
    
    // Status label
    m_statusLabel = new QLabel("No selection", this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);
    
    mainLayout->addStretch();
    
    setLayout(mainLayout);
    
    // Connect slots
    connect(m_nameEdit, &QLineEdit::textChanged, this, &InspectorPanel::onNameChanged);
    connect(m_colorButton, &QPushButton::clicked, this, &InspectorPanel::onColorChanged);
    connect(m_widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &InspectorPanel::onWidthChanged);
    connect(m_heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &InspectorPanel::onHeightChanged);
}

void InspectorPanel::onSelectionChanged(int entityIndex)
{
    m_currentEntityIndex = entityIndex;
    
    if (entityIndex < 0) {
        clearDisplay();
        m_statusLabel->setText("No selection");
    } else {
        m_statusLabel->setText(QString("Entity selected:\nIndex: %1").arg(entityIndex));
        // For now, just show the index
        // In next step, we'll fetch the entity and display its properties
    }
}

void InspectorPanel::clearDisplay()
{
    m_nameEdit->clear();
    m_colorButton->setStyleSheet("background-color: rgb(200, 200, 200);");
    m_widthSpin->setValue(0);
    m_heightSpin->setValue(0);
}


void InspectorPanel::updateFromEntity(int id, const QString &name, const QColor &color, int width, int height)
{
    // Block signals to prevent triggering property change handlers
    blockSignals(true);
    
    // Update all UI fields with entity data
    m_nameEdit->setText(name);
    
    // Update color button appearance
    QString colorStyle = QString("background-color: rgb(%1, %2, %3);")
                            .arg(color.red())
                            .arg(color.green())
                            .arg(color.blue());
    m_colorButton->setStyleSheet(colorStyle);
    
    // Update size fields
    m_widthSpin->setValue(width);
    m_heightSpin->setValue(height);
    
    m_statusLabel->setText(QString("Entity ID: %1").arg(id));
    
    // Restore signals
    blockSignals(false);
}

void InspectorPanel::blockSignals(bool block)
{
    // Block signals on all widgets to prevent recursive updates
    m_nameEdit->blockSignals(block);
    m_colorButton->blockSignals(block);
    m_widthSpin->blockSignals(block);
    m_heightSpin->blockSignals(block);
}

void InspectorPanel::onNameChanged()
{
    if (!m_canvas || m_currentEntityIndex < 0) {
        return;
    }
    
    Entity *entity = m_canvas->getEntity(m_currentEntityIndex);
    if (entity) {
        QString newName = m_nameEdit->text();
        if (newName != entity->name()) {
            entity->setName(newName);
            // Request canvas repaint
            m_canvas->update();
        }
    }
}

void InspectorPanel::onColorChanged()
{
    if (!m_canvas || m_currentEntityIndex < 0) {
        return;
    }
    
    Entity *entity = m_canvas->getEntity(m_currentEntityIndex);
    if (entity) {
        // Open color dialog
        QColor newColor = QColorDialog::getColor(entity->color(), this, "Choose Color");
        
        if (newColor.isValid() && newColor != entity->color()) {
            entity->setColor(newColor);
            
            // Update button appearance
            QString colorStyle = QString("background-color: rgb(%1, %2, %3);")
                                    .arg(newColor.red())
                                    .arg(newColor.green())
                                    .arg(newColor.blue());
            m_colorButton->setStyleSheet(colorStyle);
            
            // Request canvas repaint
            m_canvas->update();
        }
    }
}

void InspectorPanel::onWidthChanged(int value)
{
    if (!m_canvas || m_currentEntityIndex < 0) {
        return;
    }
    
    Entity *entity = m_canvas->getEntity(m_currentEntityIndex);
    if (entity && value != entity->rect().width()) {
        int currentHeight = entity->rect().height();
        entity->setSize(value, currentHeight);
        // Request canvas repaint
        m_canvas->update();
    }
}

void InspectorPanel::onHeightChanged(int value)
{
    if (!m_canvas || m_currentEntityIndex < 0) {
        return;
    }
    
    Entity *entity = m_canvas->getEntity(m_currentEntityIndex);
    if (entity && value != entity->rect().height()) {
        int currentWidth = entity->rect().width();
        entity->setSize(currentWidth, value);
        // Request canvas repaint
        m_canvas->update();
    }
}
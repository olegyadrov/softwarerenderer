#include "CanvasWidget.h"
#include <QPainter>
#include <cmath>
#include <QDebug>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent)
    , m_clearColor(QColor(0, 0, 0))
    , m_renderer(nullptr)
    , m_zBuffer(nullptr)
    , m_windowResized(true)
{
}

CanvasWidget::~CanvasWidget()
{
}

void CanvasWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(rect(), m_clearColor);
    if (m_renderer) {
        m_renderer->setCanvas(this);
        const int pixelCount = width() * height();
        if (m_windowResized) {
            if (m_zBuffer)
                delete m_zBuffer;
            m_zBuffer = new int[pixelCount];
            m_windowResized = false;
        }
        for (int i = 0; i < pixelCount; i++) {
            m_zBuffer[i] = std::numeric_limits<int>::min();
        }
        m_renderer->setZBuffer(m_zBuffer);
        for (Model *model : m_models) {
            m_renderer->setModel(model);
            m_renderer->render();
        }
    }
}

void CanvasWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_windowResized = true;
}

QColor CanvasWidget::clearColor() const
{
    return m_clearColor;
}

void CanvasWidget::setClearColor(const QColor &color)
{
    m_clearColor = color;
}

Renderer *CanvasWidget::renderer() const
{
    return m_renderer;
}

void CanvasWidget::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

QList<Model *> CanvasWidget::models() const
{
    return m_models;
}

void CanvasWidget::addModel(Model *model)
{
    m_models.append(model);
}

void CanvasWidget::removeModel(Model *model)
{
    m_models.removeOne(model);
}

bool CanvasWidget::containsModel(Model *model)
{
    return m_models.contains(model);
}

#include "LineRenderer.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

LineRenderer::LineRenderer()
    : Renderer()
    , m_lineColor(QColor(255, 255, 255))
{
}

LineRenderer::~LineRenderer()
{

}

void LineRenderer::drawLine(QPainter *painter, int x1, int y1, int x2, int y2)
{
    if (std::abs(x2 - x1) > std::abs(y2 - y1)) {
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for (int x = x1; x <= x2; x++) {
            if (x < 0 || x > m_canvas->width() - 1)
                continue;
            float progress = (x - x1) / static_cast<float>(x2 - x1);
            int y = y1 + std::round(progress * (y2 - y1));
            if (y < 0 || y > m_canvas->height() - 1)
                continue;
            painter->drawPoint(x, y);
        }
    } else {
        if (y1 > y2) {
            std::swap(y1, y2);
            std::swap(x1, x2);
        }
        for (int y = y1; y <= y2; y++) {
            if (y < 0 || y > m_canvas->height() - 1)
                continue;
            float progress = (y - y1) / static_cast<float>(y2 - y1);
            int x = x1 + std::round(progress * (x2 - x1));
            if (x < 0 || x > m_canvas->width() - 1)
                continue;
            painter->drawPoint(x, y);
        }
    }
}

void LineRenderer::render()
{
    if (!m_model) {
        qDebug() << "Line renderer: no model to render";
        return;
    }
    QPainter painter(m_canvas);
    painter.setPen(QColor(255, 255, 255));
    for (Face face : m_model->faces()) {
        QVector3D vertexA = m_model->vertices().at(face.vertexA);
        QVector3D vertexB = m_model->vertices().at(face.vertexB);
        QVector3D vertexC = m_model->vertices().at(face.vertexC);
        applyVertexTransformations(vertexA);
        applyVertexTransformations(vertexB);
        applyVertexTransformations(vertexC);
        drawLine(&painter,
                 mapXToScreenCoordinate(vertexA.x()), mapYToScreenCoordinate(vertexA.y()),
                 mapXToScreenCoordinate(vertexB.x()), mapYToScreenCoordinate(vertexB.y()));
        drawLine(&painter,
                 mapXToScreenCoordinate(vertexB.x()), mapYToScreenCoordinate(vertexB.y()),
                 mapXToScreenCoordinate(vertexC.x()), mapYToScreenCoordinate(vertexC.y()));
        drawLine(&painter,
                 mapXToScreenCoordinate(vertexA.x()), mapYToScreenCoordinate(vertexA.y()),
                 mapXToScreenCoordinate(vertexC.x()), mapYToScreenCoordinate(vertexC.y()));
    }
}

QColor LineRenderer::lineColor() const
{
    return m_lineColor;
}

void LineRenderer::setLineColor(QColor color)
{
    m_lineColor = color;
}

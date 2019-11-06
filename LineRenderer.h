#ifndef LINERENDERER_H
#define LINERENDERER_H

#include "Renderer.h"

class LineRenderer : public Renderer
{
public:
    LineRenderer();
    ~LineRenderer() override;
    void render() override;
    QColor lineColor() const;
    void setLineColor(QColor color);

private:
    void drawLine(QPainter *painter, int x1, int y1, int x2, int y2);
    QColor m_lineColor;
};

#endif // LINERENDERER_H

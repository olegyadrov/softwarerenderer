#ifndef TRIANGLERENDERER_H
#define TRIANGLERENDERER_H

#include <Renderer.h>
#include <QVector3D>

class TriangleRenderer : public Renderer
{
public:
    TriangleRenderer();
    ~TriangleRenderer() override;
    void render() override;
    QVector3D lightDirection() const;
    void setLightDirection(QVector3D direction);
private:
    void drawTriangle(QPainter *painter, int x1, int y1, int z1,
                                         int x2, int y2, int z2,
                                         int x3, int y3, int z3);
    QVector3D m_lightDirection;
};

#endif // TRIANGLERENDERER_H

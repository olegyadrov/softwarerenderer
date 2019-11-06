#ifndef TEXTUREDTRIANGLERENDERER_H
#define TEXTUREDTRIANGLERENDERER_H

#include <Renderer.h>
#include <QVector3D>

class TexturedTriangleRenderer : public Renderer
{
public:
    TexturedTriangleRenderer();
    ~TexturedTriangleRenderer() override;
    void render() override;
    QVector3D lightDirection() const;
    void setLightDirection(QVector3D direction);
private:
    void drawTriangle(QPainter *painter, int x1, int y1, int z1,
                                         int x2, int y2, int z2,
                                         int x3, int y3, int z3,
                                         int u1, int v1,
                                         int u2, int v2,
                                         int u3, int v3,
                                         float lightIntensity);
    QVector3D m_lightDirection;
};

#endif // TEXTUREDTRIANGLERENDERER_H

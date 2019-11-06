#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>
#include <cmath>
#include "Model.h"

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    virtual void render() = 0;
    void setCanvas(QWidget *canvas);
    QPaintDevice *canvas() const;
    void setModel(Model *model);
    Model *model() const;
    void setZBuffer(int *zBuffer);
    int *zBuffer() const;
    void setDepth(int depth);
    int depth() const;
    void setPerspectiveTransformationEnabled(bool enabled);
    bool isPerspectiveTransformationEnabled() const;
    void setAutomaticRotationEnabled(bool enabled);
    bool isAutomaticRotationEnabled() const;
    void setRotationAngle(float angle);
    float rotationAngle() const;
    inline int mapXToScreenCoordinate(float x) {
        if (!m_canvas)
            return 0;
        const float w = m_canvas->width();
        const float h = m_canvas->height();
        if (w < h)
            return (x + 1) / 2.0f * (w - 1);
        else {
            return (x + 1) / 2.0f * (h - 1) + (w / 2.0f - h / 2.0f);
        }
    }
    inline int mapYToScreenCoordinate(float y) {
        if (!m_canvas)
            return 0;
        const float w = m_canvas->width();
        const float h = m_canvas->height();
        if (h < w)
            return (y + 1) / 2.0f * (h - 1);
        else {
            return (y + 1) / 2.0f * (w - 1) + (h / 2.0f - w / 2.0f);
        }
    }
    inline int mapUToTextureCoordinate(float u) {
        if (!m_model)
            return 0;
        return std::round(u * (m_model->texture().width() - 1));
    }
    inline int mapVToTextureCoordinate(float v) {
        if (!m_model)
            return 0;
        return std::round(v * (m_model->texture().height() - 1));
    }
protected:
    void applyVertexTransformations(QVector3D &vertex);
    void applyNormalTransformations(QVector3D &normal);
    QWidget *m_canvas;
    Model *m_model;
    int *m_zBuffer;
    int m_depth;
    bool m_perspectiveTransformationEnabled;
    float m_rotationAngle;
};

#endif // RENDERER_H

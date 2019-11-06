#include "Renderer.h"
#include <QMatrix4x4>

Renderer::Renderer()
    : m_canvas(nullptr)
    , m_model(nullptr)
    , m_zBuffer(nullptr)
    , m_depth(255)
    , m_perspectiveTransformationEnabled(false)
    , m_rotationAngle(0.0f)
{
}

Renderer::~Renderer()
{
}

void Renderer::setCanvas(QWidget *canvas)
{
    m_canvas = canvas;
}

QPaintDevice *Renderer::canvas() const
{
    return m_canvas;
}

void Renderer::setModel(Model *model)
{
    m_model = model;
}

Model *Renderer::model() const
{
    return m_model;
}

void Renderer::setZBuffer(int *zBuffer)
{
    m_zBuffer = zBuffer;
}

int *Renderer::zBuffer() const
{
    return m_zBuffer;
}

void Renderer::setDepth(int depth)
{
    m_depth = depth;
}

int Renderer::depth() const
{
    return m_depth;
}

void Renderer::setPerspectiveTransformationEnabled(bool enabled)
{
    if (m_perspectiveTransformationEnabled == enabled)
        return;
    m_perspectiveTransformationEnabled = enabled;
    if (m_canvas)
        m_canvas->update();
}

bool Renderer::isPerspectiveTransformationEnabled() const
{
    return m_perspectiveTransformationEnabled;
}

void Renderer::setRotationAngle(float angle)
{
    m_rotationAngle = angle;
    if (m_canvas)
        m_canvas->update();
}

float Renderer::rotationAngle() const
{
    return m_rotationAngle;
}

void Renderer::applyVertexTransformations(QVector3D &vertex)
{
    if (!m_perspectiveTransformationEnabled && qFuzzyCompare(m_rotationAngle, 0.0f))
        return;
    QMatrix4x4 transformationMatrix;
    transformationMatrix.setToIdentity();
    if (m_perspectiveTransformationEnabled) {
        QMatrix4x4 perspectiveMatrix;
        perspectiveMatrix.setToIdentity();
        perspectiveMatrix.setRow(3, QVector4D(0.0f, 0.0f, -1/3.0f, 1.0f));
        transformationMatrix *= perspectiveMatrix;
    }
    if (!qFuzzyCompare(m_rotationAngle, 0.0f)) {
        QMatrix4x4 rotationMatrix;
        rotationMatrix.setToIdentity();
        rotationMatrix.rotate(m_rotationAngle, 0.0f, 1.0f, 0.0f);
        transformationMatrix *= rotationMatrix;
    }
    if (!transformationMatrix.isIdentity()) {
        vertex = transformationMatrix * vertex;
    }
}

void Renderer::applyNormalTransformations(QVector3D &normal)
{
    if (qFuzzyCompare(m_rotationAngle, 0.0f))
        return;
    QMatrix4x4 transformationMatrix;
    transformationMatrix.setToIdentity();
    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity();
    rotationMatrix.rotate(m_rotationAngle, 0.0f, 1.0f, 0.0f);
    transformationMatrix *= rotationMatrix;
    if (!transformationMatrix.isIdentity()) {
        normal = transformationMatrix * normal;
    }
}

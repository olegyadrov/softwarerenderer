#include "NormalLightingRenderer.h"
#include <QDebug>
#include <QPainter>
#include <cmath>
#include <QMatrix4x4>

NormalLightingRenderer::NormalLightingRenderer()
    : Renderer()
    , m_lightDirection(0.0f, 0.0f, 1.0f)
{
}

NormalLightingRenderer::~NormalLightingRenderer()
{
}

void NormalLightingRenderer::drawTriangle(QPainter *painter, int x1, int y1, int z1,
                                                   int x2, int y2, int z2,
                                                   int x3, int y3, int z3,
                                                   int u1, int v1,
                                                   int u2, int v2,
                                                   int u3, int v3,
                                                   float n1x, float n1y, float n1z,
                                                   float n2x, float n2y, float n2z,
                                                   float n3x, float n3y, float n3z)
{
    if (y1 == y2 && y2 == y3)
        return;
    if (y2 < y1) {
        std::swap(x1, x2);
        std::swap(y1, y2);
        std::swap(z1, z2);
        std::swap(u1, u2);
        std::swap(v1, v2);
        std::swap(n1x, n2x);
        std::swap(n1y, n2y);
        std::swap(n1z, n2z);
    }
    if (y3 < y1) {
        std::swap(x1, x3);
        std::swap(y1, y3);
        std::swap(z1, z3);
        std::swap(u1, u3);
        std::swap(v1, v3);
        std::swap(n1x, n3x);
        std::swap(n1y, n3y);
        std::swap(n1z, n3z);
    }
    if (y3 < y2) {
        std::swap(x2, x3);
        std::swap(y2, y3);
        std::swap(z2, z3);
        std::swap(u2, u3);
        std::swap(v2, v3);
        std::swap(n2x, n3x);
        std::swap(n2y, n3y);
        std::swap(n2z, n3z);
    }
    for (int y = y1; y <= y2; y++) {
        if (y < 0 || y > m_canvas->height() - 1)
            continue;
        float localProgress = (y2 - y1 == 0) ? 0.0f : (y - y1) / static_cast<float>(y2 - y1);
        float globalProgress = (y3 - y1 == 0) ? 0.0f :  (y - y1) / static_cast<float>(y3 - y1);
        int xLeft = x1 + std::round(localProgress * (x2 - x1));
        int xRight = x1 + std::round(globalProgress * (x3 - x1));
        int zLeft = z1 + std::round(localProgress * (z2 - z1));
        int zRight = z1 + std::round(globalProgress * (z3 - z1));
        int uLeft = u1 + std::round(localProgress * (u2 - u1));
        int uRight = u1 + std::round(globalProgress * (u3 - u1));
        float nxLeft = n1x + localProgress * (n2x - n1x);
        float nxRight = n1x + globalProgress * (n3x - n1x);
        float nzLeft = n1z + localProgress * (n2z - n1z);
        float nzRight = n1z + globalProgress * (n3z - n1z);
        if (xRight < xLeft) {
            std::swap(xLeft, xRight);
            std::swap(zLeft, zRight);
            std::swap(uLeft, uRight);
            std::swap(nxLeft, nxRight);
            std::swap(nzLeft, nzRight);
        }
        for (int x = xLeft; x <= xRight; x++) {
            if (x < 0 || x > m_canvas->width() - 1)
                continue;
            float xProgress = (xRight - xLeft == 0) ? 0.0f : (x - xLeft) / static_cast<float>(xRight - xLeft);
            int z = zLeft + std::round(xProgress * (zRight - zLeft));
            int zIndex = x + y * m_canvas->width();
            if (m_zBuffer[zIndex] < z) {
                int u = uLeft + std::round(xProgress * (uRight - uLeft));
                int v = v1 + std::round(localProgress * (v2 - v1));
                float nx = nxLeft + xProgress * (nxRight - nxLeft);
                float ny = n1y + localProgress * (n2y - n1y);
                float nz = nzLeft + xProgress * (nzRight - nzLeft);
                QVector3D normalToPixel = QVector3D(nx, ny, nz);
                float lightIntensity = QVector3D::dotProduct(normalToPixel, m_lightDirection);
                if (lightIntensity > 0) {
                    QColor pixelColor = m_model->texture().isNull() ? QColor(255, 255, 255)
                                                                    : m_model->texture().pixelColor(u, v);
                    pixelColor = QColor(pixelColor.red() * lightIntensity,
                                        pixelColor.green() * lightIntensity,
                                        pixelColor.blue() * lightIntensity);
                    painter->setPen(pixelColor);
                    painter->drawPoint(x, y);
                    m_zBuffer[zIndex] = z;
                }
            }
        }
    }
    for (int y = y2 + 1; y <= y3; y++) {
        if (y < 0 || y > m_canvas->height() - 1)
            continue;
        float localProgress = (y3 - y2 == 0) ? 0.0f : (y - y2) / static_cast<float>(y3 - y2);
        float globalProgress = (y3 - y1 == 0) ? 0.0f :  (y - y1) / static_cast<float>(y3 - y1);
        int xLeft = x2 + std::round(localProgress * (x3 - x2));
        int xRight = x1 + std::round(globalProgress * (x3 - x1));
        int zLeft = z2 + std::round(localProgress * (z3 - z2));
        int zRight = z1 + std::round(globalProgress * (z3 - z1));
        int uLeft = u2 + std::round(localProgress * (u3 - u2));
        int uRight = u1 + std::round(globalProgress * (u3 - u1));
        float nxLeft = n2x + localProgress * (n3x - n2x);
        float nxRight = n1x + globalProgress * (n3x - n1x);
        float nzLeft = n2z + localProgress * (n3z - n2z);
        float nzRight = n1z + globalProgress * (n3z - n1z);
        if (xRight < xLeft) {
            std::swap(xLeft, xRight);
            std::swap(zLeft, zRight);
            std::swap(uLeft, uRight);
            std::swap(nxLeft, nxRight);
            std::swap(nzLeft, nzRight);
        }
        for (int x = xLeft; x <= xRight; x++) {
            if (x < 0 || x > m_canvas->width() - 1)
                continue;
            float xProgress = (xRight - xLeft == 0) ? 0.0f : (x - xLeft) / static_cast<float>(xRight - xLeft);
            int z = zLeft + std::round(xProgress * (zRight - zLeft));
            int zIndex = x + y * m_canvas->width();
            if (m_zBuffer[zIndex] < z) {
                int u = uLeft + std::round(xProgress * (uRight - uLeft));
                int v = v2 + std::round(localProgress * (v3 - v2));
                float nx = nxLeft + xProgress * (nxRight - nxLeft);
                float ny = n2y + localProgress * (n3y - n2y);
                float nz = nzLeft + xProgress * (nzRight - nzLeft);
                QVector3D normalToPixel = QVector3D(nx, ny, nz);
                float lightIntensity = QVector3D::dotProduct(normalToPixel, m_lightDirection);
                if (lightIntensity > 0) {
                    QColor pixelColor = m_model->texture().isNull() ? QColor(255, 255, 255)
                                                                    : m_model->texture().pixelColor(u, v);
                    pixelColor = QColor(pixelColor.red() * lightIntensity,
                                        pixelColor.green() * lightIntensity,
                                        pixelColor.blue() * lightIntensity);
                    painter->setPen(pixelColor);
                    painter->drawPoint(x, y);
                    m_zBuffer[zIndex] = z;
                }
            }
        }
    }
}

void NormalLightingRenderer::render()
{
    if (!m_model) {
        qDebug() << "Test renderer: no model to render";
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
        QVector2D textureCoordinatesA = m_model->uvs().isEmpty() ? QVector2D() : m_model->uvs().at(face.textureCoordinateA);
        QVector2D textureCoordinatesB = m_model->uvs().isEmpty() ? QVector2D() : m_model->uvs().at(face.textureCoordinateB);
        QVector2D textureCoordinatesC = m_model->uvs().isEmpty() ? QVector2D() : m_model->uvs().at(face.textureCoordinateC);
        QVector3D normalA = m_model->normals().at(face.normalA);
        QVector3D normalB = m_model->normals().at(face.normalB);
        QVector3D normalC = m_model->normals().at(face.normalC);
        applyNormalTransformations(normalA);
        applyNormalTransformations(normalB);
        applyNormalTransformations(normalC);
        drawTriangle(&painter,
                     mapXToScreenCoordinate(vertexA.x()), mapYToScreenCoordinate(vertexA.y()), vertexA.z() * m_depth,
                     mapXToScreenCoordinate(vertexB.x()), mapYToScreenCoordinate(vertexB.y()), vertexB.z() * m_depth,
                     mapXToScreenCoordinate(vertexC.x()), mapYToScreenCoordinate(vertexC.y()), vertexC.z() * m_depth,
                     mapUToTextureCoordinate(textureCoordinatesA.x()), mapVToTextureCoordinate(textureCoordinatesA.y()),
                     mapUToTextureCoordinate(textureCoordinatesB.x()), mapVToTextureCoordinate(textureCoordinatesB.y()),
                     mapUToTextureCoordinate(textureCoordinatesC.x()), mapVToTextureCoordinate(textureCoordinatesC.y()),
                     normalA.x(), normalA.y(), normalA.z(),
                     normalB.x(), normalB.y(), normalB.z(),
                     normalC.x(), normalC.y(), normalC.z());
    }
}

QVector3D NormalLightingRenderer::lightDirection() const
{
    return m_lightDirection;
}

void NormalLightingRenderer::setLightDirection(QVector3D direction)
{
    m_lightDirection = direction;
}

#include "Model.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <cmath>

Model::Model()
{
}

bool Model::loadVertexData(const QString &source)
{
    clearVertexData();
    QFile file(source);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file" << source;
        return false;
    }
    float largestVertexComponent = 1.0f;
    QTextStream textStream(&file);
    while (!textStream.atEnd()) {
        QString line = textStream.readLine().simplified();
        if (line.startsWith("v ")) { // vertex
            QStringList vertexComponents = line.split(' ');
            float vertexComponentA = vertexComponents[1].toFloat();
            float vertexComponentB = vertexComponents[2].toFloat() * -1;
            float vertexComponentC = vertexComponents[3].toFloat();
            if (std::abs(vertexComponentA) > largestVertexComponent)
                largestVertexComponent = std::abs(vertexComponentA);
            if (std::abs(vertexComponentB) > largestVertexComponent)
                largestVertexComponent = std::abs(vertexComponentB);
            if (std::abs(vertexComponentC) > largestVertexComponent)
                largestVertexComponent = std::abs(vertexComponentC);
            m_vertices << QVector3D(vertexComponentA, vertexComponentB, vertexComponentC);
        } else if (line.startsWith("f ")) { // face
            QStringList faceComponents = line.split(' ');
            QStringList faceComponentA = faceComponents[1].split('/');
            QStringList faceComponentB = faceComponents[2].split('/');
            QStringList faceComponentC = faceComponents[3].split('/');
            int faceVertexA = faceComponentA[0].toInt() - 1;
            int faceVertexB = faceComponentB[0].toInt() - 1;
            int faceVertexC = faceComponentC[0].toInt() - 1;
            int faceTextureCoordinateA = faceComponentA[1].toInt() - 1;
            int faceTextureCoordinateB = faceComponentB[1].toInt() - 1;
            int faceTextureCoordinateC = faceComponentC[1].toInt() - 1;
            int faceNormalA = faceComponentA[2].toInt() - 1;
            int faceNormalB = faceComponentB[2].toInt() - 1;
            int faceNormalC = faceComponentC[2].toInt() - 1;
            m_faces << Face(faceVertexA, faceVertexB, faceVertexC,
                            faceTextureCoordinateA, faceTextureCoordinateB, faceTextureCoordinateC,
                            faceNormalA, faceNormalB, faceNormalC);
        } else if (line.startsWith("vt ")) { // texture coordinates
            QStringList uvComponents = line.split(' ');
            float uvComponentA = uvComponents[1].toFloat();
            float uvComponentB = 1.0f - uvComponents[2].toFloat();
            m_uvs << QVector2D(uvComponentA, uvComponentB);
        } else if (line.startsWith("vn ")) { // normals
            QStringList normalComponents = line.split(' ');
            float normalComponentA = normalComponents[1].toFloat();
            float normalComponentB = normalComponents[2].toFloat();
            float normalComponentC = normalComponents[3].toFloat();
            m_normals << QVector3D(normalComponentA, normalComponentB, normalComponentC);
        }
    }
    if (largestVertexComponent > 1.0f) {
        for (QVector3D &vertex : m_vertices) {
            vertex /= largestVertexComponent;
        }
    }
    return true;
}

bool Model::loadTexture(const QString &source)
{
    clearTexture();
    return m_texture.load(source);
}

bool Model::hasVertexData() const
{
    return !m_vertices.isEmpty();
}

bool Model::hasTexture() const
{
    return !m_texture.isNull();
}

QVector<QVector3D> Model::vertices() const
{
    return m_vertices;
}

QVector<Face> Model::faces() const
{
    return m_faces;
}

QVector<QVector2D> Model::uvs() const
{
    return m_uvs;
}

QVector<QVector3D> Model::normals() const
{
    return m_normals;
}

QImage Model::texture() const
{
    return m_texture;
}

void Model::clearVertexData()
{
    m_vertices.clear();
    m_faces.clear();
    m_uvs.clear();
    m_normals.clear();
}

void Model::clearTexture()
{
    m_texture = QImage();
}

#ifndef MODEL_H
#define MODEL_H

#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QImage>

class Face {
public:
    Face()
        : vertexA(0)
        , vertexB(0)
        , vertexC(0)
        , textureCoordinateA(0)
        , textureCoordinateB(0)
        , textureCoordinateC(0)
        , normalA(0)
        , normalB(0)
        , normalC(0)
    {}
    Face(int vertexA, int vertexB, int vertexC,
         int textureCoordinateA, int textureCoordinateB, int textureCoordinateC,
         int normalA, int normalB, int normalC)
        : vertexA(vertexA)
        , vertexB(vertexB)
        , vertexC(vertexC)
        , textureCoordinateA(textureCoordinateA)
        , textureCoordinateB(textureCoordinateB)
        , textureCoordinateC(textureCoordinateC)
        , normalA(normalA)
        , normalB(normalB)
        , normalC(normalC)
    {}
    int vertexA;
    int vertexB;
    int vertexC;
    int textureCoordinateA;
    int textureCoordinateB;
    int textureCoordinateC;
    int normalA;
    int normalB;
    int normalC;
};

class Model
{
public:
    Model();
    bool loadVertexData(const QString &source);
    bool loadTexture(const QString &source);
    bool hasVertexData() const;
    bool hasTexture() const;
    QVector<QVector3D> vertices() const;
    QVector<Face> faces() const;
    QVector<QVector2D> uvs() const;
    QVector<QVector3D> normals() const;
    QImage texture() const;

private:
    void clearVertexData();
    void clearTexture();
    QVector<QVector3D> m_vertices;
    QVector<Face> m_faces;
    QVector<QVector2D> m_uvs;
    QVector<QVector3D> m_normals;
    QImage m_texture;
};

#endif // MODEL_H

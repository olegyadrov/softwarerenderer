#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include "Renderer.h"

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = nullptr);
    ~CanvasWidget() override;
    QColor clearColor() const;
    void setClearColor(const QColor &color);
    Renderer *renderer() const;
    void setRenderer(Renderer *renderer);
    QList<Model *> models() const;
    void addModel(Model *model);
    void removeModel(Model *model);
    bool containsModel(Model *model);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QColor m_clearColor;
    Renderer *m_renderer;
    QList<Model *> m_models;
    int *m_zBuffer;
    bool m_windowResized;
};

#endif // CANVASWIDGET_H

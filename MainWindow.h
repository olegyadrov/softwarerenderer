#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onRenderModeChanged();
    void onRotationChanged();
    void onPerspectiveModeChanged();

private:
    void showLoadMeshDialog();
    void showLoadTextureDialog();
    void showBackgroundColorDialog();
    void showSaveRenderedImageDialog();
    Ui::MainWindow *ui;
    Model m_model;
};

#endif // MAINWINDOW_H

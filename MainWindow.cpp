#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CanvasWidget.h"
#include "LineRenderer.h"
#include "TriangleRenderer.h"
#include "TexturedTriangleRenderer.h"
#include "NormalLightingRenderer.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_loadMesh, &QAction::triggered, this, &MainWindow::showLoadMeshDialog);
    connect(ui->action_loadTexture, &QAction::triggered, this, &MainWindow::showLoadTextureDialog);
    connect(ui->active_setBackgroundColor, &QAction::triggered, this, &MainWindow::showBackgroundColorDialog);
    connect(ui->action_saveRenderedImage, &QAction::triggered, this, &MainWindow::showSaveRenderedImageDialog);
    connect(ui->comboBox_renderMode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::onRenderModeChanged);
    connect(ui->horizontalSlider_rotation, &QSlider::valueChanged, this, &MainWindow::onRotationChanged);
    connect(ui->checkBox_perspective, &QCheckBox::stateChanged, this, &MainWindow::onPerspectiveModeChanged);
    m_model.loadVertexData(":/assets/head_mesh.obj");
    m_model.loadTexture(":/assets/head_texture.png");
    ui->canvasWidget->addModel(&m_model);
    onRenderModeChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRenderModeChanged()
{
    delete ui->canvasWidget->renderer();
    const int index = ui->comboBox_renderMode->currentIndex();
    switch (index) {
    case 0:
        ui->canvasWidget->setRenderer(new LineRenderer());
        break;
    case 1:
        ui->canvasWidget->setRenderer(new TriangleRenderer());
        break;
    case 2:
        ui->canvasWidget->setRenderer(new TexturedTriangleRenderer());
        break;
    case 3:
        ui->canvasWidget->setRenderer(new NormalLightingRenderer());
        break;
    }
    if (ui->checkBox_perspective->isChecked())
        ui->canvasWidget->renderer()->setPerspectiveTransformationEnabled(true);
    ui->canvasWidget->renderer()->setRotationAngle(ui->horizontalSlider_rotation->value());
    ui->canvasWidget->update();
}

void MainWindow::onRotationChanged()
{
    ui->canvasWidget->renderer()->setRotationAngle(ui->horizontalSlider_rotation->value());
}

void MainWindow::onPerspectiveModeChanged()
{
    ui->canvasWidget->renderer()->setPerspectiveTransformationEnabled(ui->checkBox_perspective->isChecked());
}

void MainWindow::showLoadMeshDialog()
{
    QString folder;
    QStringList downloadLocations = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    if (!downloadLocations.isEmpty())
        folder = downloadLocations.first();
    QString fileName = QFileDialog::getOpenFileName(this,"Load Mesh", folder, "OBJ Model (*.obj)");
    if (!fileName.isEmpty()) {
        m_model.loadVertexData(fileName);
        m_model.loadTexture("");
        ui->canvasWidget->update();
    }
}

void MainWindow::showLoadTextureDialog()
{
    QString folder;
    QStringList downloadLocations = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    if (!downloadLocations.isEmpty())
        folder = downloadLocations.first();
    QString fileName = QFileDialog::getOpenFileName(this, "Load Texture", folder, "Image Files (*.png *.jpg *.jpeg *.bmp *.tga)");
    if (!fileName.isEmpty()) {
        m_model.loadTexture(fileName);
        ui->canvasWidget->update();
    }
}

void MainWindow::showBackgroundColorDialog()
{
    QColor color = QColorDialog::getColor(ui->canvasWidget->clearColor(), this, "Background color");
    ui->canvasWidget->setClearColor(color);
    ui->canvasWidget->update();
}
#include <QDebug>
void MainWindow::showSaveRenderedImageDialog()
{
    QString folder = "/render.png";
    QStringList downloadLocations = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    if (!downloadLocations.isEmpty())
        folder = downloadLocations.first() + "/render.png";
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", folder, "Images (*.png *.xpm *.jpg)");
    if (!fileName.isEmpty()) {
        QString format = fileName.split('.').last();
        qDebug() << "saving" << format << "image";
        QPixmap image = ui->canvasWidget->grab();
        image.save(fileName, format.toLocal8Bit(), 100);
    }
}

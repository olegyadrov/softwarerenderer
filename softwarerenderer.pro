QT += core gui widgets

TARGET = softwarerenderer
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        LineRenderer.cpp \
        MainWindow.cpp \
        Model.cpp \
        NormalLightingRenderer.cpp \
        Renderer.cpp \
        TexturedTriangleRenderer.cpp \
        TriangleRenderer.cpp \
        main.cpp \
        CanvasWidget.cpp

HEADERS += \
        CanvasWidget.h \
        LineRenderer.h \
        MainWindow.h \
        Model.h \
        NormalLightingRenderer.h \
        Renderer.h \
        TexturedTriangleRenderer.h \
        TriangleRenderer.h

RESOURCES += \
    resources.qrc

FORMS += \
    MainWindow.ui

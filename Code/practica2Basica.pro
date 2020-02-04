#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T16:04:09
#
#-------------------------------------------------

QT       += core gui opengl
QT       += widgets


QMAKE_CXXFLAGS += -std=c++11

TARGET = practica2Basica
TEMPLATE = app

SOURCES +=\
    Common.cpp \
    cara.cpp \
    readfile.cpp \
    Scene.cpp \
    Camera.cpp \
    Light.cpp \
    Object.cpp \
    Material.cpp \
    MainWindow.cpp \
    Main.cpp \
    GLWidget.cpp \
    PointLight.cpp \
    DirectionalLight.cpp \
    SpotLight.cpp

HEADERS  += \
    vec.h \
    mat.h \
    Common.h \
    cara.h \
    readfile.h \
    Scene.h \
    Camera.h \
    Light.h \
    Object.h \
    Material.h \
    MainWindow.h \
    GLWidget.h \
    PointLight.h \
    DirectionalLight.h \
    SpotLight.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    vshaderGouraud.glsl \
    fshaderGouraud.glsl \
    vshaderPhong.glsl \
    fshaderPhong.glsl \
    vshaderToon.glsl \
    fshaderToon.glsl \
    vshaderEnvironmental.glsl \
    fshaderEnvironmental.glsl

RESOURCES += resources.qrc

DISTFILES +=

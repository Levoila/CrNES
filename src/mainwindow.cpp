#include "mainwindow.h"

#include <GL/gl.h>
#include <QDebug>

#include <iostream>

MainWindow::MainWindow()
{
    pixels.fill(128);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::resizeGL(int w, int h)
{
    qDebug() << "Resize to " << "(" << w << ", " << h << ")";

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, w, 0.0f, h, -1.0, 1.0);
}

void MainWindow::paintGL()
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    glDrawPixels(nesWidth, nesHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
}

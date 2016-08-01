#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include <array>
#include <cstdint>

class MainWindow : public QOpenGLWidget
{
public:
    MainWindow();
    ~MainWindow();

private:
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void initializeGL() override;

    static constexpr int nesWidth = 256;
    static constexpr int nesHeight = 240;
    std::array<uint8_t, nesWidth * nesHeight * 3> pixels;
};

#endif

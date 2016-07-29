#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

#include <cstdint>
#include <iostream>
#include "cpu.h"

int main(int argc, char** argv)
{
	QApplication a(argc, argv);

	qDebug() << "Hello, world";

	MainWindow window;
    window.show();

    CPU cpu;

	return a.exec();
}

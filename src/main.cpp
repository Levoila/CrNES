#include <QApplication>
#include <QDebug>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "mainwindow.h"
#include "cpu.h"
#include "cpuram.h"
#include "cpubus.h"
#include "cartridgemapper.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    qDebug() << "Hello, world";

    MainWindow window;
    window.show();

    Cpu cpu;
    CpuRam cpuRam;

    CpuBus cpuBus(&cpuRam);
    cpu.setMediator(&cpuBus);

    CartridgeMapper* cartridge = loadCartridgeMapperFromFile("testRoms/instr_misc/rom_singles/03-dummy_reads.nes");
    cpuBus.setCartridge(cartridge);

    while (true) {
        cpu.tick();
    }

    std::cout << "Done!" << std::endl;

    return a.exec();
}

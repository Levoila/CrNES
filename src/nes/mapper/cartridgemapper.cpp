#include "cartridgemapper.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <array>

#include "cartridgemapper001.h"

CartridgeMapper* loadCartridgeMapperFromFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios_base::binary);

    if (!file) {
        std::cout << "File does not exist : " << filename << std::endl;
        return nullptr;
    }

    char NesStr[4] {0, 0, 0, 0};
    uint8_t nbPrgRom = 0, nbChrRom = 0, flag6 = 0, flag7 = 0;
    int mapperId = -1;
    Mirroring mirroring = Mirroring::BAD_MIRRORING;
    //bool batteryBackedSram = false;


    file.read(NesStr, 4);
    if (NesStr[0] != 'N' || NesStr[1] != 'E' || NesStr[2] != 'S' || NesStr[3] != 0x1A) {
        std::cout << "Bad ROM file : " << filename << std::endl;
        return nullptr;
    }

    file.read(reinterpret_cast<char*>(&nbPrgRom), 1);
    file.read(reinterpret_cast<char*>(&nbChrRom), 1);
    file.read(reinterpret_cast<char*>(&flag6), 1);
    file.read(reinterpret_cast<char*>(&flag7), 1);


    if ((flag7 & 0x0C) == 0x08) { //NES 2.0
        mapperId = (flag6 >> 4) | (flag7 & 0xF0);
    } else { //iNES
        mapperId = (flag6 >> 4) | (flag7 & 0x70);
    }

    //batteryBackedSram = flag6 & 0x02;

    //TODO: Some mappers control directly the mirroring.
    if (flag6 & 0x01) {
        mirroring = Mirroring::VERTICAL;
    } else {
        mirroring = Mirroring::HORIZONTAL;
    }

    if (flag6 & 0x80) {
        mirroring = Mirroring::FOUR_SCREEN;
    }

    if (flag6 & 0x04) {
        std::cout << "Trainers are not supported." << std::endl;
        return nullptr;
    }

    if (flag7 & 0x02) {
        std::cout << "Playchoice 10 games not supported." << std::endl;
        return nullptr;
    }

    if (flag7 & 0x01) {
        std::cout << "Vs. Unisystem games not supported." << std::endl;
        return nullptr;
    }

    file.seekg(8, file.cur); //Unsupported bytes

    std::cout << "Nb Program ROM pages : " << static_cast<unsigned int>(nbPrgRom)
              << "\nNb Character ROM pages : " << static_cast<unsigned int>(nbChrRom)
              << "\nMapper : " << mapperId
              << "\nMirroring : " << static_cast<int>(mirroring) << std::endl;

    std::vector<std::array<uint8_t, 0x4000>> prgRoms(nbPrgRom);
    std::vector<std::array<uint8_t, 0x2000>> chrRoms(nbChrRom);

    for (auto& prgRom : prgRoms) {
        file.read(reinterpret_cast<char*>(prgRom.data()), 0x4000);
    }

    for (auto& chrRom : chrRoms) {
        file.read(reinterpret_cast<char*>(chrRom.data()), 0x2000);
    }

    //Instantiate the correct CartridgeMapper
    switch (mapperId) {
    case 0:
        return new CartridgeMapper001(mirroring, prgRoms, chrRoms);
        break;
    default:
        std::cout << "Mapper #" << mapperId << " is not supported." << std::endl;
        return nullptr;
    }
}

CartridgeMapper::CartridgeMapper(Mirroring mirroring, const std::vector<std::array<uint8_t, 0x4000>>& prgRom, const std::vector<std::array<uint8_t, 0x2000>>& chrRom)
    : mirroring(mirroring), mapperId(0), prgRom(toContiguousVector(prgRom)), chrRom(toContiguousVector(chrRom))
{

}

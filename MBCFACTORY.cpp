#include "MBCFACTORY.h"
#include "MBC3.h"
#include <fstream>
#include "NOMBC.h"

MBC* MBCFACTORY::createMBC(std::string romPath, std::string savePath)
{
    //take a look at hex byte 147 of rom then figure what to do from there like a huge switch statement
    uint8_t determinerByte;
    std::ifstream file(romPath, std::ios::binary);
    auto bytes = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    determinerByte = bytes[0x147];

    switch (determinerByte)
    {
    case 0x01:
        return new MBC3(romPath, savePath);
    case 0x00:
        return new NOMBC(romPath, savePath);
    case 0x11:
        return new MBC3(romPath, savePath);
    case 0x12:
        return new MBC3(romPath, savePath);
    case 0x13:
        return new MBC3(romPath, savePath);
    default:
        printf("didn't find appropiate MBC\n");
        return nullptr;
    }
}

#include "AddressSpace.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include "helpers.h"
#include "MBC3.h"
#include "MBC.h"
#include "NOMBC.h"
#include "MBCFACTORY.h"
#include <iterator>

// todo! create different regions so that 640000 byte array is not needed.

static void print_binary(uint8_t n) {
    unsigned int mask = 1 << (sizeof(n) * 8 - 1);  // Mask to start from the most significant bit

    // Loop through all bits
    for (int i = 0; i < sizeof(n) * 8; i++) {
        // Print the current bit
        putchar((n & mask) ? '1' : '0');

        // Shift the mask to the right
        mask >>= 1;

        // Print space every 8 bits (optional for better readability)
        if ((i + 1) % 8 == 0) {
            putchar(' ');
        }
    }
    putchar('\n');
}


//make accessing hardware registers easier with an alternate function for some, i.e
//ppu uses 

AddressSpace::AddressSpace(std::string bootPath, std::string romPath, std::string savePath) : apu(wram) {
    std::ifstream inputFile(bootPath, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    std::copy(buffer.begin(), buffer.end(),bootupRom.begin());
 
    mbc = MBCFACTORY::createMBC(romPath,savePath);
}

void AddressSpace::setCpuWriteable(bool cond){
    cpuWriteable = cond;
}





void AddressSpace::incr(uint16_t add){
    if (add < 0x7FFF) {
        mbc->incr(add);
        return;
    }
    if (add == LY)
        incrLY();
}

std::vector<uint8_t> AddressSpace::get_range(uint16_t start, uint16_t end){
    auto ve = std::vector<uint8_t>(end - start + 1);
    std::copy(&memory[start], &memory[end + 1], ve.begin());
    return ve;
}

void AddressSpace::mapbuttons(std::array<bool,8>& state){
    if (state[0] != buttonstate[0] || state[1] != buttonstate[1] 
        || state[2] != buttonstate[2] || state[3] != buttonstate[3] || state[4] != buttonstate[4]
        || state[5] != buttonstate[5] || state[6] != buttonstate[6] || state[7] != buttonstate[7]) {
        uint8_t iff = ifr;
        printf("requesting interrupt\n");
        ifr = set_bit(iff, 4, true);
    }

    std::copy(state.begin(), state.end(), buttonstate.begin());
}

AddressSpace::~AddressSpace()
{
    delete mbc;
}

void AddressSpace::writeSTAT(uint8_t stat){
    this->stat = stat;
}



void AddressSpace::incrLY()
{
    ly++;
}

void AddressSpace::setIF(uint8_t iff)
{
    ifr = iff;
}

void AddressSpace::setSTAT(uint8_t stat)
{
    memory[STAT] = stat;
}

uint8_t AddressSpace::getVRAMADD(uint16_t address)
{
    return memory[address];
}

std::vector<uint8_t> AddressSpace::saveBytes()
{
    auto saveFile = std::vector<uint8_t>();
    printf("saved startup value as %d\n", inStartup);
    saveFile.push_back(inStartup);
    saveFile.insert(saveFile.end(),memory.begin(), memory.end());
    auto mbcfile = mbc->saveBytes();
    saveFile.insert(saveFile.end(), mbcfile.begin(), mbcfile.end());
    return saveFile;
}

void AddressSpace::loadSave(std::string savePath)
{
    std::ifstream inputFile(savePath, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    inStartup = buffer[23396];
    printf("loaded startup value as %d\n", inStartup);
    std::copy(&buffer[23237], &buffer[(uint32_t)23397 + 0xFFFF], &memory[0]);

}

void AddressSpace::tickAPU(uint8_t cycles)
{
    for (auto i = 0 ; i < cycles ; i++)
        apu.tick(cycles);
}

uint8_t AddressSpace::read(uint16_t address) {

    // joypad registers
    if (address == 0xFF00) {
        uint8_t joyp = memory[address];
        uint8_t slcbutt = get_bit(joyp, 5);
        uint8_t slcdpad = get_bit(joyp, 4);

        uint8_t lowernipble = 0;
        if (slcbutt == 0) {
            lowernipble = set_bit(lowernipble, 0, !buttonstate[4]);
            lowernipble = set_bit(lowernipble, 1, !buttonstate[5]);
            lowernipble = set_bit(lowernipble, 2, !buttonstate[6]);
            lowernipble = set_bit(lowernipble, 3, !buttonstate[7]);
        }
        else if (slcdpad == 0) {
            lowernipble = set_bit(lowernipble, 0, !buttonstate[3]);
            lowernipble = set_bit(lowernipble, 1, !buttonstate[2]);
            lowernipble = set_bit(lowernipble, 2, !buttonstate[0]);
            lowernipble = set_bit(lowernipble, 3, !buttonstate[1]);
        }
        else {
            lowernipble = set_bit(lowernipble, 0, !(buttonstate[3] || buttonstate[4]));
            lowernipble = set_bit(lowernipble, 1, !(buttonstate[2] || buttonstate[5]));
            lowernipble = set_bit(lowernipble, 2, !(buttonstate[0] || buttonstate[6]));
            lowernipble = set_bit(lowernipble, 3, !(buttonstate[1] || buttonstate[7]));
        }

        uint8_t complete = (slcbutt << 5) | (slcdpad << 4) | lowernipble;
        return complete;
    }


    //bootup sequence address
    if (inStartup && address < 256) {
        return bootupRom[address];
    } 

    // mbc / rom addresses
    if (address <= 0x7FFF ||(address >= 0xA000 && address <= 0xBFFF)) {
        return mbc->read(address);
    }

    // APU registers
    if (address >= 0xFF10 && address <= 0xFF26) {
        return apu.read(address);
    }

    return memory[address];
}

void AddressSpace::loadRom(std::string file_path){
    std::ifstream inputFile(file_path, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    if (buffer.size() > 32768) {
        printf("too big needs to be under %lld bytes but is %d bytes\n",SIZE,buffer.size());
        return;
    }
    std::copy(buffer.begin(), buffer.end(), memory.begin());
    inputFile.close();
}



void AddressSpace::write(uint16_t address, uint8_t value, bool isCPU) {
    //TODO remove!!
    if (testMode && address == LY) {
        return;
    }

    //mbc address
    if (address <= 0x7FFF || (address >= 0xA000 && address <= 0xBFFF)) {
        mbc->write(address,value);
        return;
    }



    if (address == 0xFF00) {
        uint8_t dpad = get_bit(value, 4);
        uint8_t sel = get_bit(value, 5);
        uint8_t byt = memory[0xFF00];
        byt = set_bit(byt,4,dpad);
        byt = set_bit(byt,5, sel);
        memory[0xFF00] = byt;
    }

    if (isCPU && !cpuWriteable) {
        if (address >= 0xFE00 && address <= 0xFE9F) {
            return;
        }
    }

    if (address == 0xFF50 && value != 0) {
        if (debug) {
            printf("disabled bootup rom\n");
        }
       
        inStartup = false;
    }

    if (address >= 0xFF10 && address <= 0xFF26) {
        apu.write(address, value);
    }

    //some dma stuff
    if (address == 0xFF46) {
        uint16_t start_add = (uint16_t)value * 0x100;
        auto range = get_range(start_add, start_add + 160);
        std::copy(range.begin(), range.end(), &memory[0xFE00]);
        return;
    }

    memory[address] = value;
}
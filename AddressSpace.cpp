#include "AddressSpace.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include "helpers.h"
#include "MBC3.h"
#include "MBC.h"
#include "NOMBC.h"

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

AddressSpace::AddressSpace(std::string bootPath, std::string romPath) {
	std::fill(memory, memory + SIZE, 0);
    std::ifstream inputFile(bootPath, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    std::copy(buffer.begin(), buffer.end(),bootupRom.begin());
    //loadRom(romPath);
    if (testMode)
        memory[LY] = 0x90;
    mbc = new MBC3(romPath);
}

void AddressSpace::setCpuWriteable(bool cond){
    cpuWriteable = cond;
}


std::array<uint8_t,160> AddressSpace::getOAM(){
    std::array<uint8_t, 160> spriteData;
    std::copy(memory + 0xFE00, memory + 0xFE9F + 1, spriteData.begin());
    return spriteData;
}

std::array<uint8_t, 6144> AddressSpace::getVRAM()
{
    std::array<uint8_t, 6144> vram;
    std::copy(memory + 0x8000, memory + 0x97FF + 1, vram.begin());
    return vram;
}

void AddressSpace::incr(uint16_t add){
    //TODO remove!!
    if (add < 0x7FFF) {
        mbc->incr(add);
        return;
    }
    memory[add] += 1;
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
        uint8_t iff = memory[IF];
        printf("requesting interrupt\n");
        memory[IF] = set_bit(iff, 4, true);
    }

    std::copy(state.begin(), state.end(), buttonstate.begin());
}

AddressSpace::~AddressSpace()
{
    delete mbc;
}

void AddressSpace::writeSTAT(uint8_t stat){
    memory[STAT] = stat;
}



void AddressSpace::incrLY()
{
    memory[LY] += 1;
}

void AddressSpace::setIF(uint8_t iff)
{
    memory[IF] = iff;
}

void AddressSpace::setSTAT(uint8_t stat)
{
    memory[STAT] = stat;
}

uint8_t AddressSpace::getVRAMADD(uint16_t address)
{
    return memory[address];
}

uint8_t AddressSpace::read(uint16_t address) {

    

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


    if (inStartup && address < 256) {
        return bootupRom[address];
    } 

    if (address < 0x7FFF) {
        return mbc->read(address);
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
    std::copy(buffer.begin(), buffer.end(), memory);
    inputFile.close();
}



void AddressSpace::readRom(std::string file_path) {
    std::ifstream inputFile(file_path, std::ios::binary);
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());
    if (buffer.size() > SIZE) {
        printf("too big");
        return;
    }
    std::copy(buffer.begin(), buffer.end(), memory);
    inputFile.close();
}

void AddressSpace::saveRom(std::string file_path) {
    std::ofstream outputFile(file_path, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(memory), SIZE);
    printf("saving");
    outputFile.close();
}

void AddressSpace::write(uint16_t address, uint8_t value, bool isCPU) {
    //TODO remove!!
    if (testMode && address == LY) {
        return;
    }

    //mbc address
    if (address <= 0x7FFF) {
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

    //if (isCPU && !cpuWriteable) {
    //    if (address >= 0xFE00 && address <= 0xFE9F) {
    //        //printf("blocked oam write\n");
    //        return;
    //    }
    //}
    if (address == 0xFF50 && value != 0) {
        if (debug) {
            printf("disabled bootup rom\n");
        }
       
        inStartup = false;
    }

    if (address == 0xFF46) {
        //oam dma
       // printf("oam dma triggered\n");
        uint16_t start_add = (uint16_t)value * 0x100;
        auto range = get_range(start_add, start_add + 160);
        std::copy(range.begin(), range.end(), &memory[0xFE00]);
    }
    memory[address] = value;
}
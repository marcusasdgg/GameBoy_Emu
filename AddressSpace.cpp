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
 
    mbc = new NOMBC(romPath, savePath);// MBCFACTORY::createMBC(romPath, savePath);
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
    this->stat = stat;
}




uint8_t AddressSpace::getVRAMADD(uint16_t address)
{
    if (address >= 0xFE00 && address <= 0xFE9F) {
        return oam[address - 0xFE00];
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        return vram[address - 0x8000];
    }

    switch (address)
    {
    case SB:
        return sb;
    case SC:
        return sc;
    case DIV:
        return div;
    case TIMA:
        return tima;
    case 0xff06:
        return tma;
    case 0xff07:
        return tac;
    case 0xff0f:
        return ifr;
    case 0xff40:
        return lcdc;
    case 0xff41:
        return stat;
    case SCX:
        return scx;
    case SCY:
        return scy;
    case 0xff44:
        return ly;
    case 0xff45:
        return lyc;
    case 0xff46:
        return dma;
    case 0xff47:
        return bgp;
    case 0xff48:
        return obp0;
    case 0xff49:
        return obp1;
    case 0xff4a:
        return wy;
    case 0xff4b:
        return wx;
    case 0xffff:
        return ie;
    default:
        break;
    }
    printf("cant find address ppu is looking for");
    return 0xFF;
}

void AddressSpace::tickAPU(uint8_t cycles)
{
    for (auto i = 0 ; i < cycles ; i++)
        apu.tick(1);
}


uint8_t AddressSpace::read(uint16_t address) {
    // check rom bank 0


    //bootup sequence address
    if (inStartup && address < 256) {
        return bootupRom[address];
    }


    if (address <= 0x7FFF) {
        return mbc->read(address);
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        return vram[address - 0x8000];
    }

    if (address >= 0xA000 && address <= 0xBFFF) {
        return mbc->read(address);
    }

    if (address >= 0xC000 && address <= 0xCFFF) {
        return fixedRam[address - 0xC000];
    }

    if (address >= 0xD000 && address <= 0xDFFF) {
        return switchableRam[address - 0xD000];
    }


    // blah blah echoram
        
    if (address >= 0xFE00 && address <= 0xFE9F) {
        return oam[address - 0xFE00];
    }

    if (address >= 0xFF80 && address <= 0xFFFE) {
        return hram[address - 0xFF80];
    }

    // joypad registers
    if (address == 0xFF00) {
        uint8_t joyp = this->joyp;
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

    // APU registers
    if (address >= 0xFF10 && address <= 0xFF26) {
        return apu.read(address);
    }

        

    switch (address)
    {
	case SB:
		return sb;
	case 0xff02:
		return sc;
	case 0xff04:
		return div;
	case 0xff05:
		return tima;
	case 0xff06:
		return tma;
	case 0xff07:
		return tac;
	case 0xff0f:
		return ifr;
	case 0xff40:
		return lcdc;
	case 0xff41:
		return stat;
	case SCX:
		return scx;
	case SCY:
		return scy;
	case 0xff44:
		return ly;
	case 0xff45:
		return lyc;
	case 0xff46:
		return dma;
	case 0xff47:
		return bgp;
	case 0xff48:
		return obp0;
	case 0xff49:
		return obp1;
	case 0xff4a:
		return wy;
	case 0xff4b:
		return wx;
	case 0xffff:
        return ie;
    default:
        break;
    }




    return 0xFF;
}




// todo: manage side effects of register changing. i.e dma etc. think ti should just be dma and bootup rom disable
void AddressSpace::write(uint16_t address, uint8_t value, bool isCPU) {
    if (testMode && address == LY) {
        return;
    }

    //mbc address
    if (address <= 0x7FFF || (address >= 0xA000 && address <= 0xBFFF)) {
        mbc->write(address,value);
        return;
    }



    if (address == P1JOYP) {
        uint8_t dpad = get_bit(value, 4);
        uint8_t sel = get_bit(value, 5);
        uint8_t byt = joyp;
        byt = set_bit(byt,4,dpad);
        byt = set_bit(byt,5, sel);
        joyp = byt;
    } 

    //if (isCPU && !cpuWriteable) {
    //    if (address >= 0xFE00 && address <= 0xFE9F) {
    //        return;
    //    }
    //}

    // wrambank 0
    if (address >= 0xC000 && address <= 0xCFFF) {
        fixedRam[address - 0xC000] = value;
    }
        
    //switchableRam
    if (address >= 0xD000 && address <= 0xDFFF) {
        switchableRam[address - 0xD000] = value;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        vram[address - 0x8000] = value;
    }

    if (address == 0xFF50 && value != 0) {
        if (debug) {
            printf("disabled bootup rom\n");
        }
        printf("disabled bootup rom\n");
        
       
        inStartup = false;
    }

    if (address >= 0xFF10 && address <= 0xFF26) {
        apu.write(address, value);
    }

    if (address >= 0xFE00 && address <= 0xFE9F) {
        oam[address - 0xFE00] = value;
    }


    if (address >= 0xFF80 && address <= 0xFFFE) {
        hram[address - 0xFF80] = value;
    }


    
    switch (address)
    {
    case 0xff01:
        sb = value;
        return;
    case 0xff02:
        sc = value;
        return;
    case 0xff04:
        div = 0;
        return;
    case 0xff05:
        tima = value;
        return;
    case 0xff06:
        tma = value;
        return;
    case 0xff07:
        tac = value;
        return;
    case 0xff0f:
        ifr = value;
        return;
    case 0xff40:
        lcdc = value;
        return;
    case 0xff41:
        stat = value;
        return;
    case SCX:
        scx = value;
        return;
    case SCY:
        scy = value;
        return;
    case 0xff44:
        ly = value;
        return;
    case 0xff45:
        lyc = value;
        return;
    case 0xff47:
        bgp = value;
        return;
    case 0xff48:
        obp0 = value;
        return;
    case 0xff49:
        obp1 = value;
        return;
    case 0xff4a:
        wy = value;
        return;
    case 0xff4b:
        wx = value;
        return;
    case 0xffff:
        ie = value;
        return;
    default:
        break;
    }



    //some dma stuff
    // find some other way to do dma instead of using memroy
    if (address == DMA) {
        //use to copy 160 bytes from given address to oam
        uint16_t start_add = (uint16_t)value * 0x100;

        // determine if this address belongs in rom/eram/ram/wram

        // mbc/rom
        if (start_add <= 0x7FFF || (start_add >= 0xA000 && start_add <= 0xBFFF)) {
            for (int i = 0; i < 160; i++) {
                oam[i] = mbc->read(start_add + i);
            }
        }

        if (start_add >= 0x8000 && start_add <= 0x9FFF) {
            for (int i = 0; i < 160; i++) {
                oam[i] = vram[start_add - 0x8000 + i];
            }
        }

        if (start_add >= 0xC000 && start_add <= 0xCFFF) {
            for (int i = 0; i < 160; i++) {
                oam[i] = fixedRam[start_add - 0xC000 + i];
            }
        }

        if (start_add >= 0xD000 && start_add <= 0xDFFF) {
            for (int i = 0; i < 160; i++) {
                oam[i] = switchableRam[start_add - 0xD000 + i];
            }
        }

        return;
    }

    return;
}
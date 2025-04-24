#include "CPU.h"
#include "helpers.h"

uint16_t CPU::prefixedCodes(uint16_t program_counter) {
	if (debug) {
		printf("PC: %s ", to_string(program_counter));
	}
	uint8_t code = address_space.read(program_counter);
	uint8_t cycles = 0;

	switch (code) {
case 0x00 : {
		rlcr8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x01 : {
		rlcr8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x02 : {
		rlcr8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x03 : {
		rlcr8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x04 : {
		rlcr8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x05 : {
		rlcr8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x06 : {
		rlchl();
		cycles = 16;
		break;
	}
case 0x07 : {
		rlcr8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x08 : {
		rrcr8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x09 : {
		rrcr8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x0A : {
		rrcr8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x0B : {
		rrcr8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x0C : {
		rrcr8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x0D : {
		rrcr8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x0E : {
		rrchl();
		cycles = 16;
		break;
	}
case 0x0F : {
		rrcr8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x10 : {
		rlr8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x11 : {
		rlr8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x12 : {
		rlr8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x13 : {
		rlr8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x14 : {
		rlr8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x15 : {
		rlr8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x16 : {
		rlhl();
		cycles = 16;
		break;
	}
case 0x17 : {
		rla();
		cycles = 8;
		break;
	}
case 0x18 : {
		rrr8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x19 : {
		rrr8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x1A : {
		rrr8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x1B : {
		rrr8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x1C : {
		rrr8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x1D : {
		rrr8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x1E : {
		rrhl();
		cycles = 16;
		break;
	}
case 0x1F : {
		rrr8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x20 : {
		slar8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x21 : {
		slar8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x22 : {
		slar8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x23 : {
		slar8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x24 : {
		slar8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x25 : {
		slar8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x26 : {
		slahl();
		cycles = 16;
		break;
	}
case 0x27 : {
		slar8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x28 : {
		srar8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x29 : {
		srar8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x2A : {
		srar8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x2B : {
		srar8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x2C : {
		srar8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x2D : {
		srar8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x2E : {
		srahl();
		cycles = 16;
		break;
	}
case 0x2F : {
		srar8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x30 : {
		swapr8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x31 : {
		swapr8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x32 : {
		swapr8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x33 : {
		swapr8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x34 : {
		swapr8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x35 : {
		swapr8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x36 : {
		swaphl();
		cycles = 16;
		break;
	}
case 0x37 : {
		swapr8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x38 : {
		srlr8(registerCalls::B);
		cycles = 8;
		break;
	}
case 0x39 : {
		srlr8(registerCalls::C);
		cycles = 8;
		break;
	}
case 0x3A : {
		srlr8(registerCalls::D);
		cycles = 8;
		break;
	}
case 0x3B : {
		srlr8(registerCalls::E);
		cycles = 8;
		break;
	}
case 0x3C : {
		srlr8(registerCalls::H);
		cycles = 8;
		break;
	}
case 0x3D : {
		srlr8(registerCalls::L);
		cycles = 8;
		break;
	}
case 0x3E : {
		srlhl();
		cycles = 16;
		break;
	}
case 0x3F : {
		srlr8(registerCalls::A);
		cycles = 8;
		break;
	}
case 0x40 : {
		bitu3r8(0, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x41 : {
		bitu3r8(0, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x42 : {
		bitu3r8(0, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x43 : {
		bitu3r8(0, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x44 : {
		bitu3r8(0, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x45 : {
		bitu3r8(0, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x46 : {
		bitu3hl(0);
		cycles = 12;
		break;
	}
case 0x47 : {
		bitu3r8(0, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x48 : {
		bitu3r8(1, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x49 : {
		bitu3r8(1, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x4A : {
		bitu3r8(1, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x4B : {
		bitu3r8(1, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x4C : {
		bitu3r8(1, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x4D : {
		bitu3r8(1, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x4E : {
		bitu3hl(1);
		cycles = 12;
		break;
	}
case 0x4F : {
		bitu3r8(1, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x50 : {
		bitu3r8(2, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x51 : {
		bitu3r8(2, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x52 : {
		bitu3r8(2, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x53 : {
		bitu3r8(2, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x54 : {
		bitu3r8(2, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x55 : {
		bitu3r8(2, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x56 : {
		bitu3hl(2);
		cycles = 12;
		break;
	}
case 0x57 : {
		bitu3r8(2, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x58 : {
		bitu3r8(3, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x59 : {
		bitu3r8(3, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x5A : {
		bitu3r8(3, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x5B : {
		bitu3r8(3, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x5C : {
		bitu3r8(3, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x5D : {
		bitu3r8(3, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x5E : {
		bitu3hl(3);
		cycles = 12;
		break;
	}
case 0x5F : {
		bitu3r8(3, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x60 : {
		bitu3r8(4, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x61 : {
		bitu3r8(4, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x62 : {
		bitu3r8(4, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x63 : {
		bitu3r8(4, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x64 : {
		bitu3r8(4, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x65 : {
		bitu3r8(4, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x66 : {
		bitu3hl(4);
		cycles = 12;
		break;
	}
case 0x67 : {
		bitu3r8(4, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x68 : {
		bitu3r8(5, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x69 : {
		bitu3r8(5, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x6A : {
		bitu3r8(5, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x6B : {
		bitu3r8(5, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x6C : {
		bitu3r8(5, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x6D : {
		bitu3r8(5, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x6E : {
		bitu3hl(5);
		cycles = 12;
		break;
	}
case 0x6F : {
		bitu3r8(5, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x70 : {
		bitu3r8(6, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x71 : {
		bitu3r8(6, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x72 : {
		bitu3r8(6, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x73 : {
		bitu3r8(6, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x74 : {
		bitu3r8(6, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x75 : {
		bitu3r8(6, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x76 : {
		bitu3hl(6);
		cycles = 12;
		break;
	}
case 0x77 : {
		bitu3r8(6, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x78 : {
		bitu3r8(7, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x79 : {
		bitu3r8(7, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x7A : {
		bitu3r8(7, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x7B : {
		bitu3r8(7, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x7C : {
		bitu3r8(7, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x7D : {
		bitu3r8(7, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x7E : {
		bitu3hl(7);
		cycles = 12;
		break;
	}
case 0x7F : {
		bitu3r8(7, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x80 : {
		resu3r8(0, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x81 : {
		resu3r8(0, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x82 : {
		resu3r8(0, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x83 : {
		resu3r8(0, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x84 : {
		resu3r8(0, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x85 : {
		resu3r8(0, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x86 : {
		resu3hl(0);
		cycles = 16;
		break;
	}
case 0x87 : {
		resu3r8(0, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x88 : {
		resu3r8(1, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x89 : {
		resu3r8(1, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x8A : {
		resu3r8(1, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x8B : {
		resu3r8(1, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x8C : {
		resu3r8(1, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x8D : {
		resu3r8(1, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x8E : {
		resu3hl(1);
		cycles = 16;
		break;
	}
case 0x8F : {
		resu3r8(1, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x90 : {
		resu3r8(2, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x91 : {
		resu3r8(2, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x92 : {
		resu3r8(2, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x93 : {
		resu3r8(2, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x94 : {
		resu3r8(2, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x95 : {
		resu3r8(2, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x96 : {
		resu3hl(2);
		cycles = 16;
		break;
	}
case 0x97 : {
		resu3r8(2, registerCalls::A);
		cycles = 8;
		break;
	}
case 0x98 : {
		resu3r8(3, registerCalls::B);
		cycles = 8;
		break;
	}
case 0x99 : {
		resu3r8(3, registerCalls::C);
		cycles = 8;
		break;
	}
case 0x9A : {
		resu3r8(3, registerCalls::D);
		cycles = 8;
		break;
	}
case 0x9B : {
		resu3r8(3, registerCalls::E);
		cycles = 8;
		break;
	}
case 0x9C : {
		resu3r8(3, registerCalls::H);
		cycles = 8;
		break;
	}
case 0x9D : {
		resu3r8(3, registerCalls::L);
		cycles = 8;
		break;
	}
case 0x9E : {
		resu3hl(3);
		cycles = 16;
		break;
	}
case 0x9F : {
		resu3r8(3, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xA0 : {
		resu3r8(4, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xA1 : {
		resu3r8(4, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xA2 : {
		resu3r8(4, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xA3 : {
		resu3r8(4, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xA4 : {
		resu3r8(4, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xA5 : {
		resu3r8(4, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xA6 : {
		resu3hl(4);
		cycles = 16;
		break;
	}
case 0xA7 : {
		resu3r8(4, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xA8 : {
		resu3r8(5, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xA9 : {
		resu3r8(5, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xAA : {
		resu3r8(5, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xAB : {
		resu3r8(5, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xAC : {
		resu3r8(5, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xAD : {
		resu3r8(5, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xAE : {
		resu3hl(5);
		cycles = 16;
		break;
	}
case 0xAF : {
		resu3r8(5, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xB0 : {
		resu3r8(6, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xB1 : {
		resu3r8(6, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xB2 : {
		resu3r8(6, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xB3 : {
		resu3r8(6, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xB4 : {
		resu3r8(6, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xB5 : {
		resu3r8(6, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xB6 : {
		resu3hl(6);
		cycles = 16;
		break;
	}
case 0xB7 : {
		resu3r8(6, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xB8 : {
		resu3r8(7, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xB9 : {
		resu3r8(7, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xBA : {
		resu3r8(7, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xBB : {
		resu3r8(7, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xBC : {
		resu3r8(7, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xBD : {
		resu3r8(7, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xBE : {
		resu3hl(7);
		cycles = 16;
		break;
	}
case 0xBF : {
		resu3r8(7, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xC0 : {
		setu3r8(0,registerCalls::B);
		cycles = 8;
		break;
	}
case 0xC1 : {
		setu3r8(0, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xC2 : {
		setu3r8(0, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xC3 : {
		setu3r8(0, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xC4 : {
		setu3r8(0, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xC5 : {
		setu3r8(0, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xC6 : {
		setu3hl(0);
		cycles = 16;
		break;
	}
case 0xC7 : {
		setu3r8(0, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xC8 : {
		setu3r8(1, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xC9 : {
		setu3r8(1, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xCA : {
		setu3r8(1, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xCB : {
		setu3r8(1, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xCC : {
		setu3r8(1, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xCD : {
		setu3r8(1, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xCE : {
		setu3hl(1);
		cycles = 16;
		break;
	}
case 0xCF : {
		setu3r8(1, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xD0 : {
		setu3r8(2, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xD1 : {
		setu3r8(2, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xD2 : {
		setu3r8(2, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xD3 : {
		setu3r8(2, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xD4 : {
		setu3r8(2, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xD5 : {
		setu3r8(2, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xD6 : {
		setu3hl(2);
		cycles = 16;
		break;
	}
case 0xD7 : {
		setu3r8(2, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xD8 : {
		setu3r8(3, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xD9 : {
		setu3r8(3, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xDA : {
		setu3r8(3, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xDB : {
		setu3r8(3, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xDC : {
		setu3r8(3, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xDD : {
		setu3r8(3, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xDE : {
		setu3hl(3);
		cycles = 16;
		break;
	}
case 0xDF : {
		setu3r8(3, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xE0 : {
		setu3r8(4, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xE1 : {
		setu3r8(4, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xE2 : {
		setu3r8(4, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xE3 : {
		setu3r8(4, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xE4 : {
		setu3r8(4, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xE5 : {
		setu3r8(4, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xE6 : {
		setu3hl(4);
		cycles = 16;
		break;
	}
case 0xE7 : {
		setu3r8(4, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xE8 : {
		setu3r8(5, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xE9 : {
		setu3r8(5, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xEA : {
		setu3r8(5, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xEB : {
		setu3r8(5, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xEC : {
		setu3r8(5, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xED : {
		setu3r8(5, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xEE : {
		setu3hl(5);
		cycles = 16;
		break;
	}
case 0xEF : {
		setu3r8(5, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xF0 : {
		setu3r8(6, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xF1 : {
		setu3r8(6, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xF2 : {
		setu3r8(6, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xF3 : {
		setu3r8(6, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xF4 : {
		setu3r8(6, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xF5 : {
		setu3r8(6, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xF6 : {
		setu3hl(6);
		cycles = 16;
		break;
	}
case 0xF7 : {
		setu3r8(6, registerCalls::A);
		cycles = 8;
		break;
	}
case 0xF8 : {
		setu3r8(7, registerCalls::B);
		cycles = 8;
		break;
	}
case 0xF9 : {
		setu3r8(7, registerCalls::C);
		cycles = 8;
		break;
	}
case 0xFA : {
		setu3r8(7, registerCalls::D);
		cycles = 8;
		break;
	}
case 0xFB : {
		setu3r8(7, registerCalls::E);
		cycles = 8;
		break;
	}
case 0xFC : {
		setu3r8(7, registerCalls::H);
		cycles = 8;
		break;
	}
case 0xFD : {
		setu3r8(7, registerCalls::L);
		cycles = 8;
		break;
	}
case 0xFE : {
		setu3hl(7);
		cycles = 16;
		break;
	}
case 0xFF : {
		setu3r8(7, registerCalls::A);
		cycles = 8;
		break;
	}
	}

	advance_cycles(cycles);
	return program_counter+1;
}
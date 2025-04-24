#include "CPU.h"
#include "helpers.h"

uint16_t CPU::unprefixedCodes(uint16_t program_counter) {
	if (debug) {
		printf("PC: %s ", to_string(program_counter));
	}
	uint8_t code = address_space.read(program_counter);
	uint8_t cycles = 0;
	bool jumped = false;

	switch (code) {
	case 0x00 : {
		nop();
		cycles = 4;
		break;
	}
	case 0x01 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2,byte1);
		ldr16n16(registerCalls::BC, n16);
		cycles = 12;
		break;
	}
	case 0x02 : {
		ldr16a(registerCalls::BC);
		cycles = 8;
		break;
	}
	case 0x03 : {
		incr16(registerCalls::BC);
		cycles = 8;
		break;
	}
	case 0x04 : {
		incr8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x05 : {
		decr8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x06 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::B, byte1);
		cycles = 8;
		break;
	}
	case 0x07 : {
		rlca();
		cycles = 4;
		break;
	}
	case 0x08 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		ldn16sp(n16);
		cycles = 20;
		break;
	}
	case 0x09 : {
		addhlr16(registerCalls::BC);
		cycles = 8;
		break;
	}
	case 0x0A : {
		ldar16(registerCalls::BC);
		cycles = 8;
		break;
	}
	case 0x0B : {
		decr16(registerCalls::BC);
		cycles = 8;
		break;
	}
	case 0x0C : {
		incr8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x0D : {
		decr8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x0E : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::C, byte1);
		cycles = 8;
		break;
	}
	case 0x0F : {
		rrca();
		cycles = 4;
		break;
	}
	case 0x10 : {
		//supposed to take n8???
		stop();
		cycles = 4;
		break;
	}
	case 0x11 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		ldr16n16(registerCalls::DE, n16);
		cycles = 12;
		break;
	}
	case 0x12 : {
		ldr16a(registerCalls::DE);
		cycles = 8;
		break;
	}
	case 0x13 : {
		incr16(registerCalls::DE);
		cycles = 8;
		break;
	}
	case 0x14 : {
		incr8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x15 : {
		decr8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x16 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::D,byte1);
		cycles = 8;
		break;
	}
	case 0x17 : {
		rla();
		cycles = 4;
		break;
	}
	case 0x18 : {
		uint8_t byte1 = address_space.read(++program_counter);
		program_counter = jre8(byte1,program_counter,&jumped);
		if (jumped) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0x19 : {
		addhlr16(registerCalls::DE);
		cycles = 8;
		break;
	}
	case 0x1A : {
		ldar16(registerCalls::DE);
		cycles = 8;
		break;
	}
	case 0x1B : {
		decr16(registerCalls::DE);
		cycles = 8;
		break;
	}
	case 0x1C : {
		incr8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x1D : {
		decr8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x1E : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::E, byte1);
		cycles = 8;
		break;
	}
	case 0x1F : {
		rra();
		cycles = 4;
		break;
	}
	case 0x20 : {
		uint8_t byte1 = address_space.read(++program_counter);
		program_counter = jrcce8(Cond::nz,byte1,program_counter, &jumped);
		if (jumped) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0x21 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		ldr16n16(registerCalls::HL, n16);
		cycles = 12;
		break;
	}
	case 0x22 : {
		ldhlia();
		cycles = 8;
		break;
	}
	case 0x23 : {
		incr16(registerCalls::HL);
		cycles = 8;
		break;
	}
	case 0x24 : {
		incr8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x25 : {
		decr8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x26 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::H, byte1);
		cycles = 8;
		break;
	}
	case 0x27 : {
		daa();
		cycles = 4;
		break;
	}
	case 0x28 : {
		uint8_t byte1 = address_space.read(++program_counter);
		program_counter = jrcce8(Cond::z,byte1,program_counter, &jumped);
		if (jumped) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0x29 : {
		addhlr16(registerCalls::HL);
		cycles = 8;
		break;
	}
	case 0x2A : {
		ldahli();
		cycles = 8;
		break;
	}
	case 0x2B : {
		decr16(registerCalls::HL);
		cycles = 8;
		break;
	}
	case 0x2C : {
		incr8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x2D : {
		decr8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x2E : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::L, byte1);
		cycles = 8;
		break;
	}
	case 0x2F : {
		cpl();
		cycles = 4;
		break;
	}
	case 0x30 : {
		uint8_t byte1 = address_space.read(++program_counter);
		program_counter = jrcce8(Cond::nc, byte1, program_counter, &jumped);
		if (jumped) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0x31 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		ldspn16(n16);
		cycles = 12;
		break;
	}
	case 0x32 : {
		ldhlda();
		cycles = 8;
		break;
	}
	case 0x33 : {
		incsp();
		cycles = 8;
		break;
	}
	case 0x34 : {
		inchl();
		cycles = 12;
		break;
	}
	case 0x35 : {
		dechl();
		cycles = 12;
		break;
	}
	case 0x36 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldhln8(byte1);
		cycles = 12;
		break;
	}
	case 0x37 : {
		scf();
		cycles = 4;
		break;
	}
	case 0x38 : {
		uint8_t byte1 = address_space.read(++program_counter);
		program_counter = jrcce8(Cond::c, byte1, program_counter, &jumped);
		if (jumped) {
			cycles = 12;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0x39 : {
		addhlsp();
		cycles = 8;
		break;
	}
	case 0x3A : {
		ldahld();
		cycles = 8;
		break;
	}
	case 0x3B : {
		decsp();
		cycles = 8;
		break;
	}
	case 0x3C : {
		incr8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x3D : {
		decr8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x3E : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::A, byte1);
		cycles = 8;
		break;
	}
	case 0x3F : {
		ccf();
		cycles = 4;
		break;
	}
	case 0x40 : {
		ldr8r8(registerCalls::B, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x41 : {
		ldr8r8(registerCalls::B, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x42 : {
		ldr8r8(registerCalls::B, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x43 : {
		ldr8r8(registerCalls::B, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x44 : {
		ldr8r8(registerCalls::B, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x45 : {
		ldr8r8(registerCalls::B, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x46 : {
		ldr8hl(registerCalls::B);
		cycles = 8;
		break;
	}
	case 0x47 : {
		ldr8r8(registerCalls::B, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x48 : {
		ldr8r8(registerCalls::C, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x49 : {
		ldr8r8(registerCalls::C, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x4A : {
		ldr8r8(registerCalls::C, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x4B : {
		ldr8r8(registerCalls::C, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x4C : {
		ldr8r8(registerCalls::C, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x4D : {
		ldr8r8(registerCalls::C, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x4E : {
		ldr8hl(registerCalls::C);
		cycles = 8;
		break;
	}
	case 0x4F : {
		ldr8r8(registerCalls::C, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x50 : {
		ldr8r8(registerCalls::D, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x51 : {
		ldr8r8(registerCalls::D, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x52 : {
		ldr8r8(registerCalls::D, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x53 : {
		ldr8r8(registerCalls::D, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x54 : {
		ldr8r8(registerCalls::D, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x55 : {
		ldr8r8(registerCalls::D, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x56 : {
		ldr8hl(registerCalls::D);
		cycles = 8;
		break;
	}
	case 0x57 : {
		ldr8r8(registerCalls::D, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x58 : {
		ldr8r8(registerCalls::E, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x59 : {
		ldr8r8(registerCalls::E, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x5A : {
		ldr8r8(registerCalls::E, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x5B : {
		ldr8r8(registerCalls::E, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x5C : {
		ldr8r8(registerCalls::E, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x5D : {
		ldr8r8(registerCalls::E, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x5E : {
		ldr8hl(registerCalls::E);
		cycles = 8;
		break;
	}
	case 0x5F : {
		ldr8r8(registerCalls::E, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x60 : {
		ldr8r8(registerCalls::H, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x61 : {
		ldr8r8(registerCalls::H, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x62 : {
		ldr8r8(registerCalls::H, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x63 : {
		ldr8r8(registerCalls::H, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x64 : {
		ldr8r8(registerCalls::H, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x65 : {
		ldr8r8(registerCalls::H, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x66 : {
		ldr8hl(registerCalls::H);
		cycles = 8;
		break;
	}
	case 0x67 : {
		ldr8r8(registerCalls::H, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x68 : {
		ldr8r8(registerCalls::L, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x69 : {
		ldr8r8(registerCalls::L, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x6A : {
		ldr8r8(registerCalls::L, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x6B : {
		ldr8r8(registerCalls::L, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x6C : {
		ldr8r8(registerCalls::L, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x6D : {
		ldr8r8(registerCalls::L, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x6E : {
		ldr8hl(registerCalls::L);
		cycles = 8;
		break;
	}
	case 0x6F : {
		ldr8r8(registerCalls::L, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x70 : {
		ldhlr8(registerCalls::B);
		cycles = 8;
		break;
	}
	case 0x71 : {
		ldhlr8(registerCalls::C);
		cycles = 8;
		break;
	}
	case 0x72 : {
		ldhlr8(registerCalls::D);
		cycles = 8;
		break;
	}
	case 0x73 : {
		ldhlr8(registerCalls::E);
		cycles = 8;
		break;
	}
	case 0x74 : {
		ldhlr8(registerCalls::H);
		cycles = 8;
		break;
	}
	case 0x75 : {
		ldhlr8(registerCalls::L);
		cycles = 8;
		break;
	}
	case 0x76 : {
		halt();
		cycles = 4;
		break;
	}
	case 0x77 : {
		ldhlr8(registerCalls::A);
		cycles = 8;
		break;
	}
	case 0x78 : {
		ldr8r8(registerCalls::A, registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x79 : {
		ldr8r8(registerCalls::A, registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x7A : {
		ldr8r8(registerCalls::A, registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x7B : {
		ldr8r8(registerCalls::A, registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x7C : {
		ldr8r8(registerCalls::A, registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x7D : {
		ldr8r8(registerCalls::A, registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x7E : {
		ldr8hl(registerCalls::A);
		cycles = 8;
		break;
	}
	case 0x7F : {
		ldr8r8(registerCalls::A, registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x80 : {
		addar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x81 : {
		addar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x82 : {
		addar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x83 : {
		addar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x84 : {
		addar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x85 : {
		addar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x86 : {
		addahl();
		cycles = 8;
		break;
	}
	case 0x87 : {
		addar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x88 : {
		adcar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x89 : {
		adcar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x8A : {
		adcar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x8B : {
		adcar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x8C : {
		adcar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x8D : {
		adcar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x8E : {
		adcahl();
		cycles = 8;
		break;
	}
	case 0x8F : {
		adcar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x90 : {
		subar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x91 : {
		subar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x92 : {
		subar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x93 : {
		subar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x94 : {
		subar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x95 : {
		subar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x96 : {
		subahl();
		cycles = 8;
		break;
	}
	case 0x97 : {
		subar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0x98 : {
		sbcar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0x99 : {
		sbcar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0x9A : {
		sbcar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0x9B : {
		sbcar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0x9C : {
		sbcar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0x9D : {
		sbcar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0x9E : {
		sbcahl();
		cycles = 8;
		break;
	}
	case 0x9F : {
		sbcar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0xA0 : {
		andar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0xA1 : {
		andar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0xA2 : {
		andar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0xA3 : {
		andar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0xA4 : {
		andar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0xA5 : {
		andar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0xA6 : {
		andahl();
		cycles = 8;
		break;
	}
	case 0xA7 : {
		andar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0xA8 : {
		xorar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0xA9 : {
		xorar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0xAA : {
		xorar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0xAB : {
		xorar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0xAC : {
		xorar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0xAD : {
		xorar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0xAE : {
		xorahl();
		cycles = 8;
		break;
	}
	case 0xAF : {
		xorar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0xB0 : {
		orar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0xB1 : {
		orar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0xB2 : {
		orar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0xB3 : {
		orar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0xB4 : {
		orar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0xB5 : {
		orar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0xB6 : {
		orahl();
		cycles = 8;
		break;
	}
	case 0xB7 : {
		orar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0xB8 : {
		cpar8(registerCalls::B);
		cycles = 4;
		break;
	}
	case 0xB9 : {
		cpar8(registerCalls::C);
		cycles = 4;
		break;
	}
	case 0xBA : {
		cpar8(registerCalls::D);
		cycles = 4;
		break;
	}
	case 0xBB : {
		cpar8(registerCalls::E);
		cycles = 4;
		break;
	}
	case 0xBC : {
		cpar8(registerCalls::H);
		cycles = 4;
		break;
	}
	case 0xBD : {
		cpar8(registerCalls::L);
		cycles = 4;
		break;
	}
	case 0xBE : {
		cpahl();
		cycles = 8;
		break;
	}
	case 0xBF : {
		cpar8(registerCalls::A);
		cycles = 4;
		break;
	}
	case 0xC0 : {
		program_counter = retcc(Cond::nz, program_counter,&jumped);
		if (jumped) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0xC1 : {
		popr16(registerCalls::BC);
		cycles = 12;
		break;
	}
	case 0xC2 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = jpccn16(Cond::nz, n16, program_counter, &jumped);
		if (jumped) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xC3 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = jpn16(n16);
		cycles = 16;
		break;
	}
	case 0xC4 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = callccn16(Cond::nz, n16,program_counter,&jumped);
		if (jumped) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xC5 : {
		pushr16(registerCalls::BC);
		cycles = 16;
		break;
	}
	case 0xC6 : {
		uint8_t byte1 = address_space.read(++program_counter);
		addan8(byte1);
		cycles = 8;
		break;
	}
	case 0xC7 : {
		program_counter = rstvec(0x00,program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xC8 : {
		program_counter = retcc(Cond::z,program_counter,&jumped);
		if (jumped) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0xC9 : {
		program_counter = ret();
		cycles = 16;
		break;
	}
	case 0xCA : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = jpccn16(Cond::z, n16,program_counter, &jumped);
		if (jumped) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xCC : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = callccn16(Cond::z, n16, program_counter, &jumped);
		if (jumped) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xCD : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = calln16(n16,program_counter);
		cycles = 24;
		break;
	}
	case 0xCE : {
		uint8_t byte1 = address_space.read(++program_counter);
		adcan8(byte1);
		cycles = 8;
		break;
	}
	case 0xCF : {
		program_counter = rstvec(0x08,program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xD0 : {
		program_counter = retcc(Cond::nc,program_counter,&jumped);
		if (jumped) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0xD1 : {
		popr16(registerCalls::DE);
		cycles = 12;
		break;
	}
	case 0xD2 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = jpccn16(Cond::nc, n16,program_counter,&jumped);
		if (jumped) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xD4 : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = callccn16(Cond::nc, n16,program_counter,&jumped);
		if (jumped) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xD5 : {
		pushr16(registerCalls::DE);
		cycles = 16;
		break;
	}
	case 0xD6 : {
		uint8_t byte1 = address_space.read(++program_counter);
		suban8(byte1);
		cycles = 8;
		break;
	}
	case 0xD7 : {
		program_counter = rstvec(0x10, program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xD8 : {
		program_counter = retcc(Cond::c,program_counter,&jumped);
		if (jumped) {
			cycles = 20;
		}
		else {
			cycles = 8;
		}
		break;
	}
	case 0xD9 : {
		program_counter = reti();
		cycles = 16;
		break;
	}
	case 0xDA : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter  = jpccn16(Cond::c, n16,program_counter,&jumped);
		if (jumped) {
			cycles = 16;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xDC : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		program_counter = callccn16(Cond::c, n16, program_counter,&jumped);
		if (jumped) {
			cycles = 24;
		}
		else {
			cycles = 12;
		}
		break;
	}
	case 0xDE : {
		uint8_t byte1 = address_space.read(++program_counter);
		sbcn8(byte1);
		cycles = 8;
		break;
	}
	case 0xDF : {
		program_counter = rstvec(0x18, program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xE0 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldhn8a(byte1);
		cycles = 12;
		break;
	}
	case 0xE1 : {
		popr16(registerCalls::HL);
		cycles = 12;
		break;
	}
	case 0xE2 : {
		ldhca();
		cycles = 8;
		break;
	}
	case 0xE5 : {
		pushr16(registerCalls::HL);
		cycles = 16;
		break;
	}
	case 0xE6 : {
		uint8_t byte1 = address_space.read(++program_counter);
		andan8(byte1);
		cycles = 8;
		break;
	}
	case 0xE7 : {
		program_counter = rstvec(0x20, program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xE8 : {
		uint8_t byte1 = address_space.read(++program_counter);
		addspe8(byte1);
		cycles = 16;
		break;
	}
	case 0xE9 : {
		program_counter = jphl();
		cycles = 4;
		break;
	}
	case 0xEA : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		ldn16a(n16);
		cycles = 16;
		break;
	}
	case 0xEE : {
		uint8_t byte1 = address_space.read(++program_counter);
		xoran8(byte1);
		cycles = 8;
		break;
	}
	case 0xEF : {
		program_counter = rstvec(0x28, program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xF0 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldhan8(byte1);
		cycles = 12;
		break;
	}
	case 0xF1 : {
		popaf();
		cycles = 12;
		break;
	}
	case 0xF2 : {
		ldhac();
		cycles = 8;
		break;
	}
	case 0xF3 : {
		di();
		cycles = 4;
		break;
	}
	case 0xF5 : {
		pushaf();
		cycles = 16;
		break;
	}
	case 0xF6 : {
		uint8_t byte1 = address_space.read(++program_counter);
		oran8(byte1);
		cycles = 8;
		break;
	}
	case 0xF7 : {
		program_counter = rstvec(0x30, program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	case 0xF8 : {
		uint8_t byte1 = address_space.read(++program_counter);
		ldhlspe8(byte1);
		cycles = 12;
		break;
	}
	case 0xF9 : {
		ldsphl();
		cycles = 8;
		break;
	}
	case 0xFA : {
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2, byte1);
		ldan16(n16);
		cycles = 16;
		break;
	}
	case 0xFB : {
		ei();
		cycles = 4;
		break;
	}
	case 0xFE : {
		uint8_t byte1 = address_space.read(++program_counter);
		cpan8(byte1);
		cycles = 8;
		break;
	}
	case 0xFF : {
		program_counter = rstvec(0x38, program_counter);
		jumped = true;
		cycles = 16;
		break;
	}
	default:
		printf("invalid opcode?\n");
	}

	if (debug && jumped) {
		printf("jumped!\n");
	}

		
	
	advance_cycles(cycles);
	//printf("pc is now %s\n",to_string((uint16_t)(program_counter+1)));
	return program_counter+1;
}
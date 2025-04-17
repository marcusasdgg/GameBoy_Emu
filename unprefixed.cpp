#include "CPU.h"
#include "helpers.h"

uint16_t CPU::unprefixedCodes(uint8_t program_counter) {
	uint8_t code = address_space.read(program_counter);
	uint8_t cycles = 0;
	switch (cycles) {
	case 0x00:
		nop();
		cycles = 4;
	case 0x01:
		uint8_t byte1 = address_space.read(++program_counter);
		uint8_t byte2 = address_space.read(++program_counter);
		uint16_t n16 = join_bytes(byte2,byte1);
		ldr16n16(registerCalls::BC, n16);
		cycles = 12;
	case 0x02:
		ldr16a(registerCalls::BC);
		cycles = 8;
	case 0x03:
		incr16(registerCalls::BC);
		cycles = 8;
	case 0x04:
		incr8(registerCalls::B);
		cycles = 4;
	case 0x05:
		decr8(registerCalls::B);
		cycles = 4;
	case 0x06:
		uint8_t byte1 = address_space.read(++program_counter);
		ldr8n8(registerCalls::B, byte1);
		cycles = 8;
	case 0x07:
		RLCA();
		cycles = 4;
	case 0x08:
		LDa16 SP();
		cycles = 20;
	case 0x09:
		ADDHL BC();
		cycles = 8;
	case 0x0A:
		LDA BC();
		cycles = 8;
	case 0x0B:
		DECBC();
		cycles = 8;
	case 0x0C:
		INCC();
		cycles = 4;
	case 0x0D:
		DECC();
		cycles = 4;
	case 0x0E:
		LDC n8();
		cycles = 8;
	case 0x0F:
		RRCA();
		cycles = 4;
	case 0x10:
		STOPn8();
		cycles = 4;
	case 0x11:
		LDDE n16();
		cycles = 12;
	case 0x12:
		LDDE A();
		cycles = 8;
	case 0x13:
		INCDE();
		cycles = 8;
	case 0x14:
		INCD();
		cycles = 4;
	case 0x15:
		DECD();
		cycles = 4;
	case 0x16:
		LDD n8();
		cycles = 8;
	case 0x17:
		RLA();
		cycles = 4;
	case 0x18:
		JRe8();
		cycles = 12;
	case 0x19:
		ADDHL DE();
		cycles = 8;
	case 0x1A:
		LDA DE();
		cycles = 8;
	case 0x1B:
		DECDE();
		cycles = 8;
	case 0x1C:
		INCE();
		cycles = 4;
	case 0x1D:
		DECE();
		cycles = 4;
	case 0x1E:
		LDE n8();
		cycles = 8;
	case 0x1F:
		RRA();
		cycles = 4;
	case 0x20:
		JRNZ e8();
		cycles = 12;
	case 0x21:
		LDHL n16();
		cycles = 12;
	case 0x22:
		LDHL A();
		cycles = 8;
	case 0x23:
		INCHL();
		cycles = 8;
	case 0x24:
		INCH();
		cycles = 4;
	case 0x25:
		DECH();
		cycles = 4;
	case 0x26:
		LDH n8();
		cycles = 8;
	case 0x27:
		DAA();
		cycles = 4;
	case 0x28:
		JRZ e8();
		cycles = 12;
	case 0x29:
		ADDHL HL();
		cycles = 8;
	case 0x2A:
		LDA HL();
		cycles = 8;
	case 0x2B:
		DECHL();
		cycles = 8;
	case 0x2C:
		INCL();
		cycles = 4;
	case 0x2D:
		DECL();
		cycles = 4;
	case 0x2E:
		LDL n8();
		cycles = 8;
	case 0x2F:
		CPL();
		cycles = 4;
	case 0x30:
		JRNC e8();
		cycles = 12;
	case 0x31:
		LDSP n16();
		cycles = 12;
	case 0x32:
		LDHL A();
		cycles = 8;
	case 0x33:
		INCSP();
		cycles = 8;
	case 0x34:
		INCHL();
		cycles = 12;
	case 0x35:
		DECHL();
		cycles = 12;
	case 0x36:
		LDHL n8();
		cycles = 12;
	case 0x37:
		SCF();
		cycles = 4;
	case 0x38:
		JRC e8();
		cycles = 12;
	case 0x39:
		ADDHL SP();
		cycles = 8;
	case 0x3A:
		LDA HL();
		cycles = 8;
	case 0x3B:
		DECSP();
		cycles = 8;
	case 0x3C:
		INCA();
		cycles = 4;
	case 0x3D:
		DECA();
		cycles = 4;
	case 0x3E:
		LDA n8();
		cycles = 8;
	case 0x3F:
		CCF();
		cycles = 4;
	case 0x40:
		LDB B();
		cycles = 4;
	case 0x41:
		LDB C();
		cycles = 4;
	case 0x42:
		LDB D();
		cycles = 4;
	case 0x43:
		LDB E();
		cycles = 4;
	case 0x44:
		LDB H();
		cycles = 4;
	case 0x45:
		LDB L();
		cycles = 4;
	case 0x46:
		LDB HL();
		cycles = 8;
	case 0x47:
		LDB A();
		cycles = 4;
	case 0x48:
		LDC B();
		cycles = 4;
	case 0x49:
		LDC C();
		cycles = 4;
	case 0x4A:
		LDC D();
		cycles = 4;
	case 0x4B:
		LDC E();
		cycles = 4;
	case 0x4C:
		LDC H();
		cycles = 4;
	case 0x4D:
		LDC L();
		cycles = 4;
	case 0x4E:
		LDC HL();
		cycles = 8;
	case 0x4F:
		LDC A();
		cycles = 4;
	case 0x50:
		LDD B();
		cycles = 4;
	case 0x51:
		LDD C();
		cycles = 4;
	case 0x52:
		LDD D();
		cycles = 4;
	case 0x53:
		LDD E();
		cycles = 4;
	case 0x54:
		LDD H();
		cycles = 4;
	case 0x55:
		LDD L();
		cycles = 4;
	case 0x56:
		LDD HL();
		cycles = 8;
	case 0x57:
		LDD A();
		cycles = 4;
	case 0x58:
		LDE B();
		cycles = 4;
	case 0x59:
		LDE C();
		cycles = 4;
	case 0x5A:
		LDE D();
		cycles = 4;
	case 0x5B:
		LDE E();
		cycles = 4;
	case 0x5C:
		LDE H();
		cycles = 4;
	case 0x5D:
		LDE L();
		cycles = 4;
	case 0x5E:
		LDE HL();
		cycles = 8;
	case 0x5F:
		LDE A();
		cycles = 4;
	case 0x60:
		LDH B();
		cycles = 4;
	case 0x61:
		LDH C();
		cycles = 4;
	case 0x62:
		LDH D();
		cycles = 4;
	case 0x63:
		LDH E();
		cycles = 4;
	case 0x64:
		LDH H();
		cycles = 4;
	case 0x65:
		LDH L();
		cycles = 4;
	case 0x66:
		LDH HL();
		cycles = 8;
	case 0x67:
		LDH A();
		cycles = 4;
	case 0x68:
		LDL B();
		cycles = 4;
	case 0x69:
		LDL C();
		cycles = 4;
	case 0x6A:
		LDL D();
		cycles = 4;
	case 0x6B:
		LDL E();
		cycles = 4;
	case 0x6C:
		LDL H();
		cycles = 4;
	case 0x6D:
		LDL L();
		cycles = 4;
	case 0x6E:
		LDL HL();
		cycles = 8;
	case 0x6F:
		LDL A();
		cycles = 4;
	case 0x70:
		LDHL B();
		cycles = 8;
	case 0x71:
		LDHL C();
		cycles = 8;
	case 0x72:
		LDHL D();
		cycles = 8;
	case 0x73:
		LDHL E();
		cycles = 8;
	case 0x74:
		LDHL H();
		cycles = 8;
	case 0x75:
		LDHL L();
		cycles = 8;
	case 0x76:
		HALT();
		cycles = 4;
	case 0x77:
		LDHL A();
		cycles = 8;
	case 0x78:
		LDA B();
		cycles = 4;
	case 0x79:
		LDA C();
		cycles = 4;
	case 0x7A:
		LDA D();
		cycles = 4;
	case 0x7B:
		LDA E();
		cycles = 4;
	case 0x7C:
		LDA H();
		cycles = 4;
	case 0x7D:
		LDA L();
		cycles = 4;
	case 0x7E:
		LDA HL();
		cycles = 8;
	case 0x7F:
		LDA A();
		cycles = 4;
	case 0x80:
		ADDA B();
		cycles = 4;
	case 0x81:
		ADDA C();
		cycles = 4;
	case 0x82:
		ADDA D();
		cycles = 4;
	case 0x83:
		ADDA E();
		cycles = 4;
	case 0x84:
		ADDA H();
		cycles = 4;
	case 0x85:
		ADDA L();
		cycles = 4;
	case 0x86:
		ADDA HL();
		cycles = 8;
	case 0x87:
		ADDA A();
		cycles = 4;
	case 0x88:
		ADCA B();
		cycles = 4;
	case 0x89:
		ADCA C();
		cycles = 4;
	case 0x8A:
		ADCA D();
		cycles = 4;
	case 0x8B:
		ADCA E();
		cycles = 4;
	case 0x8C:
		ADCA H();
		cycles = 4;
	case 0x8D:
		ADCA L();
		cycles = 4;
	case 0x8E:
		ADCA HL();
		cycles = 8;
	case 0x8F:
		ADCA A();
		cycles = 4;
	case 0x90:
		SUBA B();
		cycles = 4;
	case 0x91:
		SUBA C();
		cycles = 4;
	case 0x92:
		SUBA D();
		cycles = 4;
	case 0x93:
		SUBA E();
		cycles = 4;
	case 0x94:
		SUBA H();
		cycles = 4;
	case 0x95:
		SUBA L();
		cycles = 4;
	case 0x96:
		SUBA HL();
		cycles = 8;
	case 0x97:
		SUBA A();
		cycles = 4;
	case 0x98:
		SBCA B();
		cycles = 4;
	case 0x99:
		SBCA C();
		cycles = 4;
	case 0x9A:
		SBCA D();
		cycles = 4;
	case 0x9B:
		SBCA E();
		cycles = 4;
	case 0x9C:
		SBCA H();
		cycles = 4;
	case 0x9D:
		SBCA L();
		cycles = 4;
	case 0x9E:
		SBCA HL();
		cycles = 8;
	case 0x9F:
		SBCA A();
		cycles = 4;
	case 0xA0:
		ANDA B();
		cycles = 4;
	case 0xA1:
		ANDA C();
		cycles = 4;
	case 0xA2:
		ANDA D();
		cycles = 4;
	case 0xA3:
		ANDA E();
		cycles = 4;
	case 0xA4:
		ANDA H();
		cycles = 4;
	case 0xA5:
		ANDA L();
		cycles = 4;
	case 0xA6:
		ANDA HL();
		cycles = 8;
	case 0xA7:
		ANDA A();
		cycles = 4;
	case 0xA8:
		XORA B();
		cycles = 4;
	case 0xA9:
		XORA C();
		cycles = 4;
	case 0xAA:
		XORA D();
		cycles = 4;
	case 0xAB:
		XORA E();
		cycles = 4;
	case 0xAC:
		XORA H();
		cycles = 4;
	case 0xAD:
		XORA L();
		cycles = 4;
	case 0xAE:
		XORA HL();
		cycles = 8;
	case 0xAF:
		XORA A();
		cycles = 4;
	case 0xB0:
		ORA B();
		cycles = 4;
	case 0xB1:
		ORA C();
		cycles = 4;
	case 0xB2:
		ORA D();
		cycles = 4;
	case 0xB3:
		ORA E();
		cycles = 4;
	case 0xB4:
		ORA H();
		cycles = 4;
	case 0xB5:
		ORA L();
		cycles = 4;
	case 0xB6:
		ORA HL();
		cycles = 8;
	case 0xB7:
		ORA A();
		cycles = 4;
	case 0xB8:
		CPA B();
		cycles = 4;
	case 0xB9:
		CPA C();
		cycles = 4;
	case 0xBA:
		CPA D();
		cycles = 4;
	case 0xBB:
		CPA E();
		cycles = 4;
	case 0xBC:
		CPA H();
		cycles = 4;
	case 0xBD:
		CPA L();
		cycles = 4;
	case 0xBE:
		CPA HL();
		cycles = 8;
	case 0xBF:
		CPA A();
		cycles = 4;
	case 0xC0:
		RETNZ();
		cycles = 20;
	case 0xC1:
		POPBC();
		cycles = 12;
	case 0xC2:
		JPNZ a16();
		cycles = 16;
	case 0xC3:
		JPa16();
		cycles = 16;
	case 0xC4:
		CALLNZ a16();
		cycles = 24;
	case 0xC5:
		PUSHBC();
		cycles = 16;
	case 0xC6:
		ADDA n8();
		cycles = 8;
	case 0xC7:
		RST$00();
		cycles = 16;
	case 0xC8:
		RETZ();
		cycles = 20;
	case 0xC9:
		RET();
		cycles = 16;
	case 0xCA:
		JPZ a16();
		cycles = 16;
	case 0xCB:
		PREFIX();
		cycles = 4;
	case 0xCC:
		CALLZ a16();
		cycles = 24;
	case 0xCD:
		CALLa16();
		cycles = 24;
	case 0xCE:
		ADCA n8();
		cycles = 8;
	case 0xCF:
		RST$08();
		cycles = 16;
	case 0xD0:
		RETNC();
		cycles = 20;
	case 0xD1:
		POPDE();
		cycles = 12;
	case 0xD2:
		JPNC a16();
		cycles = 16;
	case 0xD4:
		CALLNC a16();
		cycles = 24;
	case 0xD5:
		PUSHDE();
		cycles = 16;
	case 0xD6:
		SUBA n8();
		cycles = 8;
	case 0xD7:
		RST$10();
		cycles = 16;
	case 0xD8:
		RETC();
		cycles = 20;
	case 0xD9:
		RETI();
		cycles = 16;
	case 0xDA:
		JPC a16();
		cycles = 16;
	case 0xDC:
		CALLC a16();
		cycles = 24;
	case 0xDE:
		SBCA n8();
		cycles = 8;
	case 0xDF:
		RST$18();
		cycles = 16;
	case 0xE0:
		LDHa8 A();
		cycles = 12;
	case 0xE1:
		POPHL();
		cycles = 12;
	case 0xE2:
		LDHC A();
		cycles = 8;
	case 0xE5:
		PUSHHL();
		cycles = 16;
	case 0xE6:
		ANDA n8();
		cycles = 8;
	case 0xE7:
		RST$20();
		cycles = 16;
	case 0xE8:
		ADDSP e8();
		cycles = 16;
	case 0xE9:
		JPHL();
		cycles = 4;
	case 0xEA:
		LDa16 A();
		cycles = 16;
	case 0xEE:
		XORA n8();
		cycles = 8;
	case 0xEF:
		RST$28();
		cycles = 16;
	case 0xF0:
		LDHA a8();
		cycles = 12;
	case 0xF1:
		POPAF();
		cycles = 12;
	case 0xF2:
		LDHA C();
		cycles = 8;
	case 0xF3:
		DI();
		cycles = 4;
	case 0xF5:
		PUSHAF();
		cycles = 16;
	case 0xF6:
		ORA n8();
		cycles = 8;
	case 0xF7:
		RST$30();
		cycles = 16;
	case 0xF8:
		LDHL SP e8();
		cycles = 12;
	case 0xF9:
		LDSP HL();
		cycles = 8;
	case 0xFA:
		LDA a16();
		cycles = 16;
	case 0xFB:
		EI();
		cycles = 4;
	case 0xFE:
		CPA n8();
		cycles = 8;
	case 0xFF:
		RST$38();
		cycles = 16;
	}


	block_cycle_n(cycles);
}
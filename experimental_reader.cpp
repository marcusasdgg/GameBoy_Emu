#include "CPU.h"

if (byte == 0x00)
NOP();
if (byte == 0x01)
LDBCn16();
if (byte == 0x02)
LDBCA();
if (byte == 0x03)
INCBC();
if (byte == 0x04)
INCB();
if (byte == 0x05)
DECB();
if (byte == 0x06)
LDBn8();
if (byte == 0x07)
RLCA();
if (byte == 0x08)
LDa16SP();
if (byte == 0x09)
ADDHLBC();
if (byte == 0x0A)
LDABC();
if (byte == 0x0B)
DECBC();
if (byte == 0x0C)
INCC();
if (byte == 0x0D)
DECC();
if (byte == 0x0E)
LDCn8();
if (byte == 0x0F)
RRCA();
if (byte == 0x10)
STOPn8();
if (byte == 0x11)
LDDEn16();
if (byte == 0x12)
LDDEA();
if (byte == 0x13)
INCDE();
if (byte == 0x14)
INCD();
if (byte == 0x15)
DECD();
if (byte == 0x16)
LDDn8();
if (byte == 0x17)
RLA();
if (byte == 0x18)
JRe8();
if (byte == 0x19)
ADDHLDE();
if (byte == 0x1A)
LDADE();
if (byte == 0x1B)
DECDE();
if (byte == 0x1C)
INCE();
if (byte == 0x1D)
DECE();
if (byte == 0x1E)
LDEn8();
if (byte == 0x1F)
RRA();
if (byte == 0x20)
JRNZe8();
if (byte == 0x21)
LDHLn16();
if (byte == 0x22)
LDHLA();
if (byte == 0x23)
INCHL();
if (byte == 0x24)
INCH();
if (byte == 0x25)
DECH();
if (byte == 0x26)
LDHn8();
if (byte == 0x27)
DAA();
if (byte == 0x28)
JRZe8();
if (byte == 0x29)
ADDHLHL();
if (byte == 0x2A)
LDAHL();
if (byte == 0x2B)
DECHL();
if (byte == 0x2C)
INCL();
if (byte == 0x2D)
DECL();
if (byte == 0x2E)
LDLn8();
if (byte == 0x2F)
CPL();
if (byte == 0x30)
JRNCe8();
if (byte == 0x31)
LDSPn16();
if (byte == 0x32)
LDHLA();
if (byte == 0x33)
INCSP();
if (byte == 0x34)
INCHL();
if (byte == 0x35)
DECHL();
if (byte == 0x36)
LDHLn8();
if (byte == 0x37)
SCF();
if (byte == 0x38)
JRCe8();
if (byte == 0x39)
ADDHLSP();
if (byte == 0x3A)
LDAHL();
if (byte == 0x3B)
DECSP();
if (byte == 0x3C)
INCA();
if (byte == 0x3D)
DECA();
if (byte == 0x3E)
LDAn8();
if (byte == 0x3F)
CCF();
if (byte == 0x40)
LDBB();
if (byte == 0x41)
LDBC();
if (byte == 0x42)
LDBD();
if (byte == 0x43)
LDBE();
if (byte == 0x44)
LDBH();
if (byte == 0x45)
LDBL();
if (byte == 0x46)
LDBHL();
if (byte == 0x47)
LDBA();
if (byte == 0x48)
LDCB();
if (byte == 0x49)
LDCC();
if (byte == 0x4A)
LDCD();
if (byte == 0x4B)
LDCE();
if (byte == 0x4C)
LDCH();
if (byte == 0x4D)
LDCL();
if (byte == 0x4E)
LDCHL();
if (byte == 0x4F)
LDCA();
if (byte == 0x50)
LDDB();
if (byte == 0x51)
LDDC();
if (byte == 0x52)
LDDD();
if (byte == 0x53)
LDDE();
if (byte == 0x54)
LDDH();
if (byte == 0x55)
LDDL();
if (byte == 0x56)
LDDHL();
if (byte == 0x57)
LDDA();
if (byte == 0x58)
LDEB();
if (byte == 0x59)
LDEC();
if (byte == 0x5A)
LDED();
if (byte == 0x5B)
LDEE();
if (byte == 0x5C)
LDEH();
if (byte == 0x5D)
LDEL();
if (byte == 0x5E)
LDEHL();
if (byte == 0x5F)
LDEA();
if (byte == 0x60)
LDHB();
if (byte == 0x61)
LDHC();
if (byte == 0x62)
LDHD();
if (byte == 0x63)
LDHE();
if (byte == 0x64)
LDHH();
if (byte == 0x65)
LDHL();
if (byte == 0x66)
LDHHL();
if (byte == 0x67)
LDHA();
if (byte == 0x68)
LDLB();
if (byte == 0x69)
LDLC();
if (byte == 0x6A)
LDLD();
if (byte == 0x6B)
LDLE();
if (byte == 0x6C)
LDLH();
if (byte == 0x6D)
LDLL();
if (byte == 0x6E)
LDLHL();
if (byte == 0x6F)
LDLA();
if (byte == 0x70)
LDHLB();
if (byte == 0x71)
LDHLC();
if (byte == 0x72)
LDHLD();
if (byte == 0x73)
LDHLE();
if (byte == 0x74)
LDHLH();
if (byte == 0x75)
LDHLL();
if (byte == 0x76)
HALT();
if (byte == 0x77)
LDHLA();
if (byte == 0x78)
LDAB();
if (byte == 0x79)
LDAC();
if (byte == 0x7A)
LDAD();
if (byte == 0x7B)
LDAE();
if (byte == 0x7C)
LDAH();
if (byte == 0x7D)
LDAL();
if (byte == 0x7E)
LDAHL();
if (byte == 0x7F)
LDAA();
if (byte == 0x80)
ADDAB();
if (byte == 0x81)
ADDAC();
if (byte == 0x82)
ADDAD();
if (byte == 0x83)
ADDAE();
if (byte == 0x84)
ADDAH();
if (byte == 0x85)
ADDAL();
if (byte == 0x86)
ADDAHL();
if (byte == 0x87)
ADDAA();
if (byte == 0x88)
ADCAB();
if (byte == 0x89)
ADCAC();
if (byte == 0x8A)
ADCAD();
if (byte == 0x8B)
ADCAE();
if (byte == 0x8C)
ADCAH();
if (byte == 0x8D)
ADCAL();
if (byte == 0x8E)
ADCAHL();
if (byte == 0x8F)
ADCAA();
if (byte == 0x90)
SUBAB();
if (byte == 0x91)
SUBAC();
if (byte == 0x92)
SUBAD();
if (byte == 0x93)
SUBAE();
if (byte == 0x94)
SUBAH();
if (byte == 0x95)
SUBAL();
if (byte == 0x96)
SUBAHL();
if (byte == 0x97)
SUBAA();
if (byte == 0x98)
SBCAB();
if (byte == 0x99)
SBCAC();
if (byte == 0x9A)
SBCAD();
if (byte == 0x9B)
SBCAE();
if (byte == 0x9C)
SBCAH();
if (byte == 0x9D)
SBCAL();
if (byte == 0x9E)
SBCAHL();
if (byte == 0x9F)
SBCAA();
if (byte == 0xA0)
ANDAB();
if (byte == 0xA1)
ANDAC();
if (byte == 0xA2)
ANDAD();
if (byte == 0xA3)
ANDAE();
if (byte == 0xA4)
ANDAH();
if (byte == 0xA5)
ANDAL();
if (byte == 0xA6)
ANDAHL();
if (byte == 0xA7)
ANDAA();
if (byte == 0xA8)
XORAB();
if (byte == 0xA9)
XORAC();
if (byte == 0xAA)
XORAD();
if (byte == 0xAB)
XORAE();
if (byte == 0xAC)
XORAH();
if (byte == 0xAD)
XORAL();
if (byte == 0xAE)
XORAHL();
if (byte == 0xAF)
XORAA();
if (byte == 0xB0)
ORAB();
if (byte == 0xB1)
ORAC();
if (byte == 0xB2)
ORAD();
if (byte == 0xB3)
ORAE();
if (byte == 0xB4)
ORAH();
if (byte == 0xB5)
ORAL();
if (byte == 0xB6)
ORAHL();
if (byte == 0xB7)
ORAA();
if (byte == 0xB8)
CPAB();
if (byte == 0xB9)
CPAC();
if (byte == 0xBA)
CPAD();
if (byte == 0xBB)
CPAE();
if (byte == 0xBC)
CPAH();
if (byte == 0xBD)
CPAL();
if (byte == 0xBE)
CPAHL();
if (byte == 0xBF)
CPAA();
if (byte == 0xC0)
RETNZ();
if (byte == 0xC1)
POPBC();
if (byte == 0xC2)
JPNZa16();
if (byte == 0xC3)
JPa16();
if (byte == 0xC4)
CALLNZa16();
if (byte == 0xC5)
PUSHBC();
if (byte == 0xC6)
ADDAn8();
if (byte == 0xC7)
RST$00();
if (byte == 0xC8)
RETZ();
if (byte == 0xC9)
RET();
if (byte == 0xCA)
JPZa16();
if (byte == 0xCB)
PREFIX();
if (byte == 0xCC)
CALLZa16();
if (byte == 0xCD)
CALLa16();
if (byte == 0xCE)
ADCAn8();
if (byte == 0xCF)
RST$08();
if (byte == 0xD0)
RETNC();
if (byte == 0xD1)
POPDE();
if (byte == 0xD2)
JPNCa16();
if (byte == 0xD4)
CALLNCa16();
if (byte == 0xD5)
PUSHDE();
if (byte == 0xD6)
SUBAn8();
if (byte == 0xD7)
RST$10();
if (byte == 0xD8)
RETC();
if (byte == 0xD9)
RETI();
if (byte == 0xDA)
JPCa16();
if (byte == 0xDC)
CALLCa16();
if (byte == 0xDE)
SBCAn8();
if (byte == 0xDF)
RST$18();
if (byte == 0xE0)
LDHa8A();
if (byte == 0xE1)
POPHL();
if (byte == 0xE2)
LDHCA();
if (byte == 0xE5)
PUSHHL();
if (byte == 0xE6)
ANDAn8();
if (byte == 0xE7)
RST$20();
if (byte == 0xE8)
ADDSPe8();
if (byte == 0xE9)
JPHL();
if (byte == 0xEA)
LDa16A();
if (byte == 0xEE)
XORAn8();
if (byte == 0xEF)
RST$28();
if (byte == 0xF0)
LDHAa8();
if (byte == 0xF1)
POPAF();
if (byte == 0xF2)
LDHAC();
if (byte == 0xF3)
DI();
if (byte == 0xF5)
PUSHAF();
if (byte == 0xF6)
ORAn8();
if (byte == 0xF7)
RST$30();
if (byte == 0xF8)
LDHLSPe8();
if (byte == 0xF9)
LDSPHL();
if (byte == 0xFA)
LDAa16();
if (byte == 0xFB)
EI();
if (byte == 0xFE)
CPAn8();
if (byte == 0xFF)
RST$38();

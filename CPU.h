#pragma once

#include<cstdint>
#include<vector>
#include "AddressSpace.h"
typedef unsigned char byte;
typedef uint16_t address;

//valid 16bit pairs

enum registerCalls {
	A,B,C,D,E,H,L,AF,BC,DE,HL,SP,PC
};


class CPU{
	private:
// registers

		//general registers
		byte A;
		byte B;
		byte C;
		byte D;
		byte E;
		byte H;
		byte L;
		
		//flag register
		byte F;

		// stack pointer
		uint16_t SP;

		// program counter
		uint16_t PC;

		AddressSpace& address_space;

// interrupt_check
		bool interrupt_received();

//operations
		void store_in_register(registerCalls, uint16_t n16);

		void store_in_register(registerCalls, uint8_t n8);

		uint16_t retrieve_register_16(registerCalls);

		uint8_t retrieve_register_8(registerCalls);

		//ld operations
		void ldr8r8(registerCalls a, registerCalls b);
		void ldr8n8(registerCalls, uint8_t);
		void ldr16n16(registerCalls, uint16_t);
		void ldhlr8(registerCalls);
		void ldhln8(uint8_t);
		void ldr8hl(registerCalls);
		void ldr16a(registerCalls);
		void ldn16a(uint16_t);
		void ldhn16a(uint16_t);
		void ldhac();
		void ldhlia();
		void ldhlda();
		void ldahld();
		void ldahli();
		void ldspn16(uint16_t);
		void ldn16sp(uint16_t);
		void ldhlspe8(int16_t e8);
		void ldsphl();

		void NOP();

		// arithmetic operations
		//add
		void add(registerCalls, registerCalls);
		void add(registerCalls, int);
		void add_indirect(registerCalls, registerCalls);
		void adc(registerCalls, registerCalls);
		void adc_indirect(registerCalls, registerCalls);
		void addc(registerCalls, int);

		//sub
		void sub(registerCalls, registerCalls);
		void sub(registerCalls, int);
		void sub_indirect(registerCalls, registerCalls);
		void subc(registerCalls, registerCalls);
		void subc_indirect(registerCalls, registerCalls);
		void subc(registerCalls, int);

		// and
		void andd(registerCalls, registerCalls);
		void andd(registerCalls, bool);
		void andd_indirect(registerCalls, registerCalls);

		// OR
		void orr(registerCalls, registerCalls);
		void orr(registerCalls, bool);
		void orr_indirect(registerCalls, registerCalls);

		// XOR
		void xorr(registerCalls, registerCalls);
		void xorr(registerCalls, bool);
		void xorr_indirect(registerCalls, registerCalls);

		// CP
		void cp(registerCalls, registerCalls);
		void cp(registerCalls, bool);
		void cp_indirect(registerCalls, registerCalls);

		//increment
		void inc(registerCalls);
		void inc_indirect(registerCalls);

		// decrmenet
		void dec(registerCalls);
		void dec_indirect(registerCalls);

		void print_registers();



	public:

		CPU(AddressSpace& addressSpace);

		void inititialise();

		void execute(std::vector<byte>);

		void arithmetic_test();


};


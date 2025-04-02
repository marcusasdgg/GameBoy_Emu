
#pragma once

#include<cstdint>
#include<vector>
#include "AddressSpace.h"
typedef unsigned char byte;
typedef uint16_t address;

//valid 16bit pairs

enum registerCalls {
	A,B,C,D,E,H,L,AF,BC,DE,HL,SP,PC, HLI, HLD
};

enum Cond {
	nz,z,nc,c
};

static const registerCalls val_to_r8[] = {registerCalls::B , registerCalls::C, registerCalls::D, registerCalls::E, registerCalls::H, registerCalls::HL, registerCalls::A};
static const registerCalls val_to_r16[] = { BC, DE, HL, SP };
static const registerCalls val_to_r16stk[] = { BC,DE,HL,AF };
static const registerCalls val_to_r16mem[] = { BC,DE,HLI, HLD };
static const Cond val_to_cond[] = { nz,z,nc,c };

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

// flag operations
		static bool full_carry(uint8_t, uint8_t);
		static bool full_carry(uint16_t, uint16_t);
		bool get_carry();
		void set_carry(bool);
		bool borrow(uint8_t temp, uint8_t temp2);

		static bool half_carry(uint8_t, uint8_t);
		static bool half_carry(uint16_t, uint16_t);
		bool get_half_carry();
		void set_half_carry(bool);
		static bool half_borrow(uint8_t temp, uint8_t temp2);

		bool get_zero();
		void set_zero(bool);

		void set_n(bool);
		bool get_n();
		

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
		void ldhca();
		void ldar16(registerCalls a);
		void ldan16(uint16_t val);
		void ldhan16(uint16_t val);
		void ldhac();
		void ldhlia();
		void ldhlda();
		void ldahld();
		void ldahli();
		void ldspn16(uint16_t);
		void ldn16sp(uint16_t);
		void ldhlspe8(int16_t e8);
		void ldsphl();


		// arithmetic operations
		//add
		void adcar8(registerCalls a);
		void adcahl();
		void adcan8(uint8_t val);
		void addar8(registerCalls a);
		void addahl();
		void addan8(uint8_t val);
		void addhlr16(registerCalls a);
		void addhlsp();
		void addspe8(int8_t e8);


		//sub
		void subar8(registerCalls a);
		void subahl();
		void suban8(uint8_t val);
		void sbcar8(registerCalls a);
		void sbcahl();
		void sbcn8(uint8_t);

		//increment
		void incr8(registerCalls);
		void inchl();
		void incr16(registerCalls);
		void incsp();

		// decrmenet
		void decr8(registerCalls);
		void dechl();
		void decr16(registerCalls);
		void decsp();

		//bitwise logic
		void andar8(registerCalls);
		void andahl();
		void andan8(uint8_t);
		void orar8(registerCalls);
		void orahl();
		void oran8(uint8_t);
		void xorar8(registerCalls);
		void xorahl();
		void xoran8(uint8_t);

		//bitflag instructions
		void bitu3r8(uint8_t bit, registerCalls a);
		void bitu3hl(uint8_t bit);
		void resu3r8(uint8_t, registerCalls);
		void resu3hl(uint8_t);
		void setu3r8(uint8_t, registerCalls);
		void setu3hl(uint8_t);

		// bit shift instructions
		void rlr8(registerCalls);
		void rlhl();
		void rla();
		void rlcr8(registerCalls);
		void rlchl();
		void rlca();
		void rrr8(registerCalls);
		void rrhl();
		void rra();
		void rrcr8(registerCalls);
		void rrchl();
		void rrca();
		void slar8(registerCalls);
		void slahl();
		void srar8(registerCalls);
		void srahl();
		void srlr8(registerCalls);
		void srlhl();
		void swapr8(registerCalls);
		void swaphl();

		//jump and subroutine
		void calln16();
		void callccn16();
		void jphl();
		void jpn16();
		void jpccn16();
		void jrn16();
		void jrccn16();
		void retcc();
		void ret();
		void reti();
		void rstvec();

		//carry flag instructions
		void ccf();
		void scf();

		//stack manipulation instructions
		void popaf();
		void popr16();
		void pushaf();
		void pushr16();

		//interrupts
		void di();
		void ei();
		void halt();

		//misce  instructions

		void daa();
		void nop();
		void stop();

		void print_registers();

		uint16_t handle_block_0(uint16_t program_counter);
		uint16_t handle_block_1(uint16_t program_counter);
		uint16_t handle_block_2(uint16_t program_counter);
		uint16_t handle_block_3(uint16_t program_counter);

		//decodes the instruction
		uint16_t decode_execute_instruction(uint16_t program_counter);

		//given ranges starting from bit start to end inclusive get the number start is the right most least significant left is most signficiant.
		uint8_t get_bit_range(uint8_t value, uint8_t start, uint8_t end);

	public:

		CPU(AddressSpace& addressSpace);

		void inititialise();

		void execute(uint16_t start_ptr = 0);

		
		// aint gonna work with jumps
		void execute_single(std::vector<byte>);

		void arithmetic_test();

};


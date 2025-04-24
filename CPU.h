
#pragma once

#include<cstdint>
#include<vector>
#include "AddressSpace.h"
#include "Clock.h"
typedef unsigned char byte;
typedef uint16_t address;

#define debug false

//valid 16bit pairs

enum registerCalls {
	A,B,C,D,E,H,L,AF,BC,DE,HL,SP,PC, HLI, HLD
};

enum Interrupt {
	JOYPAD,Serial, Timer,LCD,VBLANK
};

enum Cond {
	nz,z,nc,c
};

static const registerCalls val_to_r8[] = {registerCalls::B , registerCalls::C, registerCalls::D, registerCalls::E, registerCalls::H, registerCalls::HL, registerCalls::A};
static const registerCalls val_to_r16[] = { BC, DE, HL, SP };
static const registerCalls val_to_r16stk[] = { BC,DE,HL,AF };
static const registerCalls val_to_r16mem[] = { BC,DE,HLI, HLD };
static const Cond val_to_cond[] = { nz,z,nc,c };

//worry about cpu registers and jumps, maybe make it so that the block handles calculate the neccesary jump of bytes
//cpu should be running in a separate thread-loop
class CPU{
	private:
// registers
		bool tempfuse = false;
		//general registers
		byte A = 0;
		byte B = 0;
		byte C = 0;
		byte D = 0;
		byte E = 0;
		byte H = 0;
		byte L = 0;

		//
		bool IME = false;
		
		//flag register
		byte F = 0;

		// stack pointer
		uint16_t SP = 0;

		// program counter
		uint16_t PC = 0;

		AddressSpace& address_space;

		Clock& clock;


// interrupt_check
		
		void interrupt_handler();

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
		void ldhlspe8(int8_t e8);
		void ldsphl();
		void ldhan8(uint8_t val);
		void ldhn8a(uint8_t val);
		//23

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
		//9

		//sub
		void subar8(registerCalls a);
		void subahl();
		void suban8(uint8_t val);
		void sbcar8(registerCalls a);
		void sbcahl();
		void sbcn8(uint8_t);
		//6

		//increment
		void incr8(registerCalls);
		void inchl();
		void incr16(registerCalls, bool print = true);
		void incsp();
		//4

		// decrmenet
		void decr8(registerCalls);
		void dechl();
		void decr16(registerCalls,bool print = true);
		void decsp();
		//4

		// compare functions
		void cpar8(registerCalls a);
		void cpahl();
		void cpan8(uint8_t val);
		void cpl();
		//4

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
		//9

		//bitflag instructions
		void bitu3r8(uint8_t bit, registerCalls a);
		void bitu3hl(uint8_t bit);
		void resu3r8(uint8_t, registerCalls);
		void resu3hl(uint8_t);
		void setu3r8(uint8_t, registerCalls);
		void setu3hl(uint8_t);
		//6

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
		//20

		//jump and subroutine
		uint16_t calln16(uint16_t val, uint16_t program_counter);
		uint16_t callccn16(Cond c, uint16_t val, uint16_t current_PC, bool* jumped);
		uint16_t jphl();
		uint16_t jpn16(uint16_t val);
		uint16_t jpccn16(Cond c, uint16_t val, uint16_t current_PC, bool* jumped);
		uint16_t jre8(uint8_t, uint16_t current_PC, bool* jumped);
		uint16_t jrcce8(Cond c, uint8_t val, uint16_t current_PC, bool* jumped);
		uint16_t retcc(Cond c, uint16_t current_PC, bool* jumped);
		uint16_t ret(bool print = true);
		uint16_t reti();
		//10

		// if rst vector i.e 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, and 0x38 jump
		uint16_t rstvec(uint8_t val, uint16_t current_PC);

		//carry flag instructions
		void ccf();
		void scf();

		//stack manipulation instructions
		void popaf();
		void popr16(registerCalls a);
		uint16_t popn16(bool print = false);
		void pushaf();
		void pushn8(uint8_t val, bool print = false);
		void pushn16(uint16_t val, bool print = false);
		void pushr16(registerCalls a);

		//interrupts
		void di();
		void ei();
		void halt();

		//misce  instructions

		void daa();
		void nop();
		void stop();

	
		//interrupt stuff
		uint8_t get_interrupt_count();
		Interrupt get_highest_priority_interrupt();

		//decodes the instruction and executes it fetch can be in there somewhere maybe in execute loop
		uint16_t decode_execute_instruction(uint16_t program_counter);

		//given ranges starting from bit start to end inclusive get the number start is the right most least significant left is most signficiant.
		uint8_t get_bit_range(uint8_t value, uint8_t start, uint8_t end);

		// get bit n of byte
		bool get_bit(uint8_t byte, uint8_t bit);

		//given a cond return if the cond applies 
		bool check_cond(Cond c);


		//step through the cpu
		

		void advance_cycles(uint8_t cycles);
	public:

		CPU(AddressSpace& addressSpace, Clock& clock);

		void inititialise();
		void print_registers();

		void print_format();

		void execute(uint16_t start_ptr = 0);
		void stop_execute();
		// aint gonna work with jumps
		void execute_single(std::vector<byte>);

		void step();

		void arithmetic_test();
		uint16_t prefixedCodes(uint16_t program_counter);
		uint16_t unprefixedCodes(uint16_t program_counter);
};


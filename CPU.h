
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

		void NOP();

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

		void print_registers();



	public:

		CPU(AddressSpace& addressSpace);

		void inititialise();

		void execute(uint16_t start_ptr);

		void arithmetic_test();


};


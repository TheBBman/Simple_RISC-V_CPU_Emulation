#include <iostream>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tuple>
using namespace std;


class instruction {
public:
	bitset<32> instr;				// instruction
	instruction(bitset<32> fetch); 	// constructor
};

class CPU {
private:
	int dmemory[4096]; 	// data memory byte addressable in little endian fashion;
	unsigned long PC; 	// pc 
	bool branch;
	bool mem_read;
	bool mem_write;
	bool mem_to_reg;
	bool reg_write;
	bool ALU_src;
	int ALU_control;
	int reg[32];	// Registers

public:
	CPU();
	unsigned long readPC();
	bitset<32> Fetch(bitset<8> *instmem);
	bool Decode(instruction* instr);
	int generate_immediate(instruction* instr);
	tuple<int, int, int> get_registers(instruction* instr);
	int Execute(bitset<32> in1, bitset<32> in2);
};


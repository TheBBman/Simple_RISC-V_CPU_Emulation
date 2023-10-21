#include "CPU.h"
#include <tuple>

instruction::instruction(bitset<32> fetch)
{
	instr = fetch;
}

CPU::CPU()
{
	PC = 0; //set PC to 0
	ALU_control = 0;
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}
	branch = false;
	mem_read = false;
	mem_write = false;
	mem_to_reg = false;
	reg_write = false;
	ALU_src = false;
	ALU_control = 0;
}

unsigned long CPU::readPC()
{
	return PC;
}

bitset<32> CPU::Fetch(bitset<8> *instmem) {
	bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));  //get 32 bit instruction
	PC += 4;	//increment PC
	return instr;
}

// Decode instruction and set correct operation flags for execution
bool CPU::Decode(instruction* curr)
{
	if (!curr->instr[0]) {
		return true;
	}
	// JALR 
	if (curr->instr[2]) {
		branch = true;
		reg_write = true;
		cout << "JALR" << endl;
	}
	// Branch
	else if (curr->instr[6]) {
		branch = true;
		cout << "Branch" << endl;
	}
	// Load
	else if (!curr->instr[4] && !curr->instr[5]) {
		mem_read = true;
		mem_to_reg = true;
		reg_write = true;
		ALU_src = true;
		cout << "Load" << endl;
	} 
	// Store
	else if (!curr->instr[4] && curr->instr[5]) {
		mem_write = true;
		reg_write = true;
		ALU_src = true;
		cout << "Store" << endl;
	}
	// Arithmetic
	else if (curr->instr[5]) {

		// All arithmetic instructions store result in regiter
		reg_write = true;

		// SRA
		if (curr->instr[12]) {
			ALU_control = 1;
			cout << "SRA" << endl;
		}
		// XOR
		else if (curr->instr[14]) {
			ALU_control = 2;
			cout << "XOR" << endl;
		}
		// Sub
		else if (curr->instr[30]) {
			ALU_control = 3;
			cout << "Sub" << endl;
		}
		// Add
		else {
			cout << "Add" << endl;
		}
	}
	// Immediate related
	else {

		// Write to register, immediate -> ALU
		reg_write = true;
		ALU_src = true;

		// AndI
		if (curr->instr[12]) {
			ALU_control = 4;
			cout << "AndI" << endl;
		}
		// AddI
		else {
			cout << "AddI" << endl;
		}
	}
	return false;
}

// Generate immediate (int form) to be used in execution based on instruction type
int CPU::generate_immediate(instruction* curr)
{
	int result;

	// SB (BLT)
	if (!curr->instr[2] && curr->instr[6]) {
		bitset<13> immediate;
		immediate[0] = 0;
		for (int i = 1; i < 5; i++)
			immediate[i] = curr->instr[i+7];
		for (int i = 5; i < 11; i++) 
			immediate[i] = curr->instr[i+20];
		immediate[11] = curr->instr[7];
		immediate[12] = curr->instr[31];

		// Convert to signed integer, use sign + magnitude
		if (!immediate[12]) 
			result = (int)immediate.to_ulong();
		else {
			immediate.flip();
			result = (int) -1*(immediate.to_ulong() + 1);
		}
	} 
	// S (SW)
	else if (!curr->instr[4] && curr->instr[5] && !curr->instr[6]) {
		bitset<12> immediate;
		for (int i = 0; i < 5; i++)
			immediate[i] = curr->instr[i+7];
		for (int i = 5; i < 12; i++)
			immediate[i] = curr->instr[i+20];

		// Convert to signed integer, use sign + magnitude
		if (!immediate[11]) 
			result = (int)immediate.to_ulong();
		else {
			immediate.flip();
			result = (int) -1*(immediate.to_ulong() + 1);
		}
	}
	// I or R, just return I immediate since unused in R
	else {
		bitset<12> immediate;
		for (int i = 0; i < 12; i++)
			immediate[i] = curr->instr[i+20];

		// Convert to signed integer, use sign + magnitude
		if (!immediate[11]) 
			result = (int)immediate.to_ulong();
		else {
			immediate.flip();
			result = (int) -1*(immediate.to_ulong() + 1);
		}
	}

    return result;
}

tuple<int, int, int> CPU::get_registers(instruction *curr)
{
	bitset<5> rs1, rs2, rd;
	for (int i = 0; i < 5; i++) 
		rd[i] = curr->instr[i+7];
	for (int i = 0; i < 5; i++) 
		rs1[i] = curr->instr[i+15];
	for (int i = 0; i < 5; i++) 
		rs2[i] = curr->instr[i+20];
    return make_tuple((int)rs1.to_ulong(), (int)rs2.to_ulong(), (int)rd.to_ulong());
}

int CPU::Execute(bitset<32> in1, bitset<32> in2)
{
    return 0;
}
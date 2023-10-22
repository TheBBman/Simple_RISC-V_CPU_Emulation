#include "CPU.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<fstream>
#include <sstream>
using namespace std;

int main(int argc, char* argv[])
{
	bitset<8> instMem[4096];

	if (argc < 2) {
		cout << "No file name entered. Exiting...";
		return -1;
	}

	ifstream infile(argv[1]); //open the file
	if (!(infile.is_open() && infile.good())) {
		cout<<"Error opening file\n";
		return 0; 
	}

	string line; 
	int i = 0;
	while (infile) {
			infile>>line;
			stringstream line2(line);
			int x; 
			line2>>x;
			instMem[i] = bitset<8>(x);
			i++;
		}
	int maxPC = i; 

	CPU myCPU;  // call the approriate constructor here to initialize the processor...  

	bitset<32> curr;
	instruction instr = instruction(curr);
	bool done = false;
	int immediate, rs1, rs2, rd, ALU_result, read_data;

	while (!done) // processor's main loop. Each iteration is equal to one clock cycle.  
	{
		// Fetch
		curr = myCPU.Fetch(instMem); // fetching the instruction
		instr = instruction(curr);

		// Decode 
		done = myCPU.Decode(&instr);
		if (done) // break from loop so stats are not mistakenly updated
			break;

		immediate = myCPU.generate_immediate(&instr);
		auto [rs1, rs2, rd] = myCPU.get_registers(&instr);

		// Execute 
		ALU_result = myCPU.Execute(rs1, rs2, immediate);

		// Memory access
		read_data = myCPU.Memory(ALU_result, rs2);

		// Update registers
		myCPU.Writeback(read_data, ALU_result, rd);

		// Update PC
		myCPU.update_PC(rs1, immediate);
	}
	auto [a0, a1] = myCPU.get_results();
	// print the results (you should replace a0 and a1 with your own variables that point to a0 and a1)
	cout << "(" << a0 << "," << a1 << ")" << endl;
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "mu-mips.h"

/***************************************************************/
/* Print out a list of commands available                                                                  */
/***************************************************************/
void help() {        
	printf("------------------------------------------------------------------\n\n");
	printf("\t**********MU-MIPS Help MENU**********\n\n");
	printf("sim\t-- simulate program to completion \n");
	printf("run <n>\t-- simulate program for <n> cycles\n");
        printf("forwarding\t-- toggle forwarding enabled\n");
	printf("rdump\t-- dump register values\n");
	printf("reset\t-- clears all registers/memory and re-loads the program\n");
	printf("input <reg> <val>\t-- set GPR <reg> to <val>\n");
	printf("mdump <start> <stop>\t-- dump memory from <start> to <stop> address\n");
	printf("high <val>\t-- set the HI register to <val>\n");
	printf("low <val>\t-- set the LO register to <val>\n");
	printf("print\t-- print the program loaded into memory\n");
	printf("show\t-- print the current content of the pipeline registers\n");
	printf("?\t-- display help menu\n");
	printf("quit\t-- exit the simulator\n\n");
	printf("------------------------------------------------------------------\n\n");
}

void print_instruction(uint32_t addr, int type){
	uint32_t instruction, opcode, function, rs, rt, rd, sa, immediate, target;
	
        if(type == 0){
            instruction = mem_read_32(addr);
        }else{
            instruction = addr;
        }
	opcode = (instruction & 0xFC000000) >> 26;
	function = instruction & 0x0000003F;
	rs = (instruction & 0x03E00000) >> 21;
	rt = (instruction & 0x001F0000) >> 16;
	rd = (instruction & 0x0000F800) >> 11;
	sa = (instruction & 0x000007C0) >> 6;
	immediate = instruction & 0x0000FFFF;
	target = instruction & 0x03FFFFFF;
	
	if(opcode == 0x00){
		/*R format instructions here*/
		
		switch(function){
			case 0x00:
                                if(instruction != 0){
                                    printf("SLL $r%u, $r%u, 0x%x\n", rd, rt, sa);
                                }else{
                                    printf("\n");
                                    
                                }
                                break;
			case 0x02:
				printf("SRL $r%u, $r%u, 0x%x\n", rd, rt, sa);
				break;
			case 0x03:
				printf("SRA $r%u, $r%u, 0x%x\n", rd, rt, sa);
				break;
			case 0x08:
				printf("JR $r%u\n", rs);
				break;
			case 0x09:
				if(rd == 31){
					printf("JALR $r%u\n", rs);
				}
				else{
					printf("JALR $r%u, $r%u\n", rd, rs);
				}
				break;
			case 0x0C:
				printf("SYSCALL\n");
				break;
			case 0x10:
				printf("MFHI $r%u\n", rd);
				break;
			case 0x11:
				printf("MTHI $r%u\n", rs);
				break;
			case 0x12:
				printf("MFLO $r%u\n", rd);
				break;
			case 0x13:
				printf("MTLO $r%u\n", rs);
				break;
			case 0x18:
				printf("MULT $r%u, $r%u\n", rs, rt);
				break;
			case 0x19:
				printf("MULTU $r%u, $r%u\n", rs, rt);
				break;
			case 0x1A:
				printf("DIV $r%u, $r%u\n", rs, rt);
				break;
			case 0x1B:
				printf("DIVU $r%u, $r%u\n", rs, rt);
				break;
			case 0x20:
				printf("ADD $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x21:
				printf("ADDU $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x22:
				printf("SUB $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x23:
				printf("SUBU $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x24:
				printf("AND $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x25:
				printf("OR $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x26:
				printf("XOR $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x27:
				printf("NOR $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			case 0x2A:
				printf("SLT $r%u, $r%u, $r%u\n", rd, rs, rt);
				break;
			default:
				printf("Instruction is not implemented!\n");
				break;
		}
	}
	else{
		switch(opcode){
			case 0x01:
				if(rt == 0){
					printf("BLTZ $r%u, 0x%x\n", rs, immediate<<2);
				}
				else if(rt == 1){
					printf("BGEZ $r%u, 0x%x\n", rs, immediate<<2);
				}
				break;
			case 0x02:
				printf("J 0x%x\n", (addr & 0xF0000000) | (target<<2));
				break;
			case 0x03:
				printf("JAL 0x%x\n", (addr & 0xF0000000) | (target<<2));
				break;
			case 0x04:
				printf("BEQ $r%u, $r%u, 0x%x\n", rs, rt, immediate<<2);
				break;
			case 0x05:
				printf("BNE $r%u, $r%u, 0x%x\n", rs, rt, immediate<<2);
				break;
			case 0x06:
				printf("BLEZ $r%u, 0x%x\n", rs, immediate<<2);
				break;
			case 0x07:
				printf("BGTZ $r%u, 0x%x\n", rs, immediate<<2);
				break;
			case 0x08:
				printf("ADDI $r%u, $r%u, 0x%x\n", rt, rs, immediate);
				break;
			case 0x09:
				printf("ADDIU $r%u, $r%u, 0x%x\n", rt, rs, immediate);
				break;
			case 0x0A:
				printf("SLTI $r%u, $r%u, 0x%x\n", rt, rs, immediate);
				break;
			case 0x0C:
				printf("ANDI $r%u, $r%u, 0x%x\n", rt, rs, immediate);
				break;
			case 0x0D:
				printf("ORI $r%u, $r%u, 0x%x\n", rt, rs, immediate);
				break;
			case 0x0E:
				printf("XORI $r%u, $r%u, 0x%x\n", rt, rs, immediate);
				break;
			case 0x0F:
				printf("LUI $r%u, 0x%x\n", rt, immediate);
				break;
			case 0x20:
				printf("LB $r%u, 0x%x($r%u)\n", rt, immediate, rs);
				break;
			case 0x21:
				printf("LH $r%u, 0x%x($r%u)\n", rt, immediate, rs);
				break;
			case 0x23:
				printf("LW $r%u, 0x%x($r%u)\n", rt, immediate, rs);
				break;
			case 0x28:
				printf("SB $r%u, 0x%x($r%u)\n", rt, immediate, rs);
				break;
			case 0x29:
				printf("SH $r%u, 0x%x($r%u)\n", rt, immediate, rs);
				break;
			case 0x2B:
				printf("SW $r%u, 0x%x($r%u)\n", rt, immediate, rs);
				break;
			default:
				printf("Instruction is not implemented!\n");
				break;
		}
	}
}

/***************************************************************/
/* Read a 32-bit word from memory                                                                            */
/***************************************************************/
uint32_t mem_read_32(uint32_t address)
{
	int i;
	for (i = 0; i < NUM_MEM_REGION; i++) {
		if ( (address >= MEM_REGIONS[i].begin) &&  ( address <= MEM_REGIONS[i].end) ) {
			uint32_t offset = address - MEM_REGIONS[i].begin;
			return (MEM_REGIONS[i].mem[offset+3] << 24) |
					(MEM_REGIONS[i].mem[offset+2] << 16) |
					(MEM_REGIONS[i].mem[offset+1] <<  8) |
					(MEM_REGIONS[i].mem[offset+0] <<  0);
		}
	}
	return 0;
}

/***************************************************************/
/* Write a 32-bit word to memory                                                                                */
/***************************************************************/
void mem_write_32(uint32_t address, uint32_t value)
{
	int i;
	uint32_t offset;
	for (i = 0; i < NUM_MEM_REGION; i++) {
		if ( (address >= MEM_REGIONS[i].begin) && (address <= MEM_REGIONS[i].end) ) {
			offset = address - MEM_REGIONS[i].begin;

			MEM_REGIONS[i].mem[offset+3] = (value >> 24) & 0xFF;
			MEM_REGIONS[i].mem[offset+2] = (value >> 16) & 0xFF;
			MEM_REGIONS[i].mem[offset+1] = (value >>  8) & 0xFF;
			MEM_REGIONS[i].mem[offset+0] = (value >>  0) & 0xFF;
		}
	}
}

/***************************************************************/
/* Execute one cycle                                                                                                              */
/***************************************************************/
void cycle() {                                                
	handle_pipeline();
	CURRENT_STATE = NEXT_STATE;
	CYCLE_COUNT++;
}

/***************************************************************/
/* Simulate MIPS for n cycles                                                                                       */
/***************************************************************/
void run(int num_cycles) {                                      
	
	if (RUN_FLAG == FALSE) {
		printf("Simulation Stopped\n\n");
		return;
	}

	printf("Running simulator for %d cycles...\n\n", num_cycles);
	int i;
	for (i = 0; i < num_cycles; i++) {
		if (RUN_FLAG == FALSE) {
			printf("Simulation Stopped.\n\n");
			break;
		}
		cycle();
	}
}

/***************************************************************/
/* simulate to completion                                                                                               */
/***************************************************************/
void runAll() {                                                     
	if (RUN_FLAG == FALSE) {
		printf("Simulation Stopped.\n\n");
		return;
	}

	printf("Simulation Started...\n\n");
	while (RUN_FLAG){
		cycle();
	}
	printf("Simulation Finished.\n\n");
}

/***************************************************************/ 
/* Dump a word-aligned region of memory to the terminal                              */
/***************************************************************/
void mdump(uint32_t start, uint32_t stop) {          
	uint32_t address;

	printf("-------------------------------------------------------------\n");
	printf("Memory content [0x%08x..0x%08x] :\n", start, stop);
	printf("-------------------------------------------------------------\n");
	printf("\t[Address in Hex (Dec) ]\t[Value]\n");
	for (address = start; address <= stop; address += 4){
		printf("\t0x%08x (%d) :\t0x%08x\n", address, address, mem_read_32(address));
	}
	printf("\n");
}

/***************************************************************/
/* Dump current values of registers to the teminal                                              */   
/***************************************************************/
void rdump() {                               
	int i; 
	printf("-------------------------------------\n");
	printf("Dumping Register Content\n");
	printf("-------------------------------------\n");
	printf("# Instructions Executed\t: %u\n", INSTRUCTION_COUNT);
	printf("# Cycles Executed\t: %u\n", CYCLE_COUNT);
	printf("PC\t: 0x%08x\n", CURRENT_STATE.PC);
	printf("-------------------------------------\n");
	printf("[Register]\t[Value]\n");
	printf("-------------------------------------\n");
	for (i = 0; i < MIPS_REGS; i++){
		printf("[R%d]\t: 0x%08x\n", i, CURRENT_STATE.REGS[i]);
	}
	printf("-------------------------------------\n");
	printf("[HI]\t: 0x%08x\n", CURRENT_STATE.HI);
	printf("[LO]\t: 0x%08x\n", CURRENT_STATE.LO);
	printf("-------------------------------------\n");
}

/***************************************************************/
/* Read a command from standard input.                                                               */  
/***************************************************************/
void handle_command() {                         
	char buffer[20];
	uint32_t start, stop, cycles;
	uint32_t register_no;
	int register_value;
	int hi_reg_value, lo_reg_value;

	printf("MU-MIPS SIM:> ");

	if (scanf("%s", buffer) == EOF){
		exit(0);
	}

	switch(buffer[0]) {
		case 'S':
		case 's':
			if (buffer[1] == 'h' || buffer[1] == 'H'){
				show_pipeline();
			}else {
				runAll(); 
			}
			break;
		case 'M':
		case 'm':
			if (scanf("%x %x", &start, &stop) != 2){
				break;
			}
			mdump(start, stop);
			break;
		case '?':
			help();
			break;
		case 'Q':
		case 'q':
			printf("**************************\n");
			printf("Exiting MU-MIPS! Good Bye...\n");
			printf("**************************\n");
			exit(0);
		case 'R':
		case 'r':
			if (buffer[1] == 'd' || buffer[1] == 'D'){
				rdump();
			}else if(buffer[1] == 'e' || buffer[1] == 'E'){
				reset();
			}
			else {
				if (scanf("%d", &cycles) != 1) {
					break;
				}
				run(cycles);
			}
			break;
		case 'I':
		case 'i':
			if (scanf("%u %i", &register_no, &register_value) != 2){
				break;
			}
			CURRENT_STATE.REGS[register_no] = register_value;
			NEXT_STATE.REGS[register_no] = register_value;
			break;
		case 'H':
		case 'h':
			if (scanf("%i", &hi_reg_value) != 1){
				break;
			}
			CURRENT_STATE.HI = hi_reg_value; 
			NEXT_STATE.HI = hi_reg_value; 
			break;
		case 'L':
		case 'l':
			if (scanf("%i", &lo_reg_value) != 1){
				break;
			}
			CURRENT_STATE.LO = lo_reg_value;
			NEXT_STATE.LO = lo_reg_value;
			break;
		case 'P':
		case 'p':
			print_program(); 
			break;
                case 'f':
                case 'F':
                        if(ENABLE_FORWARDING == 0){
                            ENABLE_FORWARDING = 1;
                            printf("Forwarding ON\n");
                        }else{
                            ENABLE_FORWARDING = 0;
                            printf("Forwarding OFF\n");
                        }
                        break;
		default:
			printf("Invalid Command.\n");
			break;
	}
}

/***************************************************************/
/* reset registers/memory and reload program                                                    */
/***************************************************************/
void reset() {   
	int i;
	/*reset registers*/
	for (i = 0; i < MIPS_REGS; i++){
		CURRENT_STATE.REGS[i] = 0;
	}
	CURRENT_STATE.HI = 0;
	CURRENT_STATE.LO = 0;
	
	for (i = 0; i < NUM_MEM_REGION; i++) {
		uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
		memset(MEM_REGIONS[i].mem, 0, region_size);
	}
	
	IF_ID.PC = 0;
        IF_ID.IR = 0;
        IF_ID.A = 0;
        IF_ID.B = 0;
        IF_ID.imm = 0;
        IF_ID.ALUOutput = 0;
        IF_ID.LMD = 0;
        ID_EX.PC = 0;
        ID_EX.IR = 0;
        ID_EX.A = 0;
        ID_EX.B = 0;
        ID_EX.imm = 0;
        ID_EX.ALUOutput = 0;
        ID_EX.LMD = 0;
        EX_MEM.PC = 0;
        EX_MEM.IR = 0;
        EX_MEM.A = 0;
        EX_MEM.B = 0;
        EX_MEM.imm = 0;
        EX_MEM.ALUOutput = 0;
        EX_MEM.LMD = 0;
        EX_MEM.RegWrite = 0;
        MEM_WB.PC = 0;
        MEM_WB.IR = 0;
        MEM_WB.A = 0;
        MEM_WB.B = 0;
        MEM_WB.imm = 0;
        MEM_WB.ALUOutput = 0;
        MEM_WB.LMD = 0;
        MEM_WB.RegWrite = 0;
	STALL_FLAG = 0;
        
        
	/*load program*/
	load_program();
	
	/*reset PC*/
	INSTRUCTION_COUNT = 0;
        CYCLE_COUNT = 0;
	CURRENT_STATE.PC =  MEM_TEXT_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = TRUE;
}

/***************************************************************/
/* Allocate and set memory to zero                                                                            */
/***************************************************************/
void init_memory() {                                           
	int i;
	for (i = 0; i < NUM_MEM_REGION; i++) {
		uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
		MEM_REGIONS[i].mem = malloc(region_size);
		memset(MEM_REGIONS[i].mem, 0, region_size);
	}
}

/**************************************************************/
/* load program into memory                                                                                      */
/**************************************************************/
void load_program() {                   
	FILE * fp;
	int i, word;
	uint32_t address;

	/* Open program file. */
	fp = fopen(prog_file, "r");
	if (fp == NULL) {
		printf("Error: Can't open program file %s\n", prog_file);
		exit(-1);
	}

	/* Read in the program. */

	i = 0;
	while( fscanf(fp, "%x\n", &word) != EOF ) {
		address = MEM_TEXT_BEGIN + i;
		mem_write_32(address, word);
		printf("writing 0x%08x into address 0x%08x (%d)\n", word, address, address);
		i += 4;
	}
	PROGRAM_SIZE = i/4;
	printf("Program loaded into memory.\n%d words written into memory.\n\n", PROGRAM_SIZE);
	fclose(fp);
}

/************************************************************/
/* maintain the pipeline                                                                                           */ 
/************************************************************/
void handle_pipeline()
{
	/*INSTRUCTION_COUNT should be incremented when instruction is done*/
	/*Since we do not have branch/jump instructions, INSTRUCTION_COUNT should be incremented in WB stage */
	
	WB();
	MEM();
	EX();
	ID();
	IF();
}

/************************************************************/
/* writeback (WB) pipeline stage:                                                                          */ 
/************************************************************/
void WB()
{
    uint32_t rt, rd, opcode, backEnd;    
    rt = (MEM_WB.IR & 0x001F0000) >> 16;
    rd = (MEM_WB.IR & 0x0000F800) >> 11;
    opcode = (MEM_WB.IR & 0xFC000000) >> 26;
    backEnd = MEM_WB.IR & 0x0000003F;
    
    switch(opcode){
        case 0x00: //Reg-Reg
            if(backEnd == 0xC && MEM_WB.ALUOutput == 0xA){
                    RUN_FLAG = FALSE;
                    MEM_WB.ALUOutput = 0x0;
            }
			if(MEM_WB.IR != 0x00 && MEM_WB.RegWrite != 0){
            	NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
			}
            break;
					case 0x01:
        case 0x02: //J
        case 0x03: //JAL
        case 0x04: //BEQ
        case 0x05: //BNE
        case 0x06: //BLEZ
        case 0x07: //BGTZ
			break;
        case 0x20: //LB
        case 0x21: //LH
        case 0x23: //LW
            NEXT_STATE.REGS[rt] = MEM_WB.LMD;
            break;
        case 0x28: //SB
        case 0x29: //SH
        case 0x2B: //SW
            break;
        default: //Reg-Imm
            NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
            break;
    }
    if(MEM_WB.IR != 0){
        INSTRUCTION_COUNT++;
    }
    printf("[0x%x] \t",MEM_WB.PC -4);
    print_instruction(MEM_WB.IR,1);
}

/************************************************************/
/* memory access (MEM) pipeline stage:                                                          */ 
/************************************************************/
void MEM()
{
    if(STALL_FLAG == 2){
        MEM_WB.IR = 0x00;
    }else{
        MEM_WB.IR = EX_MEM.IR;
        MEM_WB.PC = EX_MEM.PC;
    }
    //printf("MEM_WB: ");
    //print_instruction(MEM_WB.IR,1);
    
    uint32_t data, opcode, backEnd;
    MEM_WB.ALUOutput = EX_MEM.ALUOutput;
    MEM_WB.LMD = EX_MEM.LMD;
    opcode = (MEM_WB.IR & 0xFC000000) >> 26;
    backEnd = (MEM_WB.IR & 0x0000003F);
    MEM_WB.RegWrite = 1;
    
    switch(opcode){
         case 0x00:
             switch(backEnd){
				 case 0x08: //JR
                 case 0x09: //JALR
                 case 0x0C: //SYSCALL
                 case 0x11: //MTHI
                 case 0x13: //MTLO
                 case 0x18: //MULT
                 case 0x19: //MULTU
                 case 0x1A: //DIV
                 case 0x1B: //DIVU
                     MEM_WB.RegWrite = 0;
                 default:
                     break;
            }
	 	case 0x20: //LB
            data = mem_read_32(EX_MEM.ALUOutput);
            MEM_WB.LMD = ((data & 0x000000FF) & 0x8000) > 0 ? (data | 0xFFFFFF00) : ( data & 0x000000FF);
            break;
        case 0x21: //LH
            data = mem_read_32(EX_MEM.ALUOutput);
			//printf("DATA %x MEM %x\n",data, EX_MEM.ALUOutput);
            MEM_WB.LMD = ((data & 0x0000FFFF) & 0x8000) > 0 ? (data | 0xFFFF0000) : ( data & 0x0000FFFF);
            break;
        case 0x23: //LW
            data = mem_read_32(EX_MEM.ALUOutput);
            MEM_WB.LMD = data;
            break;
        case 0x28: //SB
            data = mem_read_32(EX_MEM.ALUOutput);
            data = (data & 0xFFFFFF00) | (EX_MEM.LMD & 0x000000FF);
            mem_write_32(EX_MEM.ALUOutput, EX_MEM.LMD);
            MEM_WB.RegWrite = 0;
            break;
        case 0x29: //SH
            data = mem_read_32(EX_MEM.ALUOutput);
            data = (data & 0xFFFF0000) | (EX_MEM.LMD & 0x0000FFFF);
            mem_write_32(EX_MEM.ALUOutput, EX_MEM.LMD);
            MEM_WB.RegWrite = 0;
            break;
        case 0x2B: //SW
            mem_write_32(EX_MEM.ALUOutput, EX_MEM.LMD);
            MEM_WB.RegWrite = 0;
            break;
        default:
            break;
    }
}

/************************************************************/
/* execution (EX) pipeline stage:                                                                          */ 
/************************************************************/
void EX()
{
    if(STALL_FLAG != 0){
        EX_MEM.IR = 0x00;
    }else{
        EX_MEM.IR = ID_EX.IR;
        EX_MEM.PC = ID_EX.PC;
    }
    
    //printf("EX_MEM: ");
    //print_instruction(EX_MEM.IR, 1);
    
    uint64_t product, p1, p2, rt;
    uint32_t opcode, backEnd;
    rt = (EX_MEM.IR & 0x001F0000) >> 16;
    opcode = (EX_MEM.IR & 0xFC000000) >> 26;
    backEnd = EX_MEM.IR & 0x0000003F;
    EX_MEM.RegWrite = 1;
    BRANCH_FLAG = 0;
    
    if(EX_MEM.IR == 0x00){
        return;
    }
    
    switch(opcode){
        case 0x00: //backEnd
            switch(backEnd){
                 case 0x00: //SLL
                     EX_MEM.ALUOutput = ID_EX.B << ID_EX.A;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x02: //SRL
                     EX_MEM.ALUOutput = ID_EX.B >> ID_EX.A;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x03: //SRA
                     if((ID_EX.A & 0x80000000) == 1){
                        EX_MEM.ALUOutput = ~(~ID_EX.B >> ID_EX.A);
                     }else{
                        EX_MEM.ALUOutput = ID_EX.B >> ID_EX.A;
                     }
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x08: //JR
                     CURRENT_STATE.PC = ID_EX.A;
                     BRANCH_FLAG = 1;
                     STALL_FLAG = 1;
                     break;
                 case 0x09: //JALR
                     CURRENT_STATE.PC = ID_EX.A;
                     NEXT_STATE.REGS[31] = ID_EX.PC;
                     BRANCH_FLAG = 1;
                     STALL_FLAG = 1;
                     break;
                 case 0x0C: //SYSCALL
                     EX_MEM.ALUOutput = 0xA;
                     EX_MEM.RegWrite = 0;
                     break;
                 case 0x10: //MFHI
                     EX_MEM.ALUOutput = CURRENT_STATE.HI;
                     //EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x11: //MTHI
                     CURRENT_STATE.HI = ID_EX.A;
                     EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x12: //MFLO
                     EX_MEM.ALUOutput = CURRENT_STATE.LO;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x13: //MTLO
                     CURRENT_STATE.LO = ID_EX.A;
                     EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x18: //MULT
                     if ((ID_EX.A & 0x80000000) == 0x80000000){
						p1 = 0xFFFFFFFF00000000 | ID_EX.A;
                     }else{
						p1 = 0x00000000FFFFFFFF & ID_EX.A;
                     }
                     if ((CURRENT_STATE.REGS[rt] & 0x80000000) == 0x80000000){
						p2 = 0xFFFFFFFF00000000 | ID_EX.B;
                     }else{
						p2 = 0x00000000FFFFFFFF & ID_EX.B;
                     }
                     product = p1 * p2;
                     NEXT_STATE.LO = (product & 0X00000000FFFFFFFF);
                     NEXT_STATE.HI = (product & 0XFFFFFFFF00000000)>>32;
                     EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x19: //MULTU
                     product = (uint64_t)ID_EX.A * (uint64_t)ID_EX.B;
                     NEXT_STATE.LO = (product & 0x00000000FFFFFFFF);
                     NEXT_STATE.HI = (product & 0xFFFFFFFF00000000) >> 32;
                     EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x1A: //DIV
                     if(ID_EX.B != 0){
                        NEXT_STATE.LO = (int32_t)ID_EX.A / (int32_t)ID_EX.B;
                        NEXT_STATE.HI = (int32_t)ID_EX.A % (int32_t)ID_EX.B;
                     }
                     EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x1B: //DIVU
                     if(ID_EX.B != 0){
                        NEXT_STATE.LO = ID_EX.A / ID_EX.B;
                        NEXT_STATE.HI = ID_EX.A % ID_EX.B;
                     }
                     EX_MEM.RegWrite = 0;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x20: //ADD
                     EX_MEM.ALUOutput = ID_EX.A + ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x21: //ADDU
                     EX_MEM.ALUOutput = ID_EX.A + ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x22: //SUB
                     EX_MEM.ALUOutput = ID_EX.A - ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x23: //SUBU
                     EX_MEM.ALUOutput = ID_EX.A - ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x24: //AND
                     EX_MEM.ALUOutput = ID_EX.A & ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x25: //OR
                     EX_MEM.ALUOutput = ID_EX.A | ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x26: //XOR
                     EX_MEM.ALUOutput = ID_EX.A ^ ID_EX.B;
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x27: //NOR
                     EX_MEM.ALUOutput = ~(ID_EX.A | ID_EX.B);
                     //print_instruction(EX_MEM.PC);
                     break;
                 case 0x2A: //SLT
                     if(ID_EX.A < ID_EX.B){
                        EX_MEM.ALUOutput = 0x1;
                     }else{
                        EX_MEM.ALUOutput = 0x0;
                     }
                     //print_instruction(EX_MEM.PC);
                     break;
            }
            break;
        case 0x01:
            if(ID_EX.B == 0){  //BLTZ
                    if((ID_EX.A & 0x80000000) > 0){
                        CURRENT_STATE.PC =  (ID_EX.PC + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm | 0xFFFF0000)<<2 : (ID_EX.imm & 0x0000FFFF)<<2)) - 4; 
                        BRANCH_FLAG = 1;
                    }
            }
            else if(ID_EX.B == 1){ //BGEZ
                    if((ID_EX.A & 0x80000000) == 0x0){
                        CURRENT_STATE.PC =  (ID_EX.PC + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm | 0xFFFF0000)<<2 : (ID_EX.imm & 0x0000FFFF)<<2)) - 4; 
                        BRANCH_FLAG = 1;
                    }
            }
            //STALL_FLAG = 1;
            break;
        case 0x02: //J
            CURRENT_STATE.PC =  (ID_EX.PC & 0xF0000000) | ((ID_EX.IR & 0x03FFFFFF) << 2);  
            BRANCH_FLAG = 1;
            //STALL_FLAG = 1;
            break;
        case 0x03: //JAL
            CURRENT_STATE.PC =  (ID_EX.PC & 0xF0000000) | ((ID_EX.IR & 0x03FFFFFF) << 2); 
            NEXT_STATE.REGS[31] = ID_EX.PC;
            BRANCH_FLAG = 1;
            //STALL_FLAG = 1;
            break;
        case 0x04: //BEQ
            if(ID_EX.A == ID_EX.B){
                //printf("TEST BEQ LOOP %x\n", ID_EX.PC);
                CURRENT_STATE.PC =  (ID_EX.PC + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm | 0xFFFF0000)<<2 : (ID_EX.imm & 0x0000FFFF)<<2)) - 4; 
                BRANCH_FLAG = 1;
                    //        printf("TEST BEQ LOOP %x\n", CURRENT_STATE.PC);
            }
            break;
        case 0x05: //BNE
            if(ID_EX.A != ID_EX.B){
                CURRENT_STATE.PC =  (ID_EX.PC + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm | 0xFFFF0000)<<2 : (ID_EX.imm & 0x0000FFFF)<<2)) - 4;   
                BRANCH_FLAG = 1;
            }
            //STALL_FLAG = 1;
            break;
        case 0x06: //BLEZ
            if((ID_EX.A & 0x80000000) > 0 || ID_EX.A == 0){
                CURRENT_STATE.PC =  (ID_EX.PC + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm | 0xFFFF0000)<<2 : (ID_EX.imm & 0x0000FFFF)<<2)) - 4;   
                BRANCH_FLAG = 1;
            }
            break;
        case 0x07: //BGTZ
            if((ID_EX.A & 0x80000000) == 0x0 || ID_EX.A != 0){
                CURRENT_STATE.PC =  (ID_EX.PC + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm | 0xFFFF0000)<<2 : (ID_EX.imm & 0x0000FFFF)<<2)) - 4;  
                BRANCH_FLAG = 1;
            }
            //STALL_FLAG = 1;
            break;
        case 0x08: //ADDI
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            //print_instruction(EX_MEM.PC);
            break;
        case 0x09: //ADDIU
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            //print_instruction(EX_MEM.PC);
            break;
        case 0x0A: //SLTI
            if((int32_t)ID_EX.A - (int32_t)((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF)) < 0){
                EX_MEM.ALUOutput = 0x1;
            }else{
                EX_MEM.ALUOutput = 0x0;
            }
            //print_instruction(EX_MEM.PC);
            break;
        case 0x0C: //ANDI
            EX_MEM.ALUOutput = ID_EX.A & (ID_EX.imm & 0x0000FFFF);
            //print_instruction(EX_MEM.PC);            data = (data & 0xFFFFFF00) | (EX_MEM.B & 0x000000FF);
            break;
        case 0x0D: //ORI
            EX_MEM.ALUOutput = ID_EX.A | (ID_EX.imm & 0x0000FFFF);
            //print_instruction(EX_MEM.PC);
            break;
        case 0x0E: //XORI
            EX_MEM.ALUOutput = ID_EX.A ^ (ID_EX.imm & 0x0000FFFF);
            //print_instruction(EX_MEM.PC);
            break;
        case 0x0F: //LUI
            EX_MEM.ALUOutput = ID_EX.imm << 16;
            //print_instruction(EX_MEM.PC);
            break;
        case 0x20: //LB
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            EX_MEM.LMD = ID_EX.B;
            //print_instruction(EX_MEM.PC);
            break; 
        case 0x21: //LH
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            EX_MEM.LMD = ID_EX.B;
            //print_instruction(EX_MEM.PC);
            break;
        case 0x23: //LW
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            EX_MEM.LMD = ID_EX.B;
            //print_instruction(EX_MEM.PC);
            break;
        case 0x28: //SB
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));   
            EX_MEM.LMD = ID_EX.B;
            EX_MEM.RegWrite = 0;
            //print_instruction(EX_MEM.PC);
            break;
        case 0x29: //SH
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            EX_MEM.LMD = ID_EX.B;
            EX_MEM.RegWrite = 0;
            //print_instruction(EX_MEM.PC);
            break;
        case 0x2B: //SW
            EX_MEM.ALUOutput = ID_EX.A + ((ID_EX.imm & 0x8000) > 0 ? (ID_EX.imm |0xFFFF0000) : (ID_EX.imm & 0x0000FFFF));
            EX_MEM.LMD = ID_EX.B;
            EX_MEM.RegWrite = 0;
            //print_instruction(EX_MEM.PC);
            break;
        default:
            printf("Instruction is not implemented at 0x%x\n", EX_MEM.IR);
            EX_MEM.RegWrite = 0;
            break;
    }
}

/************************************************************/
/* instruction decode (ID) pipeline stage:                                                         */ 
/************************************************************/
void ID()
{
    uint32_t opcode, backEnd, rs, rt, sa, immediate, rdEX, rdMEM;
    if(STALL_FLAG == 0){
        ID_EX.IR = IF_ID.IR;
    }
    if(BRANCH_FLAG == 1){
        ID_EX.IR = 0x0;
    }
    ID_EX.PC = IF_ID.PC;
    opcode = (ID_EX.IR & 0xFC000000) >> 26;
    backEnd = ID_EX.IR & 0x0000003F;
    rs = (ID_EX.IR & 0x03E00000) >> 21;
    rt = (ID_EX.IR & 0x001F0000) >> 16;
    sa = (ID_EX.IR & 0x000007C0) >> 6;
    immediate = ID_EX.IR & 0x0000FFFF;
    
    STALL_FLAG = 0;
    
    switch((EX_MEM.IR & 0xFC000000) >> 26){
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
        case 0x20:
        case 0x21:
        case 0x23:
            rdEX = (EX_MEM.IR & 0x001F0000) >> 16;
			//printf("%x rdEX : %x \n",ID_EX.IR,rdEX);
            break;
        default:
            rdEX = (EX_MEM.IR & 0x0000F800) >> 11;
			//printf("%x rdEX : %x \n",ID_EX.IR,rdEX);
            break;
    }

    switch((MEM_WB.IR & 0xFC000000) >> 26){
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
        case 0x20:
        case 0x21:
        case 0x23:
            rdMEM = (MEM_WB.IR & 0x001F0000) >> 16;
                //printf("%x rdMEM : %x \n",ID_EX.IR,rdMEM);
            break;
        default:
            rdMEM = (MEM_WB.IR & 0x0000F800) >> 11;
                //printf("%x rdMEM : %x \n",ID_EX.IR,rdMEM);
            break;
    }
	
    if(ENABLE_FORWARDING){
        if((MEM_WB.RegWrite == 1) && (rdMEM != 0) && ~((EX_MEM.RegWrite) && (rdEX != 0))){
			//printf("TEST\n");
			if((rdMEM == rs)){
				//printf("ForwardAMem\n");
				FORWARD_A = 0x01;
			}else if((rdMEM == rt)){
				//printf("ForwardBMem\n");
				FORWARD_B = 0x01;
			}
		}
		
		if((EX_MEM.RegWrite == 1) && (rdEX != 0x0)){
			//printf("TEST\n");
            if(rdEX == rs){
                FORWARD_A = 0x10;
                //printf("forwardA\n");
            }else if(rdEX == rt){
                FORWARD_B = 0x10; 
                //printf("ForwardB\n");
            }
            switch((EX_MEM.IR & 0xFC000000) >> 26){
                case 0x20: //LB
                case 0x21: //LH
                case 0x23: //LW
                    STALL_FLAG = 1;
                    break;
            }
        }

    }else{
        if((EX_MEM.RegWrite == 1) && (rdEX != 0x0)){
            if(rdEX == rs){
                STALL_FLAG = 1;
            }else if(rdEX == rt){
                STALL_FLAG = 1;        
            }
        }
        if((MEM_WB.RegWrite == 1) && (rdMEM != 0)){
            if(rdMEM == rs){
                STALL_FLAG = 2;
            }else if(rdMEM == rt){
                STALL_FLAG = 2;
            }
        }
    }
    //printf("FA %x FB %x\n",FORWARD_A, FORWARD_B);
    
    if(opcode == 0x00){
        switch(backEnd){
            case 0x00: //SLL
            case 0x02: //SRL
            case 0x03: //SRA
                ID_EX.B = NEXT_STATE.REGS[rs];
                ID_EX.A = sa;
                ID_EX.imm = 0;
                //printf("SA %x RS %x\n", sa, NEXT_STATE.REGS[rs]);
                //printf("A %x B %x\n", ID_EX.A, ID_EX.B);
                break;
            default:
                ID_EX.A = NEXT_STATE.REGS[rs];
                ID_EX.B = NEXT_STATE.REGS[rt];
                ID_EX.imm = 0;
                break;
        }
    }else if(opcode == 0x01){
        ID_EX.A = NEXT_STATE.REGS[rs];
        ID_EX.B = rt;
        ID_EX.imm = immediate;
    }else{
        ID_EX.A = NEXT_STATE.REGS[rs];
        ID_EX.B = NEXT_STATE.REGS[rt];
        ID_EX.imm = immediate;
    }
    
    if(ENABLE_FORWARDING){
        if(FORWARD_A == 0x10){
            ID_EX.A = EX_MEM.ALUOutput;
        }else if(FORWARD_B  == 0x10){
            ID_EX.B = EX_MEM.ALUOutput;
            //printf("EX_MEM Forward B %x\n", ID_EX.B);
        }
        if(FORWARD_A == 0x01){ 
            switch((MEM_WB.IR & 0xFC000000) >> 26){
                case 0x20: //LB
                case 0x21: //LH
                case 0x23: //LW
                    ID_EX.A = MEM_WB.LMD;
                    break;
                default:
                    ID_EX.A = MEM_WB.ALUOutput;
                    break;
            }
        }else if(FORWARD_B == 0x01){
            switch((MEM_WB.IR & 0xFC000000) >> 26){
                case 0x20: //LB
                case 0x21: //LH
                case 0x23: //LW
                    ID_EX.B = MEM_WB.LMD;
                    break;
                default:
                    ID_EX.B = MEM_WB.ALUOutput;
                    break;
            }
        }
      FORWARD_A = 0x00;
      FORWARD_B = 0x00;
    }
}

/************************************************************/
/* instruction fetch (IF) pipeline stage:                                                              */ 
/************************************************************/
void IF()
{
    if(STALL_FLAG == 0){
        IF_ID.IR = mem_read_32(CURRENT_STATE.PC);
        IF_ID.PC = CURRENT_STATE.PC + 4;
        NEXT_STATE.PC = IF_ID.PC;
        //print_instruction(CURRENT_STATE.PC, 0);
    }else{
        //printf("Stall Flag Value: %d\n",STALL_FLAG);
        printf("STALLED\n");
    }
    
}


/************************************************************/
/* Initialize Memory                                                                                                    */ 
/************************************************************/
void initialize() { 
	init_memory();
	CURRENT_STATE.PC = MEM_TEXT_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = TRUE;
}

/************************************************************/
/* Print the program loaded into memory (in MIPS assembly format)    */ 
/************************************************************/
void print_program(){
	int i;
	uint32_t addr;
	
	for(i=0; i<PROGRAM_SIZE; i++){
		addr = MEM_TEXT_BEGIN + (i*4);
		printf("[0x%x]\t", addr);
		print_instruction(addr, 0);
	}
}

/************************************************************/
/* Print the current pipeline                                                                                    */ 
/************************************************************/
void show_pipeline(){
	printf("\nCurrent PC:   %x\n", CURRENT_STATE.PC);
        printf("IF/ID.IR      %x\t", IF_ID.IR);
        print_instruction(IF_ID.IR, 1);
        printf("IF/ID.PC      %x\n\n", IF_ID.PC);
        
        printf("ID/EX.IR      %x\t", ID_EX.IR);
        print_instruction(ID_EX.IR, 1);
        printf("ID/EX.A       %x\n", ID_EX.A);
        printf("ID/EX.B       %x\n", ID_EX.B);
        printf("ID/EX.imm     %x\n\n", ID_EX.imm);
        
        printf("EX/MEM.IR     %x\t", EX_MEM.IR);
        print_instruction(EX_MEM.IR, 1);
        printf("EX/MEM.ALUOutput %x\n\n", EX_MEM.ALUOutput);
        
        printf("MEM/WB.IR     %x\t", MEM_WB.IR);
        print_instruction(MEM_WB.IR, 1);
        printf("MEM/WB.ALUOutput %x\n", MEM_WB.ALUOutput);
        printf("MEM/WB.LMD    %x\n\n", MEM_WB.LMD);
        
}

/***************************************************************/
/* main                                                                                                                                   */
/***************************************************************/
int main(int argc, char *argv[]) {                              
	printf("\n**************************\n");
	printf("Welcome to MU-MIPS SIM...\n");
	printf("**************************\n\n");
	
	if (argc < 2) {
		printf("Error: You should provide input file.\nUsage: %s <input program> \n\n",  argv[0]);
		exit(1);
	}

	strcpy(prog_file, argv[1]);
	initialize();
	load_program();
	help();
	while (1){
		handle_command();
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "mu-mips.h"

int MUX_stall = 0;         //flags created for allowing forwarding
int MUX1_forward = 0;
int MUX2_forward = 0;
int FORWARD_enabledFlag = 0;

/***************************************************************/
/* Print out a list of commands available                                                                  */
/***************************************************************/
void help() {
    printf("------------------------------------------------------------------\n\n");
    printf("\t**********MU-MIPS Help MENU**********\n\n");
    printf("sim\t-- simulate program to completion \n");
    printf("run <n>\t-- simulate program for <n> instructions\n");
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

        case 'F':
        case 'f':
            if (scanf("%d", &FORWARD_enabledFlag) != 1)
            {
                break;
            }
            //FORWARD_enabledFlag == 0 ? printf("Forwarding OFF\n") : printf("Forwarding ON\n");                   /////
            if(FORWARD_enabledFlag == 0)
            {
                printf("Forwarding OFF\n");
            }
            else
            {
                printf("Forwarding ON\n");
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

    MUX_stall = 0;
    MUX1_forward = 0;
    MUX2_forward = 0;
    FORWARD_enabledFlag = 0;

    IF_EX.PC = 0;
    IF_EX.IR = 0;
    IF_EX.A = 0;
    IF_EX.B = 0;
    IF_EX.imm = 0;
    IF_EX.ALUOutput = 0;
    IF_EX.LMD = 0;
    IF_EX.write = 0;

    ID_IF.PC = 0;
    ID_IF.IR = 0;
    ID_IF.A = 0;
    ID_IF.B = 0;
    ID_IF.imm = 0;
    ID_IF.ALUOutput = 0;
    ID_IF.LMD = 0;
    ID_IF.write = 0;

    EX_MEM.PC = 0;
    EX_MEM.IR = 0;
    EX_MEM.A = 0;
    EX_MEM.B = 0;
    EX_MEM.imm = 0;
    EX_MEM.ALUOutput = 0;
    EX_MEM.LMD = 0;
    EX_MEM.write = 0;

    MEM_WB.PC = 0;
    MEM_WB.IR = 0;
    MEM_WB.A = 0;
    MEM_WB.B = 0;
    MEM_WB.imm = 0;
    MEM_WB.ALUOutput = 0;
    MEM_WB.LMD = 0;
    MEM_WB.write = 0;

    /*load program*/
    load_program();

    /*reset PC*/
    INSTRUCTION_COUNT = 0;
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
/* Print the instruction at given memory address (in MIPS assembly format)    */
/************************************************************/
void print_instruction(uint32_t addr){
    /*IMPLEMENT THIS*/

    uint32_t instruction = mem_read_32(addr);
    uint32_t specialOpCode = 0;
    uint32_t opcode = 0, rs = 0, rt = 0, rd = 0, sa = 0;
    uint32_t immediate = 0;
    uint32_t base = 0;              //equals rs but we changed it because thats how it was in manuel
    uint32_t target = 0;            //equals target but we changed it because of manuel
    uint32_t memoryLocation = 0;

    if((instruction | 0x03ffffff) == 0x03ffffff)
    {
        specialOpCode = instruction & 0x0000003f;
        rs = instruction & 0x03E00000;
        rs = rs >> 21;
        rt = instruction & 0x001F0000;
        rt = rt >> 16;
        rd = instruction & 0x0000F800;
        rd = rd >> 11;
        sa = instruction & 0x000007C0;
        sa = sa >> 6;

        switch(specialOpCode)
        {
            case 0x00000020: //ADD
                printf("ADD $%u, $%u, $%u\n",rd, rs, rt);
                break;

            case 0x00000021: //ADDU
                printf("ADDU $%u, $%u, $%u\n",rd, rs, rt);
                break;

            case 0x00000022: //SUB
                printf("SUB $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x00000023: //SUBU
                printf("SUBU $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x00000018: //MULT
                printf("MULT $%u, $%u\n", rs, rt);
                break;

            case 0x00000019: //MULTU
                printf("MULTU $%u, $%u\n", rs, rt);
                break;

            case 0x0000001A: //DIV
                printf("DIV $%u, $%u\n", rs, rt);
                break;

            case 0x00000001B: //DIVU
                printf("DIVU $%u, $%u\n", rs, rt);
                break;

            case 0x00000024: //AND
                printf("AND $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x00000025: //OR
                printf("OR $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x00000026: //XOR
                printf("XOR $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x00000027: //NOR
                printf("NOR $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x0000002A: //SLT
                printf("SLT $%u, $%u, $%u\n", rd, rs, rt);
                break;

            case 0x00000000: //SLL
                printf("SLL $%u, $%u, %u\n", rd, rt, sa);
                break;

            case 0x00000002: //SRL
                printf("SRL $%u, $%u, %u\n", rd, rt, sa);
                break;

            case 0x00000003: //SRA
                printf("SRA $%u, $%u, %u\n", rd, rt, sa);
                break;

            case 0x00000010: //MFHI
                printf("MFHI $%u\n",rd);
                break;

            case 0x00000012: //MFLO
                printf("MFHI $%u\n",rd);
                break;

            case 0x00000011: //MTHI
                printf("MTHI $%u\n", rs);
                break;

            case 0x00000013: //MTLO
                printf("MTLO $%u\n", rs);
                break;

            case 0x00000008: //JR
                printf("JR $%u\n", rs);
                break;

            case 0x00000009: //JALR
                printf("JALR $%u\n", rs);
                break;

            case 0x0000000C:  //SYSTEMCALL
                printf("SYSTEMCALL\n");
                break;
        }
    }
    else
    {
        opcode = instruction & 0xFC000000;
        immediate = instruction & 0x0000FFFF;
        // immediate = immediate << 16;
        rs = instruction & 0x03E00000;
        rs = rs >> 21;
        rt = instruction & 0x001F0000;
        rt = rt >> 16;
        base = instruction & 0x03E00000;
        base = base >> 21;
        target = instruction & 0x03FFFFFF;
        target = target << 2;

        switch(opcode)
        {
            case 0x3C000000: //LUI
                printf("LUI $%u, %u\n", rt, immediate);
                break;

            case 0x24000000: //ADDIU
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                printf("ADDIU $%u, $%u, %u\n", rt, rs, immediate);
                break;

            case 0x20000000: //ADDI
                if((immediate & 0x00008000) == 0x00008000){
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                printf("ADDI $%u, $%u, %u\n", rt, rs, immediate);
                break;

            case 0x8C000000: //LW
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                memoryLocation = CURRENT_STATE.REGS[base] + immediate;
                memoryLocation = memoryLocation | 0x00010000;
                printf("LW $%u, %u(%u)\n",rt, immediate, base);
                break;

            case 0x84000000: //LH
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                memoryLocation = CURRENT_STATE.REGS[base] + immediate;
                memoryLocation = memoryLocation | 0x00010000;
                printf("LH $%u, %u(%u)\n",rt, immediate, base);
                break;

            case 0xAC000000: //SW
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                memoryLocation = CURRENT_STATE.REGS[base] + immediate;
                memoryLocation = memoryLocation | 0x00010000;
                printf("SW $%u, %u(%u)\n",rt, immediate, base);
                break;

            case 0x30000000: //ANDI
                printf("ANDI $%u, $%u, %u\n", rt, rs, immediate);
                break;

            case 0x34000000: //ORI
                printf("ORI $%u, $%u, %u\n", rt, rs, immediate);
                break;

            case 0x38000000: //XORI
                printf("XORI $%u, $%u, %u\n", rt, rs, immediate);
                break;

            case 0x28000000: //SLTI
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                printf("SLTI $%u, %u, $%u\n", rt, rs, immediate);
                break;

            case 0xA4000000: //SH
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                memoryLocation = CURRENT_STATE.REGS[base] + immediate;
                memoryLocation = memoryLocation | 0x00010000;
                printf("SH $%u, %u(%u)\n",rt, immediate, base);
                break;

            case 0xA0000000: //SB
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                memoryLocation = CURRENT_STATE.REGS[base] + immediate;
                memoryLocation = memoryLocation | 0x00010000;
                printf("SB $%u, %u(%u)\n",rt, immediate, base);
                break;

            case 0x0C000000: //JAL
                printf("JAL %x\n", target);
                break;

            case 0x08000000: //J
                printf("J %x\n", target);
                break;

            case 0x80000000: //LB
                if((immediate & 0x00008000) == 0x00008000)
                {
                    immediate = immediate | 0xFFFF0000;
                }
                else
                {
                    immediate = immediate & 0x0000FFFF;
                }
                memoryLocation = CURRENT_STATE.REGS[base] + immediate;
                memoryLocation = memoryLocation | 0x00010000;
                printf("LB $%u, %u(%u)\n",rt, immediate, base);
                break;

            case 0x10000000: //BEQ
                if((target & 0x00008000) == 0x00008000)
                {
                    target = target | 0xFFFF0000;
                }
                else
                {
                    target = target & 0x0000FFFF;
                }
                printf("BEQ $%u, $%u, %u\n", rs, rt, target);
                break;

            case 0x14000000: //BNE
                if((target & 0x00008000) == 0x00008000)
                {
                    target = target | 0xFFFF0000;
                }
                else
                {
                    target = target & 0x0000FFFF;
                }
                printf("BNE $%u, $%u, %u\n", rs, rt, target);
                break;

            case 0x18000000: //BLEZ
                if((target & 0x00008000) == 0x00008000)
                {
                    target = target | 0xFFFF0000;
                }
                else
                {
                    target = target & 0x0000FFFF;
                }
                printf("BLEZ $%u, %u\n", rs, target);
                break;

            case 0x1C000000: //BGTZ
                if((target & 0x00008000) == 0x00008000)
                {
                    target = target | 0xFFFF0000;
                }
                else
                {
                    target = target & 0x0000FFFF;
                }
                printf("BGTZ $%u, %u\n", rs, target);
                break;

            case 0x04000000: //BLTZ and BGEZ
                if((target & 0x00008000) == 0x00008000)
                {
                    target = target | 0xFFFF0000;
                }
                else
                {
                    target = target & 0x0000FFFF;
                }
                if(rt == 0x01) //BGEZ
                {
                    printf("BGEZ $%u, %u\n", rs, target);
                }
                else //BLTZ
                {
                    printf("BLTZ $%u, %u\n", rs, target);
                }
                break;
        }

    }

}


/************************************************************/
/* writeback (WB) pipeline stage:                                                                          */
/************************************************************/
void WB()
{
    /*IMPLEMENT THIS*/

    uint32_t opcode = 0, rt = 0, rd = 0, funct = 0;

    opcode = (MEM_WB.IR & 0xFC000000) >> 26;
    rt = (MEM_WB.IR & 0x001F0000) >> 16;
    rd = (MEM_WB.IR & 0x0000F800) >> 11;
    funct = MEM_WB.IR & 0x0000003F;

    if(opcode == 0x00)
    {
        if(MEM_WB.ALUOutput == 0xa && funct == 0xC)
        {
            RUN_FLAG = FALSE;
        }
        NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
    }
    else
    {
        switch(opcode)
        {
            case 0x2B: 		//SW
                break;

            case 0x29: 		//SH
                break;

            case 0x28: 		//SB
                break;

            case 0x23: 		//LW
                NEXT_STATE.REGS[rt] = MEM_WB.LMD;
                break;

            case 0x21: 		//LH
                NEXT_STATE.REGS[rt] = MEM_WB.LMD;
                break;

            case 0x20: 		//LB
                NEXT_STATE.REGS[rt] = MEM_WB.LMD;
                break;

            default:   		//used for I-Type
                NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
                break;
        }
    }

    if(MEM_WB.IR != 0)
    {
        INSTRUCTION_COUNT++;
    }
}

/************************************************************/
/* memory access (MEM) pipeline stage:                                                          */
/************************************************************/
void MEM()
{
    /*IMPLEMENT THIS*/

    uint32_t opcode = 0, funct = 0;

    if(MUX_stall == 2)
    {
        MEM_WB.IR = 0x00;
    }
    else
    {
        MEM_WB.IR = EX_MEM.IR;
        MEM_WB.PC = EX_MEM.PC;
    }

    opcode = (MEM_WB.IR & 0xFC000000) >> 26;
    funct = (MEM_WB.IR & 0x0000003F);

    MEM_WB.ALUOutput = EX_MEM.ALUOutput;
    MEM_WB.LMD = EX_MEM.LMD;
    MEM_WB.write = 1;

    if(opcode == 0x00)
    {
        switch(funct)
        {
            case 0x18: 		//MULT
                MEM_WB.write = 0;
                break;

            case 0x19: 		//MULTU
                MEM_WB.write = 0;
                break;

            case 0x1A: 		//DIV
                MEM_WB.write = 0;
                break;

            case 0x1B: 		//DIVU
                MEM_WB.write = 0;
                break;

            case 0x10: 		//MFHI
                MEM_WB.write = 0;
                break;

            case 0x11: 		//MTHI
                MEM_WB.write = 0;
                break;

            case 0x12: 		//MFLO
                MEM_WB.write = 0;
                break;

            case 0x13: 		//MTLO
                MEM_WB.write = 0;
                break;

            case 0x0C: 		//SYSCALL
                MEM_WB.write = 0;
                break;

            default:
                break;
        }
    }

    switch(opcode)
    {
        case 0x2B:		//SW
            mem_write_32(EX_MEM.ALUOutput, EX_MEM.LMD);
            MEM_WB.write = 0;
            break;

        case 0x29: 		//SH
            mem_write_32(EX_MEM.ALUOutput & 0x0000FFFF, EX_MEM.LMD);
            MEM_WB.write = 0;
            break;

        case 0x28: 		//SB
            mem_write_32(EX_MEM.ALUOutput & 0x000000FF, EX_MEM.LMD);
            MEM_WB.write = 0;
            break;

        case 0x23: 		//LW
            MEM_WB.LMD = mem_read_32(EX_MEM.ALUOutput);
            MEM_WB.write = 1;
            break;

        case 0x21: 		//LH
            MEM_WB.LMD = mem_read_32(EX_MEM.ALUOutput) & 0x0000FFFF;
            MEM_WB.write = 1;
            break;

        case 0x20: 		//LB
            MEM_WB.LMD = mem_read_32(EX_MEM.ALUOutput) & 0x0000FFFF;
            MEM_WB.write = 1;
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
    /*IMPLEMENT THIS*/

    uint32_t opcode = 0, rt = 0, funct = 0, i = 0;
    uint64_t product = 0;

    if(MUX_stall != 0)
    {
        EX_MEM.IR = 0x00;
        return;
    }

    EX_MEM.IR = IF_EX.IR;
    EX_MEM.PC = IF_EX.PC;

    opcode = (EX_MEM.IR & 0xFC000000) >> 26;
    rt = (EX_MEM.IR & 0x001F0000) >> 16;
    funct = EX_MEM.IR & 0x0000003F;

    EX_MEM.write = 0;

    if(opcode == 0x00)
    {
        switch(funct)
        {
            case 0x18: 		//MULT
                product = IF_EX.A * IF_EX.B;
                NEXT_STATE.LO = (product & 0x00000000FFFFFFFF);
                NEXT_STATE.HI = (product & 0xFFFFFFFF00000000) >> 32;
                break;

            case 0x19:		// MULTU
                product = IF_EX.A * IF_EX.B;
                NEXT_STATE.LO = (product & 0x00000000FFFFFFFF);
                NEXT_STATE.HI = (product & 0xFFFFFFFF00000000) >> 32;
                break;

            case 0x1A: 		//DIV
                if(IF_EX.B != 0)
                {
                    NEXT_STATE.LO = IF_EX.A / IF_EX.B;
                    NEXT_STATE.HI = IF_EX.A % IF_EX.B;
                }
                break;

            case 0x1B:		//DIVU
                if(IF_EX.B != 0)
                {
                    NEXT_STATE.LO = IF_EX.A / IF_EX.B;
                    NEXT_STATE.HI = IF_EX.A % IF_EX.B;
                }
                break;

            case 0x20: 		//ADD
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x21: 		//ADDU
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x22: 		//SUB
                EX_MEM.ALUOutput = IF_EX.A - IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x23: 		//SUBU
                EX_MEM.ALUOutput = IF_EX.A - IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x24: 		//AND
                EX_MEM.ALUOutput = IF_EX.A & IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x25: 		//OR
                EX_MEM.ALUOutput = IF_EX.A | IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x26: 		//XOR
                EX_MEM.ALUOutput = IF_EX.A ^ IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x27: 		//NOR
                EX_MEM.ALUOutput = ~(IF_EX.A | IF_EX.B);
                EX_MEM.write = 1;
                break;

            case 0x2A: 		//SLT
                if(IF_EX.A < IF_EX.B)
                {
                    EX_MEM.ALUOutput = 0x1;
                }
                else
                {
                    EX_MEM.ALUOutput = 0x0;
                }
                EX_MEM.write = 1;
                break;

            case 0x00: 		//SLL
                EX_MEM.ALUOutput = IF_EX.A << IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x02: 		//SRL
                EX_MEM.ALUOutput = IF_EX.A >> IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x03: 		//SRA
                if((IF_EX.A & 0x80000000) == 0x80000000)
                {
                    for(i = 0; i < IF_EX.B; i++)
                    {
                        EX_MEM.ALUOutput = (IF_EX.A >> 1) | 0x80000000;
                    }
                }
                else
                {
                    EX_MEM.ALUOutput = IF_EX.A >> IF_EX.B;
                }
                EX_MEM.write = 1;
                break;

            case 0x10: 		//MFHI
                EX_MEM.ALUOutput = CURRENT_STATE.HI;
                break;

            case 0x11: 		//MTHI
                CURRENT_STATE.HI = IF_EX.A;
                break;

            case 0x12: 		//MFLO
                EX_MEM.ALUOutput = CURRENT_STATE.LO;
                break;

            case 0x13: 		//MTLO
                CURRENT_STATE.LO = IF_EX.A;
                break;

            case 0x0C: 		//SYSCALL
                EX_MEM.ALUOutput = 0xA;
                break;
        }
    }
    else
    {
        switch(opcode)
        {
            case 0x08: 		//ADDI
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.write = 1;
                break;

            case 0x09: 		//ADDIU
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.write = 1;
                break;

            case 0x0A: 		//SLTI
                if(IF_EX.A - IF_EX.imm < 0)
                {
                    EX_MEM.ALUOutput = 0x1;
                }
                else
                {
                    EX_MEM.ALUOutput = 0x0;
                }
                EX_MEM.write = 1;
                break;

            case 0x0C: 		//ANDI
                EX_MEM.ALUOutput = IF_EX.A & IF_EX.imm;
                EX_MEM.write = 1;
                break;

            case 0x0D: 		//ORI
                EX_MEM.ALUOutput = IF_EX.A | IF_EX.imm;
                EX_MEM.write = 1;
                break;

            case 0x0E: 		//XORI
                EX_MEM.ALUOutput = IF_EX.A ^ IF_EX.imm;
                EX_MEM.write = 1;
                break;

            case 0x0F: 		//LUI
                EX_MEM.ALUOutput = IF_EX.imm << 16;
                EX_MEM.write = 1;
                break;

            case 0x20: 		//LB
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.LMD = IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x21: 		//LH
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.LMD = IF_EX.B;
                EX_MEM.write = 1;
                break;

            case 0x23: 		//LW
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.write = 1;
                EX_MEM.LMD = IF_EX.B;
                break;

            case 0x28: 		//SB
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.LMD = IF_EX.B;
                break;

            case 0x29: 		//SH
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.LMD = IF_EX.B;
                break;

            case 0x2B: 		//SW
                EX_MEM.ALUOutput = IF_EX.A + IF_EX.imm;
                EX_MEM.LMD = IF_EX.B;
                break;

            default:
                break;
        }
    }
}

/************************************************************/
/* instruction decode (ID) pipeline stage:                                                         */
/************************************************************/
void ID()
{
    /*IMPLEMENT THIS*/

    uint32_t opcode = 0, rs = 0, rt = 0, sa = 0, funct = 0, immediate = 0, EX_RD = 0, MEM_RD = 0, temp = 0, temp2 = 0;

    if (MUX_stall == 0)
    {
        IF_EX.IR = ID_IF.IR;
    }
    MUX_stall = 0;

    opcode = (IF_EX.IR & 0xFC000000) >> 26;
    rs = (IF_EX.IR & 0x03E00000) >> 21;
    rt = (IF_EX.IR & 0x001F0000) >> 16;
    sa = (IF_EX.IR & 0x000007C0) >> 6;
    funct = IF_EX.IR & 0x0000003F;
    immediate = IF_EX.IR & 0x0000FFFF;

    switch((EX_MEM.IR & 0xFC000000) >> 26)
    {
        case 0x08:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x09:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x0A:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x0C:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x0D:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x0E:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x0F:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x20:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x21:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        case 0x23:
            EX_RD = (EX_MEM.IR & 0x001F0000) >> 16;
            break;

        default:
            EX_RD = (EX_MEM.IR & 0x0000F800) >> 11;
            break;
    }

    switch((MEM_WB.IR & 0xFC000000) >> 26)
    {
        case 0x08:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x09:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x0A:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x0C:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x0D:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x0E:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x0F:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x20:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x21:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        case 0x23:
            MEM_RD = (MEM_WB.IR & 0x001F0000) >> 16;
            break;

        default:
            MEM_RD = (MEM_WB.IR & 0x0000F800) >> 11;
            break;
    }

    if(FORWARD_enabledFlag)
    {
        if((EX_MEM.write == 1) && (EX_RD != 0x0) && (EX_RD == rs))
        {
            MUX1_forward = 2;
            temp = (EX_MEM.IR & 0xFC000000) >> 26;

            if((temp == 0x20) || (temp == 0x21) || (temp == 0x23))
            {
                MUX_stall = 1;
            }
        }
        else if((EX_MEM.write == 1) && (EX_RD != 0x0) && (EX_RD == rt))
        {
            MUX2_forward = 2;
            temp = (EX_MEM.IR & 0xFC000000) >> 26;

            if((temp == 0x20) || (temp == 0x21) || (temp == 0x23))
            {
                MUX_stall = 1;
            }
        }

        if((MEM_RD == rs) && (EX_RD == 0) && (MEM_WB.write == 1) && (MEM_RD != 0) && (((EX_MEM.write == 1) && (EX_RD != 0)) == 0))
        {
            MUX1_forward = 1;
        }
        else if((MEM_RD == rt) && (EX_RD == 0) && (MEM_WB.write == 1) && (MEM_RD != 0) && (((EX_MEM.write == 1) && (EX_RD != 0)) == 0))
        {
            MUX2_forward = 1;
        }
    }
    else
    {
        if ((EX_MEM.write == 1) && (EX_RD != 0) && ((EX_RD == rs) || (EX_RD == rt)))
        {
            MUX_stall = 1;
        }
        if ((MEM_WB.write == 1) && (MEM_RD != 0) && ((MEM_RD == rs) || (MEM_RD == rt)))
        {
            MUX_stall = 2;
        }
    }

    if(opcode == 0x00)
    {
        switch(funct)
        {
            case 0x00: 		//SLL
                IF_EX.A = NEXT_STATE.REGS[rs];
                IF_EX.B = NEXT_STATE.REGS[sa];
                IF_EX.imm = 0;
                break;

            case 0x02: 		//SRL
                IF_EX.A = NEXT_STATE.REGS[rs];
                IF_EX.B = NEXT_STATE.REGS[sa];
                IF_EX.imm = 0;
                break;

            case 0x03: 		//SRA
                IF_EX.A = NEXT_STATE.REGS[rs];
                IF_EX.B = NEXT_STATE.REGS[sa];
                IF_EX.imm = 0;
                break;

            default:
                IF_EX.A = NEXT_STATE.REGS[rs];
                IF_EX.B = NEXT_STATE.REGS[rt];
                IF_EX.imm = 0;
                break;
        }
    }
    else
    {
        IF_EX.A = NEXT_STATE.REGS[rs];
        IF_EX.B = NEXT_STATE.REGS[rt];
        IF_EX.imm = immediate;
    }

    if((IF_EX.imm & 0x00008000) == 0x00008000)
    {
        IF_EX.imm = IF_EX.imm | 0xFFFF0000;
    }
    else
    {
        IF_EX.imm = IF_EX.imm & 0x0000FFFF;
    }

    if(FORWARD_enabledFlag == 1)
    {
        if(MUX1_forward == 1)
        {
            temp2 = (MEM_WB.IR & 0xFC000000) >> 26;

            if((temp2 == 0x20) || (temp2 == 0x21) || (temp2 == 0x23))
            {
                IF_EX.A = MEM_WB.LMD;
            }
            else
            {
                IF_EX.A = MEM_WB.ALUOutput;
            }
        }
        else if(MUX2_forward == 1)
        {
            temp2 = (MEM_WB.IR & 0xFC000000) >> 26;

            if((temp2 == 0x20) || (temp2 == 0x21) || (temp2 == 0x23))
            {
                IF_EX.B = MEM_WB.LMD;
            }
            else
            {
                IF_EX.B = MEM_WB.ALUOutput;
            }
        }

        if(MUX1_forward == 2)
        {
            IF_EX.A = EX_MEM.ALUOutput;
        }
        else if(MUX2_forward == 2)
        {
            IF_EX.B = EX_MEM.ALUOutput;
        }

        MUX1_forward = 0x00;
        MUX2_forward = 0x00;
    }
}

/************************************************************/
/* instruction fetch (IF) pipeline stage:                                                              */
/************************************************************/
void IF()
{
    /*IMPLEMENT THIS*/

    if(MUX_stall == 0)
    {
        ID_IF.IR = mem_read_32(CURRENT_STATE.PC);
        ID_IF.PC = CURRENT_STATE.PC + 4;
        NEXT_STATE.PC = ID_IF.PC;
    }
    else
    {
        printf(" STALL \n");
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
    /*IMPLEMENT THIS*/

    uint32_t address = 0;
    int i = 0;

    while(i<PROGRAM_SIZE)
    {
        address = MEM_TEXT_BEGIN + (i*4);
        printf("[0x%08x]\t", address);
        print_instruction(address);
        i++;
    }
}

/************************************************************/
/* Print the current pipeline                                                                                    */
/************************************************************/
void show_pipeline(){
    /*IMPLEMENT THIS*/

    printf("\nCurrent PC:   %x\n", CURRENT_STATE.PC);
    printf("IF/ID.IR      %x\n", ID_IF.IR);
    printf("IF/ID.PC      %x\n\n", ID_IF.PC);
    printf("ID/EX.IR      %x\n", IF_EX.IR);
    printf("ID/EX.A       %x\n", IF_EX.A);
    printf("ID/EX.B       %x\n", IF_EX.B);
    printf("ID/EX.imm     %x\n\n", IF_EX.imm);
    printf("EX/MEM.IR     %x\n", EX_MEM.IR);
    printf("EX/MEM.A     %x\n", EX_MEM.A);
    printf("EX/MEM.B     %x\n", EX_MEM.B);
    printf("EX/MEM.ALUOutput %x\n\n", EX_MEM.ALUOutput);
    printf("MEM/WB.IR     %x\n", MEM_WB.IR);
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#include "mu-mips.h"

/***************************************************************/
/* Print out a list of commands available                                                                  */
void print_instruction(uint32_t addr);

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
/*Help Extend the register from 16 bits to 32 bits                                                                                         */
/************************************************************/

uint32_t extend_sign( uint32_t im )
{
    uint32_t data = ( im & 0x0000FFFF );
    uint32_t mask = 0x00008000;
    if ( mask & data )
    {
        data = data | 0xFFFF0000;
    }

    return data;
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
    /*Zach Taylor*/
    uint32_t rt = (0x001F0000 & MEM_WB.IR) >> 16;
    uint32_t rd = (0x0000F800 & MEM_WB.IR) >> 11;

    ++INSTRUCTION_COUNT;

    switch(MEM_WB.type)
    {
        case (0):
            //printf(" rd : % " PRIu32, rd);
            NEXT_STATE.REGS[rd] = MEM_WB.ALUOutput;
            break;
        case 1:
            //printf(" rt : % " PRIu32, rt);
            NEXT_STATE.REGS[rt] = MEM_WB.ALUOutput;
            break;
        case 2:
            //printf(" rt : % " PRIu32, rt);
            NEXT_STATE.REGS[rt] = MEM_WB.LMD;
        case 3:
           // printf(MEM_WB.ALUOutput);
            break;
        case 4:
            RUN_FLAG = FALSE;
            break;
    }

}

/************************************************************/
/* memory access (MEM) pipeline stage:                           */
/************************************************************/
void MEM()
{
    /*Zach Taylor*/
    MEM_WB.PC = EX_MEM.PC;
    if( EX_MEM.IR == 0x0 )
    {
        MEM_WB.IR = EX_MEM.IR;
        MEM_WB.ALUOutput = 0x0;
        return;
    }

    MEM_WB.IR = EX_MEM.IR;
    MEM_WB.type = EX_MEM.type;

    if(EX_MEM.type <= 1)
    {
        MEM_WB.ALUOutput = EX_MEM.ALUOutput;
    }
    if(EX_MEM.type <= 2)
    {
        MEM_WB.LMD = mem_read_32(EX_MEM.ALUOutput);
        MEM_WB.ALUOutput = EX_MEM.ALUOutput;
    }
    if(EX_MEM.type <= 3)
    {

        printf("\nEX_MEM.ALUOutput : %" PRIu32, EX_MEM.ALUOutput);
        mem_write_32(EX_MEM.ALUOutput, EX_MEM.B);
        MEM_WB.ALUOutput = EX_MEM.ALUOutput;
        //printf(EX_MEM.ALUOutput);
        printf("\n");
    }
}

/************************************************************/
/* execution (EX) pipeline stage:                                                                          */
/************************************************************/
void EX()
{
    //if there is no intruction to be executed, just skip the rest. This prevents SLL (which has opcode 0x00) from running. V IMPORTANT
    if( ID_EX.IR == 0x0 )
    {
        EX_MEM.IR = ID_EX.IR;
        EX_MEM.ALUOutput = 0x0;
        return;
    }

    /*Zach Taylor*/

    //Load Instruction from Buffer
    EX_MEM.PC = ID_EX.PC;
    EX_MEM.IR = ID_EX.IR;
    uint32_t opcode = ( 0xFC000000 & ID_EX.IR  );
    uint32_t imm = ID_EX.imm;
    uint32_t func = (ID_EX.IR & 0x0000003F);

    switch(opcode)
    {
        //R-Type
        case 0x00000000:
        {
            EX_MEM.type = 0;

            switch (func)
            {
                case 0x20:
                    puts ("ADD");
                    EX_MEM.ALUOutput = ID_EX.A + ID_EX.B;
                    break;

                case 0x21:
                    puts ("ADDU");
                    EX_MEM.ALUOutput = ID_EX.A + ID_EX.B;
                    break;

                case 0x22:
                    puts ("SUB");
                    EX_MEM.ALUOutput = ID_EX.A - ID_EX.B;
                    break;

                case 0x23:
                    puts ("SUBU");
                    EX_MEM.ALUOutput = ID_EX.A - ID_EX.B;
                    break;

                case 0x18:
                    puts ("MULT");
                    EX_MEM.ALUOutput = ID_EX.A * ID_EX.B;
                    break;

                case 0x19:
                    puts ("MULTU");
                    EX_MEM.ALUOutput = ID_EX.A * ID_EX.B;
                    break;

                case 0x1A:
                    puts ("DIV");
                    EX_MEM.ALUOutput = ID_EX.A / ID_EX.B;
                    break;

                case 0x1B:
                    puts ("DIVU");
                    EX_MEM.ALUOutput = ID_EX.A / ID_EX.B;
                    break;

                case 0x24:
                    puts ("AND");
                    EX_MEM.ALUOutput = ID_EX.A & ID_EX.B;
                    break;

                case 0x25:
                    puts ("OR");
                    EX_MEM.ALUOutput = ID_EX.A | ID_EX.B;
                    break;

                case 0x26:
                    puts ("XOR");
                    EX_MEM.ALUOutput = ID_EX.A ^ ID_EX.B;
                    break;

                case 0x27:
                    puts ("NOR");
                    EX_MEM.ALUOutput = ~( ID_EX.A | ID_EX.B );
                    break;

                case 0x2A:
                {
                    puts ("SLT");
                    if( ID_EX.A < ID_EX.B )
                    {
                        EX_MEM.ALUOutput = 0x00000001;
                    }
                    else
                    {
                        EX_MEM.ALUOutput = 0x00000000;
                    }
                    break;
                }
                case 0x00000000:
                {
                    puts ("SLL");
                    uint32_t sa = imm >> 6;
                    EX_MEM.ALUOutput = ID_EX.B << sa;
                    break;
                }
                case 0x02:
                {
                    puts ("SRL");
                    uint32_t sa = imm >> 6;
                    EX_MEM.ALUOutput = ID_EX.B >> sa;
                    break;
                }

                case 0x03:
                {
                    puts ("SRA");
                    uint32_t sa = imm >> 6;
                    EX_MEM.ALUOutput = extend_sign( ID_EX.B >> sa);
                    break;
                }

                case 0x0C: {
                    puts("SYSCALL");
                    EX_MEM.type = 4;
                    break;
                }
                case 0x13:
                    puts ("MTLO");
                    EX_MEM.LO = ID_EX.A ;
                    break;

                case 0x11:
                    puts ("MTHI");
                    EX_MEM.HI = ID_EX.A ;
                    break;

                case 0x12:
                    puts ("MFLO");
                    EX_MEM.ALUOutput = EX_MEM.LO ;
                    break;

                case 0x10:
                    puts ("MFHI");
                    EX_MEM.ALUOutput =  EX_MEM.HI;
                    break;

            }
            break;

        }
        default:
        {
            switch( opcode )
            {
                case 0x20000000:
                    //don't remove 0's to the RIGHT of numbers, because they change the value. 0's to the left can be removed as desired.
                    puts ("ADDI");
                    EX_MEM.ALUOutput = ID_EX.imm + ID_EX.A;
                    EX_MEM.type = 1;
                    break;

                case 0x24000000:
                    puts ("ADDIU");
                    EX_MEM.ALUOutput = ID_EX.imm + ID_EX.A;
                    EX_MEM.type = 1;
                    break;

                case 0x30000000:
                    puts ("ANDI");
                    EX_MEM.ALUOutput = ( ID_EX.imm & 0x0000FFFF ) & ID_EX.A;
                    EX_MEM.type = 1;
                    break;

                case 0x3C000000:
                    puts ("LUI");
                    EX_MEM.ALUOutput = ID_EX.imm << 16;
                    EX_MEM.type = 1;
                    break;

                case 0x38000000:
                    puts ("XORI");
                    EX_MEM.ALUOutput = ( ID_EX.imm & 0x0000FFFF ) ^ ID_EX.A;
                    EX_MEM.type = 1;
                    break;

                case 0x34000000:
                    puts ("ORI");
                    EX_MEM.ALUOutput = ( ID_EX.imm & 0x0000FFFF ) | ID_EX.A;
                    EX_MEM.type = 1;
                    break;

                case 0xA0000000:
                {
                    puts ("SB");
                    uint32_t eAddr = ID_EX.A + ID_EX.imm;
                    EX_MEM.ALUOutput = eAddr;
                    EX_MEM.B = ID_EX.B;
                    EX_MEM.type = 3;
                    break;
                }

                case 0xAC000000:
                {
                    puts ("SW");
                    uint32_t eAddr = ID_EX.A + ID_EX.imm;
                    EX_MEM.ALUOutput = eAddr;
                    EX_MEM.B = ID_EX.B;
                    EX_MEM.type = 3;
                    break;
                }

                case 0xA4000000:
                {
                    puts ("SH");
                    uint32_t eAddr = ID_EX.A + ID_EX.imm;
                    EX_MEM.ALUOutput = eAddr;
                    EX_MEM.B = ID_EX.B;
                    EX_MEM.type = 3;
                    break;
                }

                case 0x8C000000:
                {
                    puts ("LB");
                    uint32_t eAddr = ID_EX.A + ID_EX.imm;
                    EX_MEM.ALUOutput = eAddr;
                    EX_MEM.B = ID_EX.B;
                    EX_MEM.type = 2;
                    break;
                }

                case 0x80000000:
                {
                    puts ("LW");
                    uint32_t eAddr = ID_EX.A + ID_EX.imm;
                    EX_MEM.ALUOutput = eAddr;
                    EX_MEM.B = ID_EX.B;
                    EX_MEM.type = 2;
                    break;
                }

                case 0x84000000:
                {
                    puts ("LH");
                    uint32_t eAddr = ID_EX.A + ID_EX.imm;
                    EX_MEM.ALUOutput = eAddr;
                    EX_MEM.B = ID_EX.B;
                    EX_MEM.type = 2;
                    break;
                }
            }
        }
    }

}

/************************************************************/
/* instruction decode (ID) pipeline stage:                                                         */
/************************************************************/
void ID()
{
    /*OLA*/
    ID_EX.IR = IF_ID.IR;

    uint32_t rs = ( 0x03E00000 & IF_ID.IR  ) >> 21;
    uint32_t rt = ( 0x001F0000 & IF_ID.IR  ) >> 16;
    uint32_t imm = ( 0x0000FFFF & IF_ID.IR  );

    //Load data in ID->EX Buffer

    ID_EX.PC = IF_ID.PC;
    ID_EX.A = CURRENT_STATE.REGS[rs];
    ID_EX.B = CURRENT_STATE.REGS[rt];
    ID_EX.imm = extend_sign( imm );
    ID_EX.LO = CURRENT_STATE.LO;
    ID_EX.HI = CURRENT_STATE.HI;

}

/************************************************************/
/* instruction fetch (IF) pipeline stage:                                                              */
/************************************************************/
void IF()
{
    /*Ola*/
    uint32_t instruction = mem_read_32( CURRENT_STATE.PC );

    IF_ID.IR = instruction;
    NEXT_STATE.PC = CURRENT_STATE.PC + 0x4;
    IF_ID.PC = NEXT_STATE.PC;
}


/************************************************************/
/* Initialize Memory                                                                                                    */
/************************************************************/
void initialize(){
    init_memory();
    CURRENT_STATE.PC = MEM_TEXT_BEGIN;
    NEXT_STATE = CURRENT_STATE;
    RUN_FLAG = TRUE;
}
void print_program(){
    int i;
    uint32_t addr;

    for(i=0; i<PROGRAM_SIZE; i++){
        addr = MEM_TEXT_BEGIN + (i*4);
        printf("[0x%x]\t", addr);
        print_instruction(addr);
    }
}

void print_instruction(uint32_t addr) {

/************************************************************/
/* Print the program loaded into memory (in MIPS assembly format)    */
/************************************************************/
    uint32_t instruction, opcode, function, rs, rt, rd, sa, immediate, target;

    instruction = mem_read_32(addr);

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
                printf("SLL $r%u, $r%u, 0x%x\n", rd, rt, sa);
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
    /*IMPLEMENT THIS*/
}

/************************************************************/
/* Print the current pipeline                                                                                    */
/************************************************************/
void show_pipeline(){
    /*Mostly OLA*/
    printf("Current PC: %" PRIu32, CURRENT_STATE.PC);
    //printf("\nIF_ID.IR : %" PRIu32, IF_ID.IR);
    printf("\n");
    print_instruction(IF_ID.PC);
    printf("\n");
    printf("\nIF_ID.PC : %" PRIu32, IF_ID.PC);
    //printf("\n\nID_EX.IR : %"PRIu32, ID_EX.IR);
    printf("\n");
    print_instruction(ID_EX.PC);
    printf("\n");
    printf("\nID_EX.A : %" PRIu32, ID_EX.A);
    printf("\nID_EX.B : %" PRIu32, ID_EX.B);
    printf("\nID_EX.imm : %" PRIu32, ID_EX.imm);
    //printf("\n\nEX_MEM.IR : %"PRIu32, EX_MEM.IR);
    printf("\n");
    print_instruction(EX_MEM.PC);
    printf("\n");
    printf("\nEM_MEM.A : %"PRIu32, EX_MEM.A);
    printf("\nEX_MEM.B : %"PRIu32, EX_MEM.B);
    printf("\nEX_MEM.ALUOutput : %"PRIu32, EX_MEM.ALUOutput);
    //printf("\n\nMEM_WB.IR : %" PRIu32, MEM_WB.IR);
    printf("\n");
    print_instruction(MEM_WB.PC);
    printf("\nMEM_WB.ALUOutput : %" PRIu32, MEM_WB.ALUOutput);
    printf("\nMEM_WB.LMD : %" PRIu32, MEM_WB.LMD);
    printf("\n\n");

}

/***************************************************************/
/* main                                                                                                                                   */
/***************************************************************/
int main(int argc, char *argv[])
{
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

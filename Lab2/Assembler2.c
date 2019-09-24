//
// Created by Zach Taylor on 9/23/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t conversion(char* token);
char* get_offset(char* token);
uint32_t get_base(char* token);

int main(int argc, char* argv[]){
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* token;
    uint32_t output = 0;
    char instruction[] = "";
    uint32_t rs = 0;
    uint32_t rd = 0;
    uint32_t rt = 0;
    uint32_t immediate = 0;
    uint32_t base = 0;
    uint32_t offset = 0;
    uint32_t sa = 0;

    if(argc != 2){
        printf("Please give the input file\n");
        return 0;
    }
    FILE* fp2 = fopen(argv[1], "r");

    FILE* fp = fopen("output.in", "w+");

    if(fp2 == NULL){
        printf("Could not open file\n");
        return 0;
    }

    while((read = getline(&line, &len, fp2)) != -1){
        token = "";
        token = strtok(line, " ");
        output = 0;
        rs = 0;
        rd = 0;
        rt = 0;
        sa = 0;
        immediate = 0;
        base = 0;
        offset = 0;
        if(strcmp(token, "addiu") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            output = (0x09 << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "add") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x20;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "addu") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x21;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "sub") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x22;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "subu") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x23;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "bne") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16) >> 2;
            output = 0x00000000;
            output = (0x05 << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "beq") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x04 << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "j") == 0){
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x02 << 26) | (immediate & 0x03ffffff);
            printf("intsruction: %08x\n", output);
        }
        if(strcmp(token, "jal") == 0){
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x03 << 26) | (immediate & 0x03ffffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "jalr") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            output = 0x00000000;
            output = (rs << 21) | (0x09);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "jr") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            output = 0x00000000;
            output = (rs << 21) | (0x08);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "syscall") == 0){
            fprintf(fp,"%08x\n", 0x0000000c);
        }
        if(strcmp(token, "addi") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            output = (0x08 << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "andi") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            output = (0x0c << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "mult") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + 0x18;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "multu") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + 0x19;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "xor") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x26;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "nor") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x27;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "slt") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x2a;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "sll") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            sa = conversion(token);
            output = 0x00000000;
            output = (0x00 << 21) + (rt << 16) + (sa << 6) + (rd << 11) + 0x00;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "srl") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            sa = conversion(token);
            output = 0x00000000;
            output = (0x00 << 21) + (rt << 16) + (sa << 6) + (rd << 11) + 0x02;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "sra") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            sa = conversion(token);
            output = 0x00000000;
            output = (0x00 << 21) + (rt << 16) + (sa << 6) + (rd << 11) + 0x03;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "slti") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            output = (0x0a << 26) + (rt << 16) + (rs << 21) + immediate;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "div") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + 0x1a;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "or") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x25;
            fprintf(fp,"%08x\n", output);
        }

        if(strcmp(token, "divu") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + 0x1b;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "and") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            rt = conversion(token);
            output = 0x00000000;
            output = (0x00 << 26) + (rt << 16) + (rs << 21) + (rd << 11) + 0x24;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "xori") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            output = (0x0e << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "ori") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            output = (0x0d << 26) | (rt << 16) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "lw") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            base = get_base(token);
            offset = (uint32_t)strtol(get_offset(token), NULL, 16);
            output = 0x00000000;
            output = (0x23 << 26) | (rt << 16) | (base << 21) | (offset & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "lh") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            base = get_base(token);
            offset = (uint32_t)strtol(get_offset(token), NULL, 16);
            output = 0x00000000;
            output = (0x21 << 26) | (rt << 16) | (base << 21) | (offset & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "lb") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            base = get_base(token);
            offset = (uint32_t)strtol(get_offset(token), NULL, 16);
            output = 0x00000000;
            output = (0x20 << 26) | (rt << 16) | (base << 21) | (offset & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "sw") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            base = get_base(token);
            offset = (uint32_t)strtol(get_offset(token), NULL, 16);
            output = 0x00000000;
            output = (0x2b << 26) | (rt << 16) | (base << 21) | (offset & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "sh") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            base = get_base(token);
            offset = (uint32_t)strtol(get_offset(token), NULL, 16);
            output = 0x00000000;
            output = (0x29 << 26) | (rt << 16) | (base << 21) | (offset & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "sb") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            base = get_base(token);
            offset = (uint32_t)strtol(get_offset(token), NULL, 16);
            output = 0x00000000;
            output = (0x29 << 26) | (rt << 16) | (base << 21) | (offset & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "lui") == 0){
            token = strtok(NULL, " ");
            rt = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16);
            output = 0x00000000;
            rs = 0x00;
            output = (0x0f << 26) + (rt << 16) + (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "mfhi") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            output = 0x00000000;
            output = (rd << 11) | 0x10;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "mflo") == 0){
            token = strtok(NULL, " ");
            rd = conversion(token);
            output = 0x00000000;
            output = (rd << 11) | 0x12;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "mthi") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            output = 0x00000000;
            output = (rs << 21) | 0x11;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "mtlo") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            output = 0x00000000;
            output = (rs << 21) | 0x13;
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "blez") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x06 << 26) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "bgtz") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x07 << 26) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "bltz") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x01 << 26) | (rs << 21) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
        if(strcmp(token, "bgez") == 0){
            token = strtok(NULL, " ");
            rs = conversion(token);
            token = strtok(NULL, " ");
            immediate = (uint32_t)strtol(token, NULL, 16)  >> 2;
            output = 0x00000000;
            output = (0x01 << 26) | (rs << 21) | (0x01 << 16) | (immediate & 0x0000ffff);
            fprintf(fp,"%08x\n", output);
        }
    }
    fclose(fp);
    if(line){
        free(line);
    }
    return 0;
}

uint32_t conversion(char* token){
    int length = strlen(token);
    if(*(token +(length-1)) == ','){
        *(token +(length-1)) ='\0';
    }
    else{
        if(*(token +(length-1)) == '\n'){
            *(token +(length-1)) ='\0';
        }
    }
    if(strcmp(token, "$zero") == 0)
    {
        return 0;
    }
    if(strcmp(token, "$at") == 0)
    {
        return 1;
    }
    if(strcmp(token, "$v0") == 0)
    {
        return 2;
    }
    if(strcmp(token, "$v1") == 0)
    {
        return 3;
    }
    if(strcmp(token, "$a0") == 0)
    {
        return 4;
    }
    if(strcmp(token, "$a1") == 0)
    {
        return 5;
    }
    if(strcmp(token, "$a2") == 0)
    {
        return 6;
    }
    if(strcmp(token, "$a3") == 0)
    {
        return 7;
    }
    if(strcmp(token, "$t0") == 0)
    {
        return 8;
    }
    if(strcmp(token, "$t1") == 0)
    {
        return 9;
    }
    if(strcmp(token, "$t2") == 0)
    {
        return 10;
    }
    if(strcmp(token, "$t3") == 0)
    {
        return 11;
    }
    if(strcmp(token, "$t4") == 0)
    {
        return 12;
    }
    if(strcmp(token, "$t5") == 0)
    {
        return 13;
    }
    if(strcmp(token, "$t6") == 0)
    {
        return 14;
    }
    if(strcmp(token, "$t7") == 0)
    {
        return 15;
    }
    if(strcmp(token, "$s0") == 0)
    {
        return 16;
    }
    if(strcmp(token, "$s1") == 0)
    {
        return 17;
    }
    if(strcmp(token, "$s2") == 0)
    {
        return 18;
    }
    if(strcmp(token, "$s3") == 0)
    {
        return 19;
    }
    if(strcmp(token, "$s4") == 0)
    {
        return 20;
    }
    if(strcmp(token, "$s5") == 0)
    {
        return 21;
    }
    if(strcmp(token, "$s6") == 0)
    {
        return 22;
    }
    if(strcmp(token, "$s7") == 0)
    {
        return 23;
    }
    if(strcmp(token, "$t8") == 0)
    {
        return 24;
    }
    if(strcmp(token, "$t9") == 0)
    {
        return 25;
    }
    if(strcmp(token, "$k0") == 0)
    {
        return 26;
    }
    if(strcmp(token, "$k1") == 0)
    {
        return 27;
    }
    if(strcmp(token, "$gp") == 0)
    {
        return 28;
    }
    if(strcmp(token, "$sp") == 0)
    {
        return 29;
    }
    if(strcmp(token, "$fp") == 0)
    {
        return 30;
    }
    if(strcmp(token, "$ra") == 0)
    {
        return 31;
    }
    return 0;
}

char* get_offset(char token[]){
    int i = 0;
    while(token[i] != '('){
        i++;
    }
    token[i] = '\0';
    return token;
}
uint32_t get_base(char token[]){
    char temp[4];
    int i = 0;
    int j = 0;
    while(token[i] != '('){
        i++;
    }
    i++;
    while(token[i] != ')'){
        temp[j] = token[i];
        i++;
        j++;
    }
    temp[3] = '\0';
    return conversion(temp);
}


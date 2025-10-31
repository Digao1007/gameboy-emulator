#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.hh"

typedef union {
    struct{
        uint8_t F,A;
    };
    uint16_t AF;

}REGISTER_AF;

typedef union {
    struct{
        uint8_t C,B;
    };
    uint16_t BC;

}REGISTER_BC;

typedef union {
    struct{
        uint8_t E,D;
    };
    uint16_t DE;

}REGISTER_DE;

typedef union {
    struct{
        uint8_t L,H;
    };
    uint16_t HL;

}REGISTER_HL;

typedef struct{
    REGISTER_AF af;
    REGISTER_BC bc;
    REGISTER_DE de;
    REGISTER_HL hl;

    uint16_t PC;
    uint16_t SP;
}CPU;

#define FLAG_Z (1 << 7)
#define FLAG_N (1 << 6)
#define FLAG_H (1 << 5)
#define FLAG_C (1 << 4)

static inline void set_flag (CPU *cpu, uint8_t flag, bool value){
    if (value){
        cpu-> af.F |= flag; //liga o bit
    }
    else{
        cpu-> af.F &= ~flag; // desliga o bit
    }
}

uint8_t get_flag(CPU *cpu, uint8_t flag) {
    return (cpu->af.F & flag) != 0;
}

//declaracao das funcoes opcodes
void (*opcodes[256]) (CPU *cpu);


void op_NOP(CPU *cpu){

}
void op_LD_BC_d16(CPU *cpu){
    uint16_t value = read16(cpu->PC);
    cpu->PC += 2;
    cpu->bc.BC = value;
}
void op_LD_BCm_A(CPU *cpu){
    write8(cpu->bc.BC, cpu->af.A);
}
void op_inc_bc(CPU *cpu){
    cpu->bc.BC++;
}
void op_inc_b(CPU *cpu){
    uint8_t value = cpu->bc.B;
    uint8_t result = value + 1;
    
    cpu->bc.B = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
}
void op_dec_b(CPU *cpu){
    uint8_t value = cpu->bc.B;
    uint8_t result = value - 1;
    
    cpu->bc.B = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_B_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->bc.B = value;
}
void op_RLCA(CPU *cpu){
    uint8_t a = cpu->af.A;
    uint8_t bit7 = (a >> 7) & 1;
    a = (a << 1) | bit7; 
    cpu->af.A = a;

    
    set_flag(cpu, FLAG_Z, false);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, false);
    set_flag(cpu, FLAG_C, bit7);
}
void op_LD_a16_SP(CPU *cpu){
    uint16_t addr = read16(cpu->PC);
    cpu -> PC += 2;
    write8(addr, cpu->SP & 0xFF);
    write8(addr + 1, cpu->SP >> 8);
}
void op_add_HL_BC(CPU *cpu) {
    uint32_t result = cpu->hl.HL + cpu->bc.BC;

    
    set_flag(cpu, FLAG_N, false);   
    set_flag(cpu, FLAG_H, ((cpu->hl.HL & 0x0FFF) + (cpu->bc.BC & 0x0FFF)) > 0x0FFF);
    set_flag(cpu, FLAG_C, result > 0xFFFF);

    cpu->hl.HL = (uint16_t)result;
}
void op_LD_A_BCm(CPU *cpu){
    uint8_t value = read8(cpu->bc.BC);
    cpu->af.A = value;

}
void op_dec_BC(CPU *cpu){
    cpu->bc.BC--;
}
void op_inc_C(CPU *cpu){
    uint8_t value = cpu->bc.C;
    uint8_t result = value + 1;
    
    cpu->bc.C = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
}
void op_dec_C(CPU *cpu){
    uint8_t value = cpu->bc.C;
    uint8_t result = value - 1;
    
    cpu->bc.C = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_C_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->bc.C = value;
}
void op_RRCA(CPU *cpu){
    uint8_t a = cpu->af.A;
    uint8_t carry = a & 0x01;
    cpu->af.A = (a >> 1) | (carry << 7); 

    set_flag(cpu, FLAG_Z, false);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, false);
    set_flag(cpu, FLAG_C, carry);
}
void initops(){
    opcodes[0x00] = op_NOP;
    opcodes[0x01] = op_LD_BC_d16;
    opcodes[0x02] = op_LD_BCm_A;
    opcodes[0x03] = op_inc_bc;
    opcodes[0x04] = op_inc_b;
    opcodes[0x05] = op_dec_b;
    opcodes[0x06] = op_LD_B_d8;
    opcodes[0x07] = op_RLCA;
    opcodes[0x08] = op_LD_a16_SP;
    opcodes[0x09] = op_add_HL_BC;
    opcodes[0x0A] = op_LD_A_BCm;
    opcodes[0x0B] = op_dec_BC;
    opcodes[0x0C] = op_inc_C;
    opcodes[0x0D] = op_dec_C;
    opcodes[0x0E] = op_LD_C_d8;
    opcodes[0x0F] = op_RRCA;
}


//fim opcodes


void cicle(CPU *cpu){
    uint8_t opcode = read8(cpu->PC++);
    opcodes[opcode](cpu);
}



void test_LD_BCm_A() {
    CPU cpu = {0}; 
    cpu.bc.BC = 0xC000;  
    cpu.af.A  = 0x42;   
    op_LD_BCm_A(&cpu);

   
    uint8_t mem_value = read8(0xC000);

    printf("0x42, Memória[0xC000] = 0x%02X\n", mem_value);
}

int main(){
    CPU cpu = {0};
    
    cpu.af.A = 0x12;
    cpu.af.F = 0x34;

    printf("AF = %04X\n", cpu.af.AF); // imprime 1234 

    set_flag(&cpu, FLAG_Z, true);
    printf("F (com Z ligado) = %02X\n", cpu.af.F); //B4

    test_LD_BCm_A();
    return 0;
}

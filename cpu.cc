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
    cpu->bc.BC = value;
}
void op_LD_BCm_A(CPU *cpu){
    write8(cpu->bc.BC, cpu->af.A);
}

void initops(){
    opcodes[0x00] = op_NOP;
    opcodes[0x01] = op_LD_BC_d16;
    opcodes[0x02] = op_LD_BCm_A;
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

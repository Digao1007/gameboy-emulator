#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.hh"
#include "cpu.hh"





void set_flag (CPU *cpu, uint8_t flag, bool value){
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
void op_stop(CPU *cpu){
    cpu->stop = true;
}
void op_LD_DE_d16(CPU *cpu){
    uint16_t value = read16(cpu->PC);
    cpu->PC += 2;
    cpu->de.DE = value;
}
void op_LD_DEm_A(CPU *cpu){
    write8(cpu->de.DE, cpu->af.A);
}
void op_inc_DE(CPU *cpu){
    cpu->de.DE++;
}
void op_inc_D(CPU *cpu){
    uint8_t value = cpu->de.D;
    uint8_t result = value + 1;
    
    cpu->de.D = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
}
void op_dec_D(CPU *cpu){
    uint8_t value = cpu->de.D;
    uint8_t result = value - 1;
    
    cpu->de.D = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_D_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->de.D = value;
}
void op_RLA(CPU *cpu){
    uint8_t carry_in = (cpu->af.F & 0x10) ? 1 : 0;
    uint8_t bit7 = (cpu->af.A & 0x80) >> 7;
    cpu->af.A = (cpu->af.A << 1) | carry_in;
    cpu->af.F = 0;
    if (bit7) cpu->af.F |= 0x10;
}
void op_JR_r8(CPU *cpu) {
    int8_t offset = (int8_t)read8(cpu->PC);
    cpu->PC += 1; 
    cpu->PC += offset;
}
void op_add_HL_DE(CPU *cpu){
    uint32_t result = cpu->hl.HL + cpu->de.DE;

    
    set_flag(cpu, FLAG_N, false);   
    set_flag(cpu, FLAG_H, ((cpu->hl.HL & 0x0FFF) + (cpu->de.DE & 0x0FFF)) > 0x0FFF);
    set_flag(cpu, FLAG_C, result > 0xFFFF);

    cpu->hl.HL = (uint16_t)result;
}
void op_LD_A_DEm(CPU *cpu){
    uint8_t value = read8(cpu->de.DE);
    cpu->af.A = value;
}
void op_dec_DE(CPU *cpu){
    cpu->de.DE--;
}
void op_inc_E(CPU *cpu){
    uint8_t value = cpu->de.E;
    uint8_t result = value + 1;
    
    cpu->de.E = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
}
void op_dec_E(CPU *cpu){
    uint8_t value = cpu->de.E;
    uint8_t result = value - 1;
    
    cpu->de.E = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_E_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->de.E = value;
}
void op_RRA(CPU *cpu){
    uint8_t carry_in = (cpu->af.F & 0x10) ? 1 : 0;
    uint8_t bit0 = cpu->af.A & 1;
    cpu->af.A = (cpu->af.A >> 1) | (carry_in << 7);
    cpu->af.F = 0;
    if (bit0) cpu->af.F |= 0x10;
}
void op_JR_NZ_r8(CPU *cpu){
    int8_t offset = (int8_t)read8(cpu->PC); 
    cpu->PC++;
    if(!get_flag(cpu, FLAG_Z)){
        cpu->PC += offset;
    }
}
void op_LD_HL_d16(CPU *cpu){
    uint16_t value = read16(cpu->PC);
    cpu->PC += 2;
    cpu->hl.HL = value;
}
void op_LD_HLmore_A(CPU *cpu){
    write8(cpu->bc.BC, cpu->af.A);
    cpu->hl.HL++;
}
void op_inc_HL(CPU *cpu){
    cpu->hl.HL++;
}
void op_inc_H(CPU *cpu){
    uint8_t value = cpu->hl.H;
    uint8_t result = value + 1;
    
    cpu->hl.H = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
}
void op_dec_H(CPU *cpu){
    uint8_t value = cpu->hl.H;
    uint8_t result = value - 1;
    
    cpu->hl.H = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_H_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->hl.H = value;
}
void op_DAA(CPU *cpu) {
    uint8_t A = cpu->af.A;
    uint8_t correction = 0;
    bool setC = false;

    // subtracao 
    if (get_flag(cpu, FLAG_N)) {
        if (get_flag(cpu, FLAG_H)) correction |= 0x06;
        if (get_flag(cpu, FLAG_C)) correction |= 0x60;
        A -= correction;
    } else { // soma
        if (get_flag(cpu, FLAG_H) || (A & 0x0F) > 9) correction |= 0x06;
        if (get_flag(cpu, FLAG_C) || A > 0x99) {
            correction |= 0x60;
            setC = true;
        }
        A += correction;
    }

    
    set_flag(cpu, FLAG_Z, A == 0);  
    set_flag(cpu, FLAG_H, false);   
    set_flag(cpu, FLAG_C, setC);    

    cpu->af.A = A;
}
void op_JR_Z_r8(CPU *cpu){
    int8_t offset = (int8_t)read8(cpu->PC); 
    cpu->PC++;
    if(get_flag(cpu, FLAG_Z)){
        cpu->PC += offset;
    }
}
void op_add_HL_HL(CPU *cpu){
    uint32_t result = cpu->hl.HL + cpu->hl.HL;

    
    set_flag(cpu, FLAG_N, false);   
    set_flag(cpu, FLAG_H, ((cpu->hl.HL & 0x0FFF) + (cpu->hl.HL & 0x0FFF)) > 0x0FFF);
    set_flag(cpu, FLAG_C, result > 0xFFFF);

    cpu->hl.HL = (uint16_t)result;
}
void op_LD_A_Hlmore(CPU*cpu){
    write8(cpu->af.A, cpu->hl.HL);
    cpu->hl.HL++;
}
void op_dec_HL(CPU *cpu){
    cpu->hl.HL--;
}
void op_inc_L(CPU *cpu){
    uint8_t value = cpu->hl.L;
    uint8_t result = value + 1;
    
    cpu->hl.L = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
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
    opcodes[0x10] = op_stop;
    opcodes[0x11] = op_LD_DE_d16;
    opcodes[0x12] = op_LD_DEm_A;
    opcodes[0x13] = op_inc_DE;
    opcodes[0x14] = op_inc_D;
    opcodes[0x15] = op_dec_D;
    opcodes[0x16] = op_LD_D_d8;
    opcodes[0x17] = op_RLA;
    opcodes[0x18] = op_JR_r8;
    opcodes[0x19] = op_add_HL_DE;
    opcodes[0x1A] = op_LD_A_DEm;
    opcodes[0x1B] = op_dec_DE;
    opcodes[0x1C] = op_inc_E;
    opcodes[0x1D] = op_dec_E;
    opcodes[0x1E] = op_LD_E_d8;
    opcodes[0x1F] = op_RRA;
    opcodes[0x20] = op_JR_NZ_r8;
    opcodes[0x21] = op_LD_HL_d16;
    opcodes[0x22] = op_LD_HLmore_A;
    opcodes[0x23] = op_inc_HL;
    opcodes[0x24] = op_inc_H;
    opcodes[0x25] = op_dec_H;
    opcodes[0x26] = op_LD_H_d8;
    opcodes[0x27] = op_DAA;
    opcodes[0x28] = op_JR_Z_r8;
    opcodes[0x29] = op_add_HL_HL;
    opcodes[0x2A] = op_LD_A_Hlmore;
    opcodes[0x2B] = op_dec_HL;
    opcodes[0x2C] = op_inc_L;
    for (int i = 0x2D; i <= 0xFF; i++) {
        opcodes[i] = op_NOP;
    }
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

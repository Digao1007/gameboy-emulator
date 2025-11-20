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
void op_dec_L(CPU *cpu){
    uint8_t value = cpu->hl.L;
    uint8_t result = value - 1;
    
    cpu->hl.L = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_L_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->hl.L = value;
}
void op_CPL(CPU *cpu){
    cpu->af.A = ~cpu->af.A;  
    set_flag(cpu, FLAG_N, true); 
    set_flag(cpu, FLAG_H, true);    
}
void op_JR_NC_r8(CPU *cpu){
    int8_t offset = (int8_t)read8(cpu->PC); 
    cpu->PC++;
    if(!get_flag(cpu, FLAG_C)){
        cpu->PC += offset;
    }
}
void op_LD_SP_d16(CPU *cpu){
    uint16_t value = read16(cpu->PC);
    cpu->PC += 2;
    cpu->SP = value;
}
void op_HLminus_A(CPU *cpu){
    write8(cpu->af.A, cpu->hl.HL);
    cpu->hl.HL--;
}
void op_inc_SP(CPU *cpu){
    cpu->SP++;
}
void op_inc_HLmem(CPU *cpu){
    uint8_t value = read8(cpu->hl.HL);
    uint8_t result = value + 1;

    write8(cpu->hl.HL, result);

    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, 0);
    set_flag(cpu, FLAG_H, (value & 0x0F) + 1 > 0x0F);
}
void op_dec_HLmem(CPU *cpu){
    uint8_t value = read8(cpu->hl.HL);
    uint8_t result = value + 1;

    write8(cpu->hl.HL, result);

    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, 1);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0);
}
void op_LD_HLmem_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC++;
    write8(cpu->hl.HL, value);
    
}
void op_SCF(CPU *cpu){
    set_flag(cpu, FLAG_C, 1);
    set_flag(cpu, FLAG_H, 0);
    set_flag(cpu, FLAG_N, 0);
}
void op_JR_C_r8(CPU *cpu){
    int8_t offset = (int8_t)read8(cpu->PC); 
    cpu->PC++;
    if(get_flag(cpu, FLAG_C)){
        cpu->PC += offset;
    }
}
void op_add_HL_SP(CPU *cpu){
    uint32_t result = cpu->hl.HL + cpu->SP;

    
    set_flag(cpu, FLAG_N, false);   
    set_flag(cpu, FLAG_H, ((cpu->hl.HL & 0x0FFF) + (cpu->SP & 0x0FFF)) > 0x0FFF);
    set_flag(cpu, FLAG_C, result > 0xFFFF);

    cpu->hl.HL = (uint16_t)result;
}
void op_LD_A_HLminus(CPU *cpu){
    write8(cpu->af.A, cpu->hl.HL);
    cpu->hl.HL--;
}
void op_dec_SP(CPU *cpu){
    cpu->SP--;
}
void op_inc_A(CPU *cpu){
    int8_t value = cpu->af.A;
    uint8_t result = value + 1;
    
    cpu->af.A = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, false);
    set_flag(cpu, FLAG_H, ((value & 0x0F) + 1) > 0x0F);
}
void op_dec_A(CPU *cpu){
    uint8_t value = cpu->af.A;
    uint8_t result = value - 1;
    
    cpu->af.A = result;
    
    set_flag(cpu, FLAG_Z, result == 0);
    set_flag(cpu, FLAG_N, true);
    set_flag(cpu, FLAG_H, (value & 0x0F) == 0x00);
}
void op_LD_A_d8(CPU *cpu){
    uint8_t value = read8(cpu->PC);
    cpu->PC += 1;
    cpu->bc.B = value;
}
void op_CCF(CPU *cpu){
    set_flag(cpu, FLAG_C, !get_flag(cpu, FLAG_C));
    set_flag(cpu, FLAG_N, 0);
    set_flag(cpu, FLAG_H, 0);
}
void op_LD_B_B(CPU *cpu){

}
void op_LD_B_C(CPU *cpu){
    cpu->bc.B = cpu->bc.C;
}
void op_LD_B_D(CPU *cpu){
    cpu->bc.B = cpu->de.D;
}
void op_LD_B_E(CPU *cpu){
    cpu->bc.B = cpu->de.E;
}
void op_LD_B_H(CPU *cpu){
    cpu->bc.B = cpu->hl.H;
}
void op_LD_B_L(CPU *cpu){
    cpu->bc.B = cpu->hl.L;
}
void op_LD_B_HLm(CPU *cpu){
    cpu->bc.B = read8(cpu->hl.HL);
}
void op_LD_B_A(CPU *cpu){
    cpu->bc.B = cpu->af.A;
}
void op_LD_C_B(CPU *cpu){
    cpu->bc.C = cpu->bc.B;
}
void op_LD_C_C(CPU *cpu){

}
void op_LD_C_D(CPU *cpu){
    cpu->bc.C = cpu->de.D;
}
void op_LD_C_E(CPU *cpu){
    cpu->bc.C = cpu->de.E;
}
void op_LD_C_H(CPU *cpu){
    cpu->bc.C = cpu->hl.H;
}
void op_LD_C_L(CPU *cpu){
    cpu->bc.C = cpu->hl.L;
}
void op_LD_C_HLm(CPU *cpu){
    cpu->bc.C = read8(cpu->hl.HL);
}
void op_LD_C_A(CPU *cpu){
    cpu->bc.C = cpu->af.A;
}
void op_LD_D_B(CPU *cpu){
    cpu->de.D = cpu->bc.B;
}
void op_LD_D_C(CPU *cpu){
    cpu->de.D = cpu->bc.C;
}
void op_LD_D_D(CPU *cpu){
    
}
void op_LD_D_E(CPU *cpu){
    cpu->de.D = cpu->de.E;
}
void op_LD_D_H(CPU *cpu){
    cpu->de.D = cpu->hl.H;
}
void op_LD_D_L(CPU *cpu){
    cpu->de.D = cpu->hl.L;
}
void op_LD_D_HLm(CPU *cpu){
    cpu->de.D = read8(cpu->hl.HL);
}
void op_LD_D_A(CPU *cpu){
    cpu->de.D = cpu->af.A;
}
void op_LD_E_B(CPU *cpu){
    cpu->de.E = cpu->bc.B;
}
void op_LD_E_C(CPU *cpu){
    cpu->de.E = cpu->bc.C;
}
void op_LD_E_D(CPU *cpu){
    cpu->de.E = cpu->de.D;
}
void op_LD_E_E(CPU *cpu){
    
}
void op_LD_E_H(CPU *cpu){
    cpu->de.E = cpu->hl.H;
}
void op_LD_E_L(CPU *cpu){
    cpu->de.E = cpu->hl.L;
}
void op_LD_E_HLm(CPU *cpu){
    cpu->de.E = read8(cpu->hl.HL);
}
void op_LD_E_A(CPU *cpu){
    cpu->de.E = cpu->af.A;
}
void op_LD_H_B(CPU *cpu){
    cpu->hl.H = cpu->bc.B;
}
void op_LD_H_C(CPU *cpu){
    cpu->hl.H = cpu->bc.C;
}
void op_LD_H_D(CPU *cpu){
    cpu->hl.H = cpu->de.D;
}
void op_LD_H_E(CPU *cpu){
    cpu->hl.H = cpu->de.E;
}
void op_LD_H_H(CPU *cpu){
    
}
void op_LD_H_L(CPU *cpu){
    cpu->hl.H = cpu->hl.L;
}
void op_LD_H_HLm(CPU *cpu){
    cpu->hl.H = read8(cpu->hl.HL);
}
void op_LD_H_A(CPU *cpu){
    cpu->hl.H = cpu->af.A;
}
void op_LD_L_B(CPU *cpu){
    cpu->hl.L = cpu->bc.B;
}
void op_LD_L_C(CPU *cpu){
    cpu->hl.L = cpu->bc.C;
}
void op_LD_L_D(CPU *cpu){
    cpu->hl.L = cpu->de.D;
}
void op_LD_L_E(CPU *cpu){
    cpu->hl.L = cpu->de.E;
}
void op_LD_L_H(CPU *cpu){
    cpu->hl.L = cpu->hl.H;
}
void op_LD_L_L(CPU *cpu){
    
}
void op_LD_L_HLm(CPU *cpu){
    cpu->hl.L = read8(cpu->hl.HL);
}
void op_LD_L_A(CPU *cpu){
    cpu->hl.L = cpu->af.A;
}
void op_LD_HLm_B(CPU *cpu){
    write8(cpu->hl.HL, cpu->bc.B);
}
void op_LD_HLm_C(CPU *cpu){
    write8(cpu->hl.HL, cpu->bc.C);
}
void op_LD_HLm_D(CPU *cpu){
    write8(cpu->hl.HL, cpu->de.D);
}
void op_LD_HLm_E(CPU *cpu){
    write8(cpu->hl.HL, cpu->de.E);
}
void op_LD_HLm_H(CPU *cpu){
    write8(cpu->hl.HL, cpu->hl.H);
}
void op_LD_HLm_L(CPU *cpu){
    write8(cpu->hl.HL, cpu->hl.L);
}
void op_HALT(CPU *cpu){
    cpu->halted = true;
}
void op_LD_HLm_A(CPU *cpu){
    write8(cpu->hl.HL, cpu->af.A);
}
void op_LD_A_B(CPU *cpu){
    cpu->af.A = cpu->bc.B;
}
void op_LD_A_C(CPU *cpu){
    cpu->af.A = cpu->bc.C;
}
void op_LD_A_D(CPU *cpu){
    cpu->af.A = cpu->de.D;
}
void op_LD_A_E(CPU *cpu){
    cpu->af.A = cpu->de.E;
}
void op_LD_A_H(CPU *cpu){
    cpu->af.A = cpu->hl.H;
}
void op_LD_A_L(CPU *cpu){
    cpu->af.A = cpu->hl.L;
}
void op_LD_A_HLm(CPU *cpu){
    cpu->af.A = read8(cpu->hl.HL);
}
void op_LD_A_A(CPU *cpu){
    
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
    opcodes[0x2D] = op_dec_L;
    opcodes[0x2E] = op_LD_L_d8;
    opcodes[0x2F] = op_CPL;
    opcodes[0x30] = op_JR_NC_r8;
    opcodes[0x31] = op_LD_SP_d16;
    opcodes[0x32] = op_HLminus_A;
    opcodes[0x33] = op_inc_SP;
    opcodes[0x34] = op_inc_HLmem;
    opcodes[0x35] = op_dec_HLmem;
    opcodes[0x36] = op_LD_HLmem_d8;
    opcodes[0x37] = op_SCF;
    opcodes[0x38] = op_JR_C_r8;
    opcodes[0x39] = op_add_HL_SP;
    opcodes[0x3A] = op_LD_A_HLminus;
    opcodes[0x3B] = op_dec_SP;
    opcodes[0x3C] = op_inc_A;
    opcodes[0x3D] = op_dec_A;
    opcodes[0x3E] = op_LD_A_d8;
    opcodes[0x3F] = op_CCF;
    opcodes[0x40] = op_LD_B_B;
    opcodes[0x41] = op_LD_B_C;
    opcodes[0x42] = op_LD_B_D;
    opcodes[0x43] = op_LD_B_E;
    opcodes[0x44] = op_LD_B_H;
    opcodes[0x45] = op_LD_B_L;
    opcodes[0x46] = op_LD_B_HLm;
    opcodes[0x47] = op_LD_B_A;
    opcodes[0x48] = op_LD_C_B;
    opcodes[0x49] = op_LD_C_C;
    opcodes[0x4A] = op_LD_C_D;
    opcodes[0x4B] = op_LD_C_E;
    opcodes[0x4C] = op_LD_C_H;
    opcodes[0x4D] = op_LD_C_L;
    opcodes[0x4E] = op_LD_C_HLm;
    opcodes[0x4F] = op_LD_C_A;
    opcodes[0x50] = op_LD_D_B;
    opcodes[0x51] = op_LD_D_C;
    opcodes[0x52] = op_LD_D_D;
    opcodes[0x53] = op_LD_D_E;
    opcodes[0x54] = op_LD_D_H;
    opcodes[0x55] = op_LD_D_L;
    opcodes[0x56] = op_LD_D_HLm;
    opcodes[0x57] = op_LD_D_A;
    opcodes[0x58] = op_LD_E_B;
    opcodes[0x59] = op_LD_E_C;
    opcodes[0x5A] = op_LD_E_D;
    opcodes[0x5B] = op_LD_E_E;
    opcodes[0x5C] = op_LD_E_H;
    opcodes[0x5D] = op_LD_E_L;
    opcodes[0x5E] = op_LD_E_HLm;
    opcodes[0x5F] = op_LD_E_A;
    opcodes[0x60] = op_LD_H_B;
    opcodes[0x61] = op_LD_H_C;
    opcodes[0x62] = op_LD_H_D;
    opcodes[0x63] = op_LD_H_E;
    opcodes[0x64] = op_LD_H_H;
    opcodes[0x65] = op_LD_H_L;
    opcodes[0x66] = op_LD_H_HLm;
    opcodes[0x67] = op_LD_H_A;
    opcodes[0x68] = op_LD_L_B;
    opcodes[0x69] = op_LD_L_C;
    opcodes[0x6A] = op_LD_L_D;
    opcodes[0x6B] = op_LD_L_E;
    opcodes[0x6C] = op_LD_L_H;
    opcodes[0x6D] = op_LD_L_L;
    opcodes[0x6E] = op_LD_L_HLm;
    opcodes[0x6F] = op_LD_L_A;
    opcodes[0x70] = op_LD_HLm_B;
    opcodes[0x71] = op_LD_HLm_C;
    opcodes[0x72] = op_LD_HLm_D;
    opcodes[0x73] = op_LD_HLm_E;
    opcodes[0x74] = op_LD_HLm_H;
    opcodes[0x75] = op_LD_HLm_L;
    opcodes[0x76] = op_HALT;
    opcodes[0x77] = op_LD_HLm_A;
    opcodes[0x78] = op_LD_A_B;
    opcodes[0x79] = op_LD_A_C;
    opcodes[0x7A] = op_LD_A_D;
    opcodes[0x7B] = op_LD_A_E;
    opcodes[0x7C] = op_LD_A_H;
    opcodes[0x7D] = op_LD_A_L;
    opcodes[0x7E] = op_LD_A_HLm;
    opcodes[0x7F] = op_LD_A_A;
    for (int i = 0x80; i <= 0xFF; i++) {
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

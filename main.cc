#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.hh"
#include "cpu.hh"

// mem 0
void init_mem() {
    memset(mem.rombank0, 0, sizeof(mem.rombank0));
    memset(mem.rombank1, 0, sizeof(mem.rombank1));
    memset(mem.Vram,     0, sizeof(mem.Vram));
    memset(mem.exram,    0, sizeof(mem.exram));
    memset(mem.wram,     0, sizeof(mem.wram));
    memset(mem.echoram,  0, sizeof(mem.echoram));
    memset(mem.oam,      0, sizeof(mem.oam));
    memset(mem.IO,       0, sizeof(mem.IO));
    memset(mem.Hram,     0, sizeof(mem.Hram));
    mem.IE = 0;
}

// Inicializa CPU com valores padrões
void init_cpu(CPU *cpu) {
    cpu->PC = 0x0100;
    cpu->SP = 0xFFFE;
    cpu->af.AF = 0;
    cpu->bc.BC = 0;
    cpu->de.DE = 0;
    cpu->hl.HL = 0;
    cpu->stop = false;
}

// Imprime estado atual da CPU (útil para debug)
void print_cpu_state(CPU *cpu) {
    printf("PC:%04X SP:%04X AF:%04X BC:%04X DE:%04X HL:%04X\n",
           cpu->PC, cpu->SP, cpu->af.AF, cpu->bc.BC,
           cpu->de.DE, cpu->hl.HL);
    printf("Flags: Z=%d N=%d H=%d C=%d\n",
           get_flag(cpu, FLAG_Z),
           get_flag(cpu, FLAG_N),
           get_flag(cpu, FLAG_H),
           get_flag(cpu, FLAG_C));
    printf("-------------------------------\n");
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Uso: %s <ROM.gb>\n", argv[0]);
        return 1;
    }

    init_mem(); // zera toda a memória

    if(!loadROM(argv[1])) {
        printf("Erro ao carregar ROM!\n");
        return 1;
    }

    CPU cpu;
    init_cpu(&cpu);
    initops(); // inicializa o array de funções de opcode

    printf("ROM carregada com sucesso!\nIniciando CPU...\n");

    //loop
    for (int ciclo = 0; ciclo < 1000 && !cpu.stop; ciclo++) {

    uint8_t opcode = read8(cpu.PC);

    printf("Ciclo %d | PC=%04X | OP=%02X\n",
           ciclo, cpu.PC, opcode);

    cicle(&cpu);   //função que executa o opcode
    print_cpu_state(&cpu);
}

}

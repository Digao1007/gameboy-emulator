#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.hh"

Memory mem;

int loadROM(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if(!f) {
        printf("\nerro ao abrir o arquivo\n");
        return 0;
    }

    fread(mem.rombank0, 1, 0x4000, f);
    fread(mem.rombank1, 1, 0x4000, f);

    fclose(f);
    return 1;
}

uint8_t read8(uint16_t addr) {
    if(addr < 0x4000) return mem.rombank0[addr];
    else if(addr < 0x8000) return mem.rombank1[addr - 0x4000];
    else if(addr < 0xA000) return mem.Vram[addr - 0x8000];
    else if(addr < 0xC000) return mem.exram[addr - 0xA000];
    else if(addr < 0xE000) return mem.wram[addr - 0xC000];
    else if(addr < 0xFE00) return mem.echoram[addr - 0xE000];
    else if(addr < 0xFEA0) return mem.oam[addr - 0xFE00];
    else if(addr < 0xFF00) return 0xFF;
    else if(addr < 0xFF80) return mem.IO[addr - 0xFF00];
    else if(addr < 0xFFFF) return mem.Hram[addr - 0xFF80];
    else if(addr == 0xFFFF) return mem.IE;

    return 0xFF;
}

void write8(uint16_t addr, uint8_t value) {
    if(addr < 0x8000) return; // ROM não pode ser escrita
    else if(addr < 0xA000) mem.Vram[addr - 0x8000] = value;
    else if(addr < 0xC000) mem.exram[addr - 0xA000] = value;
    else if(addr < 0xE000) {
        mem.wram[addr - 0xC000] = value;
        mem.echoram[addr - 0xE000] = value;
    }
    else if(addr < 0xFE00) { // Espelho
        mem.echoram[addr - 0xE000] = value;
        mem.wram[addr - 0xE000] = value;
    }
    else if(addr < 0xFEA0) mem.oam[addr - 0xFE00] = value;
    else if(addr < 0xFF00) return;
    else if(addr < 0xFF80) mem.IO[addr - 0xFF00] = value;
    else if(addr < 0xFFFF) mem.Hram[addr - 0xFF80] = value;
    else if(addr == 0xFFFF) mem.IE = value;
}

uint16_t read16(uint16_t addr) {
    uint8_t low = read8(addr);
    uint8_t high = read8(addr + 1);
    return (high << 8) | low;
}

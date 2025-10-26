#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//declaracao de cada parte da memoria
typedef struct{
uint8_t rombank0[0x4000]; //primeira parte do cartucho
uint8_t rombank1[0x4000]; //parte variavel da rom
uint8_t Vram[0x2000]; 
uint8_t exram[0x2000]; //ram externa do cartucho
uint8_t wram[0x2000]; //1 parte da ram
uint8_t echoram[0x1E00]; //espelho da wram
uint8_t oam[0xA0]; //Object Attribute Memory
uint8_t Dontuse[0x60];//area nao usada
uint8_t IO[0x80]; //area de IO
uint8_t Hram[0x7F]; //ram de alta velocidade, usada na cpu
uint8_t IE; //Interrupt Enable Register
} Memory;

Memory mem;

int loadROM(const char *filename){
    FILE *f = fopen(filename, "rb");
    if(!f){
        printf("\nerro ao abrir o arquivo\n");
        return 0;
    }
    fread(mem.rombank0, 1, 0x4000, f);
    
    
    fread(mem.rombank1, 1, 0x4000, f);

    fclose(f);
    return 1;

}


uint8_t read8(uint16_t addr){
    if(addr < 0x4000) return mem.rombank0[addr];
    else if(addr < 0x8000) return mem.rombank1[addr - 0x4000];
    else if(addr < 0xA000) return mem.Vram[addr - 0x8000];
    else if(addr < 0xC000) return mem.exram[addr - 0xA000];
    else if(addr < 0xE000) return mem.wram[addr - 0xC000];
    else if(addr >= 0xFF80 && addr <= 0xFFFE) return mem.Hram[addr - 0xFF80];
    else if(addr == 0xFFFF) return mem.IE;
    else return 0xFF;
}   




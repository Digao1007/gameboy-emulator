#ifndef MEMORY_HH
#define MEMORY_HH
#include <stdio.h>
#include <stdint.h>



typedef struct{
uint8_t rombank0[0x4000]; //primeira parte do cartucho
uint8_t rombank1[0x4000]; //parte variavel da rom
uint8_t Vram[0x2000]; 
uint8_t exram[0x2000]; 
uint8_t wram[0x2000]; //1 parte da ram
uint8_t echoram[0x1E00]; //espelho da wram
uint8_t oam[0xA0]; 
uint8_t Dontuse[0x60];
uint8_t IO[0x80]; 
uint8_t Hram[0x7F]; 
uint8_t IE; 
} Memory;

extern Memory mem;

int loadROM(const char *filename);

uint8_t read8(uint16_t addr);

void write8(uint16_t addr, uint8_t value);

uint16_t read16(uint16_t addr);





#endif 
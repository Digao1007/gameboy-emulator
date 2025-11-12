#ifndef CPU_HH
#define CPU_HH

#include <stdint.h>
#include <stdbool.h>
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

    bool stop;
}CPU;

#define FLAG_Z (1 << 7)
#define FLAG_N (1 << 6)
#define FLAG_H (1 << 5)
#define FLAG_C (1 << 4)

void set_flag(CPU *cpu, uint8_t flag, bool value);
uint8_t get_flag(CPU *cpu, uint8_t flag);

extern void (*opcodes[256])(CPU *cpu);
void initops();
void cicle(CPU *cpu);

#endif

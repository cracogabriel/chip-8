#pragma once

#include <stdint.h>

typedef struct VM {
    uint8_t RAM[4096];        // Memória 4KB
    uint16_t PC;              // Program Counter
    uint8_t V[16];            // Registradores V0..VF
    uint8_t SP;               // Stack Pointer (índice para stack)
    uint16_t I;               // Registrador de índice
    uint16_t stack[16];       // Pilha (capacidade 16)
    uint8_t DISPLAY[64*32];   // Framebuffer monocromático 64x32
    uint8_t delay_timer;      // Delay timer (60Hz)
    uint8_t sound_timer;      // Sound timer (60Hz)
    uint8_t keys[16];         // Estado do teclado (1=pressed, 0=released)
} VM;

void VM_Inicializar(VM* vm, uint16_t pc_inicial);
void VM_CarregarROM(VM* vm, char* arq_rom, uint16_t pc_inicial);
void VM_ExecutarInstrucao(VM* vm);
void VM_ImprimirRegistradores(VM* vm);

/* Helpers úteis */
void VM_TickTimers(VM* vm); /* decrementa delay & sound timers (chamar a 60Hz) */
void VM_ProcessarInput(VM* vm, int* running); /* processa input do teclado */

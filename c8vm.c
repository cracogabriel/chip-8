#include "c8vm.h"
#include <stdio.h>
#include <stdlib.h>

void VM_Inicializar(VM* vm, uint16_t pc_inicial){
    memset(vm->RAM, 0, sizeof(vm->RAM));
    memset(vm->V, 0, sizeof(vm->V));
    vm->PC = pc_inicial;
    vm->SP = 0;
    vm->I = 0;
    memset(vm->stack, 0, sizeof(vm->stack));
    memset(vm->DISPLAY, 0, sizeof(vm->DISPLAY));
    vm->delay_timer = 0;
    vm->sound_timer = 0;
    memset(vm->keys, 0, sizeof(vm->keys));
    /* carregar sprites 0-F em 0x000 (5 bytes por digit) */
    uint8_t fontset[80] = {
        0xF0,0x90,0x90,0x90,0xF0, // 0
        0x20,0x60,0x20,0x20,0x70, // 1
        0xF0,0x10,0xF0,0x80,0xF0, // 2
        0xF0,0x10,0xF0,0x10,0xF0, // 3
        0x90,0x90,0xF0,0x10,0x10, // 4
        0xF0,0x80,0xF0,0x10,0xF0, // 5
        0xF0,0x80,0xF0,0x90,0xF0, // 6
        0xF0,0x10,0x20,0x40,0x40, // 7
        0xF0,0x90,0xF0,0x90,0xF0, // 8
        0xF0,0x90,0xF0,0x10,0xF0, // 9
        0xF0,0x90,0xF0,0x90,0x90, // A
        0xE0,0x90,0xE0,0x90,0xE0, // B
        0xF0,0x80,0x80,0x80,0xF0, // C
        0xE0,0x90,0x90,0x90,0xE0, // D
        0xF0,0x80,0xF0,0x80,0xF0, // E
        0xF0,0x80,0xF0,0x80,0x80  // F
    };
    memcpy(&vm->RAM[0x000], fontset, sizeof(fontset));
    srand((unsigned)time(NULL));
}

void VM_CarregarROM(VM* vm, 
                    char* arq_rom, 
                    uint16_t pc_inicial){

    FILE* rom = fopen(arq_rom, "rb");
    if(!rom){
        fprintf(stderr, "Erro ao abrir ROM: %s\n", arq_rom);
        exit(1);
    }
    fseek(rom, 0, SEEK_END);
    int tam_rom = ftell(rom);
    rewind(rom);

    if(pc_inicial + tam_rom > 4096){
        fprintf(stderr, "ROM muito grande para memória.\n");
        fclose(rom);
        exit(1);
    }

    fread(&vm->RAM[pc_inicial], 1, tam_rom, rom);

    fclose(rom);
}

void VM_TickTimers(VM* vm){
    if(vm->delay_timer > 0) vm->delay_timer--;
    if(vm->sound_timer > 0) vm->sound_timer--;
}

void VM_ExecutarInstrucao(VM* vm){
    /* fetch */
    uint16_t inst = (vm->RAM[vm->PC] << 8) | vm->RAM[vm->PC+1];
    printf("Instrução: 0x%04X\n", inst);

    /* incrementa PC para próxima instrução (algumas instruções sobrescrevem) */
    vm->PC += 2;

    uint8_t grupo = (inst >> 12) & 0xF;
    uint8_t X     = (inst & 0x0F00) >> 8;
    uint8_t Y     = (inst & 0x00F0) >> 4;
    uint8_t N     = inst & 0x000F;
    uint8_t NN    = inst & 0x00FF;
    uint16_t NNN  = inst & 0x0FFF;

    switch(grupo){
        case 0x0:
            if(inst == 0x00E0){ // CLS
                memset(vm->DISPLAY, 0, sizeof(vm->DISPLAY));
            } else if(inst == 0x00EE){ // RET
                if(vm->SP == 0){
                    fprintf(stderr, "Stack underflow em RET\n");
                    exit(1);
                }
                vm->SP--;
                vm->PC = vm->stack[vm->SP];
            } else {
                /* 0NNN SYS addr (ignorado em emuladores modernos) */
            }
            break;

        case 0x1: // 1NNN JP addr
            vm->PC = NNN;
            break;

        case 0x2: // 2NNN CALL addr
            if(vm->SP >= 16){
                fprintf(stderr, "Stack overflow em CALL\n");
                exit(1);
            }
            vm->stack[vm->SP++] = vm->PC; // empilha endereço de retorno (PC já aponta para próxima instr)
            vm->PC = NNN;
            break;

        case 0x3: // 3XNN SE Vx, byte
            if(vm->V[X] == NN) vm->PC += 2;
            break;

        case 0x4: // 4XNN SNE Vx, byte
            if(vm->V[X] != NN) vm->PC += 2;
            break;

        case 0x5: // 5XY0 SE Vx, Vy
            if((inst & 0x000F) == 0){
                if(vm->V[X] == vm->V[Y]) vm->PC += 2;
            } else {
                fprintf(stderr, "Instr 5 com formato inesperado: 0x%04X\n", inst);
                exit(1);
            }
            break;

        case 0x6: // 6XNN LD Vx, byte
            vm->V[X] = NN;
            break;

        case 0x7: // 7XNN ADD Vx, byte
            vm->V[X] = (uint8_t)(vm->V[X] + NN);
            break;

        case 0x8:
            switch(N){
                case 0x0: // 8XY0 LD Vx, Vy
                    vm->V[X] = vm->V[Y];
                    break;
                case 0x1: // 8XY1 OR Vx, Vy
                    vm->V[X] |= vm->V[Y];
                    break;
                case 0x2: // 8XY2 AND Vx, Vy
                    vm->V[X] &= vm->V[Y];
                    break;
                case 0x3: // 8XY3 XOR Vx, Vy
                    vm->V[X] ^= vm->V[Y];
                    break;
                case 0x4: { // 8XY4 ADD Vx, Vy ; VF = carry
                    uint16_t sum = vm->V[X] + vm->V[Y];
                    vm->V[0xF] = (sum > 0xFF) ? 1 : 0;
                    vm->V[X] = (uint8_t)sum;
                    break;
                }
                case 0x5: { // 8XY5 SUB Vx, Vy ; VF = NOT borrow (Vx > Vy)
                    vm->V[0xF] = (vm->V[X] > vm->V[Y]) ? 1 : 0;
                    vm->V[X] = vm->V[X] - vm->V[Y];
                    break;
                }
                case 0x6: { // 8XY6 SHR Vx {, Vy} ; VF = least significant bit before shift
                    vm->V[0xF] = vm->V[X] & 0x1;
                    vm->V[X] >>= 1;
                    break;
                }
                case 0x7: { // 8XY7 SUBN Vx, Vy ; Vx = Vy - Vx ; VF = NOT borrow (Vy > Vx)
                    vm->V[0xF] = (vm->V[Y] > vm->V[X]) ? 1 : 0;
                    vm->V[X] = vm->V[Y] - vm->V[X];
                    break;
                }
                case 0xE: { // 8XYE SHL Vx {, Vy} ; VF = most significant bit before shift
                    vm->V[0xF] = (vm->V[X] & 0x80) ? 1 : 0;
                    vm->V[X] <<= 1;
                    break;
                }
                default:
                    fprintf(stderr, "Opcode 8XY%X não suportado: 0x%04X\n", N, inst);
                    exit(1);
            }
            break;

        case 0x9: // 9XY0 SNE Vx, Vy
            if((inst & 0x000F) == 0){
                if(vm->V[X] != vm->V[Y]) vm->PC += 2;
            } else {
                fprintf(stderr, "Instr 9 com formato inesperado: 0x%04X\n", inst);
                exit(1);
            }
            break;

        case 0xA: // ANNN LD I, addr
            vm->I = NNN;
            break;

        case 0xB: // BNNN JP V0, addr
            vm->PC = NNN + vm->V[0];
            break;

        case 0xC: { // CXNN RND Vx, byte
            uint8_t r = (uint8_t)(rand() & 0xFF);
            vm->V[X] = r & NN;
            break;
        }

        case 0xD: { // DXYN DRW Vx, Vy, nibble
            uint8_t vx = vm->V[X] % 64;
            uint8_t vy = vm->V[Y] % 32;
            uint8_t height = N;
            vm->V[0xF] = 0;
            for(uint8_t row = 0; row < height; row++){
                uint8_t sprite_byte = vm->RAM[vm->I + row];
                for(uint8_t col = 0; col < 8; col++){
                    if((sprite_byte & (0x80 >> col)) != 0){
                        uint16_t x = (vx + col) % 64;
                        uint16_t y = (vy + row) % 32;
                        uint32_t idx = y * 64 + x;
                        if(vm->DISPLAY[idx] == 1){
                            vm->V[0xF] = 1;
                        }
                        vm->DISPLAY[idx] ^= 1;
                    }
                }
            }
            break;
        }

        case 0xE: {
            if(NN == 0x9E){ // EX9E SKP Vx
                uint8_t key = vm->V[X] & 0xF;
                if(vm->keys[key]) vm->PC += 2;
            } else if(NN == 0xA1){ // EXA1 SKNP Vx
                uint8_t key = vm->V[X] & 0xF;
                if(!vm->keys[key]) vm->PC += 2;
            } else {
                fprintf(stderr, "Opcode E com formato inesperado: 0x%04X\n", inst);
                exit(1);
            }
            break;
        }

        case 0xF: {
            switch(NN){
                case 0x07: // FX07 LD Vx, DT
                    vm->V[X] = vm->delay_timer;
                    break;
                case 0x0A: { // FX0A LD Vx, K (espera por tecla)
                    int found = -1;
                    for(int k=0;k<16;k++){
                        if(vm->keys[k]){
                            found = k;
                            break;
                        }
                    }
                    if(found == -1){
                        /* não há tecla pressionada: repetir esta instrução (PC já foi incrementado) */
                        vm->PC -= 2;
                    } else {
                        vm->V[X] = (uint8_t)found;
                    }
                    break;
                }
                case 0x15: // FX15 LD DT, Vx
                    vm->delay_timer = vm->V[X];
                    break;
                case 0x18: // FX18 LD ST, Vx
                    vm->sound_timer = vm->V[X];
                    break;
                case 0x1E: // FX1E ADD I, Vx
                    vm->I = (uint16_t)(vm->I + vm->V[X]);
                    break;
                case 0x29: // FX29 LD F, Vx -> set I to location of sprite for digit Vx (each 5 bytes starting at 0x000)
                    vm->I = vm->V[X] * 5;
                    break;
                case 0x33: { // FX33 BCD Vx -> store hundreds, tens, ones at I, I+1, I+2
                    uint8_t val = vm->V[X];
                    vm->RAM[vm->I + 0] = val / 100;
                    vm->RAM[vm->I + 1] = (val / 10) % 10;
                    vm->RAM[vm->I + 2] = val % 10;
                    break;
                }
                case 0x55: { // FX55 LD [I], V0..Vx
                    for(uint8_t i = 0; i <= X; i++){
                        vm->RAM[vm->I + i] = vm->V[i];
                    }
                    /* Nota: alguns interpreters incrementam I += X + 1; especificação original não */
                    break;
                }
                case 0x65: { // FX65 LD V0..Vx, [I]
                    for(uint8_t i = 0; i <= X; i++){
                        vm->V[i] = vm->RAM[vm->I + i];
                    }
                    /* Nota: alguns interpreters incrementam I += X + 1; especificação original não */
                    break;
                }
                default:
                    fprintf(stderr, "Opcode F com formato inesperado: 0x%04X\n", inst);
                    exit(1);
            }
            break;
        }

        default:
            fprintf(stderr, "Grupo não implementado! Instrução: 0x%04X\n", inst);
            exit(1);
    }
}

void VM_ImprimirRegistradores(VM* vm) {
    printf("PC: 0x%04X I: 0x%04X SP: 0x%02X DT: 0x%02X ST: 0x%02X\n",
           vm->PC, vm->I, vm->SP, vm->delay_timer, vm->sound_timer);
    for(int i = 0; i < 16; i++) {
        printf("V[%X]: 0x%02X ", i, vm->V[i]);
    }
    printf("\n");
}

// void VM_ImprimirRegistradores(VM* vm) {
//     printf("PC: 0x%04X I: 0x%04X SP: 0x%02X\n", vm->PC, vm->I, vm->SP);
//     for(int i = 0; i < 16; i++) {
//         printf("V[%X]: 0x%02X ", i, vm->V[i]);
//     }
//     printf("\n");
// }
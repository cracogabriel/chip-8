#include "c8vm.h"
#include "display.h"
#include "sound.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char* prog) {
    printf("Uso: %s <arquivo_rom> [escala]\n", prog);
    printf("     %s <arquivo_rom> --scale <fator> [--clock <hz>]\n", prog);
    printf("Opcoes:\n");
    printf("  --scale <fator>   Fator de escala da janela (padrao: 10)\n");
    printf("  --clock <hz>      Frequencia da CPU em Hz (padrao: 500)\n");
    printf("  -h, --help        Mostra esta ajuda e sai\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    // Padroes
    int scale = 10;         // 10 => 640x320
    int clock_hz = 500;     // frequencia da CPU
    const int TIMER_HZ = 60; // frequencia dos timers

    // Argumentos
    const char* rom_path = NULL;
    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            print_usage(argv[0]);
            return 0; 
        }
        if (strcmp(arg, "--scale") == 0 && i + 1 < argc) {
            scale = atoi(argv[++i]);
            continue;
        }
        if (strncmp(arg, "--scale=", 8) == 0) {
            scale = atoi(arg + 8);
            continue;
        }
        if (strcmp(arg, "--clock") == 0 && i + 1 < argc) {
            clock_hz = atoi(argv[++i]);
            continue;
        }
        if (strncmp(arg, "--clock=", 8) == 0) {
            clock_hz = atoi(arg + 8);
            continue;
        }
        if (arg[0] == '-') {
            fprintf(stderr, "Opcao desconhecida: %s\n", arg);
            print_usage(argv[0]);
            return 1;
        }
        // Primeiro argumento posicional eh a ROM
        if (!rom_path) {
            rom_path = arg;
        } else {
            // Compat: permitir forma antiga <rom> [escala]
            // Se for numero, usar como escala
            char* endptr = NULL;
            long maybe_scale = strtol(arg, &endptr, 10);
            if (endptr && *endptr == '\0') {
                scale = (int)maybe_scale;
            } else {
                fprintf(stderr, "Argumento inesperado: %s\n", arg);
                print_usage(argv[0]);
                return 1;
            }
        }
    }

    if (!rom_path) {
        print_usage(argv[0]);
        return 1;
    }

    if (scale < 1) {
        fprintf(stderr, "Aviso: scale invalido (%d). Usando 10.\n", scale);
        scale = 10;
    }
    if (clock_hz < 1) {
        fprintf(stderr, "Aviso: clock invalido (%d). Usando 500Hz.\n", clock_hz);
        clock_hz = 500;
    }

    Display d;
    if (!Display_Inicializar(&d, scale)) return 1;

    Sound sound;
    if (!Sound_Inicializar(&sound)) {
        Display_Destruir(&d);
        return 1;
    }

    VM vm;
    VM_Inicializar(&vm, 0x200);
    VM_CarregarROM(&vm, (char*)rom_path, 0x200);

    const double MS_PER_TICK = 1000.0 / (double)clock_hz;
    const double MS_PER_TIMER = 1000.0 / (double)TIMER_HZ;

    double acc_timer = 0;
    uint32_t last = SDL_GetTicks();

    int running = 1;
    while (running) {
        uint32_t now = SDL_GetTicks();
        double dt = now - last;
        last = now;
        acc_timer += dt;

        // Processa input do teclado
        VM_ProcessarInput(&vm, &running);

        VM_ExecutarInstrucao(&vm);

        // Atualiza timers a 60Hz
        if (acc_timer >= MS_PER_TIMER) {
            VM_TickTimers(&vm);
            acc_timer -= MS_PER_TIMER;
        }

        // Atualiza o som baseado no sound_timer
        Sound_Atualizar(&sound, vm.sound_timer);

        Display_Atualizar(&d, vm.DISPLAY);
        SDL_Delay((int)MS_PER_TICK);
    }

    Sound_Destruir(&sound);
    Display_Destruir(&d);
    return 0;
}

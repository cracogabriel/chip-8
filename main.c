#include "c8vm.h"
#include "display.h"
#include "sound.h"
#include "util.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    AppConfig cfg;
    int parse_status = parse_args(argc, argv, argv[0], &cfg);
    if (parse_status == 1) {
        print_usage(argv[0]); // Ajuda solicitada
        return 0;
    } else if (parse_status != 0) {
        return 1; // Erro de parsing (mensagens já exibidas)
    }

    const int TIMER_HZ = 60; // frequencia dos timers

    Display d;
    if (!Display_Inicializar(&d, cfg.scale)) return 1;

    Sound sound;
    if (!Sound_Inicializar(&sound)) {
        Display_Destruir(&d);
        return 1;
    }

    VM vm;
    VM_Inicializar(&vm, cfg.load_addr);
    VM_CarregarROM(&vm, (char*)cfg.rom_path, cfg.load_addr);

    const double MS_PER_TICK = 1000.0 / (double)cfg.clock_hz;
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

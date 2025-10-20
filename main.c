#include "c8vm.h"
#include "display.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_rom> [escala]\n", argv[0]);
        return 1;
    }

    int scale = (argc >= 3) ? atoi(argv[2]) : 10;
    Display d;
    if (!Display_Inicializar(&d, scale)) return 1;

    VM vm;
    VM_Inicializar(&vm, 0x200);
    VM_CarregarROM(&vm, argv[1], 0x200);

    const int CLOCK_HZ = 500;       // frequência da CPU
    const int TIMER_HZ = 60;        // frequência dos timers
    const double MS_PER_TICK = 1000.0 / CLOCK_HZ;
    const double MS_PER_TIMER = 1000.0 / TIMER_HZ;

    double acc_timer = 0;
    uint32_t last = SDL_GetTicks();

    int running = 1;
    while (running) {
        uint32_t now = SDL_GetTicks();
        double dt = now - last;
        last = now;
        acc_timer += dt;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) running = 0;
                // Exemplo de mapeamento de teclas:
                // Chip-8 usa 0x0–0xF (hex keypad)
                // Aqui você pode mapear teclas a vm.keys[...]
            }
            if (e.type == SDL_KEYUP) {
                // liberar tecla correspondente
            }
        }

        VM_ExecutarInstrucao(&vm);

        // Atualiza timers a 60Hz
        if (acc_timer >= MS_PER_TIMER) {
            VM_TickTimers(&vm);
            acc_timer -= MS_PER_TIMER;
        }

        Display_Atualizar(&d, vm.DISPLAY);
        SDL_Delay((int)MS_PER_TICK);
    }

    Display_Destruir(&d);
    return 0;
}

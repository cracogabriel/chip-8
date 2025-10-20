#pragma once
#include <stdint.h>
#include <SDL2/SDL.h>
#include "c8vm.h"

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t pixels[CHIP8_WIDTH * CHIP8_HEIGHT]; // ARGB8888
    int scale; // fator de escala (ex: 10 → 640x320)
} Display;

/**
 * Inicializa o display com SDL2.
 * @param d ponteiro para struct Display
 * @param scale fator de escala (1 = 64x32, 10 = 640x320)
 */
int Display_Inicializar(Display* d, int scale);

/**
 * Atualiza a textura com o framebuffer da VM (array 64x32 de 0/1)
 */
void Display_Atualizar(Display* d, const uint8_t framebuffer[CHIP8_WIDTH * CHIP8_HEIGHT]);

/**
 * Limpa o display (pinta tudo de preto)
 */
void Display_Limpar(Display* d);

/**
 * Finaliza e libera recursos SDL
 */
void Display_Destruir(Display* d);


/**
 * Processa a entreda do teclado
 */
void Display_ProcessarInput(VM* vm, int* running);
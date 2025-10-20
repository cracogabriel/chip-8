#include "display.h"
#include <stdio.h>
#include <SDL2/SDL.h>

int Display_Inicializar(Display* d, int scale) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "Erro SDL_Init: %s\n", SDL_GetError());
        return 0;
    }

    d->scale = scale;
    int width = CHIP8_WIDTH * scale;
    int height = CHIP8_HEIGHT * scale;

    d->window = SDL_CreateWindow("CHIP-8 Emulator",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, 0);
    if (!d->window) {
        fprintf(stderr, "Erro ao criar janela: %s\n", SDL_GetError());
        return 0;
    }

    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED);
    if (!d->renderer) {
        fprintf(stderr, "Erro ao criar renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(d->window);
        return 0;
    }

    d->texture = SDL_CreateTexture(d->renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   CHIP8_WIDTH, CHIP8_HEIGHT);
    if (!d->texture) {
        fprintf(stderr, "Erro ao criar textura: %s\n", SDL_GetError());
        SDL_DestroyRenderer(d->renderer);
        SDL_DestroyWindow(d->window);
        return 0;
    }

    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++)
        d->pixels[i] = 0xFF000000;

    return 1;
}

void Display_Atualizar(Display* d, const uint8_t framebuffer[CHIP8_WIDTH * CHIP8_HEIGHT]) {
    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++)
        d->pixels[i] = framebuffer[i] ? 0xFFFFFFFF : 0xFF000000;

    SDL_UpdateTexture(d->texture, NULL, d->pixels, CHIP8_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(d->renderer);
    SDL_RenderCopy(d->renderer, d->texture, NULL, NULL);
    SDL_RenderPresent(d->renderer);
}

void Display_Limpar(Display* d) {
    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++)
        d->pixels[i] = 0xFF000000;

    SDL_UpdateTexture(d->texture, NULL, d->pixels, CHIP8_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(d->renderer);
    SDL_RenderCopy(d->renderer, d->texture, NULL, NULL);
    SDL_RenderPresent(d->renderer);
}

void Display_ProcessarInput(VM* vm, int* running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            *running = 0;
        } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            uint8_t value = (e.type == SDL_KEYDOWN) ? 1 : 0;

            switch (e.key.keysym.sym) {
                case SDLK_1: vm->keys[0x1] = value; break; 
                case SDLK_2: vm->keys[0x2] = value; break;
                case SDLK_3: vm->keys[0x3] = value; break;
                case SDLK_4: vm->keys[0xC] = value; break;

                case SDLK_q: vm->keys[0x4] = value; break;
                case SDLK_w: vm->keys[0x5] = value; break;
                case SDLK_e: vm->keys[0x6] = value; break;
                case SDLK_r: vm->keys[0xD] = value; break;

                case SDLK_a: vm->keys[0x7] = value; break;
                case SDLK_s: vm->keys[0x8] = value; break;
                case SDLK_d: vm->keys[0x9] = value; break;
                case SDLK_f: vm->keys[0xE] = value; break;

                case SDLK_z: vm->keys[0xA] = value; break;
                case SDLK_x: vm->keys[0x0] = value; break;
                case SDLK_c: vm->keys[0xB] = value; break;
                case SDLK_v: vm->keys[0xF] = value; break;

                case SDLK_ESCAPE: *running = 0; break; 
            }
        }
    }
}

void Display_Destruir(Display* d) {
    if (d->texture) SDL_DestroyTexture(d->texture);
    if (d->renderer) SDL_DestroyRenderer(d->renderer);
    if (d->window) SDL_DestroyWindow(d->window);
    SDL_Quit();
}

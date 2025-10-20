#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

/**
 * Gerencia o som do CHIP-8 usando SDL2.
 * O CHIP-8 possui apenas um beep de tom único que toca enquanto sound_timer > 0.
 */

typedef struct {
    SDL_AudioDeviceID device;
    SDL_AudioSpec spec;
    int is_playing;
    double phase; // fase atual da onda senoidal
} Sound;

/**
 * Inicializa o subsistema de som SDL2
 * @param sound ponteiro para struct Sound
 * @return 1 em sucesso, 0 em falha
 */
int Sound_Inicializar(Sound* sound);

/**
 * Inicia o beep (som)
 * @param sound ponteiro para struct Sound
 */
void Sound_IniciarBeep(Sound* sound);

/**
 * Para o beep (silencia)
 * @param sound ponteiro para struct Sound
 */
void Sound_PararBeep(Sound* sound);

/**
 * Atualiza o estado do som baseado no sound_timer da VM
 * @param sound ponteiro para struct Sound
 * @param sound_timer valor do sound_timer (0 = silêncio, >0 = beep)
 */
void Sound_Atualizar(Sound* sound, uint8_t sound_timer);

/**
 * Finaliza e libera recursos de som
 * @param sound ponteiro para struct Sound
 */
void Sound_Destruir(Sound* sound);

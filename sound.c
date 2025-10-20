#include "sound.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SAMPLE_RATE 44100
#define FREQUENCY 440.0f  // Nota A4 (Lá) 
#define AMPLITUDE 0.1f    // Volume (0.0 a 1.0)

static void sound_callback(void* userdata, Uint8* stream, int len) {
    Sound* sound = (Sound*)userdata;
    float* fstream = (float*)stream;
    int samples = len / sizeof(float);
    
    for (int i = 0; i < samples; i++) {
        if (sound->is_playing) {
            fstream[i] = AMPLITUDE * sinf(2.0f * M_PI * FREQUENCY * sound->phase / SAMPLE_RATE);
            sound->phase += 1.0;
            
            if (sound->phase >= SAMPLE_RATE) {
                sound->phase -= SAMPLE_RATE;
            }
        } else {
            fstream[i] = 0.0f;
        }
    }
}

int Sound_Inicializar(Sound* sound) {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erro ao inicializar som SDL: %s\n", SDL_GetError());
        return 0;
    }
    
    sound->is_playing = 0;
    sound->phase = 0.0;
    
    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_F32SYS;    // Float 32-bit
    want.channels = 1;             // Mono
    want.samples = 2048;          
    want.callback = sound_callback;
    want.userdata = sound;
    
    sound->device = SDL_OpenAudioDevice(NULL, 0, &want, &sound->spec, 0);
    if (sound->device == 0) {
        fprintf(stderr, "Erro ao abrir dispositivo de som: %s\n", SDL_GetError());
        return 0;
    }
    
    // Inicia o dispositivo (pausado inicialmente)
    SDL_PauseAudioDevice(sound->device, 0);
    
    return 1;
}

void Sound_IniciarBeep(Sound* sound) {
    if (!sound->is_playing) {
        sound->is_playing = 1;
        sound->phase = 0.0; // Reseta fase para evitar clique
    }
}

void Sound_PararBeep(Sound* sound) {
    sound->is_playing = 0;
}

void Sound_Atualizar(Sound* sound, uint8_t sound_timer) {
    if (sound_timer > 0) {
        Sound_IniciarBeep(sound);
    } else {
        Sound_PararBeep(sound);
    }
}

void Sound_Destruir(Sound* sound) {
    if (sound->device) {
        SDL_CloseAudioDevice(sound->device);
        sound->device = 0;
    }
}

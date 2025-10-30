#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(const char* prog) {
    printf("\n");
    printf("  ██████╗██╗  ██╗██╗██████╗        ███████╗\n");
    printf(" ██╔════╝██║  ██║██║██╔══██╗       ██╔══██║\n");
    printf(" ██║     ███████║██║██████╔╝ █████╗███████║\n");
    printf(" ██║     ██╔══██║██║██╔═══╝  ╚════╝██╔══██║\n");
    printf(" ╚██████╗██║  ██║██║██║            ███████║\n");
    printf("  ╚═════╝╚═╝  ╚═╝╚═╝╚═╝            ╚══════╝\n");
    printf("            Emulador CHIP-8 - v1.0\n");
    printf("\n");
    printf("Uso: %s <arquivo_rom> [escala]\n", prog);
    printf("     %s <arquivo_rom> --scale <fator> [--clock <hz>] [--load-addr <addr>]\n", prog);
    printf("\nOpcoes:\n");
    printf("  --scale <fator>     Fator de escala da janela (padrao: 10)\n");
    printf("  --clock <hz>        Frequencia da CPU em Hz (padrao: 500)\n");
    printf("  --load-addr <addr>  Endereco de carga da ROM (padrao: 0x200)\n");
    printf("  -h, --help          Mostra esta ajuda e sai\n");
    printf("\n");
}

int parse_args(int argc, char* argv[], const char* prog, AppConfig* cfg) {
    if (!cfg) return 2;

    // Defaults
    cfg->rom_path = NULL;
    cfg->scale = 10;
    cfg->clock_hz = 500;
    cfg->load_addr = 0x200;

    if (argc < 2) {
        print_usage(prog);
        return 2;
    }

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            return 1; // apenas ajuda
        }
        if (strcmp(arg, "--scale") == 0 && i + 1 < argc) {
            cfg->scale = atoi(argv[++i]);
            continue;
        }
        if (strncmp(arg, "--scale=", 8) == 0) {
            cfg->scale = atoi(arg + 8);
            continue;
        }
        if (strcmp(arg, "--clock") == 0 && i + 1 < argc) {
            cfg->clock_hz = atoi(argv[++i]);
            continue;
        }
        if (strncmp(arg, "--clock=", 8) == 0) {
            cfg->clock_hz = atoi(arg + 8);
            continue;
        }
        if (strcmp(arg, "--load-addr") == 0 && i + 1 < argc) {
            cfg->load_addr = (uint16_t)strtol(argv[++i], NULL, 0);
            continue;
        }
        if (strncmp(arg, "--load-addr=", 12) == 0) {
            cfg->load_addr = (uint16_t)strtol(arg + 12, NULL, 0);
            continue;
        }
        if (arg[0] == '-') {
            fprintf(stderr, "Opcao desconhecida: %s\n", arg);
            print_usage(prog);
            return 2;
        }

        if (!cfg->rom_path) {
            cfg->rom_path = arg;
        } else {
            char* endptr = NULL;
            long maybe_scale = strtol(arg, &endptr, 10);
            if (endptr && *endptr == '\0') {
                cfg->scale = (int)maybe_scale;
            } else {
                fprintf(stderr, "Argumento inesperado: %s\n", arg);
                print_usage(prog);
                return 2;
            }
        }
    }

    if (!cfg->rom_path) {
        print_usage(prog);
        return 2;
    }

    if (cfg->scale < 1) {
        fprintf(stderr, "Aviso: scale invalido (%d). Usando 10.\n", cfg->scale);
        cfg->scale = 10;
    }
    if (cfg->clock_hz < 1) {
        fprintf(stderr, "Aviso: clock invalido (%d). Usando 500Hz.\n", cfg->clock_hz);
        cfg->clock_hz = 500;
    }
    if (cfg->load_addr > 0xFFF) {
        fprintf(stderr, "Aviso: load_addr invalido (0x%X). Usando 0x200.\n", cfg->load_addr);
        cfg->load_addr = 0x200;
    }

    return 0; 
}

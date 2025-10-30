#pragma once
#include <stdint.h>

/* Funcoes utilitarias para parse de argumentos e ajuda */

typedef struct {
    const char* rom_path; /* caminho para ROM */
    int scale;            /* fator de escala da janela */
    int clock_hz;         /* frequencia da CPU em Hz */
    uint16_t load_addr;   /* endereco de carga da ROM (padrao: 0x200) */
} AppConfig;

/* Imprime a ajuda de uso */
void print_usage(const char* prog);

/*
 * Faz o parse dos argumentos de linha de comando.
 * Retornos:
 *   0 -> sucesso, cfg preenchido
 *   1 -> ajuda solicitada (nao executar)
 *   2 -> erro de parsing (mensagens ja exibidas)
 */
int parse_args(int argc, char* argv[], const char* prog, AppConfig* cfg);

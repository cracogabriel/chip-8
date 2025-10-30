# Emulador CHIP-8 - Guia de Uso

## Descrição

Este programa é um **emulador CHIP-8**, que executa ROMs (programas) escritos para o sistema CHIP-8 original.
Permite ajustar parâmetros como **escala da janela**, **frequência do clock** e **endereço de carregamento da ROM**.

---

## Mapeamento

| Teclado CHIP-8 | Teclado Físico |
| -------------- | -------------- |
| 1              | 1              |
| 2              | 2              |
| 3              | 3              |
| C              | 4              |
| 4              | Q              |
| 5              | W              |
| 6              | E              |
| D              | R              |
| 7              | A              |
| 8              | S              |
| 9              | D              |
| E              | F              |
| A              | Z              |
| 0              | X              |
| B              | C              |
| F              | V              |

---

## Uso

```
./chip8 <arquivo_rom> [escala]
./chip8 <arquivo_rom> --scale <fator> [--clock <hz>] [--load-addr <addr>]
```

---

## Opções

| Opção                | Descrição                                                                                            | Padrão  |
| :------------------- | :--------------------------------------------------------------------------------------------------- | :------ |
| `--scale <fator>`    | Define o **fator de escala** da janela de exibição. <br>Exemplo: `1 = 64x32`, `10 = 640x320`.        | `10`    |
| `--clock <hz>`       | Define a **frequência da CPU** em Hertz (Hz), isto é, quantas instruções são executadas por segundo. | `500`   |
| `--load-addr <addr>` | Define o **endereço de memória** onde a ROM será carregada. Deve estar entre `0x000` e `0xFFF`.      | `0x200` |
| `-h`, `--help`       | Exibe a tela de ajuda e sai do programa.                                                             | —       |

---

## Exemplo de Execução

### Exemplo 1 — Execução simples com escala

```
./chip8 roms/INVADERS.ch8 10
```

Carrega a ROM `INVADERS.ch8` com fator de escala 10.

### Exemplo 2 — Ajustando clock e escala

```
./chip8 roms/TETRIS.ch8 --scale 12 --clock 700
```

Carrega `TETRIS.ch8` com janela ampliada em 12x e clock de 700 Hz.

### Exemplo 3 — Especificando endereço de carga

```
./chip8 roms/DEMO.ch8 --load-addr 0x300
```

Carrega `DEMO.ch8` na memória a partir do endereço `0x300`.

---

## Notas

- O **arquivo ROM** é obrigatório.
- Caso o valor de `scale` seja menor que `1`, o emulador usará o valor padrão `10`.
- Caso o valor de `clock` seja menor que `1`, será ajustado para `500 Hz`.
- Caso `load_addr` ultrapasse `0xFFF`, ele será redefinido para `0x200`.

---

## Ajuda

Para exibir a tela de ajuda diretamente no terminal:

```
./chip8 --help
```

---

## Modo de Depuração

Se o arquivo `defs.h` contiver a definição abaixo:

```
#define DEBUG 1
```

o programa exibirá mensagens de log no terminal durante a execução, úteis para depuração e análise do comportamento interno da máquina CHIP-8.

---

## Exemplo Completo

```
./chip8 jogos/PONG.ch8 --scale 8 --clock 600 --load-addr 0x200
```

Executa o jogo **PONG** com:

- Escala 8x
- Clock de 600 Hz
- ROM carregada a partir do endereço `0x200`

---

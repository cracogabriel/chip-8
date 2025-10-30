CHIP-8 Emulator

Como compilar

```
make
```

Como executar

```
./chip8 <arquivo_rom> [escala]
./chip8 <arquivo_rom> --scale <fator> [--clock <hz>]
```

Opções

- --scale <fator>: fator de escala da janela (1 = 64x32, 10 = 640x320). Padrão: 10.
- --clock <hz>: frequência da CPU em Hz (quantas instruções por segundo). Padrão: 500.

Exemplos

```
./chip8 images/1-chip8-logo.ch8 10
./chip8 images/TETRIS --scale 12 --clock 700
```

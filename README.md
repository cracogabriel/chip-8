## CHIP-8 Emulator (C + SDL2)

Um emulador do sistema CHIP-8 implementado em C com SDL2 para vídeo, entrada e áudio. Suporta timers a 60 Hz, som, mapeamento de teclado e execução de ROMs clássicas (ex.: TETRIS, PONG, MAZE). Parâmetros de execução incluem escala da janela, frequência da CPU e endereço de carga.

### Participantes

- Gabriel Craco
- Leonardo Ozima

### Como executar (rápido)

```
make
./chip8 <rom>
```

###  ROMs disponíveis (pasta `images/`)

Alguns dos jogos e demos incluídos para testar o emulador:

- TETRIS
- PONG, PONG2
- MAZE
- INVADERS
- TANK
- BRIX, VBRIX
- BLINKY, BLITZ
- CONNECT4
- 15PUZZLE, PUZZLE
- TICTAC
- KALEID, SYZYGY
- UFO, WIPEOFF

Obs.: há mais ROMs na pasta `images/`; os nomes acima são apenas uma seleção.

### Créditos

As ROMs/jogos/demos CHIP-8 presentes na pasta `images/` foram obtidos do pacote público disponível em Zophar's Domain:
https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html


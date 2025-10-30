# Instruções de Compilação

Este documento explica, passo a passo, como compilar e gerar o executável do projeto CHIP-8 neste repositório usando Make. Também lista as dependências necessárias e inclui comandos exatos para o terminal.

Observação: o projeto usa SDL2 e um Makefile simples que depende do utilitário `sdl2-config` para obter os flags corretos de compilação e linkedição.

## Requisitos e Dependências

- Compilador C e ferramentas de build
  - GCC (pacote: build-essential no Debian/Ubuntu) ou Clang
- SDL2 (biblioteca de desenvolvimento)
  - headers e libs: pacote geralmente chamado `libsdl2-dev` (Debian/Ubuntu)
- Make

Versões testadas (Linux):
- GCC 11+ ou Clang 13+
- SDL2 2.0.10+

## Instalação das Dependências

Escolha o seu sistema e rode os comandos abaixo no terminal.

### Debian/Ubuntu (inclui WSL Ubuntu)
```bash
sudo apt update
sudo apt install -y build-essential libsdl2-dev
```

## Compilação do Projeto

Dentro da pasta raiz do projeto (onde está o `Makefile`):

```bash
make
```

- Este comando irá:
  - Criar a pasta `.build/`
  - Compilar os arquivos fonte (`c8vm.c`, `display.c`, `sound.c`, `util.c`, `main.c`)
  - Linkar tudo em um executável chamado `chip8`

Para limpar os artefatos de compilação (objetos e binário):
```bash
make clean
```

## Execução Rápida

Com o binário gerado, você pode executar uma ROM da pasta `images/`:

```bash
./chip8 images/TETRIS
```

Para mais opções de execução, siga a documentação de guia de uso.


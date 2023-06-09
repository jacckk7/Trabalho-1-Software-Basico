<h1 align="center">Trabalho 1 Software Básico</h1>

# Integrantes:
    - Breno Costa Avelino Lima (211010280)
    - Pedro Rodrigues Diógenes Macedo (211042739)

Trabalho feito no Windows 11.

### Como compilar o programa:

Abrir no codeblocks e buildar usando o gcc.

### Como rodar em linha de comando:

- Para um arquivo apenas:

```bash
./montador nome_arquivo
> Arquivo pré-processado: nome_arquivo_preprocess.exc
> Arquivo de saída: nome_arquivo.exc
```

- Para 2, 3 ou 4 arquivos:

```bash
./montador nome_arquivo_1 nome_arquivo_2 nome_arquivo_3 nome_arquivo_4
> Arquivos pré-processados: nome_arquivo_x_preprocess.exc
./linker nome_arquivo_1 nome_arquivo_2 nome_arquivo_3 nome_arquivo_4
> Arquivo de saída: nome_arquivo_1.exc
```

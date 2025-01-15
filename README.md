# Alunos:

* Guilherme Fornari Leonel - 221017023

* Pedro Avila Beneveli - 221001972


# Como compilar o projeto:

No diretório root do projeto, digite o comando no terminal:

```
    make
```

Isso vai gerar os executáveis `montador` e `ligador` no diretório root do projeto.

## OBS

* Para os CONST em hexadecimal, consideramos o número como sendo de 16 bits em complemento de 2. Também consideramos que um número em hexa escrito com menos de 4 caracteres após o 'x' será preenchido com zeros à esquerda (e.g. 0xF -> 0x000F).

* Como conversado em sala com o professor, fizemos a montagem considerando variáveis `extern` como absolutas, então serão atribuídos valores `0` no mapa de bits. (mas a ligação funciona)
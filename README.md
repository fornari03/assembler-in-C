# Como compilar o projeto:

No diretório root do projeto, digite o comando no terminal:

```
    make
```

Isso vai gerar os executáveis `montador` e `ligador` no diretório root do projeto.

## OBS

* Para os CONST em hexadecimal, consideramos o número como sendo de 16 bits em complemento de 2. Também consideramos que um número em hexa escrito com menos de 4 caracteres após o 'x' terá seu sinal extendido (e.g. 0xF -> 0xFFFF; 0x43 -> 0x0043).

* Como conversado em sala com o professor, fizemos a montagem considerando variáveis `extern` como absolutas, então serão atribuídos valores `0` no mapa de bits. (mas a ligação funciona).

* Os testes foram todos feitos em ambientes Ubuntu Linux (nativo ou WSL). No MinGW do Windows, a princípio, não funcionou.

* Consideramos as instruções de multiplicação e de salto como `MUL` e `JMP; JMPZ; JMPN; JMPP`.
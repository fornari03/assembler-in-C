# Como compilar e rodar o projeto:

No diretório root do projeto, digite o comando no terminal:

```
make
```


Isso vai gerar os executáveis `montador` e `ligador` no diretório root do projeto.

Para rodar o `montador` executando o pré-processamento:
```bash
./montador exemplo.asm    # gera o arquivo exemplo.pre
``` 

Para rodar o `montador` executando a montagem:
```bash
./montador exemplo.pre    # gera o arquivo exemplo.obj
``` 

Para rodar o `ligador`:
```bash
./ligador mod1.obj mod2.obj    # gera o arquivo mod1.e
``` 

* Os testes foram todos feitos em ambientes Ubuntu Linux (nativo ou WSL). No MinGW do Windows, a princípio, não funcionou.

## OBS

* É necessário ter pelo menos um espaço em branco ou quebra de linha entre o rótulo e a instrução/diretiva no arquivo `.asm` para o pré-processamento. E pelo menos um espaço em branco entre o rótulo e a instrução/diretiva para o funcionamento da montagem de um arquivo `.pre`.

* Para os CONST em hexadecimal, consideramos o número como sendo de 16 bits em complemento de 2. Também consideramos que um número em hexa escrito com menos de 4 caracteres após o 'x' terá seu sinal extendido (e.g. 0xF -> 0xFFFF; 0x43 -> 0x0043).

* Como conversado em sala com o professor, fizemos a montagem considerando variáveis `extern` como absolutas, então serão atribuídos valores `0` no mapa de bits. (mas a ligação funciona).

* Consideramos as instruções de multiplicação e de salto como `MUL` e `JMP; JMPZ; JMPN; JMPP`.
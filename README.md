# CSD — Compilador Simples Didático

Este README descreve como executar a VM `mvd` a partir da linha de comando, quais opções estão disponíveis e exemplos passo a passo para uso direto e em modo passo a passo.

## Requisitos
- `gcc` (para compilar `csd.c`) — disponível em MSYS2/MinGW no Windows ou em distribuições Linux/Unix.
- Terminal/Prompt para passar argumentos e interagir via stdin.
- Se quiser usar a interface gráfica do compilador será necessário também o `python`

## Compilar (opcional)
Se você recebeu o código-fonte `csd.c`, compile o executável:

Windows (MSYS2 / MinGW):
```powershell
gcc csd.c -o csd.exe
```
Unix / Linux / macOS:
```bash
gcc csd.c -o csd
```

## Uso básico
```bash
# Compila e gera um arquivo objeto de mesmo nome, mas extensão .o
./csd programa.txt
```

## Como inserir o arquivo via argumento (passo a passo)
1. Abra um terminal/Prompt de comando.
2. Navegue até a pasta que contém `csd` e o arquivo objeto (`programa.txt`).
3. Execute o compilador igual ao exemplo acima:
	```bash
	./csd path/to/code.txt
	```
4. Ao rodar esse comando, existem duas opções:
	- No caso de erro, aparecerá um erro destacando a linha onde o erro se encontra e uma explicação de qual o problema nessa linha;
	- No caso de sucesso, será gerado um arquivo objeto de mesmo nome, mas de extensão `.o` com as intruções para execução na mvd.

## Uso pela interface gráfica
1. Abra um terminal/Prompt de comando.
2. Navegue até a pasta que contém `csd_gui.py`.
3. Rode o comando:
   Windows (MSYS2 / MinGW):
    ```powershell
 	.\python csd_gui.py
 	```
   Unix / Linux / macOS:
	```bash
 	./python3 csd_gui.py
 	```
4. Abra ou escreva um arquivo de código
5. Clique no botão de compilar que ele já salvará o arquivo e depois compilará gerando um arquivo de mesmo nome de extensão `.o`.

# MVD — Máquina Virtual Didática

Este README descreve como executar a VM `mvd` a partir da linha de comando, quais opções estão disponíveis e exemplos passo a passo para uso direto e em modo passo a passo.

## Requisitos
- `gcc` (para compilar `mvd.c`) — disponível em MSYS2/MinGW no Windows ou em distribuições Linux/Unix.
- Terminal/Prompt para passar argumentos e interagir via stdin.

## Compilar (opcional)
Se você recebeu o código-fonte `mvd.c`, compile o executável:

Windows (MSYS2 / MinGW):
```powershell
gcc mvd.c -o mvd.exe
```
Unix / Linux / macOS:
```bash
gcc mvd.c -o mvd
```

## Uso básico
O formato geral é:
```
mvd <arquivo> -[opcoes]
```

- `arquivo.o` — arquivo objeto gerado pelo compilador do projeto.
- Se nenhuma opção for passada, a VM executa em modo direto (executa até `HLT`).

## Opções disponíveis
- `-h` : Mostrar ajuda e sair.
- `-s` : Modo passo a passo. A cada instrução a VM pausa e aguarda `Enter` para continuar; `ESC` pode abortar o passo (dependendo do terminal).

Exemplos:
```bash
# Executa em modo direto (padrão)
./mvd programa.o

# Executa em modo passo a passo
./mvd programa.o -s

# Mostra ajuda
./mvd programa.o -h
```

## Como inserir o arquivo via argumento (passo a passo)
1. Abra um terminal/Prompt de comando.
2. Navegue até a pasta que contém `mvd` e o arquivo objeto (`programa.o`).
3. Execute um dos comandos de exemplo acima, por exemplo:
	- `./mvd programa.o` — roda direto.
	- `./mvd programa.o -s` — roda em modo passo a passo.
4. Se a VM pedir uma entrada (instrução `RD`), você verá a mensagem `PROGRAM INPUT:` no terminal — digite um inteiro e pressione `Enter`.

### Comportamento do modo passo a passo
- Após cada instrução a VM imprime informações de depuração (linha atual, instrução, estado parcial da pilha) e aguarda `Enter` para seguir.
- Para interromper a execução no modo passo a passo, pressione `ESC` (se o terminal capturar essa tecla) ou use `Ctrl+C` para forçar término no terminal que não suporte `ESC`.

### Dicas e observações**
- Se estiver em Windows e `ESC` não funcionar no seu terminal, use `Ctrl+C` para abortar.
- Sempre confirme que o arquivo `.o` está no diretório atual ou passe o caminho absoluto/relativo correto.

## Exemplos de execução (saída esperada)

Abaixo há dois exemplos simples — um para execução direta (padrão) e outro para execução passo a passo (`-s`). As saídas são ilustrativas: seu programa `.o` pode produzir mensagens diferentes dependendo das instruções.

### Exemplo 1 — Modo direto:

**Comando:**
```bash
./mvd programa.o
```
**Fluxo esperado (ilustrativo):**
```
File opened: programa.o
PROGRAM INPUT: 5        # VM solicitou um inteiro; usuário digitou 5
PROGRAM OUTPUT: 42      # instrução PRN imprimiu o valor no topo da pilha
```

**Observações:**
- Em modo direto a VM executa até a instrução `HLT` sem pausar entre instruções.
- Se a instrução `RD` ocorrer, a VM escreverá `PROGRAM INPUT:` e aguardará um inteiro no stdin.

### Exemplo 2 — Modo passo a passo (`-s`):

**Comando:**
```bash
./mvd programa.o -s
```
**Fluxo esperado (ilustrativo):**
```
===DEBUGGING: P STACK CONTENT===
P[0]: START
P[1]: LDC 10
P[2]: LDC 32
P[3]: ADD
P[4]: PRN
=================================================
Executing Line:1
Instruction: LDC 10
Stack top (s=0):
[0]:10
Next Line:2
(pressione Enter)
=================================================
Executing Line:2
Instruction: LDC 32
Stack top (s=1):
[0]:10
[1]:32
Next Line:3
(pressione Enter)
... (continua até HLT)
```

**Observações:**
- No modo `-s` a VM imprime informações de depuração e aguarda `Enter` a cada passo.
- Pressione `ESC` (se suportado) ou `Ctrl+C` para abortar a execução passo a passo.

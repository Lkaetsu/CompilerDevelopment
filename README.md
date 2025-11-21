# CompilerDevelopment

## Modo Interface (`--iface`)
O executável `mvd_back` agora suporta um modo de execução voltado para integração com uma interface gráfica Python.

### Uso
```
./mvd_back arquivo.o --iface
```
Eventos emitidos no stdout:
- `EVENT:READY linenumb=<n>`: programa carregado.
- `EVENT:INSTR idx=<i> type=<T> m=<m> n=<n>`: próxima instrução.
- `EVENT:STACK s=<s> values=<v0>,<v1>,...`: estado da pilha.
- `EVENT:INPUT_REQUEST`: aguardando valor para instrução `RD`.
- `EVENT:OUTPUT value=<x>`: saída da instrução `PRN`.
- `EVENT:DONE idx=<novo_i>`: instrução concluída.
- `EVENT:HALT`: término normal.
- `EVENT:ABORTED`: término via comando `ABORT`.

### Comandos aceitos via stdin
- `CONTINUE`: executa próxima instrução.
- `DIRECT`: troca para modo direto (executa até o fim sem esperar novos comandos).
- `ABORT`: aborta execução.
- Valor numérico (quando em `EVENT:INPUT_REQUEST`): alimenta a instrução `RD`.

## Interface Gráfica Python (`gui.py`)
Requisitos: Python 3 com Tkinter (já incluído na maioria das instalações padrão em Windows).

### Executar
```
python gui.py
```
1. Clique em "Procurar" e selecione o arquivo objeto (`.o`).
2. Escolha "Iniciar Direto" (executa até o fim) ou "Iniciar Passo" (controle instrução a instrução).
3. No modo passo:
	- Botão `Step` envia `CONTINUE`.
	- Botão `Auto` envia passos contínuos até parar ou entrar em pedido de entrada.
4. Quando aparecer `EVENT:INPUT_REQUEST`, digite o valor no campo e clique em "Enviar".
5. "Abortar" encerra a execução enviando `ABORT` (se necessário força kill).

### Compilação
No Windows (MSYS2 ucrt64):
```
C:\msys64\ucrt64\bin\gcc.exe mvd_back.c -o mvd_back.exe
```

### Execução manual sem GUI
```
./mvd_back arquivo.o            # modos antigos interativos
./mvd_back arquivo.o --iface    # modo para automação / GUI
```

## Observações
- O código original de execução foi preservado; o modo `--iface` adiciona apenas o protocolo de eventos.
- Para abortar no modo antigo, use ESC conforme implementação.
- A GUI gerencia parsing dos eventos e comandos.
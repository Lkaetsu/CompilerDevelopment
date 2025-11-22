# CompilerDevelopment

Implementação de uma máquina virtual didática (MVD) e interface gráfica em Python para facilitar teste e depuração dos programas objeto (`.o`).

## Uso do executável C `mvd_back`

Compilação (Windows exemplo):
```
gcc mvd_back.c -o mvd.exe
```

Execução:
```
mvd.exe programa.o [modo]
```
Onde `modo` pode ser:
- `1` (padrão): execução direta até `HLT`.
- `2`: execução passo a passo; o programa espera Enter após cada instrução ou `q` (seguido de Enter) para abortar.

Entradas de instrução `RD` são solicitadas com o texto:
```
ENTRADA DO PROGRAMA:
```
Digite um inteiro e pressione Enter.

## Interface Gráfica Python `mvd_gui.py`

Requisitos: Python 3 com Tkinter.

Iniciar GUI:
```
python mvd_gui.py
```
Funcionalidades:
- Seleção de arquivo `.o`.
- Botão "Compilar MVD" (chama `gcc mvd_back.c -o mvd.exe`).
- Escolha do modo (Direto ou Passo).
- "Iniciar" executa o programa.
- Em modo Passo, botão "Avançar" envia Enter para próxima instrução.
- Detecção automática de prompt `ENTRADA DO PROGRAMA:` habilita campo para enviar valor.
- "Abortar" envia `q` e finaliza processo.

## Estrutura de Arquivos
```
mvd_back.c          # VM em C (agora com argumento opcional de modo)
mvd_gui.py          # GUI Tkinter para interação
csd_gui.py          # GUI anterior (outro componente do projeto)
*.h / *.c           # Demais fontes do compilador
output/             # Saídas geradas
```

## Atualizações Recentes
- Adicionado argumento `[modo]` ao `mvd_back` (1 direto, 2 passo).
- Removida dependência de `_getch`/ESC, usando stdin normal.
- Criada GUI `mvd_gui.py` com suporte a passo, abortar e entrada `RD`.

## Próximos Passos (sugestões)
- Destacar visualmente mudanças na pilha entre passos.
- Exportar trace de execução para arquivo.
- Adicionar validação de overflow/underflow de pilha.
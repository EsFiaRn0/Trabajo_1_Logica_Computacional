
## Requisitos

- `gcc` – Compilador de C
- `flex` – Generador de analizadores léxicos
- (Opcional) `make` – Para automatizar la compilación

---
## Compilación


### Opción 1: Usando make

```bash
make

Esto generará el ejecutable llamado tarea1.exe.

### Opción 2: Manualmente, sin make

```bash
flex sat_cnf.l

gcc -Wall -g -c sat_main.c logic.c postfix_converter.c sat_solver.c lex.yy.c

gcc -Wall -g -o tarea1.exe sat_main.o logic.o postfix_converter.o lex.yy.o -lfl


Esto generará el ejecutable llamado tarea1.exe.
---

## Ejecución

Prepará un archivo llamado expresion.txt con una fórmula lógica, por ejemplo:

```bash
$p \wedge \neg p$

Ejecutá el programa pasando ese archivo como entrada estándar:

```bash
./tarea1.exe < expresion.txt

##  Limpieza

Para eliminar los archivos compilados y el ejecutable:

```bash
make clean

o

```bash
rm -f *.o lex.yy.c tarea1.exe
---
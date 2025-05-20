TAREA 1 - LÓGICA PROPOSICIONAL

Este proyecto implementa un verificador de satisfacibilidad para expresiones proposicionales. Utiliza C y Flex para procesar fórmulas lógicas ingresadas desde un archivo.

===========================================
ESTRUCTURA DE DIRECTORIOS
===========================================

Tarea1/
├── codigos/
│   ├── expresion.txt
│   ├── expresion1.txt
│   ├── expresion2.txt
│   ├── logic.c
│   ├── logic.h
│   ├── Makefile
│   ├── postfix_converter.c
│   ├── postfix_converter.h
│   ├── sat_cnf.l
│   ├── sat_main.c
│   ├── sat_solver.c
├── Informe/
│   ├── Imagenes/
│   ├── Informe Tex/

===========================================
REQUISITOS
===========================================

- gcc       -> Compilador de C
- flex      -> Generador de analizadores léxicos
- make      -> (opcional) Para automatizar la compilación

===========================================
COMPILACIÓN
===========================================

Opción 1: Usando make

    make

Esto generará el ejecutable llamado `tarea1.exe`.

-------------------------------------------

Opción 2: Manualmente (sin make)

    flex sat_cnf.l

    gcc -Wall -g -c sat_main.c logic.c postfix_converter.c sat_solver.c lex.yy.c

    gcc -Wall -g -o tarea1.exe sat_main.o logic.o postfix_converter.o sat_solver.o lex.yy.o -lfl

Esto generará el ejecutable llamado `tarea1.exe`.

===========================================
EJECUCIÓN
===========================================

Se debe preparar un archivo de entrada, por ejemplo `expresion.txt`, que contenga una fórmula lógica, como:

    $p ∧ ¬p$

Luego se ejecuta el programa leyendo dicho archivo desde la entrada estándar:

    ./tarea1.exe < expresion.txt

===========================================
LIMPIEZA
===========================================

Para eliminar los archivos generados durante la compilación, se puede ejecutar:

    make clean

O manualmente:

    rm -f *.o lex.yy.c tarea1.exe

===========================================
Desarrolladores
===========================================
Miguel Olivares Morales & Benjamín Riveros Landeros.
miguel.olivares@usach.cl & benjamin.riveros.l@usach.cl
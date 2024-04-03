# Laboratorio 1: Simulador de Dado con PIC12f683

El objetivo de este laboratorio es tener un primer acercamiento con los pines GPIO y con un microcontrolador, en este caso, se hace uso de un PIC12f683 el cual es programado en lenguaje C y compilado mediante SDCC (**S**mall **D**evice **C** **C**ompiler), este compilador contiene las librerías necesarias para múltiples microcontroladores y la forma en la que se ejecuta el archivo es mediante un `Makefile` el cual se encarga de generar múltiples archivos a partir del `.c`. De estos archivos interesa el `.hex` pues es con este con el que se podrá cargar el firmware al microcontrolador del simulador a utilizar. En este directorio se ecuentran tres archivos: `dado.c`, `dado.simu` y `Makefile`. Para poder ejecutar el programa y generar el archivo `.hex` basta con abrir una terminal y correr:
```
$ make all
```

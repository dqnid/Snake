#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "snake.h"

estadisticas *juego(int dificultad);

int main(void)
{
    int i,j;
    int continuar=1;
    char respuesta;
    int dificultad = 3;
    char difi='3';
    estadisticas *stats;
    WINDOW * wnd;
    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();


    while (continuar)
    {
        printw("\n A) Jugar\n B) Ver resultados\n C) Escoger dificultad\n >> ");
        respuesta = getch();

        switch(respuesta)
        {
            case 'a': case 'A':
                printw("\n Entrando al juego");
                nodelay(stdscr, TRUE);
                stats = juego(dificultad);
                nodelay(stdscr, FALSE);
                break;
            case 'b': case 'B':
                clear();
                printw("\n Puntos finales: %d\n Duración: %d\n Punto de muerte: %d/%d", stats->puntos, stats->tiempo, stats->coord[0], stats->coord[1]);
                break;
            case 'c': case 'C':
                clear();
                printw("\n Indique un número de dificultad:\n 9) Muy fácil - 1) Muy difícil\n >> ");
                difi = getch();
                dificultad = (int)difi;
                break;
            case 'q': case 'Q':
                continuar=0;
                break;
            default: 
                continue;
        }
    }

    nocbreak();
    printw("\n Terminado correctamente\n");
    endwin();
    refresh();
    return 0;
}

estadisticas *juego(int dificultad)
{
    int i,j;
    int continuar;
    char respuesta;
    char **tablero;
    serpiente serp;
    estadisticas *stats = (estadisticas*)malloc(sizeof(estadisticas));

    tablero = (char **)malloc(sizeof(char*)*TAM);
    for (int i=0; i<TAM; i++)
        tablero[i]=(char *)malloc(sizeof(char)*TAM);
    for (int i=0; i<TAM; i++)
    {
        for (int j=0; j<TAM; j++)
            tablero[i][j]='-';
    } 

    stats->puntos=0;
    stats->duracion = time(NULL);
    continuar = 1;
    generarSerpiente(&serp);
    generarFruto(tablero, serp);
    while (continuar)
    {
        stats->coord[0] = serp.cabeza->coord[0];
        stats->coord[1] = serp.cabeza->coord[1];
        if (turno(tablero, &serp, dificultad, stats)==1)
            break;
    }

// falta liberar serp
    for (i=0; i<TAM; i++)
        free(tablero[i]);
    free(tablero);

    return stats;
}

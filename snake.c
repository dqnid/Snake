#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "snake.h"

int generarSerpiente(serpiente *serp)
{   
    if (serp->tam>0)
        return -1;
    serp->tam = 1;
    serp->cabeza = (nodo*)malloc(sizeof(nodo));
    serp->cola = serp->cabeza;
    serp->cabeza->ant = NULL;
    serp->cabeza->cabeza = 1;
    serp->cabeza->cola = 0;
    serp->cabeza->coord[0] = serp->cabeza->coord[1] = TAM / 2;
    serp->cabeza->sig = NULL;
    serp->cabeza->dir = drch;
    return 0;
} 

void mostrarTablero(char **tablero, serpiente serp, estadisticas *stats)
{
    int i, j;
    nodo *pivote = serp.cabeza;
    
    while (pivote != NULL)
    {
        if (pivote->cabeza == 1)
            tablero[pivote->coord[0]][pivote->coord[1]]='>';
        else if (pivote->cola == 1)
            tablero[pivote->coord[0]][pivote->coord[1]]='=';
        else 
            tablero[pivote->coord[0]][pivote->coord[1]]='o';
        pivote = pivote->sig;
    }
    clear();
    stats->tiempo = (time(NULL)-(stats->duracion));
    printw("\n  || SNAKE || Puntos: %d || Posición: %d/%d || %d ||\n", stats->puntos, stats->coord[0], stats->coord[1], stats->tiempo);
    for (i=0; i<TAM; i++)
    {
        for (j=0; j<TAM; j++)
            printw(" %c", tablero[i][j]);
        printw("\n");
    }
}

void generarFruto(char **tablero, serpiente serp)
{
    int x, y;
    //Puede que sea mejor ejecutarlo una sola vez al iniciar el programa
    srand(time(NULL));

    x = rand()%TAM;
    y = rand()%TAM;

    tablero[x][y]='*';
}

void comerFruto(serpiente *serp, char **tablero)
{
    nodo *nuevo = (nodo*)malloc(sizeof(nodo));
    nodo *pivote;

    if (serp->tam == 1)
    {
        nuevo->cabeza=0; nuevo->cola=1;
        nuevo->ant = serp->cabeza;
        nuevo->sig = NULL;
        serp->cabeza->sig = nuevo;
        serp->cola = nuevo;
        nuevo->dir = nuevo->ant->dir;
        if (nuevo->ant->dir==ab)
        {
            nuevo->coord[0] = nuevo->ant->coord[0] - 1;
            nuevo->coord[1] = nuevo->ant->coord[1];
        }else if (nuevo->ant->dir==ar)
        {
            nuevo->coord[0] = nuevo->ant->coord[0] + 1;
            nuevo->coord[1] = nuevo->ant->coord[1];
        }else if (nuevo->ant->dir==drch)
        {
            nuevo->coord[1] = nuevo->ant->coord[1] - 1;
            nuevo->coord[0] = nuevo->ant->coord[0];
        }else if (nuevo->ant->dir==izq)
        {
            nuevo->coord[1] = nuevo->ant->coord[1] + 1;
            nuevo->coord[0] = nuevo->ant->coord[0];
        }
        serp->tam++;
    }else 
    {
        nuevo->cabeza = 0; nuevo->cola = 1;
        nuevo->ant = serp->cola;
        nuevo->sig = NULL;
        serp->cola->sig = nuevo;
        serp->cola = nuevo;
        nuevo->ant->cola = 0;
        nuevo->dir = nuevo->ant->dir;
        if (nuevo->ant->dir==ab)
        {
            nuevo->coord[0] = nuevo->ant->coord[0] - 1;
            nuevo->coord[1] = nuevo->ant->coord[1];
        }else if (nuevo->ant->dir==ar)
        {
            nuevo->coord[0] = nuevo->ant->coord[0] + 1;
            nuevo->coord[1] = nuevo->ant->coord[1];
        }else if (nuevo->ant->dir==drch)
        {
            nuevo->coord[1] = nuevo->ant->coord[1] - 1;
            nuevo->coord[0] = nuevo->ant->coord[0];
        }else if (nuevo->ant->dir==izq)
        {
            nuevo->coord[1] = nuevo->ant->coord[1] + 1;
            nuevo->coord[0] = nuevo->ant->coord[0];
        }
        serp->tam++;
    }
    generarFruto(tablero, *serp);
}

int turno(char **tablero, serpiente *serp, int dif, estadisticas *stats)
{
    char entrada;
    nodo *pivote;
    mostrarTablero(tablero, *serp, stats);
    if (serp->cabeza->coord[0] >= TAM || serp->cabeza->coord[1]>=TAM || serp->cabeza->coord[0] < 0 || serp->cabeza->coord[1] < 0)
      return 1;
    
    entrada = getch();
    timeout(100*dif);
    //read(tuberia[0], &entrada, sizeof(entrada));
    if (entrada == 'w' || entrada == 'k')
    {
        serp->cabeza->dir=ar;
    }else if (entrada == 's' || entrada == 'j')
    {
        serp->cabeza->dir=ab;
    }else if (entrada == 'a' || entrada == 'h')
    {
        serp->cabeza->dir=izq;
    }else if (entrada == 'd' || entrada == 'l')
    {
        serp->cabeza->dir=drch;
    }else if (entrada == 'q' || entrada == 'o')
        return 1;    

    //borrar coordPrevias 
    pivote = serp->cabeza;
    while (pivote != NULL)
    {
        tablero[pivote->coord[0]][pivote->coord[1]]='-';
        pivote=pivote->sig;
    }

    //No puto va, mono
    pivote = serp->cabeza;
    while (pivote != NULL)
    {
        //Lo acabas de cambiar por la serpiente gilipollas
        if (pivote->dir==ar)
        {
            if (tablero[(pivote->coord[0])-1][pivote->coord[1]] == '*')
            {
                comerFruto(serp, tablero);
                stats->puntos++;
            }
        }
        else if (pivote->dir==ab)
        {
            if (tablero[(pivote->coord[0]+1)][pivote->coord[1]] == '*')
            {
                comerFruto(serp, tablero);
                stats->puntos++;
            }
        }
        else if (pivote->dir==drch)
        {
            if (tablero[(pivote->coord[0])][pivote->coord[1]+1] == '*')
            {
                comerFruto(serp, tablero);
                stats->puntos++;
            }
        }
        else if (pivote->dir==izq)
        {
            if (tablero[(pivote->coord[0])][pivote->coord[1]-1] == '*')
            {
                comerFruto(serp, tablero);
                stats->puntos++;
            }
        }
        pivote = pivote->sig;
    }
    //Moverse
    pivote = serp->cabeza;
    while (pivote != NULL)
    {
        if (pivote->dir==ar)
            pivote->coord[0]--;
        else if (pivote->dir==ab)
            pivote->coord[0]++;
        else if (pivote->dir==drch)
            pivote->coord[1]++;
        else if (pivote->dir==izq)
            pivote->coord[1]--;
        pivote = pivote->sig;
    }
    if (serp->cabeza->coord[0]>=TAM || serp->cabeza->coord[0]<0 || serp->cabeza->coord[1]>=TAM || serp->cabeza->coord[1]<0)
    {   
        clear();
        printw(" \n Has muerto \n");
        return 1;
    }
    //Actualizar direcciones
    pivote = serp->cola;
    while (pivote != serp->cabeza)
    {
        pivote->dir = pivote->ant->dir;
        pivote = pivote->ant;
    }
    return 0;
}

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}
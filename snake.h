#define TAM 30

enum direcciones {ar,ab,izq,drch};

typedef struct estadisticas{
    int puntos;
    int coord[2];
    time_t duracion;
    int tiempo;
}estadisticas;

typedef struct nodo{
    int coord[2];
    int cabeza, cola;
    enum direcciones dir;
    struct nodo *ant, *sig;
}nodo;

typedef struct serpiente{
    nodo *cabeza, *cola;
    int tam;
}serpiente;

int generarSerpiente(serpiente *serp);
void mostrarTablero(char **tablero, serpiente serp, estadisticas *stats);
void comerFruto(serpiente *serp, char **tablero);
int turno(char **tablero, serpiente *serp, int dif, estadisticas *stats);
void generarFruto(char **tablero, serpiente serp);
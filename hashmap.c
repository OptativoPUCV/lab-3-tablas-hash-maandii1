#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long posicion = hash(key, map->capacity);
    long posOriginal = posicion; //guardamos la posicion original para evitar bucles infinitos
    
    while (map->buckets[posicion] != NULL) { //si ya hay un par en esa posicion
        if (map->buckets[posicion]->key != NULL && is_equal(map->buckets[posicion]->key, key)) { //si la llave ya existe
            //clave duplicada
            return;
        }
        if (map->buckets[posicion]->key == NULL) { //si la llave es NULL, significa que el espacio esta libre
            break; //salimos del bucle para insertar el nuevo par
        }
        posicion = (posicion + 1) %map->capacity; //buscamos la siguiente posicion
        if (posicion == posOriginal) { //si volvimos a la posicion original, significa que la tabla esta llena
            return;
        }

        Pair * nuevo = createPair(key, value); //creamos el nuevo par
        map->buckets[posicion] = nuevo;
        map->size++;
        map->current = posicion;
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
}


HashMap * createMap(long capacity) {

    return NULL;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
